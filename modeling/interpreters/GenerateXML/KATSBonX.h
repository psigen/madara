#ifndef KATSBONX_H
#define KATSBONX_H

#include "BON.h"
#include "BONImpl.h"
#include "Extensions.h"

///BUP
// add your include files/class definitions here
///EUP

namespace KATS_BON { class KATSVisitor; }
namespace KATS_BON {        DECLARE_BONEXTENSION( BON::Folder, BarriersImpl, Barriers ); }
namespace KATS_BON {        DECLARE_BONEXTENSION( BON::Folder, DomainsImpl, Domains ); }
namespace KATS_BON {        DECLARE_BONEXTENSION( BON::Folder, HostsImpl, Hosts ); }
namespace KATS_BON {        DECLARE_BONEXTENSION( BON::Folder, ProcessesImpl, Processes ); }
namespace KATS_BON {        DECLARE_ABSTRACT_BONEXTENSION( BON::Model, ProcessBaseImpl, ProcessBase ); }
namespace KATS_BON {        DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, OrderedImpl, Ordered ); }
namespace KATS_BON {        DECLARE_BONEXTENSION( ProcessBase, GroupImpl, Group ); }
namespace KATS_BON {        DECLARE_BONEXTENSION2( Ordered, ProcessBase, ObserverImpl, Observer ); }
namespace KATS_BON {        DECLARE_BONEXTENSION2( Ordered, ProcessBase, ProcessImpl, Process ); }
namespace KATS_BON {        DECLARE_BONEXTENSION2( Ordered, ProcessBase, SleepImpl, Sleep ); }
namespace KATS_BON {        DECLARE_BONEXTENSION2( BON::Reference, Ordered, GroupRefImpl, GroupRef ); }
namespace KATS_BON {        DECLARE_BONEXTENSION( BON::Atom, BarrierImpl, Barrier ); }
namespace KATS_BON {        DECLARE_BONEXTENSION( BON::Atom, DomainImpl, Domain ); }
namespace KATS_BON {        DECLARE_BONEXTENSION( BON::Atom, HostImpl, Host ); }
namespace KATS_BON {        DECLARE_BONEXTENSION( BON::Atom, KillImpl, Kill ); }
namespace KATS_BON {        DECLARE_BONEXTENSION( BON::Reference, BarrierRefImpl, BarrierRef ); }
namespace KATS_BON {        DECLARE_BONEXTENSION( BON::Reference, DomainRefImpl, DomainRef ); }
namespace KATS_BON {        DECLARE_BONEXTENSION( BON::Reference, HostRefImpl, HostRef ); }


#include "KATSVisitor.h"

