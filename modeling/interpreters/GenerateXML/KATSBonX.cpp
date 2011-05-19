#include "stdafx.h"
#include "KATSBonX.h"
#include "KATSVisitor.h"


namespace BON
{

IMPLEMENT_BONEXTENSION( KATS_BON::Barriers, "Barriers" );
IMPLEMENT_BONEXTENSION( KATS_BON::Domains, "Domains" );
IMPLEMENT_BONEXTENSION( KATS_BON::Hosts, "Hosts" );
IMPLEMENT_BONEXTENSION( KATS_BON::Processes, "Processes" );
IMPLEMENT_ABSTRACT_BONEXTENSION( KATS_BON::Ordered );
IMPLEMENT_BONEXTENSION( KATS_BON::Process, "Process" );
IMPLEMENT_BONEXTENSION( KATS_BON::GroupRef, "GroupRef" );
IMPLEMENT_BONEXTENSION( KATS_BON::Group, "Group" );
IMPLEMENT_BONEXTENSION( KATS_BON::Barrier, "Barrier" );
IMPLEMENT_BONEXTENSION( KATS_BON::Domain, "Domain" );
IMPLEMENT_BONEXTENSION( KATS_BON::Host, "Host" );
IMPLEMENT_BONEXTENSION( KATS_BON::Kill, "Kill" );
IMPLEMENT_BONEXTENSION( KATS_BON::BarrierRef, "BarrierRef" );
IMPLEMENT_BONEXTENSION( KATS_BON::DomainRef, "DomainRef" );
IMPLEMENT_BONEXTENSION( KATS_BON::HostRef, "HostRef" );


} // namespace BON

//********************************************************************************
// 
//********************************************************************************
void KATS_BON::BarriersImpl::accept( KATS_BON::KATSVisitor *pVisitor)
{
	// visit the KATS_BON::Barriers
	pVisitor->visitBarriers( KATS_BON::Barriers( this));

	// then its children
	std::set<BON::Folder> subfolders = BON::FolderImpl::getChildFolders();
	for( std::set<BON::Folder>::const_iterator it = subfolders.begin(); it != subfolders.end(); ++it)
	{
		(*it)->accept( pVisitor);
	}

	std::set<BON::FCO> children = BON::FolderImpl::getRootFCOs();
	for( std::set<BON::FCO>::const_iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->accept( pVisitor);
	}
}


