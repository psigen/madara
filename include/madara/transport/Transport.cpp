#include "Transport.h"

#include "madara/utility/Utility.h"
#include "madara/expression_tree/Interpreter.h"

#include <algorithm>

Madara::Transport::Base::Base (const std::string & id,
  Settings & new_settings,
  Knowledge_Engine::Thread_Safe_Context & context) 
  : is_valid_ (false), shutting_down_ (false),
  valid_setup_ (mutex_), id_ (id),
  settings_ (new_settings), context_ (context)
{
  settings_.attach (&context_);
  packet_scheduler_.attach (&settings_);

#ifdef _USE_CID_
  settings_.setup ();
#endif // _USE_CID_
}

Madara::Transport::Base::~Base ()
{
}

int
Madara::Transport::Base::setup (void) 
{
  // check for an on_data_received ruleset
  if (settings_.on_data_received_logic.length () != 0)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Transport::Base::setup" \
      " setting rules to %s\n", 
      settings_.on_data_received_logic.c_str ()));

    Madara::Expression_Tree::Interpreter interpreter;
    on_data_received_ = interpreter.interpret (context_,
      settings_.on_data_received_logic);
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Transport::Base::setup" \
      " no permanent rules were set\n"));
  }
  
  // setup the send buffer
  if (settings_.queue_length > 0)
    buffer_ = new char [settings_.queue_length];

  return validate_transport ();
}

long
Madara::Transport::Base::setHosts (const Hosts_Vector & hosts) 
{ 
  hosts_ = hosts; 
  return 0;
}

long
Madara::Transport::Base::addHost (const std::string & host) 
{ 
  hosts_.push_back (host); 
  return 0;
}

void
Madara::Transport::Base::close (void)
{
  invalidate_transport ();
}

