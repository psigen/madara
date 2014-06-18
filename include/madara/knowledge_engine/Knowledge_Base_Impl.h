#ifndef _MADARA_KNOWLEDGE_BASE_IMPL_H
#define _MADARA_KNOWLEDGE_BASE_IMPL_H

/**
 * @file Knowledge_Base.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains the Knowledge_Base class
 */

#include <string>
#include <map>

#include <ostream>
#include <vector>

#include "madara/knowledge_engine/Compiled_Expression.h"
#include "madara/knowledge_engine/Wait_Settings.h"
#include "madara/knowledge_engine/Knowledge_Update_Settings.h"
#include "madara/knowledge_engine/Knowledge_Record.h"
#include "madara/knowledge_engine/Variable_Reference.h"
#include "madara/MADARA_export.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/knowledge_engine/Files.h"
#include "madara/transport/Transport.h"
#include "madara/expression_tree/Interpreter.h"
#include "ace/SOCK_Dgram.h"

// declare ACE class so MADARA user will not have to directly link to ACE
class ACE_SOCK_Acceptor;

namespace Madara
{

  namespace Knowledge_Engine
  {

    /**
     * @class Knowledge_Base_Impl
     * @brief This class provides a distributed knowledge base implementation
     **/
    class Knowledge_Base_Impl
    {
    public:
      /**
       * Constructor
       **/
      Knowledge_Base_Impl ();

      /**
       * Constructor
       * @param   host      hostname/ip of this machine
       * @param   transport transport to use for knowledge dissemination
       **/
      Knowledge_Base_Impl (const std::string & host, int transport);

      /**
       * Constructor
       * @param   host      hostname/ip of this machine
       * @param   transport transport to use for knowledge dissemination
       * @param   domain  knowledge realm we want to join
       **/
      Knowledge_Base_Impl (const std::string & host, int transport,
        const std::string & domain);

      /**
       * Constructor
       * @param   host      hostname/ip of this machine
       * @param   config    transport settings to use for dissemination
       **/
      Knowledge_Base_Impl (const std::string & host, 
        const Madara::Transport::Settings & config);

      /**
       * Destructor
       **/
      ~Knowledge_Base_Impl ();
      
      /**
       * Locks the context to prevent updates over the network
       **/
      void lock (void);
      
      /**
       * Unlocks the context to allow updates over the network (is only
       * necessary if the context has been explicitly locked)
       **/
      void unlock (void);

      /**
       * Starts the transport mechanism for dissemination if it is closed
       **/
      void activate_transport (void);

      /**
       * Closes the transport mechanism so no dissemination is possible
       **/
      void close_transport (void);

      /**
       * Retrieves a knowledge value
       * @param key                knowledge location
       * @param settings           settings for referring to knowledge variables
       * @return                   value at knowledge location
       **/
      Madara::Knowledge_Record get (const std::string & key,
             const Knowledge_Reference_Settings & settings =
                     Knowledge_Reference_Settings ());
      
      /**
       * Atomically returns the value of a variable.
       * @param   variable  reference to a variable (@see get_ref)
       * @param   settings  the settings for referring to variables
       * @return         the Madara::Knowledge_Record::Integer value for the variable
       **/
      Madara::Knowledge_Record
        get (const Variable_Reference & variable,
             const Knowledge_Reference_Settings & settings =
                     Knowledge_Reference_Settings ());
      
      /**
       * Atomically returns a reference to the variable. Variable references are
       * efficient mechanisms for reference variables individually--similar to
       * speedups seen from Compiled_Expression.
       * @param   key       unique identifier of the variable
       * @param settings         settings for referring to knowledge variables
       * @return            reference to the variable in the context
       **/
      Variable_Reference
        get_ref (const std::string & key,
             const Knowledge_Reference_Settings & settings =
                     Knowledge_Reference_Settings ());
      
      /**
       * Retrieves a value at a specified index within a knowledge array
       * @param key              knowledge location
       * @param index            index within the array
       * @param settings         settings for referring to knowledge variables
       * @return                 value at knowledge location
       **/
      Madara::Knowledge_Record retrieve_index (const std::string & key,
             size_t index,
             const Knowledge_Reference_Settings & settings =
                     Knowledge_Reference_Settings ());
      
