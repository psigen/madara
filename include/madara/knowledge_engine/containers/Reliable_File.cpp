#include "Reliable_File.h"
#include "madara/transport/Fragmentation.h"

Madara::Knowledge_Engine::Containers::Reliable_File::Reliable_File (
  const Knowledge_Update_Settings & settings)
: knowledge_ (0), settings_ (settings),
  max_frag_size_ (62000), id_ (0), processes_ (2)
{
}
   

Madara::Knowledge_Engine::Containers::Reliable_File::Reliable_File (
  const std::string & name,
  Knowledge_Base & knowledge,
  const Knowledge_Update_Settings & settings)
: knowledge_ (&knowledge), name_ (name), settings_ (settings),
  max_frag_size_ (62000), id_ (0), processes_ (2)
{
  fragments_.set_name (name, knowledge);
}

Madara::Knowledge_Engine::Containers::Reliable_File::Reliable_File (
  const std::string & name,
  Knowledge_Base & knowledge,
  const std::string & filename,
  const Knowledge_Update_Settings & settings)
: knowledge_ (&knowledge), name_ (name), settings_ (settings),
  max_frag_size_ (62000), id_ (0), processes_ (2)
{
  fragments_.set_name (name, knowledge);
  read (filename);
}

Madara::Knowledge_Engine::Containers::Reliable_File::Reliable_File (const Reliable_File & rhs)
  : knowledge_ (rhs.knowledge_),
    name_ (rhs.name_),
    file_ (rhs.file_), size_ (rhs.size_), max_frag_size_ (rhs.max_frag_size_),
    id_ (rhs.id_), processes_ (rhs.processes_),
    fragments_ (rhs.fragments_), acks_ (rhs.acks_),
    settings_ (rhs.settings_)
{

}


Madara::Knowledge_Engine::Containers::Reliable_File::~Reliable_File ()
{

}

void
Madara::Knowledge_Engine::Containers::Reliable_File::operator= (const Reliable_File & rhs)
{
  if (this != &rhs)
  {
    this->knowledge_ = rhs.knowledge_;
    this->name_ = rhs.name_;
    this->file_ = rhs.file_;
    this->size_ = rhs.size_;
    this->max_frag_size_ = rhs.max_frag_size_;
    this->id_ = rhs.id_;
    this->processes_ = rhs.processes_;
    this->fragments_ = rhs.fragments_;
    this->acks_ = rhs.acks_;
    this->settings_ = rhs.settings_;
  }
}

void
Madara::Knowledge_Engine::Containers::Reliable_File::exchange (Reliable_File & other)
{
  Guard guard (mutex_), guard2 (other.mutex_);

  char * temp_file = other.file_;
  other.file_ = file_;
  file_ = temp_file;

  fragments_.exchange (other.fragments_);
}

std::string
Madara::Knowledge_Engine::Containers::Reliable_File::get_name (void) const
{
  Guard guard (mutex_);
  return name_;
}

void
Madara::Knowledge_Engine::Containers::Reliable_File::set_name (
  const std::string & var_name,
  Knowledge_Base & knowledge)
{
  knowledge_ = &knowledge;
  name_ = var_name;
  fragments_.set_name (name_, knowledge);
}

bool
Madara::Knowledge_Engine::Containers::Reliable_File::operator== (
  const Reliable_File & value) const
{
  Guard guard (mutex_);
  
  return file_ == value.file_;
}

bool
Madara::Knowledge_Engine::Containers::Reliable_File::operator != (
  const Reliable_File & value) const
{
  return !(*this == value);
}

void
Madara::Knowledge_Engine::Containers::Reliable_File::set_id (size_t id)
{
  id_ = id;
}

void
Madara::Knowledge_Engine::Containers::Reliable_File::set_processes (
  size_t processes)
{
  processes_ = processes;
}

size_t
Madara::Knowledge_Engine::Containers::Reliable_File::get_id (void)
{
  return id_;
}

size_t
Madara::Knowledge_Engine::Containers::Reliable_File::get_processes (void)
{
  return processes_;
}

int
Madara::Knowledge_Engine::Containers::Reliable_File::read (
  const std::string & filename)
{
  void * buffer;
  Transport::Fragment_Map map;
  Madara::Utility::read_file (filename, buffer, size_);
  file_ = static_cast <char *> (buffer);

  split ();
  create_acks ();

  return 0;
}

void
Madara::Knowledge_Engine::Containers::Reliable_File::clear_frags (void)
{

}

void
Madara::Knowledge_Engine::Containers::Reliable_File::split (void)
{
  size_t frags = size_ / max_frag_size_;
  bool extra = false;
  size_t i = 0;
  unsigned char * buffer = (unsigned char *)file_;

  Knowledge_Update_Settings settings (true, false);

  if (size_ % max_frag_size_ != 0)
  {
    extra = true;
    fragments_.resize (frags);
  }
  else
  {
    fragments_.resize (frags + 1);
  }

  for (; i < frags; ++i, buffer += max_frag_size_)
  {
    fragments_.set_file (i, buffer, max_frag_size_, settings);
  }

  if (extra)
  {
    fragments_.set_file (i + 1, buffer, size_ % max_frag_size_, settings);
  }
}

void
Madara::Knowledge_Engine::Containers::Reliable_File::create_acks (void)
{
  for (size_t i = 0; i < fragments_.size (); ++i)
  {
    std::stringstream buffer;
    buffer << name_;
    buffer << '.';
    buffer << i;

    acks_[i].set_name (buffer.str (), *knowledge_, (int)processes_);
  }
}

int
Madara::Knowledge_Engine::Containers::Reliable_File::from_variable (
  const std::string & name)
{
  return 0;
}

int
Madara::Knowledge_Engine::Containers::Reliable_File::send (void)
{
  return 0;
}

bool
Madara::Knowledge_Engine::Containers::Reliable_File::is_finished (void) const
{
  return 0;
}

Madara::Knowledge_Engine::Knowledge_Update_Settings
Madara::Knowledge_Engine::Containers::Reliable_File::set_settings (
  const Knowledge_Update_Settings & settings)
{
  Guard guard (mutex_);
  
  Knowledge_Update_Settings old_settings = settings_;

  settings_ = settings;

  return old_settings;
}

void
Madara::Knowledge_Engine::Containers::Reliable_File::set_quality (
  uint32_t quality,
  const Knowledge_Reference_Settings & settings)
{
  Guard guard (mutex_);

  if (knowledge_)
    knowledge_->set_quality (name_, quality, settings);
}
