#include "madara/knowledge_engine/Compiled_Expression.h"
#include "madara/expression_tree/Expression_Tree.h"

Madara::Knowledge_Engine::Compiled_Expression::Compiled_Expression ()
{

}

Madara::Knowledge_Engine::Compiled_Expression::Compiled_Expression (
  const Compiled_Expression & ce)
: logic (ce.logic), expression (ce.expression)
{

}

Madara::Knowledge_Engine::Compiled_Expression::~Compiled_Expression ()
{

}


void
Madara::Knowledge_Engine::Compiled_Expression::operator= (
  const Compiled_Expression & ce)
{
  if (this != &ce)
  {
    logic = ce.logic;
    expression = ce.expression;
  }
}