      /**
       * Retrieves a value at a specified index within a knowledge array
       * @param   variable   reference to a variable (@see get_ref)
       * @param   index      index within the array
       * @param   settings   settings for referring to knowledge variables
       * @return             value at knowledge location
       **/
      Madara::Knowledge_Record retrieve_index (
             const Variable_Reference & variable,
             size_t index,
             const Knowledge_Reference_Settings & settings =
                     Knowledge_Reference_Settings ());

      /**
       * Expands a statement using variable expansion. For example, if the
       * statement were MyKnowledge.{.id}, and .id==1, then the statement
       * would be expanded to MyKnowledge.1
       *
       * @param statement          statement to expand
       * @return                   expanded statement
       **/
      std::string expand_statement (const std::string & statement) const;

      /**
       * Compiles a KaRL expression into an expression tree
       *
       * @param expression         expression to compile
       * @return                   compiled, optimized expression tree
       **/
      Compiled_Expression
        compile (const std::string & expression);

      /**
       * Read a file into the knowledge base
       * @param filename           file to read
       * @param knowledge_key      key to store the file into
       * @param settings           settings for updating/evaluating
       * @return   -1 if unsuccessful, 0 otherwise
       */
      int read_file (const std::string & knowledge_key, 
                     const std::string & filename, 
        const Eval_Settings & settings =
          Eval_Settings ());

      /**
       * Write a file from the knowledge base to a specified location
       * @param filename           file to write to
       * @param knowledge_key       key to read the file from
       * @return   -1 if unsuccessful, 0 otherwise
       */
      ssize_t write_file (const std::string & knowledge_key, 
                      const std::string & filename);

      /**
       * Read a policy into the knowledge base
       * @param policy_key          key to read the policy into
       * @param policy_file         file to read the policy from
       */
      int read_policy (const std::string & policy_key, 
                       const std::string & policy_file);
#ifdef _USE_CID_

      /**
       * Print all redeployment algorithm results
       **/
      void print_all_redeployment_results (std::ostream & output);

      /**
       * Run all redeployment algorithms specified in Transport Settings,
       * within the latencies.algorithm_configs variable.
       **/
      void run_all (void);
      
      /**
       * Starts a latency round
       * @return  result of operation or -1 if we are shutting down
       **/
      long start_latency (void);

      /**
       * Prints latencies associated with this process id
       * @param   output    stream to print latencies to
       **/
      void print_my_latencies (std::ostream & output);

      /**
       * Prints all latencies in the context for all processes
       * @param   output    stream to print latencies to
       **/
      void print_all_latencies (std::ostream & output);

      /**
       * Prints all summations in the context for all processes
       * @param   output    stream to print summations to
       **/
      void print_all_summations (std::ostream & output);
      
      /**
       * Votes with the lowest algorithm results current in the Transport settings.
       * This is only useful if latency collection is enabled and run_all has been
       * populated with useful redeployment algorithms before being ran. See the
       * Madara::Transport::Settings class for all voting parameters.
       **/
      long vote (void);

#endif // _USE_CID_

      /**
       * Sets a knowledge value to a specified value
       *
       * @param key             knowledge variable location
       * @param value           value to set at location
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (const std::string & key, Madara::Knowledge_Record::Integer value, 
        const Eval_Settings & settings);
      
      /**
       * Sets an index within an array to a specified value
       *
       * @param key             knowledge variable location
       * @param index           index of the location in the array
       * @param value           value to set at location
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set_index (const std::string & key,
        size_t index,
        Knowledge_Record::Integer value,
        const Eval_Settings & settings =
          Eval_Settings ());

      /**
       * Sets a knowledge value to a specified value
       *
       * @param key             knowledge variable location
       * @param value           array of integers to set to this location
       * @param size            number of elements in the array
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (const std::string & key,
        const Knowledge_Record::Integer * value,
        uint32_t size,
        const Eval_Settings & settings = Eval_Settings ());
      
      /**
       * Sets a knowledge value to a specified value
       *
       * @param key             knowledge variable location
       * @param value           array of integers to set to this location
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (const std::string & key,
        const std::vector <Knowledge_Record::Integer> & value,
        const Eval_Settings & settings = Eval_Settings ());
      
      /**
       * Sets a knowledge value to a specified value
       *
       * @param key             knowledge variable location
       * @param value           value to set at location
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (const std::string & key, double value, 
        const Eval_Settings & settings);
      
      /**
       * Sets an index within an array to a specified value
       *
       * @param key             knowledge variable location
       * @param index           index of the location in the array
       * @param value           value to set at location
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set_index (const std::string & key,
        size_t index,
        double value,
        const Eval_Settings & settings =
          Eval_Settings ());
      
      /**
       * Atomically reads a file into a variable
       * @param   variable  reference to a variable (@see get_ref)
       * @param   filename  file to read
       * @param   settings  settings for applying the update
       * @return   0 if the value was set. -1 if unsuccessful
       **/
      int
      read_file (
        const Variable_Reference & variable,
        const std::string & filename,
        const Eval_Settings & settings =
          Eval_Settings ());
      
