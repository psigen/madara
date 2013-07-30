#ifndef  _REDUCED_MESSAGE_HEADER_H_
#define  _REDUCED_MESSAGE_HEADER_H_

/**
* @file Message_Header.h
* @author James Edmondson <jedmondson@gmail.com>
*
* This file contains the message header used by default in some KaRL
* network transports (e.g. IP multicast and broadcast)
**/

#include "madara/utility/stdint.h"
#include "madara/MADARA_export.h"
#include "madara/transport/Message_Header.h"

namespace Madara
{
  namespace Transport
  {

    #define REDUCED_MADARA_ID    "karl1.2"

    /**
    * @class Reduced_Message_Header
    * @brief Defines a robust message header which is the default for
    *        KaRL messages. @see Reduced_Message_Header for a smaller
    *        header that supports less QoS and features but a more
    *        compact size.
    *        
    *        Format:
    *
    *        size = buffer[0] (8 byte)
    *        transport id = buffer[8] (8 byte)
    *        updates = buffer[16] (unsigned 4 byte number of updates)
    *        clock = buffer[20] (unsigned 8 byte clock for this message)
    *        knowledge = buffer[28] (the new knowledge starts here)
    */

    class MADARA_Export Reduced_Message_Header : public Message_Header
    {
    public:

      /**
       * Constructor
       **/
      Reduced_Message_Header ();

      /**
       * Destructor
       **/
      virtual ~Reduced_Message_Header ();

      /**
      * Returns the size of the encoded Message_Header class, which may be
      * different from sizeof (Message_Header) because of compiler
      * optimizations for word boundaries
      **/
      virtual int encoded_size (void) const;

      /**
      * Reads a Message_Header instance from a buffer and updates
      * the amount of buffer room remaining.
      * @param     buffer     the readable buffer where data is stored
      * @param     buffer_remaining  the count of bytes remaining in the
      *                              buffer to read
      * @return    current buffer position for next read
      **/
      virtual char * read (char * buffer, int64_t & buffer_remaining);

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
       * Tests the buffer for a reduced message identifier
       * @return   true if identifier indicates reduced message header
       **/
      static inline bool reduced_message_header_test (char * buffer)
      {
        return strncmp (&(buffer[8]), REDUCED_MADARA_ID, 7) == 0; 
      }
    };
  }
}


#endif   // _REDUCED_MESSAGE_HEADER_H_