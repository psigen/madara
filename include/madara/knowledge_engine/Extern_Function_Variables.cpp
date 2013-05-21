#ifndef _MADARA_EXTERNAL_FUNCTION_VARIABLES_CPP_
#define _MADARA_EXTERNAL_FUNCTION_VARIABLES_CPP_

#include "madara/knowledge_engine/Extern_Function_Variables.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"

#include "madara/utility/Log_Macros.h"

/**
  * Retrieves the value of a variable.
  * @param   key       unique identifier of the variable
  * @param   settings  settings when referring to variables
  * @return   the value of the keyed variable
  **/
Madara::Knowledge_Record
Madara::Knowledge_Engine::Variables::get (const std::string & key,
             const Knowledge_Reference_Settings & settings) const
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
                          Madara::Knowledge_Record::Integer value,
        const Knowledge_Update_Settings & settings)
{
  if (context_)
  {
    context_->set (key, value, settings);

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
                          const Madara::Knowledge_Record & value,
        const Knowledge_Update_Settings & settings)
{
  if (context_)
  {
    if (value.type () == Madara::Knowledge_Record::INTEGER)
      context_->set (key, value.to_integer (), settings);
    else if (value.type () == Madara::Knowledge_Record::DOUBLE)
      context_->set (key, value.to_double (), settings);
    else if (value.type () == Madara::Knowledge_Record::STRING)
      context_->set (key, value.to_string (), settings);

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
                                          double value,
        const Knowledge_Update_Settings & settings)
{
  if (context_)
  {
    context_->set (key, value, settings);

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
                                          const std::string & value,
        const Knowledge_Update_Settings & settings)
{
  if (context_)
  {
    context_->set (key, value, settings);

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
Madara::Knowledge_Engine::Variables::inc (const std::string & key,
        const Knowledge_Update_Settings & settings)
{
  if (context_)
  {
    return context_->inc (key, settings);
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
  Madara::Knowledge_Engine::Variables::dec (const std::string & key,
        const Knowledge_Update_Settings & settings)
{
  if (context_)
  {
    return context_->dec (key, settings);
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


/**
  * Compiles a KaRL expression into an expression tree. Always do this
  * before calling evaluate because it puts the expression into an
  * optimized format. Best practice is to save the Compiled_Expression
  * in a global variable or in some kind of persistent store. Pair with
  * expand_statement if you know that variable expansion is used but
  * the variable values that are expanded never change (e.g. an id that
  * is set through the command line and thus stays the same after it is
  * initially set).
  *
  * @param expression         expression to compile
  * @return                   compiled, optimized expression tree
  **/
Madara::Knowledge_Engine::Compiled_Expression
  Madara::Knowledge_Engine::Variables::compile (const std::string & expression)
{
  if (context_)
    return context_->compile (expression);
  else
  {
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_ERROR,
      "Variables context not set. Please don't create your own Madara::" \
      "Knowledge_Engine::Variables instances.\n"));

    return Compiled_Expression ();
  }
}
  
/**
  * Evaluates an expression (USE ONLY FOR PROTOTYPING; DO NOT USE IN
  * PRODUCTION SYSTEMS). Consider compiling the expression first with
  * a one-time compile call during an initialization phase for your
  * program, and then using the evaluate Compiled_Expression call
  * in any function that must be called frequently or periodically.
  * The difference in overhead between this function and the compiled
  * version is orders of magnitude (generally nanoseconds versus
  * microseconds every call).
  *
  * @param expression      KaRL expression to evaluate
  * @param settings        Settings for evaluating and printing
  * @return                value of expression
  **/
Madara::Knowledge_Record
Madara::Knowledge_Engine::Variables::evaluate (
  const std::string & expression,
  const Knowledge_Update_Settings & settings)
{
  if (context_)
  {
    Compiled_Expression compiled = context_->compile (expression);
    return compiled.expression.evaluate (settings);
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_ERROR,
      "Variables context not set. Please don't create your own Madara::" \
      "Knowledge_Engine::Variables instances.\n"));

    return Madara::Knowledge_Record::Integer (0);
  }
}
    
/**
  * Evaluates an expression. Recommended best practices are to compile the
  * expression into a global variable or persistent store outside of the
  * function call and use a reference to this Compiled_Expression from
  * within the external function that you create. In other words, avoid
  * compiling the expression within your external function call as this
  * adds compile overhead that tends to be microseconds in time.
  *
  * @param expression      KaRL expression to wait on (result of compile)
  * @param settings        Settings for updating knowledge
  * @return                value of expression
  **/
Madara::Knowledge_Record
Madara::Knowledge_Engine::Variables::evaluate (
  Compiled_Expression & expression,
  const Knowledge_Update_Settings & settings)
{
  if (context_)
  {
    return expression.expression.evaluate (settings);
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_ERROR,
      "Variables context not set. Please don't create your own Madara::" \
      "Knowledge_Engine::Variables instances.\n"));

    return Madara::Knowledge_Record::Integer (0);
  }
}

/**
  * Fills a vector with Knowledge Records that begin with a common subject
  * and have a finite range of integer values.
  * @param   subject     The common subject of the variable names. For
  *                      instance, if we are looking for a range of vars
  *                      like "var0", "var1", "var2", then the common
  *                      subject would be "var".
  * @param   start       An inclusive start index
  * @param   end         An inclusive end index
  * @param   target      The vector that will be filled with
  *                      Knowledge Record instances within the subject
  *                      range.
  * @return              entries in the resulting vector
  **/
unsigned int
Madara::Knowledge_Engine::Variables::to_vector (
                              const std::string & subject,
                              unsigned int start,
                              unsigned int end,
                              std::vector <Knowledge_Record> & target)
{
  return context_->to_vector (subject, start, end, target);
}

/**
  * Fills a variable map with Knowledge Records that match an expression.
  * At the moment, this expression must be of the form "subject*"
  * @param   expression  An expression that matches the variable names
  *                      that are of interest. Wildcards may only be
  *                      at the end.
  * @param   target      The map that will be filled with variable names
  *                      and the Knowledge Records they correspond to
  * @return              entries in the resulting map
  **/
unsigned int
Madara::Knowledge_Engine::Variables::to_map (
  const std::string & expression,
  std::map <std::string, Knowledge_Record> & target)
{
  return context_->to_map (expression, target);
}

#endif