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

      /// Default constructor
      Knowledge_Base_Impl ();

      /// Constructor for transport
      Knowledge_Base_Impl (const std::string & host, int transport);

      ///// Constructor for transport and knowledge realm
      Knowledge_Base_Impl (const std::string & host, int transport,
        const std::string & knowledge_realm);

      /// Constructor for transport and transport settings
      Knowledge_Base_Impl (const std::string & host, 
        const Madara::Transport::Settings & config);

      /// Destructor
      ~Knowledge_Base_Impl ();

      /// activate the transport for sending and receiving
      void activate_transport (void);

      /// close and cleanup the transport
      void close_transport (void);

      /// Retrieve the value associated with the provided key
      long long get (const ::std::string & key) const;

      /// Expand a statement by variable expansions 
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

      /// Set the value associated with the provided key
      int set (const ::std::string & key, 
        long long value = Madara::Knowledge_Record::MODIFIED);

      /// Set the value associated with the provided key
      int set (const ::std::string & key, long long value, 
        bool send_modifieds);

      /// Check if a key exists in the knowledge base
      bool exists (const ::std::string & key) const;

      /// Add rule to the knowledge base (preferred method for data entry)
      void add_rule (const ::std::string & expression);

      /// Set quality of writing to a variable
      void set_quality (const ::std::string & key, unsigned long quality);

      /// Evaluate an expression once
      long long evaluate (const ::std::string & expression);

      /// Evaluate an expression once
      long long evaluate (const ::std::string & expression,
        bool send_modifieds);

      /// Block for an expression to evaluate to true (conditional mutex)
      long long wait (const ::std::string & expression);

      /// Block for an expression to evaluate to true (conditional mutex)
      long long wait (const ::std::string & expression, bool send_modifieds);

      /// Print all rules that are continuously applied to knowledge
      void print_rules (void) const;

      /// Print all knowledge (does not include rules)
      void print_knowledge (void) const;

      /// Print a statement after expanding any variables (e.g. "Var{.myid}")
      void print (const std::string & statement) const;

      /// Send test messages over the transport
      void test (const long & iterations);

      /// Clear all knowledge and rules from the knowledge base
      void clear (void);

      /// Clear all knowledge from the knowledge base but keep rules
      void clear_map (void);

      /// Clear all rules from the knowledge base but keep knowledge
      void clear_rules (void);

      /// lock the underlying knowledge base against any updates
      /// until we release
      void acquire (void);

      /// release the lock on the underlying knowledge base
      void release (void);

    private:
      void setup_uniquehostport (const std::string & host);
      void setup_splitters (void);

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
