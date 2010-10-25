#ifndef _MADARA_KNOWLEDGE_BASE_H
#define _MADARA_KNOWLEDGE_BASE_H

/**
 * @file Knowledge_Base.h
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains the Knowledge_Base class
 */

#include "madara/Globals.h"
#include "madara/MADARA_export.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/transport/Transport.h"
#include "ace/SOCK_Acceptor.h"
#include "madara/expression_tree/Interpreter.h"
#include "madara/Refcounter.h"
#include "madara/knowledge_engine/Knowledge_Base_Impl.h"

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
      /// Transport type enumeration

      enum {
        TCP_TRANSPORT = 0,
        OPEN_SPLICE_TRANSPORT = 1
      };

      /// Default constructor
      Knowledge_Base ();

      /// Constructor for transport
      Knowledge_Base (const std::string & host, int transport);

      /// Constructor for transport and knowledge realm
      Knowledge_Base (const std::string & host, int transport,
        const std::string & knowledge_domain);

      /// Copy constructor
      Knowledge_Base (const Knowledge_Base & original);

      /// Destructor
      ~Knowledge_Base ();

      /// Assignment operator
      void operator= (const Knowledge_Base & original);

      /// activate the transport for sending and receiving
      void activate_transport (void);

      /// close and cleanup the transport
      void close_transport (void);

      /// Retrieve the value associated with the provided key
      int get (const ::std::string & key) const;

      /// Expand a statement by variable expansions 
      std::string expand_statement (const ::std::string & statement) const;

      /// Set the value associated with the provided key
      int set (const ::std::string & key, long value = Madara::Knowledge_Record::MODIFIED);

      /// Set the value associated with the provided key
      int set (const ::std::string & key, long value, bool send_modifieds);

      /// Set quality of writing to a variable
      void set_quality (const ::std::string & key, unsigned long quality);

      /// Check if a key exists in the knowledge base
      bool exists (const ::std::string & key) const;

      /// Add rule to the knowledge base (preferred method for data entry)
      void add_rule (const ::std::string & expression);

      /// Evaluate an expression once
      int evaluate (const ::std::string & expression);

      /// Evaluate an expression once
      int evaluate (const ::std::string & expression, bool send_modifieds);

      /// Block for an expression to evaluate to true (conditional mutex)
      int wait (const ::std::string & expression);

      /// Block for an expression to evaluate to true (conditional mutex)
      int wait (const ::std::string & expression, bool send_modifieds);

      /// Print all rules that are continuously applied to knowledge
      void print_rules (void) const;

      /// Print all knowledge (does not include rules)
      void print_knowledge (void) const;

      /// Print a statement after expanding any variables (e.g. "Var{.myid}")
      void print (const std::string & statement) const;

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

      /// Pointer to actual implementation, i.e., the "bridge", which is
      /// reference counted to automate memory management. 
      Refcounter <Knowledge_Base_Impl> impl_;
    };
  }
}

#endif   // _MADARA_KNOWLEDGE_BASE_H
