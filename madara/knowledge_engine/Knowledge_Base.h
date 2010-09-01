#ifndef MADARA_KNOWLEDGE_BASE
#define MADARA_KNOWLEDGE_BASE

/**
 * @file Knowledge_Base.h
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains the Knowledge_Base class
 */

#include "madara/Globals.h"
#include "madara/MADARA_export.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/transport/Splice_DDS_Transport.h"

namespace Madara
{

  namespace Knowledge_Engine
  {
    /**
     * @class Knowledge_Record
     * @brief This class provides a distributed knowledge base to users
     */
    class MADARA_Export Knowledge_Base
    {
    public:
      /// Default constructor
      Knowledge_Base ();

      /// Destructor
      ~Knowledge_Base ();

      /// Retrieve the value associated with the provided key
      int get (const ::std::string & key) const;

      /// Set the value associated with the provided key
      void set (const ::std::string & key, int value = Madara::Knowledge_Record::SET);

      /// Check if a key exists in the knowledge base
      bool exists (const ::std::string & key) const;

      /// Add rule to the knowledge base (preferred method for data entry)
      void add_rule (const ::std::string & expression);

      /// Evaluate an expression once
      void evaluate (const ::std::string & expression);

      /// Block for an expression to evaluate to true (conditional mutex)
      int wait (const ::std::string & expression) const;

      /// Print all rules that are continuously applied to knowledge
      void print_rules (void) const;

      /// Print all knowledge (does not include rules)
      void print_knowledge (void) const;

      /// Send test messages over the transport
      void test (const long & iterations);

      /// Clear all knowledge and rules from the knowledge base
      void clear (void);

      /// Clear all knowledge from the knowledge base but keep rules
      void clear_map (void);

      /// Clear all rules from the knowledge base but keep knowledge
      void clear_rules (void);

    private:

      ::std::vector<::std::string> statement_splitters_;
      ::std::vector<::std::string> assignment_splitters_;
      ::std::vector<::std::string> implies_splitters_;
      ::std::vector<::std::string> conditional_splitters_;
      ::std::vector<::std::string> comparison_splitters_;


      Thread_Safe_Context map_;
      Knowledge_Rules rules_;
      Madara::Transport::Base * transport_;
    };
  }
}

#endif