int
Madara::Transport::process_received_update (
  const char * buffer,
  uint32_t bytes_read,
  const std::string & id,
  Knowledge_Engine::Thread_Safe_Context & context,
  const QoS_Transport_Settings & settings,
  Bandwidth_Monitor & send_monitor,
  Bandwidth_Monitor & receive_monitor,
  Knowledge_Map & rebroadcast_records,
  Knowledge_Engine::Compiled_Expression & on_data_received,
  const char * print_prefix,
  const char * remote_host,
  Message_Header *& header)
{
  // reset header to 0, so it is safe to delete
  header = 0;
  
  // setup buffer remaining, used by the knowledge record read method
  int64_t buffer_remaining = (int64_t) bytes_read;

  // tell the receive bandwidth monitor about the transaction
  receive_monitor.add (bytes_read);

  MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
    DLINFO "%s:" \
    " Receive bandwidth = %d B/s\n",
    print_prefix,
    receive_monitor.get_bytes_per_second ()));

  buffer_remaining = (int64_t)bytes_read;
  bool is_reduced = false;

  // check the buffer for a reduced message header
  if (Reduced_Message_Header::reduced_message_header_test (buffer))
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "%s:" \
      " processing reduced KaRL message from %s\n",
      print_prefix,
      remote_host));

    header = new Reduced_Message_Header ();
    is_reduced = true;
  }
  else if (Message_Header::message_header_test (buffer))
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "%s:" \
      " processing KaRL message from %s\n",
      print_prefix,
      remote_host));
        
    header = new Message_Header ();
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "%s:" \
      " dropping non-KaRL message from %s:%d\n",
      print_prefix,
      remote_host));
    return -1;
  }
          
  const char * update = header->read (buffer, buffer_remaining);

  if (!is_reduced)
  {
    // reject the message if it is us as the originator (no update necessary)
    if (id == header->originator)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "%s:" \
        " dropping message from ourself\n",
        print_prefix));

      return -2;
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "%s:" \
        " remote id (%s) is not our own\n",
        print_prefix,
        remote_host));
    }

    if (settings.is_trusted (remote_host))
    {
      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "%s: remote id (%s) is trusted\n",
        print_prefix,
        remote_host));
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "%s:" \
        " dropping message from untrusted peer (%s\n",
        print_prefix,
        remote_host));

      // delete the header and continue to the svc loop
      return -3;
    }

    std::string originator (header->originator);
        
    if (settings.is_trusted (originator))
    {
      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "%s:" \
        " originator (%s) is trusted\n",
        print_prefix,
        originator.c_str ()));
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "%s:" \
        " dropping message from untrusted originator (%s)\n",
        print_prefix,
        originator.c_str ()));

      return -4;
    }



    // reject the message if it is from a different domain
    if (settings.domains != header->domain)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "%s:" \
        " remote id (%s) in a different domain (%s). Dropping message.\n",
        print_prefix,
        remote_host,
        header->domain));

      // delete the header and continue to the svc loop
      return -5;
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "%s:" \
        " remote id (%s) message is in our domain\n",
        print_prefix,
        remote_host));
    }
  }
      
  MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
    DLINFO "%s:" \
    " iterating over the %d updates\n",
    print_prefix,
    header->updates));
      
  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "%s: locking context\n",
    print_prefix));
      
  // temporary record for reading from the updates buffer
  Knowledge_Record record;
  record.quality = header->quality;
  record.clock = header->clock;
  std::string key;

  // lock the context
  context.lock ();
      
  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "%s:" \
    " past the lock\n",
    print_prefix));

  int actual_updates = 0;
  uint64_t current_time = time (NULL);

  // iterate over the updates
  for (uint32_t i = 0; i < header->updates; ++i)
  {
    // read converts everything into host format from the update stream
    update = record.read (update, key, buffer_remaining);
        
    if (buffer_remaining < 0)
    {
      MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
        DLINFO "%s:" \
        " unable to process message. Buffer remaining is negative." \
        " Server is likely being targeted by custom KaRL tools.\n",
        print_prefix));

      // we do not delete the header as this will be cleaned up later
      break;
    }

    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "%s:" \
      " attempting to apply %s=%s\n",
      print_prefix,
      key.c_str (), record.to_string ().c_str ()));
        
    uint32_t receive_bandwidth = receive_monitor.get_bytes_per_second ();
    uint32_t send_bandwidth = send_monitor.get_bytes_per_second ();

    Transport_Context transport_context (
      Transport_Context::REBROADCASTING_OPERATION,
      receive_bandwidth,
      send_bandwidth,
      header->timestamp,
      current_time);
        
    // if the tll is 1 or more and we are participating in rebroadcasts
    if (header->ttl > 0 && 
        settings.get_participant_ttl () > 0)
    {
      Knowledge_Record rebroadcast_temp (record);

      // if we have rebroadcast filters, apply them
      if (settings.get_number_of_rebroadcast_filtered_types () > 0)
      { 
        rebroadcast_temp = settings.filter_rebroadcast (
          record, key,
          transport_context);
      }

      // if the record was not filtered out entirely, add it to map
      if (rebroadcast_temp.status () != Knowledge_Record::UNCREATED)
      {
        MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
          DLINFO "%s:" \
          " update %s=%s is queued for rebroadcast\n",
          print_prefix,
          key.c_str (), rebroadcast_temp.to_string ().c_str ()));

        rebroadcast_records[key] = rebroadcast_temp;
      }
      else
      {
        MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
          DLINFO "%s:" \
          " update %s was filtered out of rebroadcast\n",
          print_prefix,
          key.c_str ()));
      }
    }

    // use the original message to apply receive filters
    if (settings.get_number_of_received_filtered_types () > 0)
    {
      transport_context.set_operation (
        Transport_Context::RECEIVING_OPERATION);
      transport_context.set_receive_bandwidth (receive_bandwidth);
      transport_context.set_send_bandwidth (send_bandwidth);
        
      record = settings.filter_receive (record, key,
        transport_context);
    }

    int result = 0;

    // if receive filter did not delete the update, apply it
    if (record.status () != Knowledge_Record::UNCREATED)
    {
      result = record.apply (context, key, header->quality,
        header->clock, false);
      ++actual_updates;
    }

    if (result != 1)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "%s:" \
        " update %s=%s was rejected\n",
        print_prefix,
        key.c_str (), record.to_string ().c_str ()));
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "%s:" \
        " update %s=%s was accepted\n",
        print_prefix,
        key.c_str (), record.to_string ().c_str ()));
    }
  }
      
  // before we send to others, we first execute rules
  if (settings.on_data_received_logic.length () != 0)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "%s:" \
      " evaluating rules in %s\n",
      print_prefix, 
      settings.on_data_received_logic.c_str ()));

    context.evaluate (on_data_received);
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "%s:" \
      " no permanent rules were set\n",
      print_prefix));
  }

  // unlock the context
  context.unlock ();
  context.set_changed ();
  
  return actual_updates;
}


