#ifndef _MADARA_EXTERNAL_FUNCTIONS_CPP_
#define _MADARA_EXTERNAL_FUNCTIONS_CPP_

#include "madara/Functions.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"

#include "madara/utility/Log_Macros.h"

/**
  * Retrieves the value of a variable.
  * @param   key       unique identifier of the variable
  * @return   the value of the keyed variable
  **/
Madara::Knowledge_Record
Madara::Knowledge_Engine::Variables::get (const std::string & key) const
{
  if (context_)
    return *context_->get_record (key);
  else
  {
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_ERROR,
      "Variables context not set. Please don't create your own Madara::" \
      "Knowledge_Engine::Variables instances.\n"));
    return Madara::Knowledge_Record ();
  }
}

/**
  * Sets the value of a variable.
  * @param   key       unique identifier of the variable
  * @param   value     new value of the variable
  * @return   the new value of the keyed variable
  **/
int
Madara::Knowledge_Engine::Variables::set (const std::string & key,
                          Madara::Knowledge_Record::Integer value)
{
  if (context_)
  {
    context_->set (key, value, true);

    return 1;
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_ERROR,
      "Variables context not set. Please don't create your own Madara::" \
      "Knowledge_Engine::Variables instances.\n"));
    return 0;
  }
}
  
/**
  * Sets the value of a variable.
  * @param   key       unique identifier of the variable
  * @param   value     new value of the variable
  * @return   the new value of the keyed variable
  **/
int
Madara::Knowledge_Engine::Variables::set (const std::string & key,
                          const Madara::Knowledge_Record & value)
{
  if (context_)
  {
    *(context_->get_record (key)) = value;

    return 1;
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_ERROR,
      "Variables context not set. Please don't create your own Madara::" \
      "Knowledge_Engine::Variables instances.\n"));
    return 0;
  }
}
  
/**
  * Sets the value of a variable.
  * @param   key       unique identifier of the variable
  * @param   value     new value of the variable
  * @return   the new value of the keyed variable
  **/
int
Madara::Knowledge_Engine::Variables::set (const std::string & key,
                                          double value)
{
  if (context_)
  {
    context_->set (key, value, true);

    return 1;
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_ERROR,
      "Variables context not set. Please don't create your own Madara::" \
      "Knowledge_Engine::Variables instances.\n"));
    return 0;
  }
}

/**
  * Sets the value of a variable.
  * @param   key       unique identifier of the variable
  * @param   value     new value of the variable
  * @return   the new value of the keyed variable
  **/
int
Madara::Knowledge_Engine::Variables::set (const std::string & key,
                                          const std::string & value)
{
  if (context_)
  {
    context_->set (key, value, true);

    return 1;
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_ERROR,
      "Variables context not set. Please don't create your own Madara::" \
      "Knowledge_Engine::Variables instances.\n"));
    return 0;
  }
}

/**
  * Atomically increments the value of the variable
  * @param   key            unique identifier of the variable
  * @return                 new value of variable
  **/
Madara::Knowledge_Record
Madara::Knowledge_Engine::Variables::inc (const std::string & key)
{
  if (context_)
  {
    return context_->inc (key);
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_ERROR,
      "Variables context not set. Please don't create your own Madara::" \
      "Knowledge_Engine::Variables instances.\n"));
    return Madara::Knowledge_Record ();
  }
}

/**
  * Decrements the value of the variable
  * @param   key            unique identifier of the variable
  * @return                 new value of variable
  **/
Madara::Knowledge_Engine::VALUE_TYPE
  Madara::Knowledge_Engine::Variables::dec (const std::string & key)
{
  if (context_)
  {
    return context_->dec (key);
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_ERROR,
      "Variables context not set. Please don't create your own Madara::" \
      "Knowledge_Engine::Variables instances.\n"));
    return Madara::Knowledge_Record ();
  }
}

/**
  * Prints all variables and values in the context
  * @param   level          log level. @see Log_Macros.h
  **/
void
  Madara::Knowledge_Engine::Variables::print (unsigned int level) const
{
  if (context_)
    context_->print (level);
  else
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_ERROR,
      "Variables context not set. Please don't create your own Madara::" \
      "Knowledge_Engine::Variables instances.\n"));
}

/**
  * Print a statement, similar to printf (variable expansions
  * allowed) e.g., input = "MyVar{.id} = {MyVar{.id}}\n";
  * @param   statement      templated statement to print from
  * @param   level          log level. @see Log_Macros.h
  **/
void
  Madara::Knowledge_Engine::Variables::print (const std::string & statement,
                                   unsigned int level) const
{
  if (context_)
    context_->print (statement, level);
  else
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_ERROR,
      "Variables context not set. Please don't create your own Madara::" \
      "Knowledge_Engine::Variables instances.\n"));
}
      
/**
  * Expands a string with variable expansion. For instance, if
  * .id == 5, and a statement of "MyVar{.id} = {.id} * 30" then
  * then expanded statement would be "MyVar5 = 5 * 30".
  * @param  statement  statement to expand. Useful for printing.
  * @return            variable expanded statement
  **/
std::string
  Madara::Knowledge_Engine::Variables::expand_statement (
                                  const std::string & statement) const
{
  if (context_)
    return context_->expand_statement (statement);
  else
  {
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_ERROR,
      "Variables context not set. Please don't create your own Madara::" \
      "Knowledge_Engine::Variables instances.\n"));

    return "";
  }
}

#endif