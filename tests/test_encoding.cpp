

#include "madara/knowledge_engine/Knowledge_Record.h"
#include "madara/transport/Message_Header.h"
#include "madara/transport/Transport.h"
#include "madara/utility/Log_Macros.h"
#include <stdio.h>
#include <iostream>

#define BUFFER_SIZE    1000

int main (int argc, char * argv[])
{
  // buffer for holding encoding results
  char buffer[BUFFER_SIZE];
  int64_t buffer_remaining;
  std::string key;
  bool header_decoded, string_decoded, int_decoded, double_decoded;


  // message headers for encoding and decoding
  Madara::Transport::Message_Header source_header;
  Madara::Transport::Message_Header dest_header;

  // knowledge update for encoding and decoding
  Madara::Knowledge_Record string_helloworld_source ("hello world");
  Madara::Knowledge_Record int_source (Madara::Knowledge_Record::Integer (10));
  Madara::Knowledge_Record double_source (5.5);
  
  Madara::Knowledge_Record dest;

  // Test 1: create the source header
  memset (buffer, 0, BUFFER_SIZE);
  source_header.clock = 100;
  buffer_remaining = BUFFER_SIZE;
  strncpy (source_header.domain, "encoding\0", 9);
  strncpy (source_header.madara_id, "KaRL1.0\0", 8);
  strncpy (source_header.originator, "localhost:34000\0", 16);
  source_header.size = source_header.encoded_size ();
  source_header.type = Madara::Transport::MULTIASSIGN;
  source_header.updates = 0;
  source_header.quality = 5;
  
  std::cerr << "Test 1: encoding source_header to buffer.\n";

  // Test 1: encode source header into the buffer
  source_header.write (buffer, buffer_remaining);
  
  std::cerr << "Test 1: encoded source_header to buffer. Bytes remaining=" << 
    buffer_remaining << std::endl;

  // Test 2: decode source header from buffer
  
  std::cerr << "Test 2: decoding dest_header from buffer.\n";

  buffer_remaining = BUFFER_SIZE - buffer_remaining;
  dest_header.read (buffer, buffer_remaining);
  buffer_remaining = BUFFER_SIZE - buffer_remaining;
  
  std::cerr << "Test 2: decoded dest_header from buffer. Bytes remaining=" << 
    buffer_remaining << std::endl;
  
  header_decoded = dest_header.equals (source_header);

  std::cerr << "Test 2: decoded dest_header is equal to source_header? " <<
    (header_decoded ? "true" : "false") << std::endl;
  
  std::cerr << "Test 3: encoding string_helloworld_source to buffer.\n";

  string_helloworld_source.write (buffer, "message", buffer_remaining);
  
  std::cerr << "Test 3: encoded string_helloworld_source to buffer. " <<
    "Bytes remaining=" << 
    buffer_remaining << std::endl;
  
  std::cerr << "Test 4: decoding string_helloworld_source from buffer.\n";

  buffer_remaining = BUFFER_SIZE - buffer_remaining;
  dest.read (buffer, key, buffer_remaining);
  buffer_remaining = BUFFER_SIZE - buffer_remaining;
  
  std::cerr << "Test 4: decoded " << key << "=" <<
    dest.to_string () << " from buffer. Bytes remaining=" << 
    buffer_remaining << std::endl;
  
  string_decoded = (dest == string_helloworld_source).to_integer () == 1;

  std::cerr << "Test 4: decoded knowledge record is equal to " <<
    "string_helloworld_source? " <<
    (string_decoded ? "true" : "false") <<
    std::endl;
  
  std::cerr << "Test 5: encoding int_source to buffer.\n";

  int_source.write (buffer, "message", buffer_remaining);
  
  std::cerr << "Test 5: encoded int_source to buffer. " <<
    "Bytes remaining=" << 
    buffer_remaining << std::endl;
  
  std::cerr << "Test 6: decoding int_source from buffer.\n";

  buffer_remaining = BUFFER_SIZE - buffer_remaining;
  dest.read (buffer, key, buffer_remaining);
  buffer_remaining = BUFFER_SIZE - buffer_remaining;
  
  std::cerr << "Test 6: decoded " << key << "=" <<
    dest.to_string () << " from buffer. Bytes remaining=" << 
    buffer_remaining << std::endl;
  
  int_decoded = (dest == int_source).to_integer () == 1;

  std::cerr << "Test 6: decoded knowledge record is equal to " <<
    "int_source? " <<
    (int_decoded ? "true" : "false") <<
    std::endl;
  
  std::cerr << "Test 7: encoding double_source to buffer.\n";

  double_source.write (buffer, "message", buffer_remaining);
  
  std::cerr << "Test 7: encoded double_source to buffer. " <<
    "Bytes remaining=" << 
    buffer_remaining << std::endl;
  
  std::cerr << "Test 8: decoding double_source from buffer.\n";

  buffer_remaining = BUFFER_SIZE - buffer_remaining;
  dest.read (buffer, key, buffer_remaining);
  buffer_remaining = BUFFER_SIZE - buffer_remaining;
  
  std::cerr << "Test 8: decoded " << key << "=" <<
    dest.to_string () << " from buffer. Bytes remaining=" << 
    buffer_remaining << std::endl;
  
  double_decoded = (dest == double_source).to_integer () == 1;

  std::cerr << "Test 8: decoded knowledge record is equal to " <<
    "double_source? " <<
    (double_decoded ? "true" : "false") <<
    std::endl;
  

  std::cerr << "\nRESULT: " << 
    (header_decoded && string_decoded && int_decoded && double_decoded ?
    "SUCCESS\n" : "FAIL\n");

  return 0;
}
