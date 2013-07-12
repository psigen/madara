#ifndef _KNOWLEDGE_RECORD_CPP_
#define _KNOWLEDGE_RECORD_CPP_

#include "madara/knowledge_engine/Knowledge_Record.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/utility/Log_Macros.h"
#include "madara/utility/Utility.h"
#include <sstream>
#include <algorithm>
#include <stdlib.h>

Madara::Knowledge_Record::Knowledge_Record ()
      : status (UNCREATED), clock (0), scope (LOCAL_SCOPE),
        quality (0), write_quality (0),
        size_ (sizeof (Integer)), type_ (INTEGER), int_value_ (0)
{
}

Madara::Knowledge_Record::Knowledge_Record (Integer value)
      : status (UNCREATED), clock (0), scope (LOCAL_SCOPE),
        quality (0), write_quality (0),
        size_ (sizeof (Integer)), type_ (INTEGER), int_value_ (value)
{
}

Madara::Knowledge_Record::Knowledge_Record (double value)
      : status (UNCREATED), clock (0), scope (LOCAL_SCOPE),
        quality (0), write_quality (0),
        size_ (sizeof (Integer)), type_ (DOUBLE), double_value_ (value)
{
}

Madara::Knowledge_Record::Knowledge_Record (const std::string & value)
      : status (UNCREATED), clock (0), scope (LOCAL_SCOPE),
        quality (0), write_quality (0),
        size_ (sizeof (Integer)), type_ (INTEGER)
{
  set_value (value);
}

Madara::Knowledge_Record::Knowledge_Record (const char * value)
      : status (UNCREATED), clock (0), scope (LOCAL_SCOPE),
        quality (0), write_quality (0),
        size_ (sizeof (Integer)), type_ (INTEGER)
{
  set_value (std::string (value));
}

Madara::Knowledge_Record::Knowledge_Record (const Knowledge_Record & rhs)
      : status (rhs.status), clock (rhs.clock),
      scope (rhs.scope), quality (rhs.quality),
      write_quality (rhs.write_quality),
      size_ (rhs.size_), type_ (rhs.type_)
{
  if      (rhs.type_ == INTEGER)
    int_value_ = rhs.int_value_;
  else if (rhs.type_ == DOUBLE)
    double_value_ = rhs.double_value_;
  else if (rhs.is_string_type ())
    str_value_ = rhs.str_value_;
  else if (rhs.is_file_type ())
    file_value_ = rhs.file_value_;
}

Madara::Knowledge_Record::~Knowledge_Record ()
{
}


void
Madara::Knowledge_Record::clear_value (void)
{
  if (is_string_type ())
    str_value_ = 0;
  else if (is_file_type ())
    file_value_ = 0;
}


// reset the value_ to an integer
void  
Madara::Knowledge_Record::reset_value (void)
{
  clear_value ();

  type_ = INTEGER;
  int_value_ = 0;
  size_ = sizeof (int_value_);
  quality = 0;
  write_quality = 0;
  clock = 0;
}

// set the value_ to a string
void
Madara::Knowledge_Record::set_value (const std::string & new_value)
{
  clear_value ();
  type_ = STRING;

  // create a new char array and copy over the string
  size_ = new_value.length () + 1;
  char * temp = new char [size_];
  strncpy (temp, new_value.c_str (), size_ - 1);
  temp[size_ - 1] = 0;
  str_value_ = temp;
}

// set the value_ to a string
void
Madara::Knowledge_Record::set_xml (const char * new_value, size_t size)
{
  clear_value ();
  type_ = XML;

  // create a new char array and copy over the string
  size_ = size + 1;
  char * temp = new char [size_];
  strncpy (temp, new_value, size_ - 1);
  temp[size_ - 1] = 0;
  str_value_ = temp;
}

// set the value_ to a string
void
Madara::Knowledge_Record::set_text (const char * new_value, size_t size)
{
  clear_value ();
  type_ = TEXT_FILE;

  // create a new char array and copy over the string
  size_ = size + 1;
  char * temp = new char [size_];
  strncpy (temp, new_value, size_ - 1);
  temp[size_ - 1] = 0;
  str_value_ = temp;
}

// set the value_ to a string
void
Madara::Knowledge_Record::set_jpeg (const unsigned char * new_value,
                                    size_t size)
{
  clear_value ();
  type_ = IMAGE_JPEG;

  // create a new char array and copy over the string
  size_ = size;
  unsigned char * temp = new unsigned char [size_];
  memcpy (temp, new_value, size_);
  file_value_ = temp;
}

// set the value_ to a string
void
Madara::Knowledge_Record::set_file (const unsigned char * new_value,
                                    size_t size)
{
  clear_value ();
  type_ = UNKNOWN_FILE_TYPE;

  // create a new char array and copy over the string
  size_ = size;
  unsigned char * temp = new unsigned char [size_];
  memcpy (temp, new_value, size_);
  file_value_ = temp;
}

// set the value_ to an integer
void
Madara::Knowledge_Record::set_value (const Integer & new_value_)
{
  clear_value ();
  type_ = INTEGER;
  size_ = sizeof (int_value_);
  int_value_ = new_value_;
}

// set the value_ to a double
void
Madara::Knowledge_Record::set_value (const double & new_value_)
{
  clear_value ();
  type_ = DOUBLE;
  size_ = sizeof (double_value_);
  double_value_ = new_value_;
}

/**
  * reads an XML file from a string
  **/