      /**
       * Atomically sets the value of a variable to a string
       * @param   variable  reference to a variable (@see get_ref)
       * @param   value     new value of the variable
       * @param   settings  settings for applying the update
       * @return   0 if the value was set. -1 if null key
       **/
      int set (const Variable_Reference & variable,
        const std::string & value,
        const Eval_Settings & settings =
          Eval_Settings ());
      
      /**
       * Atomically sets the value of a variable to a double array.
       * @param   variable  reference to a variable (@see get_ref)
       * @param   value     a STL vector of doubles
       * @param   settings  settings for applying the update
       * @return   0 if the value was set. -1 if null key
       **/
      int set (const Variable_Reference & variable,
        const std::vector <double> & value,
        const Eval_Settings & settings =
          Eval_Settings ());
      
      /**
       * Atomically sets the value of a variable to a double array.
       * @param   variable  reference to a variable (@see get_ref)
       * @param   value     an array of doubles
       * @param   size      number of elements in the array
       * @param   settings  settings for applying the update
       * @return   0 if the value was set. -1 if null key
       **/
      int set (const Variable_Reference & variable,
        const double * value,
        uint32_t size,
        const Eval_Settings & settings =
          Eval_Settings ());
      
      /**
       * Atomically sets the value of an array index to a double.
       * @param   variable  reference to a variable (@see get_ref)
       * @param   index     index within array
       * @param   value     new value of the array index
       * @param   settings  settings for applying the update
       * @return   0 if the value was set. -1 if null key
       **/
      int set_index (const Variable_Reference & variable,
        size_t index, double value,
        const Eval_Settings & settings =
          Eval_Settings ());
      
      /**
       * Atomically sets the value of a variable to a double.
       * @param   variable  reference to a variable (@see get_ref)
       * @param   value     new value of the variable
       * @param   settings  settings for applying the update
       * @return   0 if the value was set. -1 if null key
       **/
      int set (const Variable_Reference & variable,
        double value,
        const Eval_Settings & settings =
          Eval_Settings ());
 
      /**
       * Atomically sets the value of a variable to an integer array.
       * @param   variable  reference to a variable (@see get_ref)
       * @param   value     a STL vector of Integers
       * @param   settings  settings for applying the update
       * @return   0 if the value was set. -1 if null key
       **/
      int set (const Variable_Reference & variable,
        const std::vector <Knowledge_Record::Integer> & value,
        const Eval_Settings & settings =
          Eval_Settings ());
      
      /**
       * Atomically sets the value of a variable to an integer array.
       * @param   variable  reference to a variable (@see get_ref)
       * @param   value     an array of Integers
       * @param   size      number of elements in the array
       * @param   settings  settings for applying the update
       * @return   0 if the value was set. -1 if null key
       **/
      int set (const Variable_Reference & variable,
        const Madara::Knowledge_Record::Integer * value,
        uint32_t size,
        const Eval_Settings & settings =
          Eval_Settings ());

      /**
       * Atomically sets the value of an array index to a double.
       * @param   variable  reference to a variable (@see get_ref)
       * @param   index     index within array
       * @param   value     new value of the array index
       * @param   settings  settings for applying the update
       * @return   0 if the value was set. -1 if null key
       **/
      int set_index (const Variable_Reference & variable,
        size_t index, Knowledge_Record::Integer value,
        const Eval_Settings & settings =
          Eval_Settings ());
      
      /**
       * Atomically sets the value of a variable to an integer.
       * @param   variable  reference to a variable (@see get_ref)
       * @param   value     new value of the variable
       * @param   settings  settings for applying the update
       * @return   0 if the value was set. -1 if null key
       **/
      int set (const Variable_Reference & variable,
        Madara::Knowledge_Record::Integer value, 
        const Eval_Settings & settings =
          Eval_Settings ());
      
