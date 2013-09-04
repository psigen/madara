#ifndef  _MESSAGE_HEADER_H_
#define  _MESSAGE_HEADER_H_

/**
 * @file Message_Header.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains the message header used by default in some KaRL
 * network transports (e.g. IP multicast and broadcast)
 **/

#include "madara/utility/stdint.h"
#include "madara/MADARA_export.h"

namespace Madara
{
  namespace Transport
  {
    #define MADARA_IDENTIFIER_LENGTH    8
    #define MADARA_IDENTIFIER           "KaRL1.3"
    #define MADARA_DOMAIN_MAX_LENGTH    32
    #define PAIR_COUNT_TYPE             uint32_t
    #define KNOWLEDGE_QUALITY_TYPE      uint32_t
    #define CLOCK_TYPE                  uint64_t
    #define MAX_KNOWLEDGE_KEY_LENGTH    64
    #define MAX_ORIGINATOR_LENGTH       64
    #define KNOWLEDGE_VALUE_TYPE        Madara::Knowledge_Record

    static const int  MAX_PACKET_SIZE = 512000;  // 512kb

    
    /**
    * @class Message_Header
    * @brief Defines a robust message header which is the default for
    *        KaRL messages. @see Reduced_Message_Header for a smaller
    *        header that supports less QoS and features but a more
    *        compact size.
    *        
    *        Format:
    *
    *        [0] [64 bit unsigned size]<br />
    *        [8] [8 byte transport id]<br />
    *        [16] [32 byte domain name]<br />
    *        [48] [64 byte originator (generally host:port)]<br />
    *        [112] [32 bit unsigned type]<br />
    *           2 = MULTIASSIGN (most common type)<br />
    *        [116] [32 bit unsigned num updates]<br />
    *        [120] [32 bit unsigned quality (type of priority)]<br />
    *        [124] [64 bit unsigned Lamport clock]<br />
    *        [132] [64 bit unsigned wall clock timestamp]<br />
    *        [140] [8 bit unsigned ttl--for rebroadcasts]<br />
    *        [141] [knowledge updates start here in the buffer]
    */

    class MADARA_Export Message_Header
    {
    public:
      
      /**
       * Constructor
       **/
      Message_Header ();

      /**
       * Destructor
       **/
      virtual ~Message_Header ();

      /**
       * Returns the size of the encoded Message_Header class, which may be
       * different from sizeof (Message_Header) because of compiler
       * optimizations for word boundaries
       **/
      virtual uint32_t encoded_size (void) const;

      /**
       * Returns the size of the encoded Message_Header class. This function
       * differs from encoded_size () in that encoded_size reports
       * polymorphically, while this method may be called statically and
       * always refers to the base class's size.
       **/
      static uint32_t static_encoded_size (void);

      /**
       * Reads a Message_Header instance from a buffer and updates
       * the amount of buffer room remaining.
       * @param     buffer     the readable buffer where data is stored
       * @param     buffer_remaining  the count of bytes remaining in the
       *                              buffer to read
       * @return    current buffer position for next read
       **/
      virtual const char * read (const char * buffer,
        int64_t & buffer_remaining);
      
      /**
       * Writes a Message_Header instance to a buffer and updates
       * the amount of buffer room remaining.
       * @param     buffer     the readable buffer where data is stored
       * @param     buffer_remaining  the count of bytes remaining in the
       *                              buffer to read
       * @return    current buffer position for next write
       **/
      virtual char * write (char * buffer, int64_t & buffer_remaining);
 
      /**
       * Compares the fields of this instance to another instance
       * @param     other      the other instance to compare against
       * @return    true if equal, false otherwise
       **/
      virtual bool equals (const Message_Header & other);
      
      /**
       * Tests the buffer for a normal message identifier
       * @return   true if identifier indicates reduced message header
       **/
      static inline bool message_header_test (const char * buffer)
      {
        return strncmp (&(buffer[8]), MADARA_IDENTIFIER, 7) == 0; 
      }

      /**
       * the size of this header plus the updates
       **/
      uint64_t          size;
      
      /**
       * the identifier of this transport (MADARA_IDENTIFIER)
       **/
      char                  madara_id[MADARA_IDENTIFIER_LENGTH];
      
      /**
       * the domain that this message is intended for
       **/
      char                  domain[MADARA_DOMAIN_MAX_LENGTH];
      
      /**
       * the originator of the message (host:port)
       **/
      char                  originator[MAX_ORIGINATOR_LENGTH];
      
      /**
       * the type of message @see Madara::Transport::Messages enum
       **/
      uint32_t              type;

      /**
       * the number of knowledge variable updates in the message
       **/
      uint32_t              updates;
      
      /**
       * the quality of the message sender
       **/
      uint32_t              quality;
      
      /**
       * the clock of the sender when the message was generated
       **/
      uint64_t              clock;

      /**
       * the timestamp of the sender when the message was generated
       **/
      uint64_t              timestamp;

      /**
       * time to live (number of rebroadcasts to perform after original send
       **/
      unsigned char         ttl;
    };
  }
}


#endif   // _MESSAGE_HEADER_H_