int
Madara::Knowledge_Record::read_file (
  const std::string & filename, uint32_t read_as_type)
{
  void * buffer;
  size_t size;
  bool add_zero_char = false;
  
  // clear the old value
  clear_value ();
  
  std::string::size_type position = filename.rfind ('.');
  std::string extension = filename.substr (position,
    filename.size () - position);
  Madara::Utility::lower (extension);
  
  // do we have a text-based file
  if (is_string_type (read_as_type) || 
    extension == ".txt" || extension == ".xml")
  {
    add_zero_char = true;
  }

  // read the file into the temporary buffer
  if (Madara::Utility::read_file (filename, buffer, size, add_zero_char) == 0)
  {
    // do we have a text-based file
    if (is_string_type (read_as_type)
           || extension == ".txt" || extension == ".xml")
    {
      // change the string value and size to appropriate values
      str_value_ = (char *)buffer;
      size_ = (uint32_t)size;
      
      if (is_string_type (read_as_type))
        type_ = read_as_type;
      else if (extension == ".xml")
        type_ = XML;
      else
        type_ = TEXT_FILE;
    }
    else
    {
      file_value_ = (unsigned char *)buffer;
      size_ = (uint32_t)size;

      if (extension == ".jpg" || read_as_type == IMAGE_JPEG)
        type_ = IMAGE_JPEG;
      else
        type_ = UNKNOWN_FILE_TYPE;
    }

    return 0;
  }
  else
    return -1;
}

/**
  * writes the value to a file
  **/
ssize_t
Madara::Knowledge_Record::to_file (const std::string & filename) const
{
  if (is_string_type ())
  {
    return Madara::Utility::write_file (filename,
      (void *)str_value_.get_ptr (), size_);
  }
  else if (is_file_type ())
  {
    return Madara::Utility::write_file (filename,
      (void *)file_value_.get_ptr (), size_);
  }
  else
  {
    std::string buffer (to_string ());
    
    return Madara::Utility::write_file (filename,
      (void *)buffer.c_str (), buffer.size ());
  }
}


double
Madara::Knowledge_Record::to_double (void) const
{
  if (type_ != DOUBLE)
  {
    std::stringstream buffer;
    double value;

    // read the value_ into a stringstream and then convert it to double
    if (     type_ == INTEGER)
      buffer << int_value_;
    else if (is_string_type ())
      buffer << str_value_.get_ptr ();
    buffer >> value;

    return value;
  }
  else
    return double_value_;
}

Madara::Knowledge_Record::Integer
Madara::Knowledge_Record::to_integer (void) const
{
  if (type_ != INTEGER)
  {
    std::stringstream buffer;
    Integer value;

    // read the value_ into a stringstream and then convert it to double
    if (     type_ == DOUBLE)
      buffer << double_value_;
    else if (is_string_type ())
      buffer << str_value_.get_ptr ();
    buffer >> value;
    
    return value;
  }
  else
    return int_value_;
}

// read the value_ in a string format
std::string
Madara::Knowledge_Record::to_string (void) const
{
  if (!is_string_type ())
  {
    std::stringstream buffer;
    
    if (     type_ == INTEGER)
      buffer << int_value_;
    else if (type_ == DOUBLE)
      buffer << double_value_;
    return buffer.str ();
  }
  else
    return std::string (str_value_.get_ptr ());
}

// read the value_ in a string format
unsigned char *
Madara::Knowledge_Record::to_unmanaged_buffer (void)
{
  unsigned char * buffer = new unsigned char [size_];

  if (is_string_type ())
  {
    memcpy (buffer, str_value_.get_ptr (), size_);
  }
  else if (is_file_type ())
  {
    memcpy (buffer, file_value_.get_ptr (), size_);
  }
  else
  {
    /**
     * because we use a union of the int and double representations,
     * this actually works
     **/
    memcpy (buffer, &int_value_, size_);
  }

  return buffer;
}


Madara::Knowledge_Record
Madara::Knowledge_Record::fragment (unsigned int first, unsigned int last)
{
  if (first <= last)
  {
    if (is_string_type ())
    {
      // make sure last is accessible in the data type
      last = std::min <unsigned int> (last, size_ - 1);

       // Create a new buffer, copy over the elements, and add a null delimiter
      char * new_buffer = new char [last - first + 2];

      memcpy (new_buffer, str_value_.get_ptr () + first, last - first + 1);
      new_buffer[last-first + 1] = 0;

      return new_buffer;
    }
    else if (is_file_type ())
    {
      // make sure last is accessible in the data type
      last = std::min <unsigned int> (last, size_ - 1);

      // Unlike string types, file buffers are not ended with a null delimiter
      uint32_t size = last - first + 1;
      unsigned char * new_buffer = new unsigned char [size];

      memcpy (new_buffer, file_value_.get_ptr () + first, last - first + 1);

      // create a new record with the unsigned char buffer as contents
      Knowledge_Record ret_value;
      ret_value.set_file (new_buffer, size);
      return ret_value;
    }
    else if (type_ == INTEGER)
    {
      // make sure last is accessible in the data type
      last = std::min <unsigned int> (last, size_ - 1);
      uint32_t size = last - first + 1;

      Integer new_value;
      memcpy (&new_value, &int_value_ + first, size);

      return new_value;
    }
  }

  return Integer (0);
}

