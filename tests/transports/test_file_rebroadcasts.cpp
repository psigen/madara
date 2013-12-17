
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <assert.h>

#include "ace/High_Res_Timer.h"
#include "ace/OS_NS_Thread.h"
#include "ace/Sched_Params.h"
#include "ace/Guard_T.h"
#include "ace/Recursive_Thread_Mutex.h"

#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/utility/Log_Macros.h"

// default transport settings
std::string host ("");
const std::string default_multicast ("239.255.0.1:4150");
Madara::Transport::QoS_Transport_Settings settings;

std::string filename =
  Madara::Utility::expand_envs (
    "$(MADARA_ROOT)/tests/images/manaus_hotel_225x375.jpg");

std::string target_location;

Madara::Knowledge_Record::Integer target_id (1);

Madara::Knowledge_Engine::Compiled_Expression ack;

// keep track of time
ACE_hrtime_t elapsed_time, maximum_time;
ACE_High_Res_Timer timer;

// handle command line arguments
void handle_arguments (int argc, char ** argv)
{
  for (int i = 1; i < argc; ++i)
  {
    std::string arg1 (argv[i]);

    if (arg1 == "-m" || arg1 == "--multicast")
    {
      if (i + 1 < argc)
      {
        settings.hosts.push_back (argv[i + 1]);
        settings.type = Madara::Transport::MULTICAST;
      }
      ++i;
    }
    else if (arg1 == "-b" || arg1 == "--broadcast")
    {
      if (i + 1 < argc)
      {
        settings.hosts.push_back (argv[i + 1]);
        settings.type = Madara::Transport::BROADCAST;
      }
      ++i;
    }
    else if (arg1 == "-u" || arg1 == "--udp")
    {
      if (i + 1 < argc)
      {
        settings.hosts.push_back (argv[i + 1]);
        settings.type = Madara::Transport::UDP;
      }
      ++i;
    }
    else if (arg1 == "-o" || arg1 == "--host")
    {
      if (i + 1 < argc)
        host = argv[i + 1];

      ++i;
    }
    else if (arg1 == "-p" || arg1 == "--payload")
    {
      if (i + 1 < argc)
        filename = argv[i + 1];

      ++i;
    }
    else if (arg1 == "-d" || arg1 == "--domain")
    {
      if (i + 1 < argc)
        settings.domains = argv[i + 1];

      ++i;
    }
    else if (arg1 == "-t" || arg1 == "--target")
    {
      if (i + 1 < argc)
        target_location = argv[i + 1];

      ++i;
    }
    else if (arg1 == "-i" || arg1 == "--id")
    {
      if (i + 1 < argc)
      {
        std::stringstream buffer (argv[i + 1]);
        buffer >> settings.id;
      }

      ++i;
    }
    else if (arg1 == "-l" || arg1 == "--level")
    {
      if (i + 1 < argc)
      {
        std::stringstream buffer (argv[i + 1]);
        buffer >> MADARA_debug_level;
      }

      ++i;
    }
    else if (arg1 == "-f" || arg1 == "--logfile")
    {
      if (i + 1 < argc)
      {
        Madara::Knowledge_Engine::Knowledge_Base::log_to_file (argv[i + 1]);
      }

      ++i;
    }
    else if (arg1 == "-r" || arg1 == "--reduced")
    {
      settings.send_reduced_message_header = true;
    }
    else if (arg1 == "-e" || arg1 == "--rebroadcasts")
    {
      if (i + 1 < argc)
      {
        int hops;
        std::stringstream buffer (argv[i + 1]);
        buffer >> hops;

        settings.set_rebroadcast_ttl (hops);
        settings.enable_participant_ttl (hops);
      }

      ++i;
    }
    else if (arg1 == "-q" || arg1 == "--queue-length")
    {
      if (i + 1 < argc)
      {
        std::stringstream buffer (argv[i + 1]);
        buffer >> settings.queue_length;
      }

      ++i;
    }
    else if (arg1 == "-z" || arg1 == "--target-id")
    {
      if (i + 1 < argc)
      {
        std::stringstream buffer (argv[i + 1]);
        buffer >> target_id;
      }

      ++i;
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
"\nProgram summary for %s:\n\n" \
"  Attempts to send a file over the network with a certain number\n" \
"  of rebroadcasts (-h|--hops controls the number of rebroadcasts)\n\n" \
" [-b|--broadcast ip:port] the broadcast ip to send and listen to\n" \
" [-d|--domain domain]     the knowledge domain to send and listen to\n" \
" [-e|--rebroadcasts hops] maximum number of rebroadcasts allowed\n" \
" [-f|--logfile file]      log to a file\n" \
" [-i|--id id]             the id of this agent (should be non-negative)\n" \
" [-l|--level level]       the logger level (0+, higher is higher detail)\n" \
" [-m|--multicast ip:port] the multicast ip to send and listen to\n" \
" [-o|--host hostname]     the hostname of this process (def:localhost)\n" \
" [-p|--payload filename]  file to use as payload for the message\n" \
" [-q|--queue-length length] length of transport queue in bytes\n" \
" [-r|--reduced]           use the reduced message header\n" \
" [-t|--target path]       file system location to save received files to\n" \
" [-u|--udp ip:port]       a udp ip to send to (first is self to bind to)\n" \
" [-z|--target-id id]      id of the entity that must acknowledge receipt\n" \
"\n",
        argv[0]));
      exit (0);
    }
  }
}

