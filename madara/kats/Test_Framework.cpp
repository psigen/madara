
#include "madara/kats/Test_Framework.h"

#include <sstream>

#include <iostream>

Madara::KATS::Test_Framework::Test_Framework ()
: knowledge_ ()
{
}

Madara::KATS::Test_Framework::Test_Framework (
  const std::string & host, int transport)
: knowledge_ (host, transport)
{
}

Madara::KATS::Test_Framework::Test_Framework (
  const std::string & host, int transport, 
  const std::string & knowledge_domain)
: knowledge_ (host, transport, knowledge_domain)
{
}

Madara::KATS::Test_Framework::Test_Framework (
  const std::string & host, const Madara::Transport::Settings & settings)
: knowledge_ (host, settings)
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
