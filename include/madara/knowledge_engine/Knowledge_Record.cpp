#ifndef _KNOWLEDGE_RECORD_CPP_
#define _KNOWLEDGE_RECORD_CPP_

#include "madara/knowledge_engine/Knowledge_Record.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/utility/Log_Macros.h"
#include "madara/utility/Utility.h"
#include <sstream>
#include <algorithm>

Madara::Knowledge_Record::Knowledge_Record ()
      : status (UNCREATED), clock (0), scope (LOCAL_SCOPE),
        quality (0), write_quality (0), type_ (INTEGER),
        size_ (sizeof (Integer)), int_value_ (0)
{
}

Madara::Knowledge_Record::Knowledge_Record (Integer value)
      : status (UNCREATED), clock (0), scope (LOCAL_SCOPE),
        quality (0), write_quality (0), type_ (INTEGER),
        size_ (sizeof (Integer)), int_value_ (value)
{
}

Madara::Knowledge_Record::Knowledge_Record (double value)
      : status (UNCREATED), clock (0), scope (LOCAL_SCOPE),
        quality (0), write_quality (0), type_ (DOUBLE),
        size_ (sizeof (Integer)), double_value_ (value)
{
}

Madara::Knowledge_Record::Knowledge_Record (const std::string & value)
      : status (UNCREATED), clock (0), scope (LOCAL_SCOPE),
        quality (0), write_quality (0), type_ (INTEGER),
        size_ (sizeof (Integer))
{
  set_value (value);
}

Madara::Knowledge_Record::Knowledge_Record (const char * value)
      : status (UNCREATED), clock (0), scope (LOCAL_SCOPE),
        quality (0), write_quality (0), type_ (INTEGER),
        size_ (sizeof (Integer))
{
  set_value (std::string (value));
}

Madara::Knowledge_Record::Knowledge_Record (const Knowledge_Record & rhs)
      : status (rhs.status), clock (rhs.clock),
      scope (rhs.scope), quality (rhs.quality),
      write_quality (rhs.write_quality), type_ (rhs.type_)
{
  if      (rhs.type_ == INTEGER)
  {
    int_value_ = rhs.int_value_;
    size_ = sizeof (int_value_);
  }
  else if (rhs.type_ == DOUBLE)
  {
    double_value_ = rhs.double_value_;
    size_ = sizeof (double_value_);
  }
  else if (rhs.type_ == STRING)
  {
    // copy over the rhs scoped_array
    size_ = rhs.size_;
    str_value_ = rhs.str_value_;
  }
  else if (rhs.type_ == XML)
  {
    // copy over the rhs scoped_array
    size_ = rhs.size_;
    xml_value_ = rhs.xml_value_;
  }
}

Madara::Knowledge_Record::~Knowledge_Record ()
{
}