Madara::Knowledge_Record
Madara::Knowledge_Record::operator< (const Knowledge_Record & rhs) const
{
  Madara::Knowledge_Record record;

  // if the left hand side is an integer
  if (type_ == INTEGER)
  {
    if (rhs.is_string_type ())
    {
      // when comparing strings to anything else, convert the
      // value into a double for maximum precision
      double temp;
      std::stringstream buffer;
      buffer << rhs.str_value_.get_ptr ();
      buffer >> temp;

      record.int_value_ = int_value_ < temp;
    }

    // integer to double comparison
    else if (rhs.type_ == DOUBLE)
      record.int_value_ = int_value_ < rhs.double_value_;

    // default is the int_value comparison
    else
      record.int_value_ = int_value_ < rhs.int_value_;
  }

  // if the left hand side is a string
  else if (is_string_type ())
  {
    // string to string comparison
    if      (rhs.is_string_type ())
    {
      record.int_value_ = 
        strncmp (str_value_.get_ptr (), rhs.str_value_.get_ptr (), 
        size () >= rhs.size () ? size () : rhs.size ()) < 0;
    }
    
    // string to double comparison
    else if (rhs.type_ == DOUBLE)
    {
      // when comparing strings to anything else, convert the
      // value into a double for maximum precision
      double temp;
      std::stringstream buffer;
      buffer << str_value_.get_ptr ();
      buffer >> temp;

      record.int_value_ = temp < rhs.double_value_;
    }

    // default is string to integer comparison
    else
    {
      // when comparing strings to anything else, convert the
      // value into a double for maximum precision
      double temp;
      std::stringstream buffer;
      buffer << str_value_.get_ptr ();
      buffer >> temp;

      record.int_value_ = temp < rhs.int_value_;
    }
  }

  // if the left hand side is a double
  else if (type_ == DOUBLE)
  {
    // string to string comparison
    if      (rhs.is_string_type ())
    {
      // when comparing strings to anything else, convert the
      // value into a double for maximum precision
      double temp;
      std::stringstream buffer;
      buffer << rhs.str_value_.get_ptr ();
      buffer >> temp;

      record.int_value_ = double_value_ < temp;
    }

    // string to double comparison
    else if (rhs.type_ == DOUBLE)
    {
      record.int_value_ = double_value_ < rhs.double_value_;
    }

    // default is string to integer comparison
    else
    {
      record.int_value_ = double_value_ < rhs.int_value_;
    }
  }

  return record;
}

Madara::Knowledge_Record
Madara::Knowledge_Record::operator<= (const Knowledge_Record & rhs) const
{
  Madara::Knowledge_Record record;

  // if the left hand side is an integer
  if (type_ == INTEGER)
  {
    // for string comparisons against integers, assume the user
    // means the length of the string
    if (rhs.is_string_type ())
    {
      // when comparing strings to anything else, convert the
      // value into a double for maximum precision
      double temp;
      std::stringstream buffer;
      buffer << rhs.str_value_.get_ptr ();
      buffer >> temp;

      record.int_value_ = int_value_ <= temp;
    }

    // integer to double comparison
    else if (rhs.type_ == DOUBLE)
      record.int_value_ = int_value_ <= rhs.double_value_;

    // default is the int_value comparison
    else
      record.int_value_ = int_value_ <= rhs.int_value_;
  }

  // if the left hand side is a string
  else if (is_string_type ())
  {
    // string to string comparison
    if      (rhs.is_string_type ())
    {
      record.int_value_ = 
        strncmp (str_value_.get_ptr (), rhs.str_value_.get_ptr (), 
        size () >= rhs.size () ? size () : rhs.size ()) <= 0;
    }
    
    // string to double comparison
    else if (rhs.type_ == DOUBLE)
    {
      // when comparing strings to anything else, convert the
      // value into a double for maximum precision
      double temp;
      std::stringstream buffer;
      buffer << str_value_.get_ptr ();
      buffer >> temp;

      record.int_value_ = temp <= rhs.double_value_;
    }

    // default is string to integer comparison
    else
    {
      // when comparing strings to anything else, convert the
      // value into a double for maximum precision
      double temp;
      std::stringstream buffer;
      buffer << str_value_.get_ptr ();
      buffer >> temp;

      record.int_value_ = temp <= rhs.int_value_;
    }
  }

  // if the left hand side is a double
  else if (type_ == DOUBLE)
  {
    // string to string comparison
    if      (rhs.is_string_type ())
    {
      // when comparing strings to anything else, convert the
      // value into a double for maximum precision
      double temp;
      std::stringstream buffer;
      buffer << rhs.str_value_.get_ptr ();
      buffer >> temp;

      record.int_value_ = double_value_ <= temp;
    }

    // string to double comparison
    else if (rhs.type_ == DOUBLE)
    {
      record.int_value_ = double_value_ <= rhs.double_value_;
    }

    // default is string to integer comparison
    else
    {
      record.int_value_ = double_value_ <= rhs.int_value_;
    }
  }

  return record;
}

