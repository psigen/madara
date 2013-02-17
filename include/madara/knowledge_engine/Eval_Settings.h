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
        **/
       Eval_Settings (bool t_delay_sending_modifieds,
         std::string t_pre_print_statement,
         std::string t_post_print_statement)
         : Knowledge_Update_Settings (), 
           delay_sending_modifieds (t_delay_sending_modifieds),
           pre_print_statement (t_pre_print_statement),
           post_print_statement (t_post_print_statement)
       {
       }
       
       /**
        * Constructor
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
     };

    extern MADARA_Export const Eval_Settings DEFAULT_EVAL_SETTINGS;
  }
}
#endif