      /**
       * Atomically sets the value of a variable to a text file's contents.
       * @param   variable  reference to a variable (@see get_ref)
       * @param   value     new value of the variable
       * @param   size      indicates the size of the value buffer
       * @param   settings  settings for applying the update
       * @return   0 if the value was set. -1 if null key
       **/
      int set_text (const Variable_Reference & variable,
        const char * value, size_t size, 
        const Eval_Settings & settings =
          Eval_Settings ());
      
      /**
       * Atomically sets the value of a variable to the contents of a file.
       * @param   variable  reference to a variable (@see get_ref)
       * @param   value     new value of the variable
       * @param   size      indicates the size of the value buffer
       * @param   settings  settings for applying the update
       * @return   0 if the value was set. -1 if null key
       **/
      int set_file (const Variable_Reference & variable,
        const unsigned char * value, size_t size, 
        const Eval_Settings & settings =
          Eval_Settings ());
      
      /**
       * Atomically sets the value of a variable to a JPEG image
       * @param   variable  reference to a variable (@see get_ref)
       * @param   value     new value of the variable
       * @param   size      indicates the size of the value buffer
       * @param   settings  settings for applying the update
       * @return   0 if the value was set. -1 if null key
       **/
      int set_jpeg (const Variable_Reference & variable,
        const unsigned char * value, size_t size, 
        const Eval_Settings & settings =
          Eval_Settings ());
      
      /**
       * Atomically sets the value of a variable to an XML string.
       * @param   variable  reference to a variable (@see get_ref)
       * @param   value     new value of the variable
       * @param   size      indicates the size of the value buffer
       * @param   settings  settings for applying the update
       * @return   0 if the value was set. -1 if null key
       **/
      int set_xml (const Variable_Reference & variable,
        const char * value, size_t size, 
        const Eval_Settings & settings =
          Eval_Settings ());
      
      /**
       * Sets a knowledge value to a specified value
       *
       * @param key             knowledge variable location
       * @param value           array of doubles to set to this location
       * @param size            number of elements in the array
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (const std::string & key,
        const double * value,
        uint32_t size,
        const Eval_Settings & settings = Eval_Settings ());
      
      /**
       * Sets a knowledge value to a specified value
       *
       * @param key             knowledge variable location
       * @param value           array of doubles to set to this location
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (const std::string & key,
        const std::vector <double> & value,
        const Eval_Settings & settings = Eval_Settings ());
      
      /**
       * Sets a knowledge value to a specified value
       *
       * @param key             knowledge variable location
       * @param value           value to set at location
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (const std::string & key, const std::string & value, 
        const Eval_Settings & settings);

      /**
       * Returns a non-const reference to the Transport Settings
       * @return  settings reference
       **/
      Madara::Transport::Settings & transport_settings (void);

      /**
       * Applies current time and modified to all global variables and tries
       * to send them.
       * @param settings        Settings for evaluating and printing
       * @return 0 if there was not a problem.
       **/
      int apply_modified (const Eval_Settings & settings =
          Eval_Settings ());

      /**
       * Checks if a knowledge location exists in the context
       *
       * @param key             knowledge variable location
       * @param settings        settings for referring to knowledge variables
       * @return                true if location has been set
       **/
      bool exists (const std::string & key,
        const Knowledge_Reference_Settings & settings =
          Knowledge_Reference_Settings ()) const;
      
      /**
       * Checks if a knowledge variable exists in the context
       *
       * @param variable        knowledge variable reference
       * @param settings        settings for referring to knowledge variables
       * @return                true if location has been set
       **/
      bool exists (const Variable_Reference & variable,
        const Knowledge_Reference_Settings & settings =
          Knowledge_Reference_Settings (false)) const;

      /**
       * Sets the quality of writing to a certain variable from this entity
       *
       * @param key             knowledge variable location
       * @param quality         quality of writing to this location
       * @param settings        settings for referring to knowledge variables
       **/
      void set_quality (const std::string & key, uint32_t quality,
             const Knowledge_Reference_Settings & settings =
               Knowledge_Reference_Settings ());

      /**
       * Evaluates an expression. Always disseminates modifications.
       *
       * @param expression      KaRL expression to evaluate
       * @return                value of expression
       **/
      Madara::Knowledge_Record evaluate (const std::string & expression);
      
      /**
       * Evaluates an expression
       *
       * @param expression      KaRL expression to evaluate
       * @param settings        Settings for evaluating and printing
       * @return                value of expression
       **/
      Madara::Knowledge_Record evaluate (
        const std::string & expression,
        const Eval_Settings & settings);

