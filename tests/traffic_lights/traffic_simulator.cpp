
#define ACE_NTRACE    0
//#define ACE_NLOGGING  0
#define ACE_NDEBUG    0

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/Signal.h"

#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/utility/Utility.h"

int id = 0;
int left = 0;
int cars = 1;
int stop = 10;
long value = 0;
std::string mapfile = "/configs/maps/default.map";
std::string looppost_logicfile = "/configs/logics/simulator_looppost.klf";
std::string loopmain_logicfile = "/configs/logics/simulator_loopmain.klf";
std::string trafficlight_logicfile = "/configs/logics/traffic_light.klf";
std::string trafficlight_setupfile = "/configs/logics/traffic_light_setup.klf";
std::string settingsfile = "/configs/settings/default_simulation.klf";
std::string host = "localhost";


volatile bool terminated = 0;


// command line arguments
int parse_args (int argc, ACE_TCHAR * argv[]);

// signal handler for someone hitting control+c
extern "C" void terminate (int)
{
  terminated = true;
}

void parse_map (const std::string & map, Madara::Knowledge_Engine::Knowledge_Base & knowledge,
                std::vector <std::pair <long, long> > & spawn_points,
                std::vector <std::pair <long, long> > & hospitals,
                std::vector <std::pair <long, long> > & traffic_lights)
{
  long x = 0, y = 0;
  long max_x = 0, max_y = 0;

  for (unsigned long i = 0; i < map.size (); ++i)
  {
    knowledge.set (".x", x);
    knowledge.set (".y", y);

    // update the x and y coordinates
    if (map[i] == '\n')
    {
      if (x > max_x)
        max_x = x - 1;

      x = 0;
      ++y;
    }
    else
    {
      if (map[i] == 'T' || map[i] == 't')
      {
        // keep traffic of this traffic light info. We'll use it to build
        // information into the knowledge base about which traffic light
        // is to the north, east, south, and west in the next loop after
        // we are fully informed of all of the traffic lights
        knowledge.evaluate (
"++.light_num;\
.map.{.x}.{.y}.type=.light_type;\
.map.{.x}.{.y}.id=.light_num;\
.light{.light_num}.x=.x;\
.light{.light_num}.y=.y;\
");
      }
      if (map[i] != '\r')
      {
        ++x;
      }
    }
  }

  max_y = y - 1;

  knowledge.set (".max_x", max_x);
  knowledge.set (".max_y", max_y);

  x = 0;
  y = 0;

  ACE_DEBUG ((LM_INFO, "(%P|%t)   Found boundaries ([%d:%d])\n", max_x, max_y));

  for (unsigned long i = 0; i < map.size (); ++i)
  {
    // 
    knowledge.set (".x", x);
    knowledge.set (".y", y);

    //ACE_DEBUG ((LM_INFO, "(%P|%t) Evaluating point ([%d:%d])\n", x, y));
    // update the x and y coordinates
    if (map[i] == '\n')
    {
      x = 0;
      ++y;
    }
    else
    {
      std::pair <long, long> current (x, y);

      if      (map[i] == 'H' || map[i] == 'h')
      {
        hospitals.push_back (current);
        ACE_DEBUG ((LM_INFO, "(%P|%t)   Found hospital ([%d:%d])\n", x, y));
        knowledge.evaluate (".map.{.x}.{.y}.type=.hospital_type");
      }
      else if (map[i] == 'T' || map[i] == 't')
      {
        traffic_lights.push_back (current);
        ACE_DEBUG ((LM_INFO, "(%P|%t)   Found traffic_light ([%d:%d])\n", x, y));
      }
      else if (map[i] == '|' && (y == 0 || y == max_y))
      {
        spawn_points.push_back (current);
        ACE_DEBUG ((LM_INFO, "(%P|%t)   Found spawn point ([%d:%d])\n", x, y));
      }
      else if (map[i] == '_' && (x == 0 || x == max_x))
      {
        spawn_points.push_back (current);
        ACE_DEBUG ((LM_INFO, "(%P|%t)   Found spawn point ([%d:%d])\n", x, y));
      }
      
      // all streets need their types updated
      if (map[i] == '|')
      {
        // it is not a coincidence that phase == 1 is a northsouth green phase
        knowledge.evaluate (".map.{.x}.{.y}.type=.northsouth_road_type");
      }

      // all streets need their types updated
      if (map[i] == '_' || map[i] == '|')
      {
        // it is not a coincidence that phase == 2 is a eastwest green phase
        knowledge.evaluate (".map.{.x}.{.y}.type=.eastwest_road_type");
      }

      // streets AND traffic lights also need information concerning the next
      // traffic light in either direction. This will help us figure out
      // whether or not we have green lights based on the car pos and allows
      // us to use KaRL to look up light info (even reinforcement values)
      if (map[i] == '_' || map[i] == '|' || map[i] == 'T' || map[i] == 't')
      {
        long i = y-1;
        // try to find next light to the north
        for (knowledge.set (".i", i); knowledge.get (".i") >= 0; --i, knowledge.set (".i", i))
        {
          knowledge.evaluate (".map.{.x}.{.i}.type == .light_type => \
                                ( .map.{.x}.{.y}.light.north = .map.{.x}.{.i}.id; \
                                  .i = -1 \
                                )");
        }

        i = y + 1;
        // try to find next light to the south
        for (knowledge.set (".i", i); knowledge.get (".i") < max_y + 1; ++i, knowledge.set (".i", i))
        {
          knowledge.evaluate (".map.{.x}.{.i}.type == .light_type => \
                                ( .map.{.x}.{.y}.light.south = .map.{.x}.{.i}.id; \
                                  .i = .max_y + 1 \
                                )");
        }

        i = x + 1;
        // try to find next light to the east
        for (knowledge.set (".i", i); knowledge.get (".i") < max_x + 1; ++i, knowledge.set (".i", i))
        {
          knowledge.evaluate (".map.{.i}.{.y}.type == .light_type => \
                                ( .map.{.x}.{.y}.light.east = .map.{.i}.{.y}.id; \
                                  .i = .max_x + 1 \
                                )");
        }

        i = x - 1;
        // try to find next light to the west
        for (knowledge.set (".i", i); knowledge.get (".i") >= 0; --i, knowledge.set (".i", i))
        {
          knowledge.evaluate (".map.{.i}.{.y}.type == .light_type => \
                                ( .map.{.x}.{.y}.light.west = .map.{.i}.{.y}.id; \
                                  .i = -1 \
                                )");
        }
      }
      
      if (map[i] != '\r')
        ++x;
    }

  }
  ACE_DEBUG ((LM_INFO, 
    "(%P|%t) Map contains the following properties:\n"));
  ACE_DEBUG ((LM_INFO, 
    "(%P|%t)   Spawn points:    %d\n", spawn_points.size ()));
  ACE_DEBUG ((LM_INFO, 
    "(%P|%t)   Traffic lights:  %d\n", traffic_lights.size ()));
  ACE_DEBUG ((LM_INFO, 
    "(%P|%t)   Hospitals:       %d\n", hospitals.size ()));
}

