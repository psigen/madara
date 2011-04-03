
#include "madara/kats/Test_Framework.h"

#include <sstream>

#include <iostream>

Madara::KATS::Test_Framework::Test_Framework (
  const Madara::KATS::Settings & settings)
: knowledge_ (settings.host, settings)
{
}

Madara::KATS::Test_Framework::Test_Framework (
  const Test_Framework & original)
: knowledge_ (original.knowledge_)
{
}

Madara::KATS::Test_Framework::~Test_Framework ()
{
}