int
Madara::Transport::prep_rebroadcast (
  char * buffer,
  int64_t & buffer_remaining,
  const QoS_Transport_Settings & settings,
  const char * print_prefix,
  Message_Header * header,
  const Knowledge_Map & records,
  Packet_Scheduler & packet_scheduler)
{
  if (header->ttl > 0 && records.size () > 0 && packet_scheduler.add ())
  {
    // keep track of the message_size portion of buffer
    uint64_t * message_size = (uint64_t *)buffer;

    // the number of updates will be the size of the records map
    header->updates = uint32_t (records.size ());

    // set the update to the end of the header
    char * update = header->write (buffer, buffer_remaining);

    for (Knowledge_Map::const_iterator i = records.begin ();
         i != records.end (); ++i)
    {
      update = i->second.write (update, i->first, buffer_remaining);
    }
    
    if (buffer_remaining > 0)
    {
      int size = (int)(settings.queue_length - buffer_remaining);
      *message_size = Madara::Utility::endian_swap ((uint64_t)size);
    
      return size;
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "%s:" \
        " Not enough buffer for rebroadcasting packet\n",
        print_prefix));

      return -2;
    }
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "%s:" \
      " No rebroadcast necessary.\n",
      print_prefix,
      settings.queue_length));

    return -1;
  }
}

