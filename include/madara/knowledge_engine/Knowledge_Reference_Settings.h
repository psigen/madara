#ifndef _KNOWLEDGE_REFERENCE_SETTINGS_
#define _KNOWLEDGE_REFERENCE_SETTINGS_

/**
 * @file Knowledge_Base.h
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains the Knowledge_Reference_Settings class, which
 * provides settings for applying knowledge updates
 */

#include "madara/MADARA_export.h"
#include "madara/utility/stdint.h"

namespace Madara
{
  namespace Knowledge_Engine
  {
    /**
     * Settings for applying knowledge updates
     **/
    class MADARA_Export Knowledge_Reference_Settings
    {
    public:
      /**
       * Constructor
       **/
      Knowledge_Reference_Settings ()
        : expand_variables (true)
      {
      }
  
      /**
       * Constructor
       **/
      Knowledge_Reference_Settings (bool t_expand_variables)
        : expand_variables (t_expand_variables)
      {
      }
      
      /**
       * Copy constructor
       **/
      Knowledge_Reference_Settings (const Knowledge_Reference_Settings & rhs)
        : expand_variables (rhs.expand_variables)
      {
      }


      /**
       * Destructor
       **/
      ~Knowledge_Reference_Settings ()
      {
      }

      /**
      * Toggle for always attempting to expand variables (true) or never
      * expanding variables (false)
      **/
      bool expand_variables;

    };

    /**
     * Default settings for referencing knowledge
     **/
    extern MADARA_Export const Knowledge_Reference_Settings
      DEFAULT_KNOWLEDGE_REFERENCE_SETTINGS;
    extern MADARA_Export const Knowledge_Reference_Settings
      DO_NOT_EXPAND_VARIABLES;
  }
}
#endif