int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  int retcode = parse_args (argc, argv);

  if (retcode < 0)
    return retcode;

  ACE_LOG_MSG->priority_mask (LM_INFO, ACE_Log_Msg::PROCESS);

  ACE_TRACE (ACE_TEXT ("main"));

  // signal handler for clean exit
  ACE_Sig_Action sa ((ACE_SignalHandler) terminate, SIGINT);

  Madara::Knowledge_Engine::Knowledge_Base knowledge(
    host, Madara::Transport::SPLICE);

  ACE_DEBUG ((LM_INFO, "(%P|%t) Starting traffic simulation...\n" \
    "  logic: %s\n  map: %s\n  settings: %s\n",
    loopmain_logicfile.c_str (), mapfile.c_str (), settingsfile.c_str ()));

  // set my id
  knowledge.set (".id", id);
  knowledge.set (".hospital_type", 20);
  knowledge.set (".light_type", 10);
  knowledge.set (".northsouth_road_type", 1);
  knowledge.set (".eastwest_road_type", 2);

  // time related knowledge values
  // simulation_time = current time step in the simulation

  knowledge.set ("simulation_time", 0);   // 3 minutes (180 seconds)

  std::stringstream main_logic;

  std::string filename = getenv ("MADARA_ROOT");
  filename += mapfile;

  std::string map = Madara::Utility::file_to_string (filename);

  filename = getenv ("MADARA_ROOT");
  filename += loopmain_logicfile;

  std::string loopmain_logic = Madara::Utility::file_to_string (filename);

  filename = getenv ("MADARA_ROOT");
  filename += looppost_logicfile;

  std::string looppost_logic = Madara::Utility::file_to_string (filename);

  filename = getenv ("MADARA_ROOT");
  filename += settingsfile;

  std::string settings = Madara::Utility::file_to_string (filename);

  filename = getenv ("MADARA_ROOT");
  filename += trafficlight_logicfile;

  std::string light_logic = Madara::Utility::file_to_string (filename);

  filename = getenv ("MADARA_ROOT");
  filename += trafficlight_setupfile;

  std::string light_setup = Madara::Utility::file_to_string (filename);

  std::ofstream main_logic_file;

  main_logic_file.open ("ts_main_logic.klf");

  if (main_logic_file.is_open ())
  {
    main_logic_file << loopmain_logic;
    main_logic_file.close ();
  }

  ACE_DEBUG ((LM_INFO, "(%P|%t) Main simulator logic is the following:\n%s\n\n",
    loopmain_logic.c_str ()));

  ACE_DEBUG ((LM_INFO, "(%P|%t) Traffic light logic is the following:\n%s\n\n",
    light_logic.c_str ()));

  ACE_DEBUG ((LM_INFO, "(%P|%t) Map is the following:\n%s\n\n",
    map.c_str ()));

  ACE_DEBUG ((LM_INFO, "(%P|%t) Simulation settings are the following:\n%s\n\n",
    settings.c_str ()));

  knowledge.evaluate (settings);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting Knowledge\n"));
  knowledge.print_knowledge ();

  // this doesn't change from settings
  long spawn_rate = knowledge.get ("spawn_rate");
  long max_speed = knowledge.get ("max_speed");

  // can't have a max_speed that is zero or we'll have
  // floating point exceptions
  if (max_speed == 0)
    max_speed = 1;

  long num_cars = 0;

  std::vector <std::pair <long, long> > spawn_points;
  std::vector <std::pair <long, long> > hospitals;
  std::vector <std::pair <long, long> > traffic_lights;
  
  std::vector <std::vector <long> >     map_lookup;

  parse_map (map, knowledge, spawn_points, hospitals, traffic_lights);

  // setup traffic light defaults (like braking time)
  // note that these setup statements should be setup so that
  // users may override these setup values in the default_simulation.klf file
  for (knowledge.set (".id", 1); 
       knowledge.get (".id") <= knowledge.get (".light_num"); 
       knowledge.evaluate ("++.id"))
  {
    knowledge.evaluate (light_setup);
  }

  // termination is done via signalling from the user (Control+C)
  while (!terminated)
  {
    // run the traffic light logic next

    for (knowledge.set (".id", 1); 
         knowledge.get (".id") <= knowledge.get (".light_num"); 
         knowledge.evaluate ("++.id"))
    {
      knowledge.evaluate (light_logic);
    }

    knowledge.set (".i", 0);
    // run the main logic on the cars that currently exist
    for (knowledge.set (".i", 0); 
           knowledge.get (".i") < knowledge.get (".num_cars"); 
           knowledge.evaluate ("++.i"))
    {
      // we're going to cheat a bit since KaRL does not support
      // for loops yet. Loopmain is going to setup a few variables
      // for us: 
      // .finished = .car{.i}.finish_time;
      //
      // if !.finished, then we set up the following
      // .cur_x = .car{.i}.cur.x;
      // .cur_y = .car{.i}.cur.y;
      // .dest_x = .car{.i}.dest.x;
      // .dest_y = .car{.i}.dest.y;
      // .velocity = .car{.i}.velocity;
  
      // .y_abs_diff = .y_diff = .cur_y - .dest_y;
      // .y_diff < 0 => .y_abs_diff = -.y_diff;
  
      // .x_abs_diff = .x_diff = .cur_x - .dest_x;
      // .x_diff < 0 => .x_abs_diff = -.x_diff;
      knowledge.evaluate (loopmain_logic);

    }

    // spawn new cars
    for (long i = 0; i < spawn_rate; ++i, ++num_cars)
    {
      long destination = rand () % spawn_points.size ();
      long start = rand () % spawn_points.size ();
      long estimated_time = 0;

      // try to handle case where map has just one spawn point gracefully
      // generate a start location until we find a start location that
      // is different from its destination (e.g. it has to move)
      if (spawn_points.size () > 1)
        while (start == destination)
          start = rand () % spawn_points.size ();

      knowledge.set (".cur_start.x", spawn_points[start].first);
      knowledge.set (".cur_start.y", spawn_points[start].second);
      knowledge.set (".cur_destination.x", spawn_points[destination].first);
      knowledge.set (".cur_destination.y", spawn_points[destination].second);

      estimated_time += abs (spawn_points[start].first - spawn_points[destination].first);
      estimated_time += abs (spawn_points[start].second - spawn_points[destination].second);

      estimated_time /= max_speed;

      if (estimated_time % max_speed > 0)
        ++estimated_time;

      knowledge.set (".estimated_time", estimated_time);

      // spawned car setup expression
      std::string expression = "\
 .car{.num_cars}.start.x=.cur_start.x; \
 .car{.num_cars}.start.y=.cur_start.y; \
 .car{.num_cars}.cur.x=.cur_start.x; \
 .car{.num_cars}.cur.y=.cur_start.y; \
 .car{.num_cars}.dest.x=.cur_destination.x; \
 .car{.num_cars}.dest.y=.cur_destination.y; \
 .car{.num_cars}.est_time=.estimated_time; \
 car{.num_cars}.start_time=simulation_time; \
 .car{.num_cars}.velocity=max_speed; \
 ++.num_cars \
";
      knowledge.evaluate (expression);
    }

    // update 
    knowledge.evaluate (looppost_logic);
    knowledge.print("time: {simulation_time} of {simulation_cutoff}\n");

    // cutoff the simulation if we have passed the cutoff
    if (knowledge.get ("simulation_time") >= knowledge.get ("simulation_cutoff"))
    {
      terminated = true;
      knowledge.set ("simulation_finished");
    }

    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Knowledge at time=%d\n", 
      knowledge.get ("simulation_time")));
    knowledge.print_knowledge ();
    ACE_OS::sleep (1);
  }

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Final Knowledge\n"));
  knowledge.print_knowledge ();

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Exiting\n"));
  return 0;
}