namespace KATS_BON
{
//*******************************************************************
//   C  L  A  S  S   BarriersImpl
//*******************************************************************
class BarriersImpl :
	  public BON::FolderImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( KATS_BON::KATSVisitor *pVisitor);
	//
	// kind and subfolder getters
	virtual std::set<KATS_BON::Barrier>     getBarrier();

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace KATS_BON
{
//*******************************************************************
//   C  L  A  S  S   DomainsImpl
//*******************************************************************
class DomainsImpl :
	  public BON::FolderImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( KATS_BON::KATSVisitor *pVisitor);
	//
	// kind and subfolder getters
	virtual std::set<KATS_BON::Domain>      getDomain();

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace KATS_BON
{
//*******************************************************************
//   C  L  A  S  S   HostsImpl
//*******************************************************************
class HostsImpl :
	  public BON::FolderImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( KATS_BON::KATSVisitor *pVisitor);
	//
	// kind and subfolder getters
	virtual std::set<KATS_BON::Host>        getHost();

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace KATS_BON
{
//*******************************************************************
//   C  L  A  S  S   ProcessesImpl
//*******************************************************************
class ProcessesImpl :
	  public BON::FolderImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( KATS_BON::KATSVisitor *pVisitor);
	//
	// kind and subfolder getters
	virtual std::set<KATS_BON::Group>       getGroup();

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace KATS_BON
{
//*******************************************************************
//   C  L  A  S  S   ProcessBaseImpl
//*******************************************************************
class ProcessBaseImpl :
	  virtual public BON::ModelImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( KATS_BON::KATSVisitor *pVisitor);

	//
	// attribute getters and setters
	virtual long        getDelay() ;
	virtual long        getId() ;
	virtual long        getLogLevel() ;
	virtual std::string getPostcondition() ;
	virtual std::string getPrecondition() ;
	virtual long        getProcesses() ;
	virtual std::string getStderr() ;
	virtual std::string getStdin() ;
	virtual std::string getStdout() ;
	virtual std::string getWorkingDir() ;
	virtual bool        isDebug() ;
	virtual bool        isRealtime() ;
	virtual bool        isTiming() ;
	virtual void        setDebug( bool val);
	virtual void        setDelay( const long val);
	virtual void        setId( const long val);
	virtual void        setLogLevel( const long val);
	virtual void        setPostcondition( const std::string& val);
	virtual void        setPrecondition( const std::string& val);
	virtual void        setProcesses( const long val);
	virtual void        setRealtime( bool val);
	virtual void        setStderr( const std::string& val);
	virtual void        setStdin( const std::string& val);
	virtual void        setStdout( const std::string& val);
	virtual void        setTiming( bool val);
	virtual void        setWorkingDir( const std::string& val);
	//
	// kind and role getters
	virtual std::set<KATS_BON::BarrierRef>  getBarrierRef();
	virtual std::set<KATS_BON::DomainRef>   getDomainRef();
	virtual std::set<KATS_BON::HostRef>     getHostRef();
	virtual std::set<KATS_BON::Kill>        getKill();

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace KATS_BON
{
//*******************************************************************
//   C  L  A  S  S   OrderedImpl
//*******************************************************************
class OrderedImpl :
	  virtual public BON::FCOImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( KATS_BON::KATSVisitor *pVisitor);

	//
	// attribute getters and setters
	virtual long        getOrder() ;
	virtual bool        isEnabled() ;
	virtual void        setEnabled( bool val);
	virtual void        setOrder( const long val);

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace KATS_BON
{
//*******************************************************************
//   C  L  A  S  S   GroupImpl
//*******************************************************************
class GroupImpl :
	  public ProcessBaseImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( KATS_BON::KATSVisitor *pVisitor);

	//
	// attribute getters and setters
	virtual bool        isParallel() ;
	virtual void        setParallel( bool val);
	//
	// kind and role getters
	virtual std::set<KATS_BON::Group>       getGroup();
	virtual std::set<KATS_BON::GroupRef>    getGroupRef();
	virtual std::set<KATS_BON::Observer>    getObserver();
	virtual std::set<KATS_BON::Ordered>     getOrdered();
	virtual std::set<KATS_BON::Process>     getProcess();
	virtual std::set<KATS_BON::Sleep>       getSleep();

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace KATS_BON
{
//*******************************************************************
//   C  L  A  S  S   ObserverImpl
//*******************************************************************
class ObserverImpl :
	  public OrderedImpl
	, public ProcessBaseImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( KATS_BON::KATSVisitor *pVisitor);


	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace KATS_BON
{
//*******************************************************************
//   C  L  A  S  S   ProcessImpl
//*******************************************************************
class ProcessImpl :
	  public OrderedImpl
	, public ProcessBaseImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( KATS_BON::KATSVisitor *pVisitor);

	//
	// attribute getters and setters
	virtual std::string getCommandLine() ;
	virtual std::string getExecutable() ;
	virtual void        setCommandLine( const std::string& val);
	virtual void        setExecutable( const std::string& val);

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace KATS_BON
{
//*******************************************************************
//   C  L  A  S  S   SleepImpl
//*******************************************************************
class SleepImpl :
	  public OrderedImpl
	, public ProcessBaseImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( KATS_BON::KATSVisitor *pVisitor);


	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace KATS_BON
{
//*******************************************************************
//   C  L  A  S  S   GroupRefImpl
//*******************************************************************
class GroupRefImpl :
	  virtual public BON::ReferenceImpl
	, public OrderedImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( KATS_BON::KATSVisitor *pVisitor);

	//
	// ref getters
	virtual KATS_BON::Group                 getGroup();

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace KATS_BON
{
//*******************************************************************
//   C  L  A  S  S   BarrierImpl
//*******************************************************************
class BarrierImpl :
	  virtual public BON::AtomImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( KATS_BON::KATSVisitor *pVisitor);

	//
	// attribute getters and setters
	virtual std::string getOverride() ;
	virtual void        setOverride( const std::string& val);

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace KATS_BON
{
//*******************************************************************
//   C  L  A  S  S   DomainImpl
//*******************************************************************
class DomainImpl :
	  virtual public BON::AtomImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( KATS_BON::KATSVisitor *pVisitor);

	//
	// attribute getters and setters
	virtual std::string getOverride() ;
	virtual void        setOverride( const std::string& val);

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace KATS_BON
{
//*******************************************************************
//   C  L  A  S  S   HostImpl
//*******************************************************************
class HostImpl :
	  virtual public BON::AtomImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( KATS_BON::KATSVisitor *pVisitor);

	//
	// attribute getters and setters
	virtual std::string getOverride() ;
	virtual void        setOverride( const std::string& val);

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace KATS_BON
{
//*******************************************************************
//   C  L  A  S  S   KillImpl
//*******************************************************************
class KillImpl :
	  virtual public BON::AtomImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( KATS_BON::KATSVisitor *pVisitor);

	//
	// attribute getters and setters
	virtual std::string getSignal() ;
	virtual long        getTime() ;
	virtual void        setSignal( const std::string& val);
	virtual void        setTime( const long val);

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace KATS_BON
{
//*******************************************************************
//   C  L  A  S  S   BarrierRefImpl
//*******************************************************************
class BarrierRefImpl :
	  virtual public BON::ReferenceImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( KATS_BON::KATSVisitor *pVisitor);

	//
	// ref getters
	virtual KATS_BON::Barrier               getBarrier();

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace KATS_BON
{
//*******************************************************************
//   C  L  A  S  S   DomainRefImpl
//*******************************************************************
class DomainRefImpl :
	  virtual public BON::ReferenceImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( KATS_BON::KATSVisitor *pVisitor);

	//
	// ref getters
	virtual KATS_BON::Domain                getDomain();

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace KATS_BON
{
//*******************************************************************
//   C  L  A  S  S   HostRefImpl
//*******************************************************************
class HostRefImpl :
	  virtual public BON::ReferenceImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( KATS_BON::KATSVisitor *pVisitor);

	//
	// ref getters
	virtual KATS_BON::Host                  getHost();

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


///BUP
// add your additional class definitions here
///EUP

#endif // KATSBONX_H