      /**
       * Evaluates an expression
       *
       * @param expression      KaRL expression to wait on (result of compile)
       * @param settings        Settings for evaluating and printing
       * @return                value of expression
       **/
      Madara::Knowledge_Record evaluate (
        Compiled_Expression & expression,
        const Eval_Settings & settings);
      
      /**
       * Evaluates a root-based tree (result of compile)
       *
       * @param root            root-based tree
       * @param settings        Settings for evaluating and printing
       * @return                value of expression
       **/
      Madara::Knowledge_Record evaluate (
        Expression_Tree::Component_Node * root,
        const Eval_Settings & settings =
          Eval_Settings ());

      /**
       * Waits for an expression to be non-zero.
       * Always disseminates modifications.
       *
       * @param expression      KaRL expression to wait on
       * @return                value of expression
       **/
      Madara::Knowledge_Record wait (const std::string & expression);
      
      /**
       * Waits for an expression to be non-zero. Provides additional settings
       * for fine-tuning the time to wait and atomic print statements.
       *
       * @param expression      KaRL expression to wait on
       * @param settings        Settings for the underlying expression
       *                        evaluation and printing
       * @return                value of expression
       **/
      Madara::Knowledge_Record wait (const std::string & expression,
        const Wait_Settings & settings);

      /**
       * Waits for an expression to be non-zero. Provides additional settings
       * for fine-tuning the time to wait and atomic print statements.
       *
       * @param expression      KaRL expression to wait on (result of compile)
       * @param settings        Settings for the underlying expression
       *                        evaluation and printing
       * @return                value of expression
       **/
      Madara::Knowledge_Record wait (Compiled_Expression & expression,
        const Wait_Settings & settings);

      /**
       * Prints all knowledge variables and values in the context
       * @param   level    level to log the knowledge at
       **/
      void print (unsigned int level) const;

      /**
       * Expands and prints a user provided statement at a logging level.
       * For example, if the statement were MyKnowledge.{.id}, and .id==1,
       * then the statement would be expanded to MyKnowledge.1
       * 
       * @param   statement  statement to expand with variable values
       * @param   level      level to log the knowledge at
       **/
      void print (const std::string & statement, unsigned int level) const;

      /**
       * Clears the knowledge base
       **/
      void clear (void);

      /**
       * Clears the knowledge base
       **/
      void clear_map (void);

      /**
       * Clears the permanent knowledge rules (unimplemented)
       **/
      void clear_rules (void);

      /**
       * Acquires the recursive lock on the knowledge base. This will
       * block any other thread from updating or using the knowledge
       * base until you call @ release.
       **/
      void acquire (void);

      /**
       * Releases a recursive lock on the knowledge base. This will
       * allow other thread to access the knowledge base if you had
       * previously called @ acquire.
       **/
      void release (void);
      
      /**
       * Defines a function
       * @param  name       name of the function
       * @param  func       external function to call with this name
       **/
      void define_function (const std::string & name,
        Knowledge_Record (*func) (Function_Arguments &, Variables &));
      
      /**
       * Defines a named function that can distinguish the name it was called
       * with in MADARA
       * @param  name       name of the function
       * @param  func       external function to call with this name
       **/
      void define_function (const std::string & name,
        Knowledge_Record (*func) (const char *, Function_Arguments &, Variables &));
      
#ifdef _MADARA_JAVA_
      /**
       * Defines a named python function
       * @param  name       name of the function
       * @param  callable   external java object to call with this name
       **/
      void define_function (const std::string & name, jobject callable);
#endif
      
#ifdef _MADARA_PYTHON_CALLBACKS_
      /**
       * Defines a named python function
       * @param  name       name of the function
       * @param  callable   external python function to call with this name
       **/
      void define_function (const std::string & name, boost::python::object callable);
#endif

      /**
       * Defines a MADARA KaRL function
       * @param  name       name of the function
       * @param  expression KaRL function body       
       **/
      void define_function (const std::string & name,
        const std::string & expression);
      
      /**
       * Defines a MADARA KaRL function
       * @param  name       name of the function
       * @param  expression KaRL function body       
       **/
      void define_function (const std::string & name,
        const Compiled_Expression & expression);

      /**
       * Attaches a transport to the Knowledge Engine. Note that the
       * transport should use the same Thread_Safe_Context as the
       * Knowledge Engine.
       * @param  transport   a new transport to attach tot he Knowledge Base
       * @return             the number of transports now attached
       **/
      size_t attach_transport (Madara::Transport::Base * transport);
      
