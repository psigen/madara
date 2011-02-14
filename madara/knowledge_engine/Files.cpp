#ifndef _MADARA_KNOWLEDGE_BASE_FILES_CPP_
#define _MADARA_KNOWLEDGE_BASE_FILES_CPP_

#include "Files.h"
#include <sstream>
#include "ace/Mem_Map.h"
#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"
//#include <fcntl.h>

const std::string 
  Madara::Knowledge_Engine::Files::files_folder_ = "files";
const std::string 
  Madara::Knowledge_Engine::Files::policies_folder_ = "policies";

/// Constructor
Madara::Knowledge_Engine::Files::Files (Thread_Safe_Context & map) :
 map_(map)
{
}

/// Shape a file according to a shaping policy
int 
Madara::Knowledge_Engine::Files::shape_file (
  const std::string & source_key, const std::string & dest_key,
  const std::string & policy_key)
{
  // Stringstreams for building the name of the files
  std::stringstream source_file_name;
  std::stringstream dest_file_name;

  // using ACE for writing to the destination file
  ACE_FILE_IO dest_file;
  ACE_FILE_Connector connector;

  // build source file name
  source_file_name << files_folder_;
  source_file_name << "/";
  source_file_name << source_key;

  // build dest file name
  dest_file_name << files_folder_;
  dest_file_name << "/";
  dest_file_name << dest_key;

  ACE_DEBUG ((LM_DEBUG, "Files::shape_file %s->%s"));
  // load the source file into a mapped file of the OS's 
  // virtual memory system
  ACE_Mem_Map mapped_file (source_file_name.str ().c_str ());
  void * file_contents = mapped_file.addr ();
  size_t size = mapped_file.size ();

  if (size > 0)
  {
    // create and connect to the destination file
    connector.connect (dest_file, ACE_FILE_Addr (dest_file_name.str (). c_str ()),
      0, ACE_Addr::sap_any, 0, O_WRONLY | O_CREAT, ACE_DEFAULT_FILE_PERMS);
    
    // write the file contents to the dest_file_name
    dest_file.recv_n (file_contents, size);
    dest_file.close ();
    map_.set (dest_key, (long) size);
  }

  // unmap the memory mapped file
  mapped_file.close_filemapping_handle ();
  mapped_file.close ();

  return 0;
}

/// Read a file into the knowledge base
int 
Madara::Knowledge_Engine::Files::read_file (
  const std::string & file_name, const std::string & knowledge_key)
{
  // return value for function
  int ret = 0;

  // Stringstreams for building the name of the files
  std::stringstream source_file_name;
  std::stringstream dest_file_name;

  // build source file name
  source_file_name << file_name;

  // build dest file name
  dest_file_name << files_folder_;
  dest_file_name << "/";
  dest_file_name << knowledge_key;

  ACE_DEBUG ((LM_DEBUG, "Files::read_file : %s->%s\n", 
    source_file_name.str ().c_str (), dest_file_name.str ().c_str ()));

  // load the source file into a mapped file of the OS's 
  // virtual memory system
  ACE_Mem_Map mapped_file (source_file_name.str ().c_str ());
  void * file_contents = mapped_file.addr ();
  size_t size = mapped_file.size ();

  // using ACE for writing to the destination file
  ACE_FILE_IO dest_file;
  ACE_FILE_Connector connector;

  ACE_DEBUG ((LM_DEBUG, "Files::read_file : file size is %d, loaded at 0x%x\n", size, file_contents));

  if (size > 0)
  {
    // write the file contents to the dest_file_name
    connector.connect (dest_file, ACE_FILE_Addr (dest_file_name.str ().c_str ()),
      0, ACE_Addr::sap_any, 0, O_RDWR | O_CREAT, ACE_DEFAULT_FILE_PERMS);

    ACE_DEBUG ((LM_DEBUG, "Files::read_file : beginning recv_n of %d bytes\n", size));

    ssize_t actual = dest_file.recv (file_contents, size);
    dest_file.close ();

    if (actual > 0)
    {
      map_.set (knowledge_key, (long) size);

      ACE_DEBUG ((LM_DEBUG, "Files::read_file : file saved with %d bytes\n", actual));
    }
    else ret = -2;
  }
  else ret = -1;

  // unmap the memory mapped file
  mapped_file.close_filemapping_handle ();
  mapped_file.close ();

  if (ret < 0)
  {
    ACE_DEBUG ((LM_DEBUG, "Files::read_file : read file failed with %d\n", ret));
  }

  return ret;
}

/// Write a file from the knowledge base to a specified location
int 
Madara::Knowledge_Engine::Files::write_file (
  const std::string & file_name, const std::string & knowledge_key)
{
  return 0;
}

/// Read a policy into the knowledge base
int 
Madara::Knowledge_Engine::Files::read_policy (
  const std::string & policy_key, const std::string & policy_file)
{
  return 0;
}

#endif