Madara::Knowledge_Record
Madara::Knowledge_Record::operator== (const Knowledge_Record & rhs) const
{
  Madara::Knowledge_Record record;

  // if the left hand side is an integer
  if (type_ == INTEGER)
  {
    // for string comparisons against integers, assume the user
    // means the length of the string
    if (rhs.is_string_type ())
    {
      // when comparing strings to anything else, convert the
      // value into a double for maximum precision
      double temp;
      std::stringstream buffer;
      buffer << rhs.str_value_.get_ptr ();
      buffer >> temp;

      record.int_value_ = int_value_ == temp;
    }

    // integer to double comparison
    else if (rhs.type_ == DOUBLE)
      record.int_value_ = int_value_ == rhs.double_value_;

    // default is the int_value comparison
    else
      record.int_value_ = int_value_ == rhs.int_value_;
  }

  // if the left hand side is a string
  else if (is_string_type ())
  {
    // string to string comparison
    if      (rhs.is_string_type ())
    {
      record.int_value_ = 
        strncmp (str_value_.get_ptr (), rhs.str_value_.get_ptr (), 
        size () >= rhs.size () ? size () : rhs.size ()) == 0;
    }
    
    // string to double comparison
    else if (rhs.type_ == DOUBLE)
    {
      // when comparing strings to anything else, convert the
      // value into a double for maximum precision
      double temp;
      std::stringstream buffer;
      buffer << str_value_.get_ptr ();
      buffer >> temp;

      record.int_value_ = temp == rhs.double_value_;
    }

    // default is string to integer comparison
    else
    {
      // when comparing strings to anything else, convert the
      // value into a double for maximum precision
      double temp;
      std::stringstream buffer;
      buffer << str_value_.get_ptr ();
      buffer >> temp;

      record.int_value_ = temp == rhs.int_value_;
    }
  }

  // if the left hand side is a double
  else if (type_ == DOUBLE)
  {
    // string to string comparison
    if      (rhs.is_string_type ())
    {
      // when comparing strings to anything else, convert the
      // value into a double for maximum precision
      double temp;
      std::stringstream buffer;
      buffer << rhs.str_value_.get_ptr ();
      buffer >> temp;

      record.int_value_ = double_value_ == temp;
    }

    // string to double comparison
    else if (rhs.type_ == DOUBLE)
    {
      record.int_value_ = double_value_ == rhs.double_value_;
    }

    // default is string to integer comparison
    else
    {
      record.int_value_ = double_value_ == rhs.int_value_;
    }
  }

  return record;
}

bool
Madara::Knowledge_Record::operator== (Integer value) const
{
  // for this type of comparison, we can only be equal if we are the same
  // base type
  if (type_ == INTEGER)
    return int_value_ == value;
  else
    return false;
}

bool
Madara::Knowledge_Record::operator== (double value) const
{
  // for this type of comparison, we can only be equal if we are the same
  // base type
  if (type_ == DOUBLE)
    return double_value_ == value;
  else
    return false;
}

bool
Madara::Knowledge_Record::operator== (const std::string & value) const
{
  // for this type of comparison, we can only be equal if we are the same
  // base type
  if (is_string_type ())
    return strncmp (
      str_value_.get_ptr (), value.c_str (),
      size () >= value.size () ? size () : value.size ()) == 0;
  else
    return false;
}

bool
Madara::Knowledge_Record::operator== (const char * value) const
{
  return *this == std::string (value);
}

Madara::Knowledge_Record  
Madara::Knowledge_Record::operator!= (const Knowledge_Record & rhs) const
{
  return !(*this == rhs);
}

/**
  * Logical and.
  **/
Madara::Knowledge_Record
  Madara::Knowledge_Record::operator&& (const Knowledge_Record & rhs) const
{
  Madara::Knowledge_Record record;

  record.set_value ((Integer)(this->is_true () && rhs.is_true ()));

  return record;
}

/**
  * Logical or.
  **/
Madara::Knowledge_Record
  Madara::Knowledge_Record::operator|| (const Knowledge_Record & rhs) const
{
  Madara::Knowledge_Record record;

  record.set_value ((Integer)(this->is_true () || rhs.is_true ()));

  return record;
}

/**
  * Logical not.
  **/
Madara::Knowledge_Record
Madara::Knowledge_Record::operator! (void) const
{
  Madara::Knowledge_Record record;
  
  record.int_value_ = !is_true ();
  
  record.type_ = INTEGER;

  return record;
}

/**
  * Negate.
  **/
Madara::Knowledge_Record
  Madara::Knowledge_Record::operator- (void) const
{
  Madara::Knowledge_Record record (*this);

  if (type_ == INTEGER)
    record.int_value_ = -record.int_value_;
  else if (type_ == DOUBLE)
    record.double_value_ = -record.double_value_;

  return record;
}

Madara::Knowledge_Record
Madara::Knowledge_Record::operator> (const Knowledge_Record & rhs) const
{
  Madara::Knowledge_Record record;

  // if the left hand side is an integer
  if (type_ == INTEGER)
  {
    // for string comparisons against integers, assume the user
    // means the length of the string
    if (rhs.is_string_type ())
    {
      // when comparing strings to anything else, convert the
      // value into a double for maximum precision
      double temp;
      std::stringstream buffer;
      buffer << rhs.str_value_.get_ptr ();
      buffer >> temp;

      record.int_value_ = int_value_ > temp;
    }

    // integer to double comparison
    else if (rhs.type_ == DOUBLE)
      record.int_value_ = int_value_ > rhs.double_value_;

    // default is the int_value comparison
    else
      record.int_value_ = int_value_ > rhs.int_value_;
  }

  // if the left hand side is a string
  else if (is_string_type ())
  {
    // string to string comparison
    if      (rhs.is_string_type ())
    {
      record.int_value_ = 
        strncmp (str_value_.get_ptr (), rhs.str_value_.get_ptr (),
          size () >= rhs.size () ? size () : rhs.size ()) > 0;
    }
    
    // string to double comparison
    else if (rhs.type_ == DOUBLE)
    {
      // when comparing strings to anything else, convert the
      // value into a double for maximum precision
      double temp;
      std::stringstream buffer;
      buffer << str_value_.get_ptr ();
      buffer >> temp;

      record.int_value_ = temp > rhs.double_value_;
    }

    // default is string to integer comparison
    else
    {
      // when comparing strings to anything else, convert the
      // value into a double for maximum precision
      double temp;
      std::stringstream buffer;
      buffer << str_value_.get_ptr ();
      buffer >> temp;

      record.int_value_ = temp > rhs.int_value_;
    }
  }

  // if the left hand side is a double
  else if (type_ == DOUBLE)
  {
    // string to string comparison
    if      (rhs.is_string_type ())
    {
      // when comparing strings to anything else, convert the
      // value into a double for maximum precision
      double temp;
      std::stringstream buffer;
      buffer << rhs.str_value_.get_ptr ();
      buffer >> temp;

      record.int_value_ = double_value_ > temp;
    }

    // string to double comparison
    else if (rhs.type_ == DOUBLE)
    {
      record.int_value_ = double_value_ > rhs.double_value_;
    }

    // default is string to integer comparison
    else
    {
      record.int_value_ = double_value_ > rhs.int_value_;
    }
  }

  return record;
}

