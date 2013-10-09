
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <assert.h>

#include "ace/High_Res_Timer.h"
#include "ace/OS_NS_Thread.h"
#include "ace/Sched_Params.h"

#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/utility/Log_Macros.h"

std::string host ("");
const std::string default_multicast ("239.255.0.1:4150");
Madara::Transport::QoS_Transport_Settings settings;

// keep track of time
ACE_hrtime_t elapsed_time, maximum_time;
ACE_High_Res_Timer timer;

Madara::Knowledge_Engine::Variable_Reference  num_sent;
Madara::Knowledge_Engine::Variable_Reference  num_received;

double publish_time = 10.0;

unsigned int data_size = 1000;

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
        " [-o|--host hostname]     the hostname of this process (def:localhost)\n" \
        " [-m|--multicast ip:port] the multicast ip to send and listen to\n" \
        " [-b|--broadcast ip:port] the broadcast ip to send and listen to\n" \
        " [-u|--udp ip:port]       the udp ips to send to (first is self to bind to)\n" \
        " [-d|--domain domain]     the knowledge domain to send and listen to\n" \
        " [-i|--id id]             the id of this agent (should be non-negative)\n" \
        " [-l|--level level]       the logger level (0+, higher is higher detail)\n" \
        " [-f|--logfile file]      log to a file\n" \
        " [-r|--reduced]           use the reduced message header\n" \
        " [-s|--size size]         size of data packet to send in bytes\n" \
        " [-t|--time time]         time to burst messages for throughput test\n" \
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

      vars.inc (num_received);
    }
  }

  return result;
}

int main (int argc, char ** argv)
{
  settings.type = Madara::Transport::MULTICAST;
  settings.add_receive_filter (
    Madara::Knowledge_Record::ALL_FILE_TYPES,
    count_received);
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

  // create a knowledge base and setup our id
  Madara::Knowledge_Engine::Knowledge_Base knowledge (host, settings);
  knowledge.set (".id", (Madara::Knowledge_Record::Integer) settings.id);
  
  // setup wait settings to wait for 2 * publish_time seconds
  Madara::Knowledge_Engine::Wait_Settings wait_settings;
  wait_settings.max_wait_time = publish_time * 2;

  // get variable references for real-time, constant-time operations
  num_received = knowledge.get_ref (".num_received");
  num_sent = knowledge.get_ref (".num_sent");

  if (settings.id == 0)
  {
    unsigned char * ref_file = new unsigned char[data_size];
    knowledge.set_file (".ref_file", ref_file, data_size);
    timer.start ();
    timer.stop ();
    timer.elapsed_time (elapsed_time);
    Madara::Knowledge_Engine::Compiled_Expression payload_generation =
      knowledge.compile ("ref_file = .ref_file; ++.num_sent");
    
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
  }
  else
  {
    // other processes wait for the publisher to send the goods
    knowledge.wait ("0", wait_settings);

    timer.elapsed_time (elapsed_time);

    knowledge.set (".elapsed_time",
      Madara::Knowledge_Record::Integer (elapsed_time));

    knowledge.evaluate (
      ".elapsed_time *= 0.000000001 ;"
      ".elapsed_time > 0 => .throughput = .num_received / .elapsed_time");
  }

  knowledge.print_knowledge ();

  return 0;
}