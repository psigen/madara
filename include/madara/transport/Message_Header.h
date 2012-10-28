#ifndef  _MESSAGE_HEADER_H_
#define  _MESSAGE_HEADER_H_

#include "madara/utility/Log_Macros.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"

namespace Madara
{
  namespace Transport
  {
    #define MADARA_IDENTIFIER_LENGTH    8
    #define MADARA_IDENTIFIER           "KaRL1.0\0"
    #define MADARA_DOMAIN_MAX_LENGTH    32
    #define PAIR_COUNT_TYPE             unsigned int
    #define KNOWLEDGE_QUALITY_TYPE      unsigned int
    #define CLOCK_TYPE                  unsigned long long
    #define MAX_KNOWLEDGE_KEY_LENGTH    64
    #define MAX_ORIGINATOR_LENGTH       64
    #define KNOWLEDGE_VALUE_TYPE        Madara::Knowledge_Record::VALUE_TYPE
    #define MAX_PACKET_SIZE             512000  // 512kb

    class Message_Header
    {
    public:
      /**
       * the size of this header plus the updates
       **/
      unsigned int          size;
      
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
      unsigned int          type;

      /**
       * the number of knowledge variable updates in the message
       **/
      unsigned int          updates;
      
      /**
       * the quality of the message sender
       **/
      unsigned int          quality;
      
      /**
       * the clock of the sender when the message was generated
       **/
      unsigned long long    clock;
    };

    class Message_Update
    {
    public:
      /**
       * the knowledge key
       **/
      char                  key[MAX_KNOWLEDGE_KEY_LENGTH];

      /**
       * the knowledge value
       **/
      KNOWLEDGE_VALUE_TYPE  value;

      int apply (Madara::Knowledge_Engine::Thread_Safe_Context & context,
        unsigned int quality, unsigned long long clock, bool perform_lock)
      {
        int result = -1;

        if (key[0])
        {
          MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
              DLINFO "Message_Update::apply:" \
              " attempting to set %s=%q\n", key, value));

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
          result = context.set_if_unequal (key, value, 
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
              " received data[%s]=%q.\n", 
              key, value));
          }
          // if the data was already current
          else if (result == 0)
          {
            MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
                DLINFO "Message_Update::apply:" \
                " discarded data[%s]=%q as the value was already set.\n",
                key, value));
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
              key, value));
          }
          else if (result == -3)
          {
            MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
              DLINFO "Message_Update::apply:" \
              " discarded data[%s]=%q due to older timestamp.\n",
              key, value));
          }
        }
        return result;
      }

    };
  }
}


#endif   // _MESSAGE_HEADER_H_