Madara::Knowledge_Record  
Madara::Knowledge_Record::operator>= (const Knowledge_Record & rhs) const
{
  Madara::Knowledge_Record record;

  // if the left hand side is an integer
  if (type_ == INTEGER)
  {
    // for string comparisons against integers, assume the user
    // means the length of the string
    if (rhs.is_string_type ())
    {
      // when comparing strings to anything else, convert the
      // value into a double for maximum precision
      double temp;
      std::stringstream buffer;
      buffer << rhs.str_value_.get_ptr ();
      buffer >> temp;

      record.int_value_ = int_value_ >= temp;
    }

    // integer to double comparison
    else if (rhs.type_ == DOUBLE)
      record.int_value_ = int_value_ >= rhs.double_value_;

    // default is the int_value comparison
    else
      record.int_value_ = int_value_ >= rhs.int_value_;
  }

  // if the left hand side is a string
  else if (is_string_type ())
  {
    // string to string comparison
    if      (rhs.is_string_type ())
    {
      record.int_value_ =  
        strncmp (str_value_.get_ptr (), rhs.str_value_.get_ptr (),
          size () >= rhs.size () ? size () : rhs.size ()) >= 0;
    }
    
    // string to double comparison
    else if (rhs.type_ == DOUBLE)
    {
      // when comparing strings to anything else, convert the
      // value into a double for maximum precision
      double temp;
      std::stringstream buffer;
      buffer << str_value_.get_ptr ();
      buffer >> temp;

      record.int_value_ = temp >= rhs.double_value_;
    }

    // default is string to integer comparison
    else
    {
      // when comparing strings to anything else, convert the
      // value into a double for maximum precision
      double temp;
      std::stringstream buffer;
      buffer << str_value_.get_ptr ();
      buffer >> temp;

      record.int_value_ = temp >= rhs.int_value_;
    }
  }

  // if the left hand side is a double
  else if (type_ == DOUBLE)
  {
    // string to string comparison
    if      (rhs.is_string_type ())
    {
      // when comparing strings to anything else, convert the
      // value into a double for maximum precision
      double temp;
      std::stringstream buffer;
      buffer << rhs.str_value_.get_ptr ();
      buffer >> temp;

      record.int_value_ = double_value_ >= temp;
    }

    // string to double comparison
    else if (rhs.type_ == DOUBLE)
    {
      record.int_value_ = double_value_ >= rhs.double_value_;
    }

    // default is string to integer comparison
    else
    {
      record.int_value_ = double_value_ >= rhs.int_value_;
    }
  }

  return record;
}

Madara::Knowledge_Record
Madara::Knowledge_Record::operator= (const Knowledge_Record & rhs)
{
  if (this == &rhs)
    return *this;

  // clear any dynamic memory being used on the left hand side
  clear_value ();

  // set the instance properties accordingly
  clock = rhs.clock;
  quality = rhs.quality;
  scope = rhs.scope;
  status = rhs.status;
  type_ = rhs.type_;
  size_ = rhs.size_;

  if (is_string_type ())
    str_value_ = rhs.str_value_;
  else if (type_ == INTEGER)
    this->int_value_ = rhs.int_value_;
  else if (type_ == DOUBLE)
    this->double_value_ = rhs.double_value_;
  else if (is_file_type ())
    file_value_ = rhs.file_value_;

  return *this;
}

/**
  * Atomic addition operator
  **/
