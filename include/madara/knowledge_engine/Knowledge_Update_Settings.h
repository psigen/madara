#ifndef _KNOWLEDGE_UPDATE_SETTINGS_H_
#define _KNOWLEDGE_UPDATE_SETTINGS_H_

/**
 * @file Knowledge_Base.h
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains the Knowledge_Update_Settings class, which
 * provides settings for applying knowledge updates
 */

#include "madara/MADARA_export.h"
#include "madara/utility/stdint.h"
#include "madara/knowledge_engine/Knowledge_Reference_Settings.h"

namespace Madara
{
  namespace Knowledge_Engine
  {
    /**
     * Settings for applying knowledge updates
     **/
    class MADARA_Export Knowledge_Update_Settings
          : public Knowledge_Reference_Settings
    {
    public:
      /**
       * Constructor
       **/
      Knowledge_Update_Settings ()
        : Knowledge_Reference_Settings (), 
          treat_globals_as_locals (false),
          signal_changes (true),
          always_overwrite (false), 
          clock_increment (1)
      {
      }
  
      /**
       * Constructor
       **/
      Knowledge_Update_Settings (bool t_treat_globals_as_locals,
        bool t_signal_changes = true,
        bool t_always_overwrite = false,
        uint64_t t_clock_increment = 1)
        : Knowledge_Reference_Settings (),
          treat_globals_as_locals (t_treat_globals_as_locals),
          signal_changes (t_signal_changes),
          always_overwrite (t_always_overwrite),
          clock_increment (t_clock_increment)
      {
      }
  
      /**
       * Constructor
       **/
      Knowledge_Update_Settings (const Knowledge_Update_Settings & rhs)
        : Knowledge_Reference_Settings (rhs),
          treat_globals_as_locals (rhs.treat_globals_as_locals),
          signal_changes (rhs.signal_changes),
          always_overwrite (rhs.always_overwrite),
          clock_increment (rhs.clock_increment)
      {
      }


      /**
       * Destructor
       **/
      ~Knowledge_Update_Settings ()
      {
      }

      /**
      * Toggle whether updates to global variables are treated as
      * local variables and not marked as modified to the transport
      **/
      bool treat_globals_as_locals;

      /**
       * Toggle whether to signal changes have happened. Setting this
       * to false will increase performance, but in general should be
       * left to true. Setting this to false can result in problems
       * with wait statements.
       **/
      bool signal_changes;
      
      /**
      * Toggle for always overwriting records, regardless of quality,
      * clock values, etc.
      **/
      bool always_overwrite;
      
      /**
       * Default clock increment
       **/
      uint64_t  clock_increment;
    };

    extern MADARA_Export const
      Knowledge_Update_Settings DEFAULT_KNOWLEDGE_UPDATE_SETTINGS;

    extern MADARA_Export const
      Knowledge_Update_Settings TREAT_AS_LOCAL_UPDATE_SETTINGS;

    extern MADARA_Export const
      Knowledge_Update_Settings TREAT_AS_LOCAL_NO_SIGNALS_UPDATE_SETTINGS;

    extern MADARA_Export const
      Knowledge_Update_Settings NO_SIGNALS_UPDATE_SETTINGS;
  }
}

#endif
