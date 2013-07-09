#ifndef _EVAL_SETTINGS_
#define _EVAL_SETTINGS_

/**
 * @file Eval_Settings.h
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains the Eval_Settings class, which
 * provides settings for using the eval mechanism in the Knowledge Engine
 */

#include <string>
#include "madara/knowledge_engine/Knowledge_Update_Settings.h"

namespace Madara
{

  namespace Knowledge_Engine
  {

    /**
     * @class Eval_Settings
     * @brief Encapsulates settings for an evaluation statement
     **/
    struct Eval_Settings : public Knowledge_Update_Settings
     {
       /**
        * Constructor
        **/
       Eval_Settings ()
         : Knowledge_Update_Settings (), delay_sending_modifieds (false),
           pre_print_statement (""), post_print_statement ("")
       {
       }

       
       /**
        * Constructor
        * @param  t_delay_sending_modifieds true to send modifieds
        * @param  t_treat_globals_as_locals true if global variables should
        *                                   be marked as not to be sent to the
        *                                   transport
        * @param  t_pre_print_statement     statement to print out before
        *                                   evaluations
        * @param  t_post_print_statement    statement to print out after
        *                                   evaluations
        **/
       Eval_Settings (bool t_delay_sending_modifieds,
         bool t_treat_globals_as_locals = false,
         bool signal_updates = true,
         std::string t_pre_print_statement = "",
         std::string t_post_print_statement = "")
         : Knowledge_Update_Settings (t_treat_globals_as_locals, signal_updates),
           delay_sending_modifieds (t_delay_sending_modifieds),
           pre_print_statement (t_pre_print_statement),
           post_print_statement (t_post_print_statement)
       {
       }
       
       /**
        * Copy constructor
        * @param   rhs   Eval settings instance to copy
        **/
       Eval_Settings (const Eval_Settings & rhs)
         : Knowledge_Update_Settings (rhs), 
           delay_sending_modifieds (rhs.delay_sending_modifieds),
           pre_print_statement (rhs.pre_print_statement),
           post_print_statement (rhs.post_print_statement)
       {
       }

       /**
        * Toggle for sending modifieds in a single update event
        * after each evaluation.
        **/
       bool delay_sending_modifieds;

       /**
        * Statement to print before evaluations
        **/
       std::string pre_print_statement;

       /**
        * Statement to print after evaluations
        **/
       std::string post_print_statement;

       static inline Eval_Settings
         get_default_settings (void)
       {
         return Eval_Settings ();
       }
       
       static inline Eval_Settings
         get_treat_as_local_settings (void)
       {
         return Eval_Settings (false, true);
       }
       
       static inline Eval_Settings
         get_delay_treat_as_local_settings (void)
       {
         return Eval_Settings (true, true);
       }
       
       static inline Eval_Settings
         get_delay_settings (void)
       {
         return Eval_Settings (true);
       }
       
       static inline Eval_Settings
         get_treat_as_local_no_signals_settings (void)
       {
         return Eval_Settings (false, true, false);
       }
       
       static inline Eval_Settings
         get_no_signals_settings (void)
       {
         return Eval_Settings (false, false, false);
       }
       
       static inline Eval_Settings
         get_delay_treat_as_local_no_signals_settings (void)
       {
         return Eval_Settings (true, true, false);
       }
       
       static inline Eval_Settings
         get_delay_no_signals_settings (void)
       {
         return Eval_Settings (true, false, false);
       }


     };

  }
}
#endif
