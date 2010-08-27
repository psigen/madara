#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <string>
#include <vector>

namespace Madara
{
  namespace Transport
  {
    class Base
    {
    public:
      /// public typedef for the hosts vector
      typedef    std::vector <std::string>    Hosts_Vector;

      virtual long setHosts (const Hosts_Vector & hosts) { hosts_ = hosts; return 0;}
      virtual long addHost (const std::string & host) { hosts_.push_back (host); return 0;}
      virtual long send (const std::string & key, const long & value) = 0;
      virtual void close (void) = 0;

    protected:
      Hosts_Vector hosts_;
    };
  }
}

#endif