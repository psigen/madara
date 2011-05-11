//###############################################################################################################################################
//
//	Meta and Builder Object Network V2.0 for GME
//	BON2Component.cpp
//
//###############################################################################################################################################

/*
	Copyright (c) Vanderbilt University, 2000-2004
	ALL RIGHTS RESERVED

	Vanderbilt University disclaims all warranties with regard to this
	software, including all implied warranties of merchantability
	and fitness.  In no event shall Vanderbilt University be liable for
	any special, indirect or consequential damages or any damages
	whatsoever resulting from loss of use, data or profits, whether
	in an action of contract, negligence or other tortious action,
	arising out of or in connection with the use or performance of
	this software.
*/

#include "stdafx.h"
#include <sstream>
#include <vector>
#include <algorithm>
#include <Console.h>
#include "BON2Component.h"
#include "../Utility/tinyxml.h"


// I have to paste this into KATSBonX.h/cpp whenever I change
// the paradigm
//namespace KATS_BON
//{
//  class Process
//  {
//  public:
//    bool operator< (const Process & right)
//    {
//      if (this->getOrder () < right->getOrder ())
//        return true;
//
//      return this->getId () < right->getId ();
//    }
//  };
//}


namespace BON
{


  bool Comparison (KATS_BON::Process & lhs,
    KATS_BON::Process & rhs)
  {
    return (lhs->getOrder () < rhs->getOrder ());
  }


//###############################################################################################################################################
//
// 	C L A S S : BON::Component
//
//###############################################################################################################################################

Component::Component()
	: m_bIsInteractive( false )
{
}

Component::~Component()
{
	if ( m_project ) {
		m_project->finalizeObjects();
		finalize( m_project );
		m_project = NULL;
	}
}

// ====================================================
// This method is called after all the generic initialization is done
// This should be empty unless application-specific initialization is needed

void Component::initialize( Project& project )
{
	// ======================
	// Insert application specific code here
#ifdef GME_ADDON
	project->setAutoCommit( false);
#endif
}

// ====================================================
// This method is called before the whole BON2 project released and disposed
// This should be empty unless application-specific finalization is needed

void Component::finalize( Project& project )
{
	// ======================
	// Insert application specific code here
}

// ====================================================
// This is the obsolete component interface
// This present implementation either tries to call InvokeEx, or does nothing except of a notification

void Component::invoke( Project& project, const std::set<FCO>& setModels, long lParam )
{
	#ifdef SUPPORT_OLD_INVOKE
		Object focus;
		invokeEx( project, focus, setModels, lParam );
	#else
		if ( m_bIsInteractive )
			AfxMessageBox("This BON2 Component does not support the obsolete invoke mechanism!");
	#endif
}

void Component::process_process (KATS_BON::Process & current,
                                 TiXmlElement & parent)
{
  TiXmlElement xml_setup ("process");
  // did the user set debug mode?
  if (current->isDebug ())
  {
    TiXmlElement element ("debug");
    xml_setup.InsertEndChild (element);
  }
  
  // did the user set realtime scheduling?
  if (current->isRealtime ())
  {
    TiXmlElement element ("realtime");
    xml_setup.InsertEndChild (element);
  }

  // get the host name
  std::set<KATS_BON::HostRef> hosts = current->getHostRef ();
  std::set<KATS_BON::HostRef>::iterator hbegin = hosts.begin();
  if (hbegin != hosts.end ())
  {
    KATS_BON::HostRef href = *hbegin;
    KATS_BON::Host host = href->getHost ();

    Util::GenRefCounted * ref_ptr = host.getCounted (false);

    TiXmlElement element ("host");

    // check to see if the user initialized the reference to an
    // actual instance. If not, we use the reference's name as
    // a convenience
    if (ref_ptr)
    {
      // we have a real reference
      TiXmlText text (host->getName ());
      if (host->getOverride () != "")
      {
        text.SetValue (host->getOverride ());
      }
      element.InsertEndChild (text);
    }
    else
    {
      TiXmlText text (href->getName ());
      element.InsertEndChild (text);
    }
    xml_setup.InsertEndChild (element);
  }

  // get the domain name
  std::set<KATS_BON::DomainRef> domains = current->getDomainRef ();
  std::set<KATS_BON::DomainRef>::iterator dbegin = domains.begin();
  if (dbegin != domains.end ())
  {
    KATS_BON::DomainRef dref = *dbegin;
    KATS_BON::Domain domain = dref->getDomain ();

    Util::GenRefCounted * ref_ptr = domain.getCounted (false);

    TiXmlElement element ("domain");

    // check to see if the user initialized the reference to an
    // actual instance. If not, we use the reference's name as
    // a convenience
    if (ref_ptr)
    {
      // we have a real reference
      TiXmlText text (domain->getName ());
      if (domain->getOverride () != "")
      {
        text.SetValue (domain->getOverride ());
      }
      element.InsertEndChild (text);
    }
    else
    {
      TiXmlText text (dref->getName ());
      element.InsertEndChild (text);
    }
    xml_setup.InsertEndChild (element);
  }

  // get the barrier name
  std::set<KATS_BON::BarrierRef> barriers = current->getBarrierRef ();
  std::set<KATS_BON::BarrierRef>::iterator bbegin = barriers.begin();
  if (bbegin != barriers.end ())
  {
    KATS_BON::BarrierRef bref = *bbegin;
    KATS_BON::Barrier barrier = bref->getBarrier ();

    Util::GenRefCounted * ref_ptr = barrier.getCounted (false);

    TiXmlElement element ("barrier");

    // check to see if the user initialized the reference to an
    // actual instance. If not, we use the reference's name as
    // a convenience
    if (ref_ptr)
    {
      // we have a real reference
      TiXmlText text (barrier->getName ());
      if (barrier->getOverride () != "")
      {
        text.SetValue (barrier->getOverride ());
      }
      element.InsertEndChild (text);
    }
    else
    {
      TiXmlText text (bref->getName ());
      element.InsertEndChild (text);
    }
    xml_setup.InsertEndChild (element);
  }
 
  // did the user set an id in the process ring?
  if (current->getId () >= 0)
  {
    std::stringstream buffer;
    buffer << current->getId ();

    TiXmlElement element ("id");
    TiXmlText text (buffer.str ());
    element.InsertEndChild (text);

    xml_setup.InsertEndChild (element);
  }

  // did the user set the number of processes in the barrier?
  if (current->getProcesses () >= 1)
  {
    std::stringstream buffer;
    buffer << current->getProcesses ();

    TiXmlElement element ("processes");
    TiXmlText text (buffer.str ());
    element.InsertEndChild (text);

    xml_setup.InsertEndChild (element);
  }

  // did the user set a delay?
  if (current->getDelay () >= 1)
  {
    std::stringstream buffer;
    buffer << current->getDelay ();

    TiXmlElement element ("delay");
    TiXmlText text (buffer.str ());
    element.InsertEndChild (text);

    xml_setup.InsertEndChild (element);
  }
  
  // did the user set a precondition?
  if (current->getPrecondition () != "")
  {
    TiXmlElement element ("precondition");
    TiXmlText text (current->getPrecondition ());
    element.InsertEndChild (text);
    xml_setup.InsertEndChild (element);
  }

  // did the user set a postcondition?
  if (current->getPostcondition () != "")
  {
    TiXmlElement element ("postcondition");
    TiXmlText text (current->getPostcondition ());
    element.InsertEndChild (text);
    xml_setup.InsertEndChild (element);
  }

  // what is the executable?
  if (current->getExecutable () != "")
  {
    TiXmlElement element ("executable");
    TiXmlText text (current->getExecutable ());
    element.InsertEndChild (text);
    xml_setup.InsertEndChild (element);
  }

  // what is the working directory?
  if (current->getWorkingDir () != "")
  {
    TiXmlElement element ("workingdir");
    TiXmlText text (current->getWorkingDir ());
    element.InsertEndChild (text);
    xml_setup.InsertEndChild (element);
  }

  // what is the working directory?
  if (current->getCommandLine () != "")
  {
    TiXmlElement element ("commandline");
    TiXmlText text (current->getCommandLine ());
    element.InsertEndChild (text);
    xml_setup.InsertEndChild (element);
  }

  // get the kill settings
  std::set<KATS_BON::Kill> kills = current->getKill ();
  std::set<KATS_BON::Kill>::iterator kbegin = kills.begin();
  if (kbegin != kills.end ())
  {
    KATS_BON::Kill kill = *kbegin;
    if (kill->getTime () > 0)
    {
      TiXmlElement element ("kill");
      TiXmlElement time ("time");
      TiXmlElement signal ("signal");

      std::stringstream time_buff;
      std::stringstream signal_buff;

      time_buff << kill->getTime ();
      signal_buff << kill->getSignal ();

      TiXmlText time_text (time_buff.str ());
      TiXmlText signal_text (signal_buff.str ());

      time.InsertEndChild (time_text);
      signal.InsertEndChild (signal_text);

      element.InsertEndChild (time);
      element.InsertEndChild (signal);
      xml_setup.InsertEndChild (element);
    }
  }

  // did the user set a stdin redirect?
  if (current->getStdin () != "")
  {
    TiXmlElement element ("stdin");
    TiXmlText text (current->getStdin ());
    element.InsertEndChild (text);
    xml_setup.InsertEndChild (element);
  }

  // did the user set a stderr redirect?
  if (current->getStderr () != "")
  {
    TiXmlElement element ("stderr");
    TiXmlText text (current->getStderr ());
    element.InsertEndChild (text);
    xml_setup.InsertEndChild (element);
  }

  // did the user set a stdout redirect?
  if (current->getStdout () != "")
  {
    TiXmlElement element ("stdout");

    TiXmlText text (current->getStdout ());
    element.InsertEndChild (text);

    xml_setup.InsertEndChild (element);
  }

  parent.InsertEndChild (xml_setup);
}

void Component::process_process_group (KATS_BON::Group & current)
{
	using namespace GMEConsole;
  Console::Out::WriteLine(CString ("......Processing the process group ") + 
      current->getName ().c_str () + "...");

  TiXmlElement xml_group ("group");
  TiXmlElement xml_setup ("setup");

  // did the user set debug mode?
  if (current->isDebug ())
  {
    TiXmlElement element ("debug");
    xml_setup.InsertEndChild (element);
  }
  
  // did the user set parallel process launch?
  if (current->isParallel ())
  {
    TiXmlElement element ("parallel");
    xml_setup.InsertEndChild (element);
  }

  // did the user set realtime scheduling?
  if (current->isRealtime ())
  {
    TiXmlElement element ("realtime");
    xml_setup.InsertEndChild (element);
  }

  // get the host name
  std::set<KATS_BON::HostRef> hosts = current->getHostRef ();
  std::set<KATS_BON::HostRef>::iterator hbegin = hosts.begin();
  if (hbegin != hosts.end ())
  {
    KATS_BON::HostRef href = *hbegin;
    KATS_BON::Host host = href->getHost ();

    Util::GenRefCounted * ref_ptr = host.getCounted (false);

    TiXmlElement element ("host");

    // check to see if the user initialized the reference to an
    // actual instance. If not, we use the reference's name as
    // a convenience
    if (ref_ptr)
    {
      // we have a real reference
      TiXmlText text (host->getName ());
      if (host->getOverride () != "")
      {
        text.SetValue (host->getOverride ());
      }
      element.InsertEndChild (text);
    }
    else
    {
      TiXmlText text (href->getName ());
      element.InsertEndChild (text);
    }
    xml_setup.InsertEndChild (element);
  }

  // get the domain name
  std::set<KATS_BON::DomainRef> domains = current->getDomainRef ();
  std::set<KATS_BON::DomainRef>::iterator dbegin = domains.begin();
  if (dbegin != domains.end ())
  {
    KATS_BON::DomainRef dref = *dbegin;
    KATS_BON::Domain domain = dref->getDomain ();

    Util::GenRefCounted * ref_ptr = domain.getCounted (false);

    TiXmlElement element ("domain");

    // check to see if the user initialized the reference to an
    // actual instance. If not, we use the reference's name as
    // a convenience
    if (ref_ptr)
    {
      // we have a real reference
      TiXmlText text (domain->getName ());
      if (domain->getOverride () != "")
      {
        text.SetValue (domain->getOverride ());
      }
      element.InsertEndChild (text);
    }
    else
    {
      TiXmlText text (dref->getName ());
      element.InsertEndChild (text);
    }
    xml_setup.InsertEndChild (element);
  }

  // get the barrier name
  std::set<KATS_BON::BarrierRef> barriers = current->getBarrierRef ();
  std::set<KATS_BON::BarrierRef>::iterator bbegin = barriers.begin();
  if (bbegin != barriers.end ())
  {
    KATS_BON::BarrierRef bref = *bbegin;
    KATS_BON::Barrier barrier = bref->getBarrier ();

    Util::GenRefCounted * ref_ptr = barrier.getCounted (false);

    TiXmlElement element ("barrier");

    // check to see if the user initialized the reference to an
    // actual instance. If not, we use the reference's name as
    // a convenience
    if (ref_ptr)
    {
      // we have a real reference
      TiXmlText text (barrier->getName ());
      if (barrier->getOverride () != "")
      {
        text.SetValue (barrier->getOverride ());
      }
      element.InsertEndChild (text);
    }
    else
    {
      TiXmlText text (bref->getName ());
      element.InsertEndChild (text);
    }
    xml_setup.InsertEndChild (element);
  }
 
  // did the user set an id in the process ring?
  if (current->getId () >= 0)
  {
    std::stringstream buffer;
    buffer << current->getId ();

    TiXmlElement element ("id");
    TiXmlText text (buffer.str ());
    element.InsertEndChild (text);

    xml_setup.InsertEndChild (element);
  }

  // did the user set the number of processes in the barrier?
  if (current->getProcesses () >= 1)
  {
    std::stringstream buffer;
    buffer << current->getProcesses ();

    TiXmlElement element ("processes");
    TiXmlText text (buffer.str ());
    element.InsertEndChild (text);

    xml_setup.InsertEndChild (element);
  }
 
  // did the user set a delay?
  if (current->getDelay () >= 1)
  {
    std::stringstream buffer;
    buffer << current->getDelay ();

    TiXmlElement element ("delay");
    TiXmlText text (buffer.str ());
    element.InsertEndChild (text);

    xml_setup.InsertEndChild (element);
  }
 
  // did the user set a precondition?
  if (current->getPrecondition () != "")
  {
    TiXmlElement element ("precondition");
    TiXmlText text (current->getPrecondition ());
    element.InsertEndChild (text);
    xml_setup.InsertEndChild (element);
  }

  // did the user set a postcondition?
  if (current->getPostcondition () != "")
  {
    TiXmlElement element ("postcondition");
    TiXmlText text (current->getPostcondition ());
    element.InsertEndChild (text);
    xml_setup.InsertEndChild (element);
  }

  // get the kill settings
  std::set<KATS_BON::Kill> kills = current->getKill ();
  std::set<KATS_BON::Kill>::iterator kbegin = kills.begin();
  if (kbegin != kills.end ())
  {
    KATS_BON::Kill kill = *kbegin;
    if (kill->getTime () > 0)
    {
      TiXmlElement element ("kill");
      TiXmlElement time ("time");
      TiXmlElement signal ("signal");

      std::stringstream time_buff;
      std::stringstream signal_buff;

      time_buff << kill->getTime ();
      signal_buff << kill->getSignal ();

      TiXmlText time_text (time_buff.str ());
      TiXmlText signal_text (signal_buff.str ());

      time.InsertEndChild (time_text);
      signal.InsertEndChild (signal_text);

      element.InsertEndChild (time);
      element.InsertEndChild (signal);
      xml_setup.InsertEndChild (element);
    }
  }

  // did the user set a stdin redirect?
  if (current->getStdin () != "")
  {
    TiXmlElement element ("stdin");
    TiXmlText text (current->getStdin ());
    element.InsertEndChild (text);
    xml_setup.InsertEndChild (element);
  }

  // did the user set a stderr redirect?
  if (current->getStderr () != "")
  {
    TiXmlElement element ("stderr");
    TiXmlText text (current->getStderr ());
    element.InsertEndChild (text);
    xml_setup.InsertEndChild (element);
  }

  // did the user set a stdout redirect?
  if (current->getStdout () != "")
  {
    TiXmlElement element ("stdout");

    TiXmlText text (current->getStdout ());
    element.InsertEndChild (text);

    xml_setup.InsertEndChild (element);
  }

  xml_group.InsertEndChild (xml_setup);

  // iterate through all of the processes

  std::set <KATS_BON::Process> processes = current->getProcess ();

  std::vector <KATS_BON::Process> ordered_processes;

  for (std::set <KATS_BON::Process>::iterator process_i = processes.begin ();
                        process_i != processes.end (); ++process_i)
  {
    //process_process (*process_i, xml_group);
    ordered_processes.push_back (*process_i);
  }

  std::sort (ordered_processes.begin (), ordered_processes.end (),
    Comparison);

  for (size_t i = 0; i < ordered_processes.size (); ++i)
    process_process (ordered_processes[i], xml_group);

  // check our host information
  //std::set<KATS_BON::HostRef>::iterator i = group->getHostRef ().begin ();
  //if (i != group->getHostRef ().end ())
  //{
  //  KATS_BON::HostRef href = *i;
  //  KATS_BON::Host host = href->getHost ();

  //  TiXmlElement element ("host");
  //  TiXmlText text (host->getName ());
  //  element.InsertEndChild (text);

  //  xml_group.InsertEndChild (element);
  //}

  // write the file out
  {
    // create an xml directory
    ::CreateDirectory ("xml", 0);
    std::stringstream buffer;
    buffer << "xml\\";
    buffer << current->getName ();
    buffer << ".xml";

    TiXmlDocument xml_doc (buffer.str ());
    xml_doc.InsertEndChild (xml_group);

    Console::Out::WriteLine(CString ("......Writing to file ") + 
      buffer.str ().c_str ());

    xml_doc.SaveFile (buffer.str ());
  }
}

void Component::process_processes_folder (KATS_BON::Processes & process_folder)
{
	using namespace GMEConsole;
	Console::Out::WriteLine("....Iterating through process groups...");
  std::set<KATS_BON::Group> groups = process_folder->getGroup ();
  for (std::set<KATS_BON::Group>::iterator group = groups.begin(); group != groups.end(); ++group)
  {
    process_process_group (*group);
  }
}

// ====================================================
// This is the main component method for Interpereters and Plugins.
// May also be used in case of invokeable Add-Ons

void Component::invokeEx( Project& project, FCO& currentFCO, const std::set<FCO>& setSelectedFCOs, long lParam )
{
#ifdef GME_ADDON
	project->setAutoCommit( false);
#endif
	using namespace GMEConsole;
	Console::Out::WriteLine("Processing KATS model...");
	// ======================
	// TODO: Insert application specific code here

	Console::Out::WriteLine("..Iterating through Processes folders...");

  std::set<BON::Folder> rootFolders = project->getRootFolder ()->getChildFolders ();
  for (std::set<BON::Folder>::iterator folder = rootFolders.begin(); folder != rootFolders.end(); ++folder)
  {
    if ((*folder)->getFolderMeta ().name () == "Processes")
    {
      KATS_BON::Processes process_folder = (*folder);
      Console::Out::WriteLine(CString ("....Processing ") +
        process_folder->getName ().c_str () + " folder...");
      process_processes_folder (process_folder);
    }
  }

	Console::Out::WriteLine("Finished processing KATS model...");
}

// ====================================================
// GME currently does not use this function
// You only need to implement it if other invokation mechanisms are used

void Component::objectInvokeEx( Project& project, Object& currentObject, const std::set<Object>& setSelectedObjects, long lParam )
	{
		if ( m_bIsInteractive )
			AfxMessageBox("This BON2 Component does not support objectInvokeEx method!");
	}

// ====================================================
// Implement application specific parameter-mechanism in these functions

Util::Variant Component::getParameter( const std::string& strName )
{
	// ======================
	// Insert application specific code here

	return Util::Variant();
}

void Component::setParameter( const std::string& strName, const Util::Variant& varValue )
{
	// ======================
	// Insert application specific code here
}

#ifdef GME_ADDON

// ====================================================
// If the component is an Add-On, then this method is called for every Global Event

void Component::globalEventPerformed( globalevent_enum event )
{
	// ======================
	// Insert application specific code here
}

// ====================================================
// If the component is an Add-On, then this method is called for every Object Event

void Component::objectEventPerformed( Object& object, unsigned long event, VARIANT v )
{
	// ======================
	// Insert application specific code here
}

#endif // GME_ADDON

}; // namespace BON