Madara::Knowledge_Record
Madara::Knowledge_Record::operator+= (const Knowledge_Record & rhs)
{
  // if the left hand side is an integer
  if (type_ == INTEGER)
  {
    if (rhs.is_string_type ())
    {
      std::stringstream buffer;
      type_ = STRING;

      buffer << int_value_;
      buffer << rhs.str_value_.get_ptr ();
      size_ = buffer.str ().size () + 1;
      str_value_ = new char [size_];
      strncpy (str_value_.get_ptr (), buffer.str ().c_str (), size_ - 1);
      str_value_.get_ptr () [size_ - 1] = 0;
    }

    else if (rhs.type_ == DOUBLE)
    {
      // to save precision, convert our type to a double and
      // initialize ourselves appropriately
      type_ = DOUBLE;
      double_value_ = (double) int_value_;
      double_value_ += rhs.double_value_;
    }

    // default is the int_value comparison
    else
      int_value_ += rhs.int_value_;
  }

  // if the left hand side is a string
  else if (is_string_type ())
  {
    // string to string comparison
    if      (rhs.is_string_type ())
    {
      std::stringstream buffer;

      buffer << str_value_.get_ptr ();
      buffer << rhs.str_value_.get_ptr ();
      size_ = buffer.str ().size () + 1;
      str_value_ = new char [size_];
      strncpy (str_value_.get_ptr (), buffer.str ().c_str (), size_ - 1);
      str_value_.get_ptr () [size_ - 1] = 0;
    }

    // string to double comparison
    else if (rhs.type_ == DOUBLE)
    {
      std::stringstream buffer;

      buffer << str_value_.get_ptr ();
      buffer << rhs.double_value_;
      size_ = buffer.str ().size () + 1;
      str_value_ = new char [size_];
      strncpy (str_value_.get_ptr (), buffer.str ().c_str (), size_ - 1);
      str_value_.get_ptr () [size_ - 1] = 0;
    }

    // default is string to integer comparison
    else
    {
      std::stringstream buffer;

      buffer << str_value_.get_ptr ();
      buffer << rhs.int_value_;
      size_ = buffer.str ().size () + 1;
      str_value_ = new char [size_];
      strncpy (str_value_.get_ptr (), buffer.str ().c_str (), size_ - 1);
      str_value_.get_ptr () [size_ - 1] = 0;
    }
  }

  // if the left hand side is a double
  else if (type_ == DOUBLE)
  {
    // string subtracted from a double
    if      (rhs.is_string_type ())
    {
      std::stringstream buffer;
      double   rhs_value;
      type_ = STRING;

      buffer << rhs.str_value_.get_ptr ();
      buffer >> rhs_value;
      
      size_ = buffer.str ().size () + 1;
      str_value_ = new char [size_];
      strncpy (str_value_.get_ptr (), buffer.str ().c_str (), size_ - 1);
      str_value_.get_ptr () [size_ - 1] = 0;
    }

    // double subtracted from a double
    else if (rhs.type_ == DOUBLE)
    {
      double_value_ += rhs.double_value_;
    }

    // an integer subtracted from a double retains precision
    else
    {
      double_value_ += rhs.int_value_;
    }
  }
  return *this;
}
     
/**
  * Atomic predecrement
  **/
Madara::Knowledge_Record
Madara::Knowledge_Record::operator-- (void)
{
  // if the left hand side is an integer
  if (type_ == INTEGER)
  {
    --int_value_;
  }

  // if the left hand side is a double
  else if (type_ == DOUBLE)
  {
    --double_value_;
  }

  return *this;
}
   
/**
  * Atomic preincrement
  **/
Madara::Knowledge_Record
Madara::Knowledge_Record::operator++ (void)
{
  // if the left hand side is an integer
  if (type_ == INTEGER)
  {
    ++int_value_;
  }

  // if the left hand side is a double
  else if (type_ == DOUBLE)
  {
    ++double_value_;
  }

  return *this;
}
       
/**
  * Atomic subtraction operator
  **/
Madara::Knowledge_Record
Madara::Knowledge_Record::operator-= (const Knowledge_Record & rhs)
{
  // if the left hand side is an integer
  if (type_ == INTEGER)
  {
    if (rhs.type_ == INTEGER)
    {
      int_value_ -= rhs.int_value_;
    }
    else if (rhs.type_ == DOUBLE)
    {
      // to save precision, convert our type to a double and
      // initialize ourselves appropriately
      type_ = DOUBLE;
      double_value_ = (double) int_value_;
      double_value_ -= rhs.double_value_;
    }
  }

  // if the left hand side is a double
  else if (type_ == DOUBLE)
  {
    // string to string comparison
    if      (rhs.type_ == INTEGER)
    {
      double_value_ -= rhs.int_value_;
    }

    // double subtracted from a double
    else if (rhs.type_ == DOUBLE)
    {
      double_value_ -= rhs.double_value_;
    }
  }

  return *this;
}
    
/**
  * Atomic multiplication operator
  **/
Madara::Knowledge_Record
Madara::Knowledge_Record::operator*= (const Knowledge_Record & rhs)
{
  // if the left hand side is an integer
  if (type_ == INTEGER)
  {
    // default is the int_value comparison
    if      (rhs.type_ == INTEGER)
      int_value_ *= rhs.int_value_;
    else if (rhs.type_ == DOUBLE)
    {
      // to save precision, convert our type to a double and
      // initialize ourselves appropriately
      type_ = DOUBLE;
      double_value_ = (double) int_value_;
      double_value_ *= rhs.double_value_;
    }

  }

  // if the left hand side is a double
  else if (type_ == DOUBLE)
  {
    // double times an integer
    if (rhs.type_ == DOUBLE)
    {
      double_value_ *= rhs.double_value_;
    }
    // double times a double
    else if (rhs.type_ == INTEGER)
    {
      double_value_ *= rhs.int_value_;
    }

  }
  return *this;
}
    
/**
  * Atomic division operator
  **/
Madara::Knowledge_Record
Madara::Knowledge_Record::operator/= (const Knowledge_Record & rhs)
{
  // if the left hand side is an integer
  if (type_ == INTEGER)
  {
    if      (rhs.type_ == INTEGER)
    {
      int_value_ /= rhs.int_value_;
    }
    else if (rhs.type_ == DOUBLE)
    {
      // to save precision, convert our type to a double and
      // initialize ourselves appropriately
      type_ = DOUBLE;
      double_value_ = (double) int_value_;
      double_value_ /= rhs.double_value_;
    }
  }

  // if the left hand side is a double
  else if (type_ == DOUBLE)
  {
    if      (rhs.type_ == DOUBLE)
    {
      double_value_ /= rhs.double_value_;
    }
    else if (rhs.type_ == INTEGER)
    {
      double_value_ /= rhs.int_value_;
    }
  }
  return *this;
}
   
