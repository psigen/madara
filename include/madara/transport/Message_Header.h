#ifndef  _MESSAGE_HEADER_H_
#define  _MESSAGE_HEADER_H_

#include "madara/utility/stdint.h"
#include "madara/MADARA_export.h"

namespace Madara
{
  namespace Transport
  {
    #define MADARA_IDENTIFIER_LENGTH    8
    #define MADARA_IDENTIFIER           "KaRL1.1\0"
    #define MADARA_DOMAIN_MAX_LENGTH    32
    #define PAIR_COUNT_TYPE             uint32_t
    #define KNOWLEDGE_QUALITY_TYPE      uint32_t
    #define CLOCK_TYPE                  uint64_t
    #define MAX_KNOWLEDGE_KEY_LENGTH    64
    #define MAX_ORIGINATOR_LENGTH       64
    #define KNOWLEDGE_VALUE_TYPE        Madara::Knowledge_Record

    static const int64_t  MAX_PACKET_SIZE = 512000;  // 512kb

    class MADARA_Export Message_Header
    {
    public:

      /**
       * Returns the size of the encoded Message_Header class, which may be
       * different from sizeof (Message_Header) because of compiler
       * optimizations for word boundaries
       **/
      inline int encoded_size (void)
      {
        return sizeof (uint64_t) * 2
          + sizeof (char) * (MADARA_IDENTIFIER_LENGTH + MADARA_DOMAIN_MAX_LENGTH
                             + MAX_ORIGINATOR_LENGTH)
          + sizeof (uint32_t) * 2;
      }

      /**
       * Reads a Message_Header instance from a buffer and updates
       * the amount of buffer room remaining.
       * @param     buffer     the readable buffer where data is stored
       * @param     buffer_remaining  the count of bytes remaining in the
       *                              buffer to read
       * @return    current buffer position for next read
       **/
      char * read (char * buffer, int64_t & buffer_remaining);
      
      /**
       * Writes a Message_Header instance to a buffer and updates
       * the amount of buffer room remaining.
       * @param     buffer     the readable buffer where data is stored
       * @param     buffer_remaining  the count of bytes remaining in the
       *                              buffer to read
       * @return    current buffer position for next write
       **/
      char * write (char * buffer, int64_t & buffer_remaining);
 
      /**
       * Compares the fields of this instance to another instance
       * @param     other      the other instance to compare against
       * @return    true if equal, false otherwise
       **/
      bool equals (const Message_Header & other);

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


    };
  }
}


#endif   // _MESSAGE_HEADER_H_