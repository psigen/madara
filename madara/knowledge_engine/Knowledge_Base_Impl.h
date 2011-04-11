#ifndef _MADARA_KNOWLEDGE_BASE_IMPL_H
#define _MADARA_KNOWLEDGE_BASE_IMPL_H

/**
 * @file Knowledge_Base.h
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains the Knowledge_Base class
 */

#include "madara/Globals.h"
#include "madara/MADARA_export.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/knowledge_engine/Files.h"
#include "madara/transport/Transport.h"
#include "ace/SOCK_Acceptor.h"
#include "madara/expression_tree/Interpreter.h"

namespace Madara
{

  namespace Knowledge_Engine
  {
    /**
     * @class Knowledge_Base_Impl
     * @brief This class provides a distributed knowledge base implementation
     */
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
      long long get (const ::std::string & key) const;

      /**
       * Expands a statement using variable expansion. For example, if the
       * statement were MyKnowledge.{.id}, and .id==1, then the statement
       * would be expanded to MyKnowledge.1
       *
       * @param statement          statement to expand
       * @return                   expanded statement
       **/
      std::string expand_statement (const ::std::string & statement) const;

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
       * Sets a knowledge value to a specified value
       *
       * @param key          knowledge variable location
       * @param value        value to set at location
       * @return             0 if successful, -1 if key is null, and
       *                     -2 if quality isn't high enough
       **/
      int set (const ::std::string & key, 
        long long value = Madara::Knowledge_Record::MODIFIED);

      /**
       * Sets a knowledge value to a specified value
       *
       * @param key             knowledge variable location
       * @param value           value to set at location
       * @param send_modifieds  whether or not to dissemination modifications
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (const ::std::string & key, long long value, 
        bool send_modifieds);

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
      bool exists (const ::std::string & key) const;

      /// Add rule to the knowledge base (preferred method for data entry)
      void add_rule (const ::std::string & expression);

      /**
       * Sets the quality of writing to a certain variable from this entity
       *
       * @param key             knowledge variable location
       * @param quality         quality of writing to this location
       **/
      void set_quality (const ::std::string & key, unsigned long quality);

      /**
       * Evaluates an expression. Always disseminates modifications.
       *
       * @param expression      KaRL expression to evaluate
       * @return                value of expression
       **/
      long long evaluate (const ::std::string & expression);

      /**
       * Evaluates an expression
       *
       * @param expression      KaRL expression to evaluate
       * @param send_modifieds  whether or not to dissemination modifications
       * @return                value of expression
       **/
      long long evaluate (const ::std::string & expression,
        bool send_modifieds);

      /**
       * Waits for an expression to be non-zero.
       * Always disseminates modifications.
       *
       * @param expression      KaRL expression to wait on
       * @return                value of expression
       **/
      long long wait (const ::std::string & expression);

      /**
       * Waits for an expression to be non-zero.
       *
       * @param expression      KaRL expression to wait on
       * @param send_modifieds  whether or not to dissemination modifications
       * @return                value of expression
       **/
      long long wait (const ::std::string & expression, bool send_modifieds);

      /**
       * Prints the permanent knowledge rules (unimplemented)
       * @param   level    level to log the knowledge at
       **/
      void print_rules (unsigned int level) const;

      /**
       * Prints knowledge variables and values at a logging level
       * @param   level    level to log the knowledge at
       **/
      void print_knowledge (unsigned int level) const;

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

    private:
      void setup_uniquehostport (const std::string & host);

      ::std::vector< ::std::string> statement_splitters_;
      ::std::vector< ::std::string> assignment_splitters_;
      ::std::vector< ::std::string> implies_splitters_;
      ::std::vector< ::std::string> conditional_splitters_;
      ::std::vector< ::std::string> comparison_splitters_;


      Thread_Safe_Context           map_;
      Knowledge_Rules               rules_;
      Madara::Transport::Base *     transport_;
      ACE_SOCK_Acceptor             unique_bind_;
      std::string                   id_;
      //std::string                   domain_name_;
      //int                           transport_type_;
      Files                         files_;

      Madara::Transport::Settings              settings_;
      Madara::Expression_Tree::Interpreter     interpreter_;
    };
  }
}
    
// include the inline functions
#include "Knowledge_Base_Impl.inl"

#endif  // _MADARA_KNOWLEDGE_BASE_IMPL_H
