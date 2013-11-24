#include "madara/knowledge_engine/Knowledge_Base.h"
#include <boost/python.hpp>

/**
 * @file Madara_Module.cpp
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains Boost.Python mappings for the C++ MADARA library
 * to a Python module called Madara.
 **/

using namespace boost::python;

class Knowledge_Engine_NS {};
class Transport_NS {};

BOOST_PYTHON_MODULE (madara)
{
  // Declare classes inside Madara namespace (top namespace of Python module)
  
  class_<Madara::Knowledge_Record> ("Knowledge_Record", init <> ())
    
    // integer constructor
    .def (init <int64_t> ())

    .def (init <double> ())

    .def (init <std::vector <int64_t> > ())
    
    .def (init <std::vector <double> > ())

    .def (init <std::string> ())
    
    .def (init <const Madara::Knowledge_Record &> ())

    // clears the value to a 0 integer
    .def ("clear_value", &Madara::Knowledge_Record::clear_value,
        "Sets the value to 0 and type to integer")
    
    // sets the contents of the record to a jpeg
    .def ("read_file", &Madara::Knowledge_Record::read_file,
        "Reads the contents of a file into the record")
    
    // gets the double precision
    .def ("get_precision", &Madara::Knowledge_Record::get_precision,
        "Gets the double precision used in to_string")
    
    // decrements an index of an array
    .def ("dec_index", &Madara::Knowledge_Record::inc_index,
        "Decrements an array element at a particular index")
    
    // increments an index of an array
    .def ("inc_index", &Madara::Knowledge_Record::inc_index,
        "Increments an array element at a particular index")
    
    // retrieves an index of an array
    .def ("retrieve_index", &Madara::Knowledge_Record::retrieve_index,
        "Retrieves an array element at a particular index")
    
    // sets the double precision
    .def ("set_precision", &Madara::Knowledge_Record::set_precision,
        "Sets the double precision, generally for to_string")
    
    // reset the record to UNCREATED
    .def ("reset_value", &Madara::Knowledge_Record::reset_value,
        "Resets the record to an UNCREATED status (faster than clear_value)")
    
    // sets the contents of the record to a jpeg
    .def ("set_jpeg", &Madara::Knowledge_Record::set_jpeg,
        "Sets the value to a jpeg")
    
    // sets the contents of the record to a file
    .def ("set_file", &Madara::Knowledge_Record::set_file,
        "Sets the value to a file's contents")
    
    // sets a knowledge record to a double
    .def( "set",
      static_cast<
        void (Madara::Knowledge_Record::*)(
          const double &
        )
      > (&Madara::Knowledge_Record::set_value),
        "Sets the value to a double") 
        
    // sets a knowledge record to an array of doubles
    .def( "set",
      static_cast<
        void (Madara::Knowledge_Record::*)(
          const std::vector <double> &
        )
      > (&Madara::Knowledge_Record::set_value),
        "Sets the value to an array of doubles") 
        
    // sets a knowledge record to an integer
    .def( "set",
      static_cast<
        void (Madara::Knowledge_Record::*)(
          const Madara::Knowledge_Record::Integer &
        )
      > (&Madara::Knowledge_Record::set_value),
        "Sets the value to an integer") 
        
    // sets a knowledge record to an array of integer
    .def( "set",
      static_cast<
        void (Madara::Knowledge_Record::*)(
          const std::vector <Madara::Knowledge_Record::Integer> &
        )
      > (&Madara::Knowledge_Record::set_value),
        "Sets the value to an array of integers") 
        
    // sets a knowledge record to a string
    .def( "set",
      static_cast<
        void (Madara::Knowledge_Record::*)(
          const std::string &
        )
      > (&Madara::Knowledge_Record::set_value),
        "Sets the value to a string") 
      
    // sets an array index to an integer
    .def( "set_index",
      static_cast<
        void (Madara::Knowledge_Record::*)(
          size_t,
          Madara::Knowledge_Record::Integer)
      > (&Madara::Knowledge_Record::set_index),
        "Sets an array index to an integer") 
        
    // sets an array index to a double
    .def( "set_index",
      static_cast<
        void (Madara::Knowledge_Record::*)(
          size_t,
          double)
      > (&Madara::Knowledge_Record::set_index),
        "Sets an array index to a double (and converts the array to double)") 
        
    // sets the contents of the record to a jpeg
    .def ("size", &Madara::Knowledge_Record::size,
        "Returns the size of the value")
    
    // convert to a string
    .def ("status", &Madara::Knowledge_Record::status,
        "Returns the status of the record")

    // convert to a string
    .def ("to_string", &Madara::Knowledge_Record::to_string,
        "Converts the record to a string")

    // convert to an integer
    .def ("to_integer", &Madara::Knowledge_Record::to_integer,
        "Converts the record to an integer")
    
    // convert to a std::vector of integers
    .def ("to_integers", &Madara::Knowledge_Record::to_integers,
        "Converts the record to an array of integers")

    // convert to a double
    .def ("to_double", &Madara::Knowledge_Record::to_double,
        "Converts the record to a double")
    
    // convert to a std::vector of doubles
    .def ("to_doubles", &Madara::Knowledge_Record::to_doubles,
        "Converts the record to an array of doubles")

    // save value to a file
    .def ("to_file", &Madara::Knowledge_Record::to_file,
        "Saves the value of the record to a file")
    
    // gets the type of the record
    .def ("type", &Madara::Knowledge_Record::type,
        "Returns the value type")
    
    // fragments the record
    .def ("fragment", &Madara::Knowledge_Record::fragment,
        "Fragments the record into components")
    
    // checks if the record is false
    .def ("is_false", &Madara::Knowledge_Record::is_false,
        "Checks if the record is false")
    
    // checks if the record is true
    .def ("is_true", &Madara::Knowledge_Record::is_true,
        "Checks if the record is true")
    
    // checks if record is a binary file type
    .def( "is_binary_file_type",
      static_cast<bool (Madara::Knowledge_Record::*)(void) const> (
        &Madara::Knowledge_Record::is_binary_file_type),
        "Checks if the record is a binary file type") 
        
    // checks if record is a double type
    .def( "is_double_type",
      static_cast<bool (Madara::Knowledge_Record::*)(void) const> (
        &Madara::Knowledge_Record::is_double_type),
        "Checks if the record is a double type") 
        
    // checks if record is a file type
    .def ("is_file_type",
      static_cast<bool (Madara::Knowledge_Record::*)(void) const> (
        &Madara::Knowledge_Record::is_file_type),
        "Checks if the record is a file type") 
        
    // checks if the record is an image type
    .def ("is_image_type",
      static_cast<bool (Madara::Knowledge_Record::*)(void) const> (
        &Madara::Knowledge_Record::is_image_type),
        "Checks if the record is an image type")
      
    // checks if the record is an integer type
    .def ("is_integer_type",
      static_cast<bool (Madara::Knowledge_Record::*)(void) const> (
        &Madara::Knowledge_Record::is_integer_type),
        "Checks if the record is an integer type") 

    // checks if the record is a string type
    .def ("is_string_type",
      static_cast<bool (Madara::Knowledge_Record::*)(void) const> (
        &Madara::Knowledge_Record::is_string_type),
        "Checks if the record is a string type")
      
    // overloaded operators
    .def (self < self)
    .def (self <= self)
    .def (self == double ())
    .def (self == std::string ())
    .def (self == self)
    .def (self != self)
    .def (self > self)
    .def (self >= self)
    .def (!self)
    .def (-self)
    .def (self += self)
    .def (self -= self)
    .def (self *= self)
    .def (self /= self)
    .def (self %= self)
    .def (self * self)
    .def (self / self)
    .def (self % self)
    .def (self + self)
    .def (self - self)

    .def ("operator=", &Madara::Knowledge_Record::operator=,
        "Assigns the value of one record to another")

    .def( "operator==",
      static_cast<bool (Madara::Knowledge_Record::*)(
        Madara::Knowledge_Record::Integer) const> (
          &Madara::Knowledge_Record::operator==),
        "Compares two records for equality") 

    .def( "operator&&",
      static_cast<Madara::Knowledge_Record (Madara::Knowledge_Record::*)(
        const Madara::Knowledge_Record &) const> (
          &Madara::Knowledge_Record::operator&&),
        "Logically and two records together") 

    .def( "operator||",
      static_cast<Madara::Knowledge_Record (Madara::Knowledge_Record::*)(
        const Madara::Knowledge_Record &) const> (
          &Madara::Knowledge_Record::operator||),
        "Logically ors two records together") 

    .def( "operator++",
      static_cast<Madara::Knowledge_Record (Madara::Knowledge_Record::*)(
        void)> (
          &Madara::Knowledge_Record::operator++),
        "Adds one to the record") 

    .def( "operator--",
      static_cast<Madara::Knowledge_Record (Madara::Knowledge_Record::*)(
        void)> (
          &Madara::Knowledge_Record::operator--),
        "Subtracts one from the record") 

    // Boost.python does not appear to support this type of function
    //.def ("to_unmanaged_buffer", &Madara::Knowledge_Record::to_unmanaged_buffer)
  ;
  
  // Declare the Transport namespace
  {
    scope Transport
      = class_ <Transport_NS> ("Transport");

    // the types of packet drop policies for QoS_Transport_Settings class
    enum_<Madara::Transport::PacketDropType>("Drop_Type")
      .value("PACKET_DROP_DETERMINISTIC",
        Madara::Transport::PACKET_DROP_DETERMINISTIC)
      .value("PACKET_DROP_PROBABLISTIC",
        Madara::Transport::PACKET_DROP_PROBABLISTIC)
    ;
    
    // the types of packet drop policies for QoS_Transport_Settings class
    enum_<Madara::Transport::Types>("Transport_Types")
      .value("NO_TRANSPORT",
        Madara::Transport::NO_TRANSPORT)
      .value("SPLICE",
        Madara::Transport::SPLICE)
      .value("NDDS",
        Madara::Transport::NDDS)
      .value("TCP",
        Madara::Transport::TCP)
      .value("UDP",
        Madara::Transport::UDP)
      .value("MULTICAST",
        Madara::Transport::MULTICAST)
      .value("BROADCAST",
        Madara::Transport::BROADCAST)
    ;

    // The base class for Transport Settings
    class_<Madara::Transport::Settings> ("Settings",
      "The main transport settings class", init <> ())
      
      .def (init<const Madara::Transport::Settings &> ())

      // define readwrite variables within the class
      .def_readwrite ("domains",
      	&Madara::Transport::Settings::domains,
        "Informs the transport of the requested knowledge domain")

      .def_readwrite ("queue_length",
      	&Madara::Transport::Settings::queue_length,
        "Informs the transport of the requested queue_length in bytes")

      .def_readwrite ("type",
      	&Madara::Transport::Settings::type,
        "Indicates the type of transport (see Transport_Types)")

      .def_readwrite ("reliability",
      	&Madara::Transport::Settings::reliability,
        "Informs the transport of the requested reliability (0==BEST_EFFORT)")

      .def_readwrite ("id",
      	&Madara::Transport::Settings::id,
        "Informs the transport of the process id")

      .def_readwrite ("processes",
      	&Madara::Transport::Settings::processes,
        "Informs the transport of the number of expected processes")

      .def_readwrite ("on_data_received_logic",
      	&Madara::Transport::Settings::on_data_received_logic,
        "Provides a KaRL expression to evaluate when data is received")

      .def_readwrite ("delay_launch",
      	&Madara::Transport::Settings::delay_launch,
        "Indicates that the transport should delay launch until activated")

      .def_readwrite ("never_exit",
      	&Madara::Transport::Settings::never_exit,
        "Indicates that the transport should never exit, even in bad states")

      .def_readwrite ("send_reduced_message_header",
      	&Madara::Transport::Settings::send_reduced_message_header,
        "Indicates that a reduced message header should be used for messages")

      .def_readwrite ("hosts",
      	&Madara::Transport::Settings::hosts,
        "List of hosts for the transport layer")
    ;
    
    // The Quality-of-Server-enabled Transport Settings class
    class_<Madara::Transport::QoS_Transport_Settings,
           bases<Madara::Transport::Settings> > (
             "QoS_Transport_Settings",
             "Transport settings with quality-of-service focus", init<> ())
      
      .def (init <const Madara::Transport::QoS_Transport_Settings &> ())
      
      .def (init <const Madara::Transport::Settings &> ())

      // update the drop rate
      .def ("update_drop_rate",
        &Madara::Transport::QoS_Transport_Settings::update_drop_rate,
        "Informs the transport of the drop rate, type, and bursts")
        
      // update the drop rate
      .def ("get_drop_rate",
        &Madara::Transport::QoS_Transport_Settings::get_drop_rate,
        "Retrieves the rate of drops (0.8 is 80%)")
        
      // get the drop type
      .def ("get_drop_type",
        &Madara::Transport::QoS_Transport_Settings::get_drop_type,
        "Retrieves the type of drops")
        
      // get the drop burst rate
      .def ("get_drop_burst",
        &Madara::Transport::QoS_Transport_Settings::get_drop_burst,
        "Retrieves the drop bursts")

      // the rebroadcast time-to-live for the transport
      .add_property("participant_ttl",
        &Madara::Transport::QoS_Transport_Settings::get_participant_ttl,
        &Madara::Transport::QoS_Transport_Settings::enable_participant_ttl,
        "Informs the transport that it should participate in rebroadcasting")

      // the rebroadcast time-to-live for the transport
      .add_property("rebroadcast_ttl",
        &Madara::Transport::QoS_Transport_Settings::get_rebroadcast_ttl,
        &Madara::Transport::QoS_Transport_Settings::set_rebroadcast_ttl,
        "Informs the transport of the time-to-live attached to all updates")

      // the send bandwidth for the transport
      .add_property("send_bandwidth",
      &Madara::Transport::QoS_Transport_Settings::get_send_bandwidth_limit,
        &Madara::Transport::QoS_Transport_Settings::set_send_bandwidth_limit,
        "Informs the transport of the send bandwidth limit in B/s that"
        " throttles the sending of new updates")

      // the total bandwidth for the transport
      .add_property("total_bandwidth",
      &Madara::Transport::QoS_Transport_Settings::get_total_bandwidth_limit,
        &Madara::Transport::QoS_Transport_Settings::set_total_bandwidth_limit,
        "Informs the transport of the total bandwidth limit in B/s that"
        " throttles the sending of new updates")

      // the deadline for messages
      .add_property("deadline",
        &Madara::Transport::QoS_Transport_Settings::get_deadline,
        &Madara::Transport::QoS_Transport_Settings::set_deadline,
        "Informs the transport of the deadline, in seconds, for useful info")
    ;
  }

  // Declare the Knowledge namespace
  {
    scope Knowledge_Engine
      = class_ <Knowledge_Engine_NS> ("Knowledge_Engine");
    
    class_<Madara::Knowledge_Engine::Knowledge_Reference_Settings> (
        "Knowledge_Reference_Settings",
        "Settings for referencing knowledge records", init<> ())
        
      .def (init <bool> ())
      .def (init <
        const Madara::Knowledge_Engine::Knowledge_Reference_Settings &> ())

      // define readwrite variables within the class
      .def_readwrite ("expand_variables",
      	&Madara::Knowledge_Engine::Knowledge_Reference_Settings::expand_variables,
        "Always attempt to expand variable names")
      .def_readwrite ("never_exit",
      	&Madara::Knowledge_Engine::Knowledge_Reference_Settings::never_exit,
        "Do not exit, even if in invalid states")

    ; // end class Knowledge_Reference_Settings
          
    class_<Madara::Knowledge_Engine::Knowledge_Update_Settings,
           bases<Madara::Knowledge_Engine::Knowledge_Reference_Settings> > (
             "Knowledge_Update_Settings",
             "Settings for updating knowledge records", init <> ())
    
      // could use the other constructors here

      // define readwrite variables within the class
      .def_readwrite ("treat_globals_as_locals",
      	 &Madara::Knowledge_Engine::Knowledge_Update_Settings::treat_globals_as_locals,
         "Treat global variable changes as if they were local variables")
      .def_readwrite ("signal_changes",
      	 &Madara::Knowledge_Engine::Knowledge_Update_Settings::signal_changes,
         "Always signal on mutation (useful for multi-threaded applications)")
      .def_readwrite ("always_overwrite",
      	 &Madara::Knowledge_Engine::Knowledge_Update_Settings::always_overwrite,
         "Always overwrite records, regardless of quality or consistency checks")
      .def_readwrite ("track_local_changes",
      	 &Madara::Knowledge_Engine::Knowledge_Update_Settings::track_local_changes,
         "Track local changes for checkpointing purposes")
      .def_readwrite ("clock_increment",
      	 &Madara::Knowledge_Engine::Knowledge_Update_Settings::clock_increment,
         "Integer increment for updates to Lamport clocks")
    ; // end class Knowledge_Update_Settings
          
    class_<Madara::Knowledge_Engine::Eval_Settings,
           bases<Madara::Knowledge_Engine::Knowledge_Update_Settings> > (
             "Eval_Settings",
             "Settings for evaluate calls", init <> ())
        
      // define readwrite variables within the class
      .def_readwrite ("delay_sending_modifieds",
      	 &Madara::Knowledge_Engine::Eval_Settings::delay_sending_modifieds,
         "Do not send modifications after an evaluation")
      .def_readwrite ("pre_print_statement",
      	 &Madara::Knowledge_Engine::Eval_Settings::pre_print_statement,
         "Statement to atomically expand and print before an evaluate")
      .def_readwrite ("post_print_statement",
      	 &Madara::Knowledge_Engine::Eval_Settings::post_print_statement,
         "Statement to atomically expand and print after an evaluate")
      .def_readwrite ("send_list",
      	 &Madara::Knowledge_Engine::Eval_Settings::send_list,
         "List of variables that are allowed to be sent, if changed")

    ; // end class Eval_Settings
          
    class_<Madara::Knowledge_Engine::Wait_Settings,
           bases<Madara::Knowledge_Engine::Eval_Settings> > ("Wait_Settings",
           "Settings for wait calls", init <> ())
        
      // define readwrite variables within the class
      .def_readwrite ("poll_frequency",
      	 &Madara::Knowledge_Engine::Wait_Settings::poll_frequency,
         "Frequency, in seconds, to evaluate the expression")
      .def_readwrite ("max_wait_time",
      	 &Madara::Knowledge_Engine::Wait_Settings::max_wait_time,
         "Maximum amount of time to wait for truth")
    ; // end class Wait_Settings


    
    class_<Madara::Knowledge_Engine::Variables> ("Variables", init <> ())
      
      // prints all knowledge variables
      .def ("expand_statement",
        &Madara::Knowledge_Engine::Variables::expand_statement,
        "Expand a statement")
        
      // evaluate an expression
      .def( "print",
        static_cast<
          void (Madara::Knowledge_Engine::Variables::*)(
            unsigned int) const
        > (&Madara::Knowledge_Engine::Variables::print),
        "Prints all knowledge in the context") 
        
      // evaluate an expression
      .def( "print",
        static_cast<
          void (Madara::Knowledge_Engine::Variables::*)(
            const std::string &, unsigned int) const
        > (&Madara::Knowledge_Engine::Variables::print),
        "Prints a statement") 

      // evaluate an expression
      .def( "evaluate",
        static_cast<
          Madara::Knowledge_Record (Madara::Knowledge_Engine::Variables::*)(
            const std::string &,
            const Madara::Knowledge_Engine::Knowledge_Update_Settings &)
        > (&Madara::Knowledge_Engine::Variables::evaluate),
        "Evaluate an expression") 

      // get a knowledge record
      .def( "get",
        static_cast<
          Madara::Knowledge_Record (Madara::Knowledge_Engine::Variables::*)(
            const std::string &,
            const Madara::Knowledge_Engine::Knowledge_Reference_Settings &)
        > (&Madara::Knowledge_Engine::Variables::get),
        "Retrieves a knowledge record") 

      // sets a knowledge record to a double
      .def( "set",
        static_cast<
          int (Madara::Knowledge_Engine::Variables::*)(
            const std::string &,
            double,
            const Madara::Knowledge_Engine::Knowledge_Update_Settings &)
        > (&Madara::Knowledge_Engine::Variables::set),
        "sets a knowledge record to a double") 
        
      // sets a knowledge record to an array of doubles
      .def( "set",
        static_cast<
          int (Madara::Knowledge_Engine::Variables::*)(
            const std::string &,
            const std::vector <double> &,
            const Madara::Knowledge_Engine::Knowledge_Update_Settings &)
        > (&Madara::Knowledge_Engine::Variables::set),
        "sets a knowledge record to an array of doubles") 
        
      // sets a knowledge record to an integer
      .def( "set",
        static_cast<
          int (Madara::Knowledge_Engine::Variables::*)(
            const std::string &,
            Madara::Knowledge_Record::Integer,
            const Madara::Knowledge_Engine::Knowledge_Update_Settings &)
        > (&Madara::Knowledge_Engine::Variables::set),
        "sets a knowledge record to a integer") 
        
      // sets a knowledge record to an array of integer
      .def( "set",
        static_cast<
          int (Madara::Knowledge_Engine::Variables::*)(
            const std::string &,
            const std::vector <Madara::Knowledge_Record::Integer> &,
            const Madara::Knowledge_Engine::Knowledge_Update_Settings &)
        > (&Madara::Knowledge_Engine::Variables::set),
        "sets a knowledge record to an array of integers") 
        
      // sets a knowledge record to a string
      .def( "set",
        static_cast<
          int (Madara::Knowledge_Engine::Variables::*)(
            const std::string &,
            const std::string &,
            const Madara::Knowledge_Engine::Knowledge_Update_Settings &)
        > (&Madara::Knowledge_Engine::Variables::set),
        "sets a knowledge record to a string") 
    ;
    
    class_<Madara::Knowledge_Engine::Function_Arguments> ("Function_Arguments",
      "Arguments passed to a function");

    class_<Madara::Knowledge_Engine::Knowledge_Base> ("Knowledge_Base",
      "Network-enabled, thread-safe knowledge context", init <> ())
      
      // define constructors
      .def (init <const std::string &,
            const Madara::Transport::Settings &> ())
    
      // define constructors
      .def (init <const Madara::Knowledge_Engine::Knowledge_Base &> ())


      // expands a statement with variable expansion
      .def ("expand_statement",
        &Madara::Knowledge_Engine::Knowledge_Base::expand_statement,
        "Expand a statement")
        
      // locks the knowledge base from updates from other threads
      .def ("lock",
        &Madara::Knowledge_Engine::Knowledge_Base::lock,
        "Locks the knowledge base from updates from other threads")


      // prints all knowledge variables
      .def ("print_knowledge",
        &Madara::Knowledge_Engine::Knowledge_Base::print_knowledge,
        "Print all variables in the knowledge base")
        
      // expands and prints a statement
      .def ("print",
        &Madara::Knowledge_Engine::Knowledge_Base::print,
        "Expand and print a statement")

      // evaluate an expression
      .def( "evaluate",
        static_cast<
          Madara::Knowledge_Record (Madara::Knowledge_Engine::Knowledge_Base::*)(
            const std::string &, const Madara::Knowledge_Engine::Eval_Settings &)
        > (&Madara::Knowledge_Engine::Knowledge_Base::evaluate),
        "Evaluate an expression") 

      // wait on an expression
      .def( "wait",
        static_cast<
          Madara::Knowledge_Record (Madara::Knowledge_Engine::Knowledge_Base::*)(
            const std::string &, const Madara::Knowledge_Engine::Wait_Settings &)
        > (&Madara::Knowledge_Engine::Knowledge_Base::wait),
        "Wait for an expression to evaluate to true")

      // get a knowledge record
      .def( "get",
        static_cast<
          Madara::Knowledge_Record (Madara::Knowledge_Engine::Knowledge_Base::*)(
            const std::string &,
            const Madara::Knowledge_Engine::Knowledge_Reference_Settings &)
        > (&Madara::Knowledge_Engine::Knowledge_Base::get),
        "Retrieves a knowledge record") 
        
      // get a knowledge record at an index
      .def( "retrieve_index",
        static_cast<
          Madara::Knowledge_Record (Madara::Knowledge_Engine::Knowledge_Base::*)(
            const std::string & key,
             size_t index,
             const Madara::Knowledge_Engine::Knowledge_Reference_Settings &)
        > (&Madara::Knowledge_Engine::Knowledge_Base::retrieve_index),
        "Retrieves a knowledge record from an index") 

      // sets a knowledge record to a double
      .def( "set",
        static_cast<
          int (Madara::Knowledge_Engine::Knowledge_Base::*)(
            const std::string &,
            double,
            const Madara::Knowledge_Engine::Eval_Settings &)
        > (&Madara::Knowledge_Engine::Knowledge_Base::set),
        "sets a knowledge record to a double") 
        
      // sets a knowledge record to an array of doubles
      .def( "set",
        static_cast<
          int (Madara::Knowledge_Engine::Knowledge_Base::*)(
            const std::string &,
            const std::vector <double> &,
            const Madara::Knowledge_Engine::Eval_Settings &)
        > (&Madara::Knowledge_Engine::Knowledge_Base::set),
        "sets a knowledge record to an array of doubles") 
        
      // sets a knowledge record to an integer
      .def( "set",
        static_cast<
          int (Madara::Knowledge_Engine::Knowledge_Base::*)(
            const std::string &,
            Madara::Knowledge_Record::Integer,
            const Madara::Knowledge_Engine::Eval_Settings &)
        > (&Madara::Knowledge_Engine::Knowledge_Base::set),
        "sets a knowledge record to a integer") 
        
      // sets a knowledge record to an array of integer
      .def( "set",
        static_cast<
          int (Madara::Knowledge_Engine::Knowledge_Base::*)(
            const std::string &,
            const std::vector <Madara::Knowledge_Record::Integer> &,
            const Madara::Knowledge_Engine::Eval_Settings &)
        > (&Madara::Knowledge_Engine::Knowledge_Base::set),
        "sets a knowledge record to an array of integers") 
        
      // sets a knowledge record to a string
      .def( "set",
        static_cast<
          int (Madara::Knowledge_Engine::Knowledge_Base::*)(
            const std::string &,
            const std::string &,
            const Madara::Knowledge_Engine::Eval_Settings &)
        > (&Madara::Knowledge_Engine::Knowledge_Base::set),
        "sets a knowledge record to a string") 
        
      // sets an array index to an integer
      .def( "set_index",
        static_cast<
          int (Madara::Knowledge_Engine::Knowledge_Base::*)(
            const std::string &,
            size_t,
            Madara::Knowledge_Record::Integer,
            const Madara::Knowledge_Engine::Eval_Settings &)
        > (&Madara::Knowledge_Engine::Knowledge_Base::set_index),
        "sets an array index to an integer") 
        
      // sets an array index to a double
      .def( "set_index",
        static_cast<
          int (Madara::Knowledge_Engine::Knowledge_Base::*)(
            const std::string &,
            size_t,
            double,
            const Madara::Knowledge_Engine::Eval_Settings &)
        > (&Madara::Knowledge_Engine::Knowledge_Base::set_index),
        "sets an array index to a double") 
      
      // sets an array index to a double
      .def( "define_function",
        static_cast<
          void (Madara::Knowledge_Engine::Knowledge_Base::*)(
            const std::string &,
            Madara::Knowledge_Record (*func) (
              Madara::Knowledge_Engine::Function_Arguments &,
              Madara::Knowledge_Engine::Variables &))
        > (&Madara::Knowledge_Engine::Knowledge_Base::define_function),
        "defines an unnamed function that can be called within evaluates")
        
      // unlocks the knowledge base
      .def ("unlock",
        &Madara::Knowledge_Engine::Knowledge_Base::unlock,
        "Unlocks the knowledge base and allows other threads to access")

    ;

  }
}