void
Madara::Knowledge_Record::clear_value (void)
{
  if (type_ == STRING)
    str_value_ = 0;
  else if (type_ == FILE || type_ == IMAGE || type_ == XML)
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
void
Madara::Knowledge_Record::read_xml (const char * xml_contents)
{
  xml_value_ = new TiXmlDocument ();
  xml_value_->Parse (xml_contents);
}

/**
  * copies an XML from a TinyXML Document object
  **/
void Madara::Knowledge_Record::copy_xml (const TiXmlDocument & xml_doc)
{
  xml_value_ = new TiXmlDocument (xml_doc);
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
    else if (type_ == STRING)
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
    else if (type_ == STRING)
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
  if (type_ != STRING)
  {
    std::stringstream buffer;
    
    if (     type_ == INTEGER)
      buffer << int_value_;
    else if (type_ == DOUBLE)
      buffer << double_value_;
    else if (type_ == XML)
      // this is incorrect. Currently will stringify the doc name
      return xml_value_->ValueStr ();
    return buffer.str ();
  }
  else
    return std::string (str_value_.get_ptr ());
}

Madara::Knowledge_Record
Madara::Knowledge_Record::operator< (const Knowledge_Record & rhs) const
{
  Madara::Knowledge_Record record;

  // if the left hand side is an integer
  if (type_ == INTEGER)
  {
    if (rhs.type_ == STRING)
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
  else if (type_ == STRING)
  {
    // string to string comparison
    if      (rhs.type_ == STRING)
    {
      record.int_value_ = 
        strncmp (str_value_.get_ptr (), rhs.str_value_.get_ptr (), 
        size () >= rhs.size () ? size () : rhs.size ()) < 0;
    }
    
    // string to XML comparison
    else if (rhs.type_ == XML)
    {
      const std::string & rhs_value = rhs.xml_value_->ValueStr ();
      record.int_value_ = 
        strncmp (str_value_.get_ptr (), rhs_value.c_str (), 
        size () >= rhs_value.size () ? size () : rhs_value.size ()) < 0;
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
    if      (rhs.type_ == STRING)
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
    if (rhs.type_ == STRING)
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
  else if (type_ == STRING)
  {
    // string to string comparison
    if      (rhs.type_ == STRING)
    {
      record.int_value_ = 
        strncmp (str_value_.get_ptr (), rhs.str_value_.get_ptr (), 
        size () >= rhs.size () ? size () : rhs.size ()) <= 0;
    }
    
    // string to XML comparison
    else if (rhs.type_ == XML)
    {
      const std::string & rhs_value = rhs.xml_value_->ValueStr ();
      record.int_value_ = 
        strncmp (str_value_.get_ptr (), rhs_value.c_str (), 
        size () >= rhs_value.size () ? size () : rhs_value.size ()) <= 0;
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
    if      (rhs.type_ == STRING)
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
    if (rhs.type_ == STRING)
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
  else if (type_ == STRING)
  {
    // string to string comparison
    if      (rhs.type_ == STRING)
    {
      record.int_value_ = 
        strncmp (str_value_.get_ptr (), rhs.str_value_.get_ptr (), 
        size () >= rhs.size () ? size () : rhs.size ()) == 0;
    }
    
    // string to XML comparison
    else if (rhs.type_ == XML)
    {
      const std::string & rhs_value = rhs.xml_value_->ValueStr ();
      record.int_value_ = 
        strncmp (str_value_.get_ptr (), rhs_value.c_str (), 
        size () >= rhs_value.size () ? size () : rhs_value.size ()) == 0;
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
    if      (rhs.type_ == STRING)
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
  if (type_ == STRING)
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
    if (rhs.type_ == STRING)
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
  else if (type_ == STRING)
  {
    // string to string comparison
    if      (rhs.type_ == STRING)
    {
      record.int_value_ = 
        strncmp (str_value_.get_ptr (), rhs.str_value_.get_ptr (),
          size () >= rhs.size () ? size () : rhs.size ()) > 0;
    }
    
    // string to XML comparison
    else if (rhs.type_ == XML)
    {
      const std::string & rhs_value = rhs.xml_value_->ValueStr ();
      record.int_value_ = 
        strncmp (str_value_.get_ptr (), rhs_value.c_str (), 
        size () >= rhs_value.size () ? size () : rhs_value.size ()) > 0;
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
    if      (rhs.type_ == STRING)
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
    if (rhs.type_ == STRING)
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
  else if (type_ == STRING)
  {
    // string to string comparison
    if      (rhs.type_ == STRING)
    {
      record.int_value_ =  
        strncmp (str_value_.get_ptr (), rhs.str_value_.get_ptr (),
          size () >= rhs.size () ? size () : rhs.size ()) >= 0;
    }
    
    // string to XML comparison
    else if (rhs.type_ == XML)
    {
      const std::string & rhs_value = rhs.xml_value_->ValueStr ();
      record.int_value_ = 
        strncmp (str_value_.get_ptr (), rhs_value.c_str (), 
        size () >= rhs_value.size () ? size () : rhs_value.size ()) >= 0;
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
    if      (rhs.type_ == STRING)
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

  if (type_ == STRING)
  {
    // copy over the rhs.str_value_ into a new memory location
    size_ = rhs.size ();
    str_value_ = rhs.str_value_;
  }
  else if (type_ == INTEGER)
    this->int_value_ = rhs.int_value_;
  else if (type_ == DOUBLE)
    this->double_value_ = rhs.double_value_;
  else if (type_ == XML)
    this->xml_value_ = rhs.xml_value_;

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
    if (rhs.type_ == STRING)
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
  else if (type_ == STRING)
  {
    // string to string comparison
    if      (rhs.type_ == STRING)
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
    if      (rhs.type_ == STRING)
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
  uint32_t * size = 0;

  // Remove the key size from the buffer
  if (buffer_remaining >= sizeof (key_size))
  {
    *(uint32_t *) buffer = Madara::Utility::endian_swap (key_size);
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
    *(uint32_t *) buffer = Madara::Utility::endian_swap (type_);
    buffer += sizeof (type_);
  }
  buffer_remaining -= sizeof (type_);

  // Remove the size of value from the buffer
  if (buffer_remaining >= sizeof (size_))
  {
    // set a pointer to size, in case we need to modify it during
    // value copy (e.g. during double conversion)
    size = (uint32_t *)buffer;

    // this assignment is fixed in the case of type == DOUBLE later
    *size = size_;

    // note that we do not encode the size yet because it may change
    // and we need the architectural-specific version for other checks

    buffer += sizeof (size_);
  }
  buffer_remaining -= sizeof (size_);
  
  // Remove the value from the buffer
  if      (type_ == STRING)
  {
    // strings do not have to be converted
    if (buffer_remaining >= size_)
      strncpy (buffer, str_value_.get_ptr (), size_);
  }
  else if (type_ == INTEGER)
  {
    // strings do not have to be converted
    if (buffer_remaining >= size_)
    {
      // convert integers to network byte order
      Integer * value = (Integer *)buffer;
      *value = Madara::Utility::endian_swap (int_value_);
    }
  }
  else if (type_ == DOUBLE)
  {
    // convert doubles to strings, since it is much more practical to have
    // hosts convert string to double than it is to assume they will be using
    // the appropriate IEEE encoding by default
    std::string converted (to_string ());

    // update size to be the size of the string instead of the double
    *size = (uint32_t) (converted.length () + 1);
      
    // strings do not have to be converted
    if (buffer_remaining >= *size)
    {
      // copy the string to buffer and make sure it is ended in null.
      strncpy (buffer, converted.c_str (), *size - 1);
      buffer[*size - 1] = 0;

    }
  }

  if (size)
  {
    buffer_remaining -= *size;
    buffer += *size;
    *size = (uint32_t) Madara::Utility::endian_swap (*size);
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
    key_size = Madara::Utility::endian_swap (*(uint32_t *)buffer);
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
    type_ = Madara::Utility::endian_swap (*(uint32_t *)buffer);
    buffer += sizeof (type_);
  }
  buffer_remaining -= sizeof (type_);
  
  // Remove the size of value from the buffer
  if (buffer_remaining >= sizeof (size_))
  {
    size_ = Madara::Utility::endian_swap (*(uint32_t *)buffer);
    buff_value_size = size_;
    buffer += sizeof (size_);
  }
  buffer_remaining -= sizeof (size_);
  
  // Remove the value from the buffer
  if (size_ > 0 && buffer_remaining >= sizeof (size_))
  {
    if      (type_ == STRING)
    {
      str_value_ = new char [size_];
      strncpy (str_value_.get_ptr (), buffer, size_);
    }
  
    else if (type_ == INTEGER)
    {
      int_value_ = Madara::Utility::endian_swap (*((Integer *) buffer));
    }

    else if (type_ == DOUBLE)
    {
      // to prevent malicious update packet possibilities, only use the
      // size provided and move this into a string
      std::string double_holder;
      double_holder.assign (buffer, size_);

      // now, do the conversion from string to double
      std::stringstream temp;
      temp << buffer;
      temp >> double_value_;

      // doubles are encoded as strings, so we have to change the size to
      // the appropriate double representation
      size_ = sizeof (double);
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
        DLINFO "Message_Update::apply:" \
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
    if (type_ == INTEGER)
      result = context.set_if_unequal (key, int_value_, 
                                      quality, clock, false);
    else if (type_ == DOUBLE)
      result = context.set_if_unequal (key, double_value_, 
                                      quality, clock, false);
    else if (type_ == STRING)
      result = context.set_if_unequal (key, 
                         std::string (str_value_.get_ptr ()), 
                                      quality, clock, false);
          
    if (perform_lock)
    {
      context.unlock ();
      context.set_changed ();
    }

    // if we actually updated the value
    if (result == 1)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Message_Update::apply:" \
        " received data[%s]=%s.\n", 
        key.c_str (), to_string ().c_str ()));
    }
    // if the data was already current
    else if (result == 0)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Message_Update::apply:" \
          " discarded data[%s]=%s as the value was already set.\n",
          key.c_str (), to_string ().c_str ()));
    }
    else if (result == -1)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Message_Update::apply:" \
        " discarded data due to null key.\n"));
    }
    else if (result == -2)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Message_Update::apply:" \
        " discarded data[%s]=%q due to lower quality.\n",
        key.c_str (), to_string ().c_str ()));
    }
    else if (result == -3)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Message_Update::apply:" \
        " discarded data[%s]=%q due to older timestamp.\n",
        key.c_str (), to_string ().c_str ()));
    }
  }
  return result;
}

bool
Madara::Knowledge_Record::is_true (void) const
{
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