/**
  * Atomic division operator
  **/
Madara::Knowledge_Record
Madara::Knowledge_Record::operator%= (const Knowledge_Record & rhs)
{
  // if the left hand side is an integer
  if (type_ == INTEGER && rhs.type_ == INTEGER)
  {
      int_value_ %= rhs.int_value_;
  }

  return *this;
}
 
/**
  * Times operator
  **/
Madara::Knowledge_Record
Madara::Knowledge_Record::operator* (const Knowledge_Record & rhs) const
{
  // copy this value to a local copy
  Knowledge_Record ret_value (*this);

  return ret_value *= rhs;
}

/**
  * Divides operator
  **/
Madara::Knowledge_Record
Madara::Knowledge_Record::operator/ (const Knowledge_Record & rhs) const
{
  // copy this value to a local copy
  Knowledge_Record ret_value (*this);

  return ret_value /= rhs;
}

/**
  * Modulus operator
  **/
Madara::Knowledge_Record
Madara::Knowledge_Record::operator% (const Knowledge_Record & rhs) const
{
  // copy this value to a local copy
  Knowledge_Record ret_value (*this);

  return ret_value %= rhs;
}

/**
  * Plus operator
  **/
Madara::Knowledge_Record
Madara::Knowledge_Record::operator+ (const Knowledge_Record & rhs) const
{
  // copy this value to a local copy
  Knowledge_Record ret_value (*this);

  return ret_value += rhs;
}
    
/**
  * Minus operator
  **/
Madara::Knowledge_Record
Madara::Knowledge_Record::operator- (const Knowledge_Record & rhs) const
{
  // copy this value to a local copy
  Knowledge_Record ret_value (*this);

  return ret_value -= rhs;
}
    

char *
Madara::Knowledge_Record::write (char * buffer, const std::string & key,
   int64_t & buffer_remaining)
     
{
  // format is [key_size | key | type | value_size | value]

  uint32_t key_size = key.size () + 1;
  char * size_location = 0;
  uint32_t size_intermediate = 0;
  uint32_t uint32_temp;
  Integer integer_temp;
  double double_temp;


  // Remove the key size from the buffer
  if (buffer_remaining >= sizeof (key_size))
  {
    uint32_temp = Madara::Utility::endian_swap (key_size);
    memcpy (buffer, &uint32_temp, sizeof (uint32_temp));
    buffer += sizeof (key_size);
  }
  buffer_remaining -= sizeof (key_size);

  // Remove the key from the buffer
  if (buffer_remaining >= sizeof (char) * key_size)
  {
    // copy the string and set null terminator in buffer
    strncpy (buffer, key.c_str (), key_size - 1);
    buffer[key_size - 1] = 0;

    buffer += sizeof (char) * key_size;
  }
  buffer_remaining -= sizeof (char) * key_size;
  
  // Remove the type of value from the buffer
  if (buffer_remaining >= sizeof (type_))
  {
    uint32_temp = Madara::Utility::endian_swap (type_);
    memcpy (buffer, &uint32_temp, sizeof (uint32_temp));
    buffer += sizeof (type_);
  }
  buffer_remaining -= sizeof (type_);

  // Remove the size of value from the buffer
  if (buffer_remaining >= sizeof (size_))
  {
    // set a pointer to size, in case we need to modify it during
    // value copy (e.g. during double conversion)
    size_location = buffer;
    size_intermediate = size_;
    
    uint32_temp = Madara::Utility::endian_swap (size_);
    memcpy (buffer, &uint32_temp, sizeof (uint32_temp));

    // note that we do not encode the size yet because it may change
    // and we need the architectural-specific version for other checks

    buffer += sizeof (size_);
  }
  buffer_remaining -= sizeof (size_);
  
  // Remove the value from the buffer
  if      (is_string_type ())
  {
    // strings do not have to be converted
    if (buffer_remaining >= size_)
      memcpy (buffer, str_value_.get_ptr (), size_);
  }
  else if (type_ == INTEGER)
  {
    if (buffer_remaining >= size_)
    {
      // convert integers to network byte order
      integer_temp = Madara::Utility::endian_swap (int_value_);
      memcpy (buffer, &integer_temp, sizeof (integer_temp));
    }
  }
  else if (type_ == DOUBLE)
  {
    if (buffer_remaining >= size_)
    {
      // convert doubles to network byte order
      double_temp = Madara::Utility::endian_swap (double_value_);
      memcpy (buffer, &double_temp, sizeof (double_temp));

      /**
       * note that we once converted doubles into strings to attempt
       * portability, but we are now just assuming that the floating
       * point units in the architecture are the same endianness as
       * the integers. This is true of ARM, Intel/AMD, and most architectures.
       * We are essentially no longer supporting an architecture that
       * mixes and matches. Persons using such architectures should
       * perform their own conversions on the knowledge records before
       * using them.
       **/
    }
  }
  else if (is_file_type ())
  {
    // strings do not have to be converted
    if (buffer_remaining >= size_)
      memcpy (buffer, file_value_.get_ptr (), size_);
  }

  if (size_location)
  {
    buffer_remaining -= size_intermediate;
    buffer += size_intermediate;
    size_intermediate = (uint32_t) Madara::Utility::endian_swap (size_intermediate);
    memcpy (size_location, &size_intermediate, sizeof (size_intermediate));
  }

  return buffer;
}