      /**
       * Adds a built-in transport with the specified settings
       * @param  id          unique identifier for this agent (empty string
       *                     will use default generated id)
       * @param  settings    settings for the new transport
       * @return             the number of transports now attached
       **/
      size_t attach_transport (const std::string & id,
        Transport::Settings & settings);
      
      /**
       * Removes a transport
       * @param   index      index of the transport to remove. If invalid,
       *                     nothing is removed.
       * @return             the size of transports now attached
       **/
      size_t remove_transport (size_t index);

      /**
       * Returns the Thread_Safe_Context associated with this Knowledge
       * Base. This is necessary for creating custom transports.
       *
       * @return             the context used by the knowledge base
       **/
      Thread_Safe_Context & get_context (void);

      /**
       * Returns the unique host and ephemeral binding for this Knowlede Base
       * @return             host:port identifier for this knowledge base
       **/
      std::string get_id (void);
      
      /**
       * Fills a vector with Knowledge Records that begin with a common subject
       * and have a finite range of integer values.
       * @param   subject     The common subject of the variable names. For
       *                      instance, if we are looking for a range of vars
       *                      like "var0", "var1", "var2", then the common
       *                      subject would be "var".
       * @param   start       An inclusive start index
       * @param   end         An inclusive end index
       * @param   target      The vector that will be filled with
       *                      Knowledge Record instances within the subject
       *                      range.
       * @return              entries in the resulting vector
       **/
      size_t to_vector (const std::string & subject,
                              unsigned int start,
                              unsigned int end,
                              std::vector <Knowledge_Record> & target);
      
      /**
       * Fills a variable map with Knowledge Records that match an expression.
       * At the moment, this expression must be of the form "subject*"
       * @param   expression  An expression that matches the variable names
       *                      that are of interest. Wildcards may only be
       *                      at the end.
       * @param   target      The map that will be filled with variable names
       *                      and the Knowledge Records they correspond to
       * @return              entries in the resulting map
       **/
      size_t to_map    (const std::string & subject,
                       std::map <std::string, Knowledge_Record> & target);
      
      /**
       * Saves the context to a file
       * @param   filename    name of the file to open
       * @return  total bytes written
       **/
      int64_t save_context (const std::string & filename);
      
      /**
       * Saves a checkpoint of a list of changes to a file
       * @param   filename    name of the file to open
       * @param   reset_modifieds  if true, resets the modified list to empty.
       * @return  total bytes written
       **/

      int64_t save_checkpoint (const std::string & filename,
        bool reset_modifieds = true);
      
      /**
       * Loads the context from a file
       * @param   filename    name of the file to open
       * @param   use_id      if true, sets the unique identifier to the
       *                      one found in the saved context. If false,
       *                      keeps the default identifier.
       * @param   settings    settings for modifying context
       * @return  total bytes read
       **/
      int64_t load_context (const std::string & filename,
        bool use_id,
        const Knowledge_Update_Settings & settings = 
              Knowledge_Update_Settings (true, true, true, false));
      
      /**
       * Sends all modified variables through the attached transports.
       * @param   prefix      for logging purposes, the descriptor prefix for 
       *                      calling method
       * @param   settings    settings for sending modifications
       * @return  number of transports the modifications were sent to
       **/
      int send_modifieds (
        const std::string & prefix,
        const Eval_Settings & settings =
          Eval_Settings ());
      
      /**
       * Wait for a change to happen to the context (e.g., from transports)
       **/
      void wait_for_change (void);
      
      /**
       * Binds to an ephemeral port for unique tie breakers in global ordering.
       * Call this function if you want to generate a unique id for a
       * custom transport to use (or a built-in transport if you are creating
       * one outside of the Knowledge_Base.
       * @param  host   unique host identifier string ("" if requesting a
       *                generated id.
       * @return        unique host id (same as host param if host is not "")
       **/
      std::string setup_unique_hostport (const std::string & host = "");

    private:
      Thread_Safe_Context                 map_;
      ACE_SOCK_Dgram                      unique_bind_;
      std::string                         id_;
      Transport::QoS_Transport_Settings   settings_;
      Files                               files_;

      Madara::Transport::Transports transports_;
    };
  }
}
    
// include the inline functions
#include "Knowledge_Base_Impl.inl"

#endif  // _MADARA_KNOWLEDGE_BASE_IMPL_H
