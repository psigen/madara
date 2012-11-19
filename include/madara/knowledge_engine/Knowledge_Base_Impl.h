#ifndef _MADARA_KNOWLEDGE_BASE_IMPL_H
#define _MADARA_KNOWLEDGE_BASE_IMPL_H

/**
 * @file Knowledge_Base.h
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains the Knowledge_Base class
 */

#include "madara/knowledge_engine/Knowledge_Record.h"
#include "madara/MADARA_export.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/knowledge_engine/Files.h"
#include "madara/transport/Transport.h"
#include "madara/expression_tree/Interpreter.h"
#include "madara/knowledge_engine/Compiled_Expression.h"
#include "ace/SOCK_Acceptor.h"

#include <ostream>

// declare ACE class so MADARA user will not have to directly link to ACE
class ACE_SOCK_Acceptor;

namespace Madara
{

  namespace Knowledge_Engine
  {

    /**
     * @class Eval_Settings
     * @brief Encapsulates settings for an evaluation statement
     **/
    struct Eval_Settings
     {
       /**
        * Constructor
        **/
       Eval_Settings ()
         : send_modifieds (true),
           pre_print_statement (""), post_print_statement ("")
       {
       }

       /**
        * Constructor
        **/
       Eval_Settings (bool t_send_modifieds,
         std::string t_pre_print_statement,
         std::string t_post_print_statement)
         : send_modifieds (t_send_modifieds),
           pre_print_statement (t_pre_print_statement),
           post_print_statement (t_post_print_statement)
       {
       }

       /**
        * Toggle for sending modifieds in a single update event
        * after each evaluation.
        **/
       bool send_modifieds;

       /**
        * Statement to print before evaluations
        **/
       std::string pre_print_statement;

       /**
        * Statement to print after evaluations
        **/
       std::string post_print_statement;
     };

    /**
     * @class Wait_Settings
     * @brief Encapsulates settings for a wait statement
     **/
    struct Wait_Settings : public Eval_Settings
     {
       /**
        * Constructor
        **/
       Wait_Settings ()
         : Eval_Settings (), poll_frequency (0.010), max_wait_time (-1.0)
       {
       }

       /**
        * Constructor
        **/
       Wait_Settings (bool t_send_modifieds,
         std::string t_pre_print_statement,
         std::string t_post_print_statement,
         double t_poll_frequency, double t_max_wait_time)
         : Eval_Settings (t_send_modifieds, 
              t_pre_print_statement, t_post_print_statement),
           poll_frequency (t_poll_frequency), max_wait_time (t_max_wait_time)
       {
       }

       /**
        * Frequency to poll an expression for truth
        **/
       double poll_frequency;

       /**
        * Maximum time to wait for an expression to become true
        **/
       double max_wait_time;
     };

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
       * @param   domain    knowledge domain we want to join
       **/
      Knowledge_Base_Impl (const std::string & host, int transport,
        const std::string & knowledge_realm);

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
       * @param key          knowledge variable location
       * @param value        value to set at location
       * @return             0 if successful, -1 if key is null, and
       *                     -2 if quality isn't high enough
       **/
      int set (const std::string & key, 
        Madara::Knowledge_Record::Integer value = Madara::Knowledge_Record::MODIFIED);
      
      /**
       * Sets a knowledge value to a specified value
       *
       * @param key          knowledge variable location
       * @param value        value to set at location
       * @return             0 if successful, -1 if key is null, and
       *                     -2 if quality isn't high enough
       **/
      int set (const std::string & key, double value);
      
      /**
       * Sets a knowledge value to a specified value
       *
       * @param key          knowledge variable location
       * @param value        value to set at location
       * @return             0 if successful, -1 if key is null, and
       *                     -2 if quality isn't high enough
       **/
      int set (const std::string & key, const std::string & value);