//********************************************************************************
// getter for kind "Barrier"
//********************************************************************************
std::set<KATS_BON::Barrier> KATS_BON::BarriersImpl::getBarrier()
{
	std::set<KATS_BON::Barrier> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("Barrier");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		KATS_BON::Barrier elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::DomainsImpl::accept( KATS_BON::KATSVisitor *pVisitor)
{
	// visit the KATS_BON::Domains
	pVisitor->visitDomains( KATS_BON::Domains( this));

	// then its children
	std::set<BON::Folder> subfolders = BON::FolderImpl::getChildFolders();
	for( std::set<BON::Folder>::const_iterator it = subfolders.begin(); it != subfolders.end(); ++it)
	{
		(*it)->accept( pVisitor);
	}

	std::set<BON::FCO> children = BON::FolderImpl::getRootFCOs();
	for( std::set<BON::FCO>::const_iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->accept( pVisitor);
	}
}


//********************************************************************************
// getter for kind "Domain"
//********************************************************************************
std::set<KATS_BON::Domain> KATS_BON::DomainsImpl::getDomain()
{
	std::set<KATS_BON::Domain> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("Domain");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		KATS_BON::Domain elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::HostsImpl::accept( KATS_BON::KATSVisitor *pVisitor)
{
	// visit the KATS_BON::Hosts
	pVisitor->visitHosts( KATS_BON::Hosts( this));

	// then its children
	std::set<BON::Folder> subfolders = BON::FolderImpl::getChildFolders();
	for( std::set<BON::Folder>::const_iterator it = subfolders.begin(); it != subfolders.end(); ++it)
	{
		(*it)->accept( pVisitor);
	}

	std::set<BON::FCO> children = BON::FolderImpl::getRootFCOs();
	for( std::set<BON::FCO>::const_iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->accept( pVisitor);
	}
}


//********************************************************************************
// getter for kind "Host"
//********************************************************************************
std::set<KATS_BON::Host> KATS_BON::HostsImpl::getHost()
{
	std::set<KATS_BON::Host> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("Host");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		KATS_BON::Host elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::ProcessesImpl::accept( KATS_BON::KATSVisitor *pVisitor)
{
	// visit the KATS_BON::Processes
	pVisitor->visitProcesses( KATS_BON::Processes( this));

	// then its children
	std::set<BON::Folder> subfolders = BON::FolderImpl::getChildFolders();
	for( std::set<BON::Folder>::const_iterator it = subfolders.begin(); it != subfolders.end(); ++it)
	{
		(*it)->accept( pVisitor);
	}

	std::set<BON::FCO> children = BON::FolderImpl::getRootFCOs();
	for( std::set<BON::FCO>::const_iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->accept( pVisitor);
	}
}


//********************************************************************************
// getter for kind "Group"
//********************************************************************************
std::set<KATS_BON::Group> KATS_BON::ProcessesImpl::getGroup()
{
	std::set<KATS_BON::Group> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("Group");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		KATS_BON::Group elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::OrderedImpl::accept( KATS_BON::KATSVisitor *pVisitor)
{
	// visit the KATS_BON::Ordered
	pVisitor->visitOrdered( KATS_BON::Ordered( this));
}


//********************************************************************************
// 
//********************************************************************************
long KATS_BON::OrderedImpl::getOrder() 
{
	return FCOImpl::getAttribute("Order")->getIntegerValue();
}


//********************************************************************************
// 
//********************************************************************************
bool KATS_BON::OrderedImpl::isEnabled() 
{
	return FCOImpl::getAttribute("Enabled")->getBooleanValue();
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::OrderedImpl::setEnabled( bool val)
{
	FCOImpl::getAttribute("Enabled")->setBooleanValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::OrderedImpl::setOrder( const long val)
{
	FCOImpl::getAttribute("Order")->setIntegerValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::ProcessImpl::accept( KATS_BON::KATSVisitor *pVisitor)
{
	// visit the KATS_BON::Process
	pVisitor->visitProcess( KATS_BON::Process( this));

	// then its children
	std::set<BON::FCO> children = ModelImpl::getChildFCOs();
	for( std::set<BON::FCO>::const_iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->accept( pVisitor);
	}
}


//********************************************************************************
// 
//********************************************************************************
std::string KATS_BON::ProcessImpl::getCommandLine() 
{
	return FCOImpl::getAttribute("CommandLine")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
long KATS_BON::ProcessImpl::getDelay() 
{
	return FCOImpl::getAttribute("Delay")->getIntegerValue();
}


//********************************************************************************
// 
//********************************************************************************
std::string KATS_BON::ProcessImpl::getExecutable() 
{
	return FCOImpl::getAttribute("Executable")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
long KATS_BON::ProcessImpl::getId() 
{
	return FCOImpl::getAttribute("Id")->getIntegerValue();
}


//********************************************************************************
// 
//********************************************************************************
std::string KATS_BON::ProcessImpl::getPostcondition() 
{
	return FCOImpl::getAttribute("Postcondition")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
std::string KATS_BON::ProcessImpl::getPrecondition() 
{
	return FCOImpl::getAttribute("Precondition")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
long KATS_BON::ProcessImpl::getProcesses() 
{
	return FCOImpl::getAttribute("Processes")->getIntegerValue();
}


//********************************************************************************
// 
//********************************************************************************
std::string KATS_BON::ProcessImpl::getStderr() 
{
	return FCOImpl::getAttribute("Stderr")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
std::string KATS_BON::ProcessImpl::getStdin() 
{
	return FCOImpl::getAttribute("Stdin")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
std::string KATS_BON::ProcessImpl::getStdout() 
{
	return FCOImpl::getAttribute("Stdout")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
std::string KATS_BON::ProcessImpl::getWorkingDir() 
{
	return FCOImpl::getAttribute("WorkingDir")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
bool KATS_BON::ProcessImpl::isDebug() 
{
	return FCOImpl::getAttribute("Debug")->getBooleanValue();
}


//********************************************************************************
// 
//********************************************************************************
bool KATS_BON::ProcessImpl::isRealtime() 
{
	return FCOImpl::getAttribute("Realtime")->getBooleanValue();
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::ProcessImpl::setCommandLine( const std::string& val)
{
	FCOImpl::getAttribute("CommandLine")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::ProcessImpl::setDebug( bool val)
{
	FCOImpl::getAttribute("Debug")->setBooleanValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::ProcessImpl::setDelay( const long val)
{
	FCOImpl::getAttribute("Delay")->setIntegerValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::ProcessImpl::setExecutable( const std::string& val)
{
	FCOImpl::getAttribute("Executable")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::ProcessImpl::setId( const long val)
{
	FCOImpl::getAttribute("Id")->setIntegerValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::ProcessImpl::setPostcondition( const std::string& val)
{
	FCOImpl::getAttribute("Postcondition")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::ProcessImpl::setPrecondition( const std::string& val)
{
	FCOImpl::getAttribute("Precondition")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::ProcessImpl::setProcesses( const long val)
{
	FCOImpl::getAttribute("Processes")->setIntegerValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::ProcessImpl::setRealtime( bool val)
{
	FCOImpl::getAttribute("Realtime")->setBooleanValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::ProcessImpl::setStderr( const std::string& val)
{
	FCOImpl::getAttribute("Stderr")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::ProcessImpl::setStdin( const std::string& val)
{
	FCOImpl::getAttribute("Stdin")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::ProcessImpl::setStdout( const std::string& val)
{
	FCOImpl::getAttribute("Stdout")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::ProcessImpl::setWorkingDir( const std::string& val)
{
	FCOImpl::getAttribute("WorkingDir")->setStringValue( val);
}


//********************************************************************************
// getter for role "BarrierRef" among "KATS_BON::BarrierRef"s
//********************************************************************************
std::set<KATS_BON::BarrierRef> KATS_BON::ProcessImpl::getBarrierRef()
{
	std::set<KATS_BON::BarrierRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("BarrierRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		KATS_BON::BarrierRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "DomainRef" among "KATS_BON::DomainRef"s
//********************************************************************************
std::set<KATS_BON::DomainRef> KATS_BON::ProcessImpl::getDomainRef()
{
	std::set<KATS_BON::DomainRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("DomainRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		KATS_BON::DomainRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "HostRef" among "KATS_BON::HostRef"s
//********************************************************************************
std::set<KATS_BON::HostRef> KATS_BON::ProcessImpl::getHostRef()
{
	std::set<KATS_BON::HostRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("HostRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		KATS_BON::HostRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Kill" among "KATS_BON::Kill"s
//********************************************************************************
std::set<KATS_BON::Kill> KATS_BON::ProcessImpl::getKill()
{
	std::set<KATS_BON::Kill> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("Kill");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		KATS_BON::Kill elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::GroupRefImpl::accept( KATS_BON::KATSVisitor *pVisitor)
{
	// visit the KATS_BON::GroupRef
	pVisitor->visitGroupRef( KATS_BON::GroupRef( this));
}


//********************************************************************************
// 
//********************************************************************************
KATS_BON::Group KATS_BON::GroupRefImpl::getGroup()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return KATS_BON::Group(r);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::GroupImpl::accept( KATS_BON::KATSVisitor *pVisitor)
{
	// visit the KATS_BON::Group
	pVisitor->visitGroup( KATS_BON::Group( this));

	// then its children
	std::set<BON::FCO> children = ModelImpl::getChildFCOs();
	for( std::set<BON::FCO>::const_iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->accept( pVisitor);
	}
}


//********************************************************************************
// 
//********************************************************************************
long KATS_BON::GroupImpl::getDelay() 
{
	return FCOImpl::getAttribute("Delay")->getIntegerValue();
}


//********************************************************************************
// 
//********************************************************************************
long KATS_BON::GroupImpl::getId() 
{
	return FCOImpl::getAttribute("Id")->getIntegerValue();
}


//********************************************************************************
// 
//********************************************************************************
std::string KATS_BON::GroupImpl::getPostcondition() 
{
	return FCOImpl::getAttribute("Postcondition")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
std::string KATS_BON::GroupImpl::getPrecondition() 
{
	return FCOImpl::getAttribute("Precondition")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
long KATS_BON::GroupImpl::getProcesses() 
{
	return FCOImpl::getAttribute("Processes")->getIntegerValue();
}


//********************************************************************************
// 
//********************************************************************************
std::string KATS_BON::GroupImpl::getStderr() 
{
	return FCOImpl::getAttribute("Stderr")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
std::string KATS_BON::GroupImpl::getStdin() 
{
	return FCOImpl::getAttribute("Stdin")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
std::string KATS_BON::GroupImpl::getStdout() 
{
	return FCOImpl::getAttribute("Stdout")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
bool KATS_BON::GroupImpl::isDebug() 
{
	return FCOImpl::getAttribute("Debug")->getBooleanValue();
}


//********************************************************************************
// 
//********************************************************************************
bool KATS_BON::GroupImpl::isParallel() 
{
	return FCOImpl::getAttribute("Parallel")->getBooleanValue();
}


//********************************************************************************
// 
//********************************************************************************
bool KATS_BON::GroupImpl::isRealtime() 
{
	return FCOImpl::getAttribute("Realtime")->getBooleanValue();
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::GroupImpl::setDebug( bool val)
{
	FCOImpl::getAttribute("Debug")->setBooleanValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::GroupImpl::setDelay( const long val)
{
	FCOImpl::getAttribute("Delay")->setIntegerValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::GroupImpl::setId( const long val)
{
	FCOImpl::getAttribute("Id")->setIntegerValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::GroupImpl::setParallel( bool val)
{
	FCOImpl::getAttribute("Parallel")->setBooleanValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::GroupImpl::setPostcondition( const std::string& val)
{
	FCOImpl::getAttribute("Postcondition")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::GroupImpl::setPrecondition( const std::string& val)
{
	FCOImpl::getAttribute("Precondition")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::GroupImpl::setProcesses( const long val)
{
	FCOImpl::getAttribute("Processes")->setIntegerValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::GroupImpl::setRealtime( bool val)
{
	FCOImpl::getAttribute("Realtime")->setBooleanValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::GroupImpl::setStderr( const std::string& val)
{
	FCOImpl::getAttribute("Stderr")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::GroupImpl::setStdin( const std::string& val)
{
	FCOImpl::getAttribute("Stdin")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::GroupImpl::setStdout( const std::string& val)
{
	FCOImpl::getAttribute("Stdout")->setStringValue( val);
}


//********************************************************************************
// getter for role "BarrierRef" among "KATS_BON::BarrierRef"s
//********************************************************************************
std::set<KATS_BON::BarrierRef> KATS_BON::GroupImpl::getBarrierRef()
{
	std::set<KATS_BON::BarrierRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("BarrierRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		KATS_BON::BarrierRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "DomainRef" among "KATS_BON::DomainRef"s
//********************************************************************************
std::set<KATS_BON::DomainRef> KATS_BON::GroupImpl::getDomainRef()
{
	std::set<KATS_BON::DomainRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("DomainRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		KATS_BON::DomainRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Group" among "KATS_BON::Group"s
//********************************************************************************
std::set<KATS_BON::Group> KATS_BON::GroupImpl::getGroup()
{
	std::set<KATS_BON::Group> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("Group");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		KATS_BON::Group elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "GroupRef" among "Ordered"s and its descendants
//********************************************************************************
std::set<KATS_BON::GroupRef> KATS_BON::GroupImpl::getGroupRef()
{
	std::set<KATS_BON::GroupRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("GroupRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		KATS_BON::GroupRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "HostRef" among "KATS_BON::HostRef"s
//********************************************************************************
std::set<KATS_BON::HostRef> KATS_BON::GroupImpl::getHostRef()
{
	std::set<KATS_BON::HostRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("HostRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		KATS_BON::HostRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Kill" among "KATS_BON::Kill"s
//********************************************************************************
std::set<KATS_BON::Kill> KATS_BON::GroupImpl::getKill()
{
	std::set<KATS_BON::Kill> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("Kill");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		KATS_BON::Kill elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "KATS_BON::" among "KATS_BON::Ordered"s and its descendants
//********************************************************************************
std::set<KATS_BON::Ordered> KATS_BON::GroupImpl::getOrdered()
{
	std::set<KATS_BON::Ordered> res;
	const int len = 2;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = ModelImpl::getChildFCOsAs("Process");
	roles_vec[1] = ModelImpl::getChildFCOsAs("GroupRef");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			KATS_BON::Ordered elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "Process" among "Ordered"s and its descendants
//********************************************************************************
std::set<KATS_BON::Process> KATS_BON::GroupImpl::getProcess()
{
	std::set<KATS_BON::Process> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("Process");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		KATS_BON::Process elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::BarrierImpl::accept( KATS_BON::KATSVisitor *pVisitor)
{
	// visit the KATS_BON::Barrier
	pVisitor->visitBarrier( KATS_BON::Barrier( this));
}


//********************************************************************************
// 
//********************************************************************************
std::string KATS_BON::BarrierImpl::getOverride() 
{
	return FCOImpl::getAttribute("Override")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::BarrierImpl::setOverride( const std::string& val)
{
	FCOImpl::getAttribute("Override")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::DomainImpl::accept( KATS_BON::KATSVisitor *pVisitor)
{
	// visit the KATS_BON::Domain
	pVisitor->visitDomain( KATS_BON::Domain( this));
}


//********************************************************************************
// 
//********************************************************************************
std::string KATS_BON::DomainImpl::getOverride() 
{
	return FCOImpl::getAttribute("Override")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::DomainImpl::setOverride( const std::string& val)
{
	FCOImpl::getAttribute("Override")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::HostImpl::accept( KATS_BON::KATSVisitor *pVisitor)
{
	// visit the KATS_BON::Host
	pVisitor->visitHost( KATS_BON::Host( this));
}


//********************************************************************************
// 
//********************************************************************************
std::string KATS_BON::HostImpl::getOverride() 
{
	return FCOImpl::getAttribute("Override")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::HostImpl::setOverride( const std::string& val)
{
	FCOImpl::getAttribute("Override")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::KillImpl::accept( KATS_BON::KATSVisitor *pVisitor)
{
	// visit the KATS_BON::Kill
	pVisitor->visitKill( KATS_BON::Kill( this));
}


//********************************************************************************
// 
//********************************************************************************
std::string KATS_BON::KillImpl::getSignal() 
{
	return FCOImpl::getAttribute("Signal")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
long KATS_BON::KillImpl::getTime() 
{
	return FCOImpl::getAttribute("Time")->getIntegerValue();
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::KillImpl::setSignal( const std::string& val)
{
	FCOImpl::getAttribute("Signal")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::KillImpl::setTime( const long val)
{
	FCOImpl::getAttribute("Time")->setIntegerValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::BarrierRefImpl::accept( KATS_BON::KATSVisitor *pVisitor)
{
	// visit the KATS_BON::BarrierRef
	pVisitor->visitBarrierRef( KATS_BON::BarrierRef( this));
}


//********************************************************************************
// 
//********************************************************************************
KATS_BON::Barrier KATS_BON::BarrierRefImpl::getBarrier()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return KATS_BON::Barrier(r);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::DomainRefImpl::accept( KATS_BON::KATSVisitor *pVisitor)
{
	// visit the KATS_BON::DomainRef
	pVisitor->visitDomainRef( KATS_BON::DomainRef( this));
}


//********************************************************************************
// 
//********************************************************************************
KATS_BON::Domain KATS_BON::DomainRefImpl::getDomain()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return KATS_BON::Domain(r);
}


//********************************************************************************
// 
//********************************************************************************
void KATS_BON::HostRefImpl::accept( KATS_BON::KATSVisitor *pVisitor)
{
	// visit the KATS_BON::HostRef
	pVisitor->visitHostRef( KATS_BON::HostRef( this));
}


//********************************************************************************
// 
//********************************************************************************
KATS_BON::Host KATS_BON::HostRefImpl::getHost()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return KATS_BON::Host(r);
}