long Madara::Transport::Base::prep_send (
  const Madara::Knowledge_Records & orig_updates,
  const char * print_prefix)
{
  // check to see if we are shutting down
  long ret = this->check_transport ();
  if (-1 == ret)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "%s: transport has been told to shutdown",
      print_prefix)); 
    return ret;
  }
  else if (-2 == ret)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "%s: transport is not valid",
      print_prefix)); 
    return ret;
  }
 
  // get the maximum quality from the updates
  uint32_t quality = Madara::max_quality (orig_updates);
  bool reduced = false;

  Knowledge_Map filtered_updates;
  
  MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
    DLINFO "%s:" \
    " Applying filters before sending...\n",
      print_prefix));
  
  Transport_Context transport_context (Transport_Context::SENDING_OPERATION,
      receive_monitor_.get_bytes_per_second (),
      send_monitor_.get_bytes_per_second (),
      (uint64_t) time (NULL));

  if (packet_scheduler_.add ())
  {
    /**
     * filter the updates according to the filters specified by
     * the user in QoS_Transport_Settings (if applicable)
     **/
    for (Knowledge_Records::const_iterator i = orig_updates.begin ();
          i != orig_updates.end (); ++i)
    {
      // filter the record according to the send filter chain
      Knowledge_Record result = settings_.filter_send (*i->second, i->first,
        transport_context);

      if (result.status () != Knowledge_Record::UNCREATED)
        filtered_updates[i->first] = result;
    }
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "%:" \
      " Packet scheduler has dropped packet...\n"));

    return 0;
  }

  MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
    DLINFO "%:" \
    " Finished applying filters before sending...\n",
      print_prefix));

  if (filtered_updates.size () == 0)
  {
    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
      DLINFO "%s:" \
      " Filters removed all data. Nothing to send.\n",
      print_prefix));

    return 0;
  }

  // allocate a buffer to send
  char * buffer = buffer_.get_ptr ();
  int64_t buffer_remaining = settings_.queue_length;
  
  if (buffer == 0)
  {
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
      DLINFO "%s:" \
      " Unable to allocate buffer of size %d. Exiting thread.\n",
      print_prefix,
      settings_.queue_length));
    
    return -3;
  }


  // set the header to the beginning of the buffer
  Message_Header * header = 0;

  if (settings_.send_reduced_message_header)
  {
    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
      DLINFO "%s:" \
      " Preparing message with reduced message header.\n",
      print_prefix));
    header = new Reduced_Message_Header ();
    reduced = true;
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
      DLINFO "%s:" \
      " Preparing message with normal message header.\n",
      print_prefix));
    header = new Message_Header ();
  }

  // get the clock
  header->clock = context_.get_clock ();

  if (!reduced)
  {
    // copy the domain from settings
    strncpy (header->domain, this->settings_.domains.c_str (),
      sizeof (header->domain) - 1);

    // get the quality of the key
    header->quality = quality;

    // copy the message originator (our id)
    strncpy (header->originator, id_.c_str (), sizeof (header->originator) - 1);

    // send data is generally an assign type. However, Message_Header is
    // flexible enough to support both, and this will simply our read thread
    // handling
    header->type = Madara::Transport::MULTIASSIGN;

    // set the time-to-live
    header->ttl = settings_.get_rebroadcast_ttl ();
  }

  header->updates = uint32_t (filtered_updates.size ());

  // compute size of this header
  header->size = header->encoded_size ();

  // set the update to the end of the header
  char * update = header->write (buffer, buffer_remaining);
  uint64_t * message_size = (uint64_t *)buffer;
  
  // Message header format
  // [size|id|domain|originator|type|updates|quality|clock|list of updates]
  
  /**
   * size = buffer[0] (unsigned 64 bit)
   * transport id = buffer[8] (8 byte)
   * domain = buffer[16] (32 byte domain name)
   * originator = buffer[48] (64 byte originator host:port)
   * type = buffer[112] (unsigned 32 bit type of message--usually MULTIASSIGN)
   * updates = buffer[116] (unsigned 32 bit number of updates)
   * quality = buffer[120] (unsigned 32 bit quality of message)
   * clock = buffer[124] (unsigned 64 bit clock for this message)
   * ttl = buffer[132] (the new knowledge starts here)
   * knowledge = buffer[133] (the new knowledge starts here)
  **/

  // zero out the memory
  //memset(buffer, 0, Madara::Transport::MAX_PACKET_SIZE);

  // Message update format
  // [key|value]
  
  int j = 0;
  for (Knowledge_Map::const_iterator i = filtered_updates.begin ();
    i != filtered_updates.end (); ++i, ++j)
  {
    update = i->second.write (update, i->first, buffer_remaining);
    
    if (buffer_remaining > 0)
    {
      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "%s:" \
        " update[%d] => encoding %s of type %d and size %d\n",
        print_prefix,
        j, i->first.c_str (), i->second.type (), i->second.size ()));
    }
    else
    {
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
      DLINFO "%s:" \
      " unable to encode update[%d] => %s of type %d and size %d\n",
      print_prefix,
      j, i->first.c_str (), i->second.type (), i->second.size ()));
    }
  }
  
  long size (0);

  if (buffer_remaining > 0)
  {
    size = (long)(settings_.queue_length - buffer_remaining);
    *message_size = Madara::Utility::endian_swap ((uint64_t)size);
    
    // before we send to others, we first execute rules
    if (settings_.on_data_received_logic.length () != 0)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "%s:" \
        " evaluating rules in %s\n",
        print_prefix, 
        settings_.on_data_received_logic.c_str ()));

      on_data_received_.evaluate ();

      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "%s:" \
        " rules have been successfully evaluated\n",
      print_prefix));
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "%s:" \
        " no permanent rules were set\n",
        print_prefix));
    }

    // send the buffer contents to the multicast address
  }

  delete header;

  return size;
}