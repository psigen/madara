#ifndef _WAIT_SETTINGS_
#define _WAIT_SETTINGS_

/**
 * @file Wait_Settings.h
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains the Wait_Settings class, which
 * provides settings for the wait mechanism in the Knowledge Engine
 */

#include <string>
#include "madara/knowledge_engine/Eval_Settings.h"

namespace Madara
{
  namespace Knowledge_Engine
  {
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
       Wait_Settings (bool t_delay_sending_modifieds,
         bool t_treat_globals_as_locals,
         bool signal_updates,
         std::string t_pre_print_statement,
         std::string t_post_print_statement,
         double t_poll_frequency, double t_max_wait_time)
         : Eval_Settings (t_delay_sending_modifieds,
              t_treat_globals_as_locals, signal_updates,
              t_pre_print_statement, t_post_print_statement),
           poll_frequency (t_poll_frequency), max_wait_time (t_max_wait_time)
       {
       }
       
       /**
        * Copy constructor
        **/
       Wait_Settings (const Wait_Settings & rhs)
         : Eval_Settings (rhs),
         poll_frequency (rhs.poll_frequency), max_wait_time (rhs.max_wait_time)
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

  }
}
#endif