int parse_args (int argc, ACE_TCHAR * argv[])
{
  // options string which defines all short args
  ACE_TCHAR options [] = ACE_TEXT ("i:m:s:l:o:h");

  // create an instance of the command line args
  ACE_Get_Opt cmd_opts (argc, argv, options);

  // set up an alias for '-n' to be '--name'
  cmd_opts.long_option (ACE_TEXT ("id"), 'i', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("map"), 'm', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("settings"), 's', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("logic"), 'l', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("help"), 'h', ACE_Get_Opt::NO_ARG);
  cmd_opts.long_option (ACE_TEXT ("host"), 'o', ACE_Get_Opt::ARG_REQUIRED);
 
  // temp for current switched option
  int option;
//  ACE_TCHAR * arg;

  // iterate through the options until done
  while ((option = cmd_opts ()) != EOF)
  {
    //arg = cmd_opts.opt_arg ();
    switch (option)
    {
    case 'i':
      // thread number
      id = atoi (cmd_opts.opt_arg ());
      break;
    case 'm':
      // thread number
      mapfile = cmd_opts.opt_arg ();
      break;
    case 'l':
      // thread number
      loopmain_logicfile = cmd_opts.opt_arg ();
      break;
    case 's':
      // thread number
      settingsfile = cmd_opts.opt_arg ();
      break;
    case 'o':
      host = cmd_opts.opt_arg ();
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) host set to %s\n", host.c_str ()));
      break;
    case ':':
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("ERROR: -%c requires an argument"), 
           cmd_opts.opt_opt ()), -2); 
    case 'h':
    default:
      ACE_DEBUG ((LM_DEBUG, "Program Options:      \n\
      -i (--id)        set process id (0 default)  \n\
      -m (--map)       map file name for traffic lights/hospitals/streets \n\
                       note that this path will start from $MADARA_ROOT \n\
      -l (--logic)     logic file name from $MADARA_ROOT \n\
      -s (--settings)  configuration for simulation from $MADARA_ROOT\n\
      -o (--host)      this host ip/name (localhost default) \n\
      -h (--help)      print this menu             \n"));
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("Returning from Help Menu")), -1); 
      break;
    }
  }

  return 0;
}