Madara::Knowledge_Record
write_file (Madara::Knowledge_Map & records,
  const Madara::Transport::Transport_Context & context,
  Madara::Knowledge_Engine::Variables & vars)
{
  if (records.find ("file") != records.end ())
  {
    std::stringstream filename;

    if (target_location == "")
      filename << records["file_location"];
    else
      filename << target_location;

    filename << "/";
    filename << records["file_name"];

    if (!Madara::Utility::file_exists (filename.str ()))
    {
      std::stringstream output_buffer;
      output_buffer << "Received file ";
      output_buffer << records["file_name"];
      output_buffer << ". Saving to ";
      output_buffer << filename.str ();
      output_buffer << "\n";

      vars.print (output_buffer.str ());

      size_t size = 0;
      unsigned char * buffer = records["file"].to_unmanaged_buffer (size);

      Madara::Utility::write_file (filename.str (), (void *)buffer, size);

      delete [] buffer;
    }

    vars.evaluate (ack);
    vars.print ("Received file. Sending file ack for id {.id}.\n");
  }

  return Madara::Knowledge_Record::Integer (1);
}

int main (int argc, char ** argv)
{
  // set defaults
  settings.type = Madara::Transport::MULTICAST;
  settings.queue_length = 1000000;
  settings.enable_participant_ttl (5);
  settings.set_rebroadcast_ttl (5);

  // add receive filter for writing a file
  settings.add_receive_filter (write_file);
 
  // handle all user arguments
  handle_arguments (argc, argv);
  
  if (settings.hosts.size () == 0)
  {
    // setup default transport as multicast
    settings.hosts.push_back (default_multicast);
  }
  
  // settings for delaying the sending of modifications
  Madara::Knowledge_Engine::Eval_Settings delay_sending;
  delay_sending.delay_sending_modifieds = true;

  Madara::Knowledge_Engine::Eval_Settings suppress_globals;
  suppress_globals.treat_globals_as_locals = true;
  

  Madara::Knowledge_Engine::Wait_Settings wait_settings;
  wait_settings.poll_frequency = 2.0;
  wait_settings.max_wait_time = 20.0;

  // create a knowledge base and setup our id
  Madara::Knowledge_Engine::Knowledge_Base knowledge (host, settings);
  knowledge.set (".id", Madara::Knowledge_Record::Integer (settings.id));
  knowledge.set (".target", target_id);

  ack = knowledge.compile (knowledge.expand_statement ("file.{.id}.ack = 1"));

  if (settings.id == 0)
  {
    if (target_location == "")
      target_location = ".";
    
    knowledge.print (
      "Sending file until id {.target} acknowledges receipt.\n");

    knowledge.read_file ("file", filename, delay_sending);
    knowledge.set ("file_name",
      Madara::Utility::extract_filename (filename), delay_sending);
    knowledge.set ("file_location", target_location, delay_sending);

    knowledge.evaluate (ack, suppress_globals);

    knowledge.print (
      "Writing {file_name} to network. "
      "Suggested target is {file_location}.\n");

    knowledge.send_modifieds ();

    knowledge.wait (
      "file = file ;>"
      "file_name = file_name ;>"
      "file_location = file_location ;>"
      "file.{.target}.ack", wait_settings);

    knowledge.evaluate ("terminated = 1");

    knowledge.print (
      "Finished waiting."
      " file.{.target}.ack == {file.{.target}.ack}\n");
  }
  else
  {
    knowledge.print ("Waiting for 20s or id 0 to signal terminate.\n");

    knowledge.wait ("terminated", wait_settings);

    knowledge.print ("Finished waiting.\n");
  }

  knowledge.print ();

  return 0;
}