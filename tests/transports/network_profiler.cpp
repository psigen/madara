
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <assert.h>

#include "ace/High_Res_Timer.h"
#include "ace/OS_NS_Thread.h"
#include "ace/Sched_Params.h"

#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/utility/Log_Macros.h"

// default transport settings
std::string host ("");
const std::string default_multicast ("239.255.0.1:4150");
Madara::Transport::QoS_Transport_Settings settings;

// keep track of time
ACE_hrtime_t elapsed_time, maximum_time;
ACE_High_Res_Timer timer;

// publisher array for keeping track of ack latencies
ACE_High_Res_Timer latencies[1000];
int latencies_received[1000];

// number of sent and received messages
Madara::Knowledge_Engine::Variable_Reference  num_sent;
Madara::Knowledge_Engine::Variable_Reference  num_received;
Madara::Knowledge_Engine::Variable_Reference  ack;

// to stop sending acks, flip to false
bool send_acks = true;

// amount of time in seconds to burst payloads
double publish_time = 10.0;

// payload size to burst
unsigned int data_size = 1000;

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
        settings.hosts_.push_back (argv[i + 1]);
        settings.type = Madara::Transport::MULTICAST;
      }
      ++i;
    }
    else if (arg1 == "-b" || arg1 == "--broadcast")
    {
      if (i + 1 < argc)
      {
        settings.hosts_.push_back (argv[i + 1]);
        settings.type = Madara::Transport::BROADCAST;
      }
      ++i;
    }
    else if (arg1 == "-u" || arg1 == "--udp")
    {
      if (i + 1 < argc)
      {
        settings.hosts_.push_back (argv[i + 1]);
        settings.type = Madara::Transport::UDP;
      }
      ++i;
    }
    else if (arg1 == "-a" || arg1 == "--no-latency")
    {
      send_acks = false;
    }
    else if (arg1 == "-o" || arg1 == "--host")
    {
      if (i + 1 < argc)
        host = argv[i + 1];

      ++i;
    }
    else if (arg1 == "-d" || arg1 == "--domain")
    {
      if (i + 1 < argc)
        settings.domains = argv[i + 1];

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
    else if (arg1 == "-p" || arg1 == "--drop-rate")
    {
      if (i + 1 < argc)
      {
        double drop_rate;
        std::stringstream buffer (argv[i + 1]);
        buffer >> drop_rate;
        
        settings.update_drop_rate (drop_rate,
          Madara::Transport::PACKET_DROP_DETERMINISTIC);
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
    else if (arg1 == "-s" || arg1 == "--size")
    {
      if (i + 1 < argc)
      {
        std::stringstream buffer (argv[i + 1]);
        buffer >> data_size;
      }

      ++i;
    }
    else if (arg1 == "-t" || arg1 == "--time")
    {
      if (i + 1 < argc)
      {
        std::stringstream buffer (argv[i + 1]);
        buffer >> publish_time;
      }

      ++i;
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
        "\nProgram summary for %s:\n\n" \
        "  Profiles a network transport. Requires 2 processes. The result of\n" \
        "  running these processes should be that each process reports\n" \
        "  latency and throughput statistics.\n\n" \
        " [-a|--no-latency]        do not test for latency (throughput only)\n" \
        " [-b|--broadcast ip:port] the broadcast ip to send and listen to\n" \
        " [-d|--domain domain]     the knowledge domain to send and listen to\n" \
        " [-i|--id id]             the id of this agent (should be non-negative)\n" \
        " [-l|--level level]       the logger level (0+, higher is higher detail)\n" \
        " [-m|--multicast ip:port] the multicast ip to send and listen to\n" \
        " [-o|--host hostname]     the hostname of this process (def:localhost)\n" \
        " [-f|--logfile file]      log to a file\n" \
        " [-r|--reduced]           use the reduced message header\n" \
        " [-s|--size size]         size of data packet to send in bytes\n" \
        " [-t|--time time]         time to burst messages for throughput test\n" \
        " [-u|--udp ip:port]       the udp ips to send to (first is self to bind to)\n" \
        "\n",
        argv[0]));
      exit (0);
    }
  }
}


// filter for counting received packets and removing payload
Madara::Knowledge_Record
count_received (
  Madara::Knowledge_Engine::Function_Arguments & args,
  Madara::Knowledge_Engine::Variables & vars)
{
  Madara::Knowledge_Record result;
  
  if (args.size () > 0)
  {
    if (args[0].is_binary_file_type ())
    {
      // if this is the first message we have received, start
      // the timer.
      if (vars.get (num_received).is_false ())
        timer.start ();

      timer.stop ();

      Madara::Knowledge_Record cur_received = vars.inc (num_received);

      if (send_acks)
      {
        // only send an ack for the first 2000 messages
        if (cur_received.to_integer () < 2000)
        {
          // this is a safe way to add variables to a rebroadcast
          // without modifying the local context
          args.push_back ("ack");
          args.push_back (vars.inc (ack));
        }
        else
          send_acks = false;
      }
    }
  }

  return result;
}

// filter for counting received acks, stopping timer and removing payload
Madara::Knowledge_Record
handle_acks (
  Madara::Knowledge_Engine::Function_Arguments & args,
  Madara::Knowledge_Engine::Variables & vars)
{
  Madara::Knowledge_Record result;
  
  if (args.size () > 0)
  {
    if (args[0].is_integer_type ())
    {
      Madara::Knowledge_Record::Integer handled =
        args[0].to_integer ();
      
      if (handled >= 0 && handled < 1000)
      {
        latencies_received[handled] = 1;
        latencies[handled].stop ();
      }

      vars.inc (num_received);
    }
    result = args[0];
  }

  return result;
}

