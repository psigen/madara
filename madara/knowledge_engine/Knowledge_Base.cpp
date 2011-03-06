
#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/knowledge_engine/Knowledge_Base_Impl.h"

#include <sstream>


#ifdef _USE_OPEN_SPLICE_
  #include "madara/transport/Splice_DDS_Transport.h"
#endif // _USE_OPEN_SPLICE_

#include <iostream>

Madara::Knowledge_Engine::Knowledge_Base::Knowledge_Base ()
: impl_ (new Knowledge_Base_Impl ())
{
}

Madara::Knowledge_Engine::Knowledge_Base::Knowledge_Base (
  const std::string & host, int transport)
: impl_ (new Knowledge_Base_Impl (host, transport))
{
}

Madara::Knowledge_Engine::Knowledge_Base::Knowledge_Base (
  const std::string & host, int transport, 
  const std::string & knowledge_domain)
: impl_ (new Knowledge_Base_Impl (host, transport, knowledge_domain))
{
}

Madara::Knowledge_Engine::Knowledge_Base::Knowledge_Base (
  const std::string & host, const Madara::Transport::Settings & settings)
: impl_ (new Knowledge_Base_Impl (host, settings))
{
}

Madara::Knowledge_Engine::Knowledge_Base::Knowledge_Base (
  const Knowledge_Base & original)
: impl_ (original.impl_)
{
}

Madara::Knowledge_Engine::Knowledge_Base::~Knowledge_Base ()
{
  // taken care of by Refcounter class
}