      /**
       * Sets a knowledge value to a specified value
       *
       * @param key             knowledge variable location
       * @param value           value to set at location
       * @param send_modifieds  whether or not to dissemination modifications
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (const std::string & key, Madara::Knowledge_Record::Integer value, 
        bool send_modifieds);
      
      /**
       * Sets a knowledge value to a specified value
       *
       * @param key             knowledge variable location
       * @param value           value to set at location
       * @param send_modifieds  whether or not to dissemination modifications
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (const std::string & key, double value, 
        bool send_modifieds);
      
      /**
       * Sets a knowledge value to a specified value
       *
       * @param key             knowledge variable location
       * @param value           value to set at location
       * @param send_modifieds  whether or not to dissemination modifications
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (const std::string & key, const std::string & value, 
        bool send_modifieds);

      /**
       * Returns a non-const reference to the Transport Settings
       * @return  settings reference
       **/
      Madara::Transport::Settings & transport_settings (void);

      /**
       * Updates all global variables to current clock and then
       * sends them if a transport is available. This is useful
       * when trying to synchronize to late joiners (this process
       * will resend all global variables.
       * @returns     0 if transport was available, -1 if not
       **/
      int apply_modified (void);

      /**
       * Checks if a knowledge location exists in the context
       *
       * @param key             knowledge variable location
       * @return                true if location has been set
       **/
      bool exists (const std::string & key) const;

      /// Add rule to the knowledge base (preferred method for data entry)
      void add_rule (const std::string & expression);

      /**
       * Sets the quality of writing to a certain variable from this entity
       *
       * @param key             knowledge variable location
       * @param quality         quality of writing to this location
       **/
      void set_quality (const std::string & key, uint32_t quality);

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
       * @param send_modifieds  whether or not to dissemination modifications
       * @return                value of expression
       **/
      Madara::Knowledge_Record evaluate (const std::string & expression,
        bool send_modifieds);

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
       * Waits for an expression to be non-zero.
       * Always disseminates modifications.
       *
       * @param expression      KaRL expression to wait on
       * @return                value of expression
       **/
      Madara::Knowledge_Record wait (const std::string & expression);

      /**
       * Waits for an expression to be non-zero.
       *
       * @param expression      KaRL expression to wait on
       * @param send_modifieds  whether or not to dissemination modifications
       * @return                value of expression
       **/
      Madara::Knowledge_Record wait (const std::string & expression, bool send_modifieds);

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
       * Prints the permanent knowledge rules (unimplemented)
       * @param   level    level to log the knowledge at
       **/
      void print_rules (unsigned int level) const;

      /**
       * Prints knowledge variables and values at a logging level
       * @param   level    level to log the knowledge at
       **/
      void print_knowledge (unsigned int level);

      /**
       * Expands and prints a user provided statement at a logging level.
       * For example, if the statement were MyKnowledge.{.id}, and .id==1,
       * then the statement would be expanded to MyKnowledge.1
       * 
       * @param   statement  statement to expand with variable values
       * @param   level      level to log the knowledge at
       **/
      void print (const std::string & statement, unsigned int level);

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
        VALUE_TYPE (*func) (Variables *));

    private:
      /**
       * Binds to an ephemeral port for unique tie breakers in global ordering
       **/
      void setup_uniquehostport (const std::string & host);

      ::std::vector< std::string> statement_splitters_;
      ::std::vector< std::string> assignment_splitters_;
      ::std::vector< std::string> implies_splitters_;
      ::std::vector< std::string> conditional_splitters_;
      ::std::vector< std::string> comparison_splitters_;


      Thread_Safe_Context           map_;
      Knowledge_Rules               rules_;
      ACE_SOCK_Acceptor             unique_bind_;
      std::string                   id_;
      //std::string                   domain_name_;
      //int                           transport_type_;
      Madara::Transport::Settings   settings_;
      Files                         files_;

      Madara::Expression_Tree::Interpreter     interpreter_;
      Madara::Transport::Base *     transport_;
    };
  }
}
    
// include the inline functions
#include "Knowledge_Base_Impl.inl"

#endif  // _MADARA_KNOWLEDGE_BASE_IMPL_H