// filter for counting received acks, stopping timer and removing payload
Madara::Knowledge_Record
init_ack (
  Madara::Knowledge_Engine::Function_Arguments & args,
  Madara::Knowledge_Engine::Variables & vars)
{
  Madara::Knowledge_Record result;
  
  if (args.size () > 0)
  {
    if (args[0].is_binary_file_type ())
    {
      Madara::Knowledge_Record::Integer current_send =
        vars.get (num_sent).to_integer ();

      if (current_send >= 0 && current_send < 1000)
        latencies[current_send].start ();

      vars.inc (num_sent);
    }
    result = args[0];
  }

  return result;
}

int main (int argc, char ** argv)
{
  settings.type = Madara::Transport::MULTICAST;
  settings.queue_length = data_size * 1000;
 
  // use ACE real time scheduling class
  int prio  = ACE_Sched_Params::next_priority
    (ACE_SCHED_FIFO,
     ACE_Sched_Params::priority_max (ACE_SCHED_FIFO),
     ACE_SCOPE_THREAD);
  ACE_OS::thr_setprio (prio);

  // handle all user arguments
  handle_arguments (argc, argv);

  if (settings.hosts_.size () == 0)
  {
    // setup default transport as multicast
    settings.hosts_.resize (1);
    settings.hosts_[0] = default_multicast;
  }
  
  if (settings.id != 0)
  {
    settings.add_receive_filter (
      Madara::Knowledge_Record::ALL_FILE_TYPES,
      count_received);
    settings.enable_participant_ttl ();
  }
  else
  {
    settings.add_receive_filter (
      Madara::Knowledge_Record::INTEGER,
      handle_acks);
    settings.add_send_filter (
      Madara::Knowledge_Record::ALL_FILE_TYPES,
      init_ack);
  }

  // create a knowledge base and setup our id
  Madara::Knowledge_Engine::Knowledge_Base knowledge (host, settings);
  knowledge.set (".id", (Madara::Knowledge_Record::Integer) settings.id);
  
  // setup wait settings to wait for 2 * publish_time seconds
  Madara::Knowledge_Engine::Wait_Settings wait_settings;
  wait_settings.max_wait_time = publish_time * 2;
  
  // set poll frequency to every 10us
  //wait_settings.poll_frequency = 0.00001;

  // get variable references for real-time, constant-time operations
  num_received = knowledge.get_ref (".num_received");
  num_sent = knowledge.get_ref (".num_sent");
  ack = knowledge.get_ref (".ack");

  for (unsigned int j = 0; j < 1000; ++j)
  {
    latencies_received[j] = 0;
  }

  if (settings.id == 0)
  {
    unsigned char * ref_file = new unsigned char[data_size];
    knowledge.set_file (".ref_file", ref_file, data_size);
    timer.start ();
    timer.stop ();
    timer.elapsed_time (elapsed_time);
    Madara::Knowledge_Engine::Compiled_Expression payload_generation =
      knowledge.compile ("ref_file = .ref_file");
    
    ACE_Time_Value max_tv;
    max_tv.set (publish_time);
    maximum_time = max_tv.sec () * 1000000000;
    maximum_time += max_tv.usec () * 1000;
    
    while (maximum_time > elapsed_time)
    {
      knowledge.evaluate (payload_generation);
      timer.stop ();
      timer.elapsed_time (elapsed_time);
    }

    Madara::Knowledge_Record::Integer max_latency (0);
    Madara::Knowledge_Record::Integer min_latency (50000000000);
    Madara::Knowledge_Record::Integer avg_latency (0), cur_latency (0);

    Madara::Knowledge_Record::Integer i (0);
    Madara::Knowledge_Record::Integer received =
      knowledge.get (num_received).to_integer ();

    for (i = 0; i < 1000; ++i)
    {
      if (latencies_received[i] != 0)
      {
        latencies[i].elapsed_time (elapsed_time);
        cur_latency = Madara::Knowledge_Record::Integer (elapsed_time);

        if (cur_latency > 1000000)
          std::cerr << i << ": Unusually large latency (" << cur_latency <<")\n";

        avg_latency += cur_latency;
        min_latency = std::min (min_latency, cur_latency);
        max_latency = std::max (max_latency, cur_latency);
      }
    }

    if (received > 0)
      avg_latency = avg_latency / received;

    knowledge.set (".min_latency", min_latency);
    knowledge.set (".max_latency", max_latency);
    knowledge.set (".avg_latency", avg_latency);
  }
  else
  {
    knowledge.set (ack, Madara::Knowledge_Record::Integer (-1),
      Madara::Knowledge_Engine::Eval_Settings (false, true, false));

    // other processes wait for the publisher to send the goods
    knowledge.wait ("0", wait_settings);

    timer.elapsed_time (elapsed_time);

    // set the elapsed nanoseconds
    knowledge.set (".elapsed_time",
      Madara::Knowledge_Record::Integer (elapsed_time));

    // convert elapsed time from ns -> s and compute throughput
    knowledge.evaluate (
      ".elapsed_time *= 0.000000001 ;"
      ".elapsed_time > 0 => .throughput = .num_received / .elapsed_time");
  }

  knowledge.print_knowledge ();

  return 0;
}