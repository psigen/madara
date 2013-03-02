#ifndef _MADARA_KNOWLEDGE_BASE_H
#define _MADARA_KNOWLEDGE_BASE_H

/**
 * @file Knowledge_Base.h
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains the Knowledge_Base class
 */

#include "madara/knowledge_engine/Knowledge_Record.h"
#include "madara/knowledge_engine/Knowledge_Update_Settings.h"
#include "madara/knowledge_engine/Functions.h"
#include "madara/MADARA_export.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/transport/Transport.h"
#include "madara/expression_tree/Interpreter.h"
#include "madara/utility/Refcounter.h"
#include "madara/knowledge_engine/Knowledge_Base_Impl.h"
#include "madara/knowledge_engine/Compiled_Expression.h"

#include <ostream>

namespace Madara
{

  namespace Knowledge_Engine
  {

    /**
     * @class Knowledge_Base
     * @brief This class provides a distributed knowledge base to users
     */
    class MADARA_Export Knowledge_Base
    {
    public:
      /// Transport type enumeration
      enum {
        NO_TRANSPORT = 0,
        OPEN_SPLICE_TRANSPORT = 1,
        TCP_TRANSPORT = 2
      };

      /**
       * Constructor
       **/
      Knowledge_Base ();

      /**
       * Constructor
       * @param   host      hostname/ip of this machine
       * @param   transport transport to use for knowledge dissemination
       **/
      Knowledge_Base (const std::string & host, int transport);

      /**
       * Constructor
       * @param   host      hostname/ip of this machine
       * @param   transport transport to use for knowledge dissemination
       * @param   domain    knowledge domain we want to join
       **/
      Knowledge_Base (const std::string & host, int transport,
        const std::string & domain);

      /**
       * Constructor
       * @param   host      hostname/ip of this machine
       * @param   config    transport settings to use for dissemination
       **/
      Knowledge_Base (const std::string & host, 
        const Madara::Transport::Settings & config);

      /**
       * Copy constructor
       * @param   original  knowledge base to copy
       **/
      Knowledge_Base (const Knowledge_Base & original);

      /**
       * Destructor
       **/
      ~Knowledge_Base ();

      /**
       * Assigns another instance's knowledge and settings to this instance
       * @param   original  knowledge base to copy
       **/
      void operator= (const Knowledge_Base & original);

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
       * @return                   value at knowledge location
       **/
      Madara::Knowledge_Record get (const std::string & key);

      /**
       * Read a file into the knowledge base
       * @param file_name           file to read
       * @param knowledge_key       key to store the file into
       */
      int read_file (const std::string & knowledge_key, 
                     const std::string & file_name);

      /**
       * Write a file from the knowledge base to a specified location
       * @param file_name           file to write to
       * @param knowledge_key       key to read the file from
       */
      int write_file (const std::string & knowledge_key, 
                      const std::string & file_name);

      /**
       * Read a policy into the knowledge base
       * @param policy_key          key to read the policy into
       * @param policy_file         file to read the policy from
       */
      int read_policy (const std::string & policy_key, 
                       const std::string & policy_file);

      /**
       * Expands a statement using variable expansion. For example, if the
       * statement were MyKnowledge.{.id}, and .id==1, then the statement
       * would be expanded to MyKnowledge.1
       *
       * @param statement          statement to expand
       * @return                   expanded statement
       **/
      std::string expand_statement (const std::string & statement);

      /**
       * Compiles a KaRL expression into an expression tree
       *
       * @param expression         expression to compile
       * @return                   compiled, optimized expression tree
       **/
      Compiled_Expression
        compile (const std::string & expression);

      /**
       * Sets a knowledge value to a specified value
       *
       * @param key             knowledge variable location
       * @param value           value to set at location
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (const std::string & key,
        Madara::Knowledge_Record::Integer value = 
          Madara::Knowledge_Record::MODIFIED, 
        const Eval_Settings & settings = DEFAULT_EVAL_SETTINGS);
       
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
        const Eval_Settings & settings = DEFAULT_EVAL_SETTINGS);
      
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
        const Eval_Settings & settings = DEFAULT_EVAL_SETTINGS);

      /**
       * Sets the quality of writing to a certain variable from this entity
       *
       * @param key             knowledge variable location
       * @param quality         quality of writing to this location
       **/
      void set_quality (const std::string & key, uint32_t quality);

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
       * Returns a non-const reference to the Transport Settings
       * @return  settings reference
       **/
      Madara::Transport::Settings & transport_settings (void);

      /**
       * Checks if a knowledge location exists in the context
       *
       * @param key             knowledge variable location
       * @return                true if location has been set
       **/
      bool exists (const std::string & key) const;

      /**
       * Evaluates an expression
       *
       * @param expression      KaRL expression to evaluate
       * @param settings        Settings for evaluating and printing
       * @return                value of expression
       **/
      Madara::Knowledge_Record evaluate (
        const std::string & expression,
        const Eval_Settings & settings = DEFAULT_EVAL_SETTINGS);

      /**
       * Evaluates an expression
       *
       * @param expression      KaRL expression to evaluate (result of compile)
       * @param settings        Settings for evaluating and printing
       * @return                value of expression
       **/
      Madara::Knowledge_Record evaluate (
        Compiled_Expression & expression,
        const Eval_Settings & settings = DEFAULT_EVAL_SETTINGS);

      /**
       * Waits for an expression to be non-zero.
       * Always disseminates modifications.
       *
       * @param expression      KaRL expression to wait on
       * @param settings        Settings for the underlying expression
       *                        evaluation and printing
       * @return                value of expression
       **/
      Madara::Knowledge_Record wait (
        const std::string & expression,
        const Wait_Settings & settings = DEFAULT_WAIT_SETTINGS);

      /**
       * Waits for an expression to be non-zero. Provides additional settings
       * for fine-tuning the time to wait and atomic print statements.
       *
       * @param expression      KaRL expression to wait on (result of compile)
       * @param settings        Settings for the underlying expression
       *                        evaluation and printing
       * @return                value of expression
       **/
      Madara::Knowledge_Record wait (
        Compiled_Expression & expression,
        const Wait_Settings & settings = DEFAULT_WAIT_SETTINGS);

      /**
       * Applies current time and modified to all global variables and tries
       * to send them.
       **/
      int apply_modified (void);

      /**
       * Prints knowledge variables and values at a logging level
       * @param   level    level to log the knowledge at
       **/
      void print_knowledge (unsigned int level = 0);

      /**
       * Expands and prints a user provided statement at a logging level.
       * For example, if the statement were MyKnowledge.{.id}, and .id==1,
       * then the statement would be expanded to MyKnowledge.1
       * 
       * @param   statement  statement to expand with variable values
       * @param   level      level to log the knowledge at
       **/
      void print (const std::string & statement, 
        unsigned int level = 0);

      /**
       * Clears the knowledge base
       **/
      void clear (void);

      /**
       * Clears the knowledge base
       **/
      void clear_map (void);

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
        VALUE_TYPE (*func) (Function_Arguments &, Variables &));
      
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

    private:

      /// Pointer to actual implementation, i.e., the "bridge", which is
      /// reference counted to automate memory management. 
      Madara::Utility::Refcounter <Knowledge_Base_Impl> impl_;
    };

  }
}

// include the inline functions
#include "Knowledge_Base.inl"

#endif   // _MADARA_KNOWLEDGE_BASE_H