char *
Madara::Knowledge_Record::read (char * buffer, std::string & key,
                                int64_t & buffer_remaining)
{
  // format is [key_size | key | type | value_size | value]

  uint32_t key_size;
  uint32_t buff_value_size;

  // Remove the key size from the buffer
  if (buffer_remaining >= sizeof (key_size))
  {
    memcpy (&key_size, buffer, sizeof (key_size)); 
    key_size = Madara::Utility::endian_swap (key_size);
    buffer += sizeof (key_size);
  }
  buffer_remaining -= sizeof (key_size);

  // Remove the key from the buffer
  if (buffer_remaining >= sizeof (char) * key_size)
  {
    // don't worry about null terminator
    key.assign (buffer, key_size - 1);

    buffer += sizeof (char) * key_size;
  }
  buffer_remaining -= sizeof (char) * key_size;
  
  // Remove the type of value from the buffer
  if (buffer_remaining >= sizeof (type_))
  {
    memcpy (&type_, buffer, sizeof (type_)); 
    type_ = Madara::Utility::endian_swap (type_);
    buffer += sizeof (type_);
  }
  buffer_remaining -= sizeof (type_);
  
  // Remove the size of value from the buffer
  if (buffer_remaining >= sizeof (size_))
  {
    memcpy (&size_, buffer, sizeof (size_)); 
    size_ = Madara::Utility::endian_swap (size_);
    buff_value_size = size_;
    buffer += sizeof (size_);
  }
  buffer_remaining -= sizeof (size_);
  
  // Remove the value from the buffer
  if (size_ > 0 && buffer_remaining >= size_)
  {
    if      (is_string_type ())
    {
      str_value_ = new char [size_];
      strncpy (str_value_.get_ptr (), buffer, size_);
    }
  
    else if (type_ == INTEGER)
    {
      memcpy (&int_value_, buffer, sizeof (int_value_)); 
      int_value_ = Madara::Utility::endian_swap (int_value_);
    }

    else if (type_ == DOUBLE)
    {
      memcpy (&double_value_, buffer, sizeof (double_value_)); 
      double_value_ = Madara::Utility::endian_swap (double_value_);
    }

    else if (is_file_type ())
    {
      file_value_ = new unsigned char [size_];
      memcpy (file_value_.get_ptr (), buffer, size_);
    }

    buffer += buff_value_size;
    buffer_remaining -= sizeof (char) * buff_value_size;
  }

  return buffer;
}


int
  Madara::Knowledge_Record::apply (
  Madara::Knowledge_Engine::Thread_Safe_Context & context,
  const std::string & key, unsigned int quality, uint64_t clock,
  bool perform_lock)
{
  int result = -1;

  if (key.length () > 0)
  {
    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "Knowledge_Record::apply:" \
        " attempting to set %s=%s\n", key.c_str (), to_string ().c_str ()));

    if (perform_lock)
      context.lock ();

    // if the data we are updating had a lower clock value or less quality
    // then that means this update is the latest value. Among
    // other things, this means our solution will work even
    // without FIFO channel transports

    // if the data we are updating had a lower clock value
    // then that means this update is the latest value. Among
    // other things, this means our solution will work even
    // without FIFO channel transports
    result = context.update_record_from_external (key, *this, 
      Knowledge_Engine::Knowledge_Update_Settings (true));
          
          
    if (perform_lock)
    {
      context.unlock ();
      context.set_changed ();
    }

    // if we actually updated the value
    if (result == 1)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Knowledge_Record::apply:" \
        " received data[%s]=%s.\n", 
        key.c_str (), to_string ().c_str ()));
    }
    // if the data was already current
    else if (result == 0)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Knowledge_Record::apply:" \
          " discarded data[%s]=%s as the value was already set.\n",
          key.c_str (), to_string ().c_str ()));
    }
    else if (result == -1)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Knowledge_Record::apply:" \
        " discarded data due to null key.\n"));
    }
    else if (result == -2)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Knowledge_Record::apply:" \
        " discarded data[%s]=%q due to lower quality.\n",
        key.c_str (), to_string ().c_str ()));
    }
    else if (result == -3)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Knowledge_Record::apply:" \
        " discarded data[%s]=%q due to older timestamp.\n",
        key.c_str (), to_string ().c_str ()));
    }
  }
  return result;
}

bool
Madara::Knowledge_Record::is_true (void) const
{
  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "Knowledge_Record::apply:" \
    " checking if record is non-zero.\n"));

  if (type_ == INTEGER)
    return int_value_ != 0;
  else if (type_ == DOUBLE)
    return double_value_ < 0 || double_value_ > 0;
  else
  {
    return size_ > 1;
  }
}

bool
Madara::Knowledge_Record::is_false (void) const
{
  return !is_true ();
}

/**
  * output stream buffering
  **/
std::ostream & operator<< (std::ostream & stream,
  const Madara::Knowledge_Record & rhs)
{
  if      (rhs.type () == Madara::Knowledge_Record::INTEGER)
    stream << rhs.to_integer ();
  else if (rhs.type () == Madara::Knowledge_Record::DOUBLE)
    stream << rhs.to_double ();
  else if (rhs.type () == Madara::Knowledge_Record::STRING)
    stream << rhs.to_string ();
  else if (rhs.type () == Madara::Knowledge_Record::XML)
    stream << rhs.to_string ();


  return stream;
}

  

#endif   // _KNOWLEDGE_RECORD_CPP_
