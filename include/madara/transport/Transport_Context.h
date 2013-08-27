#ifndef   _TRANSPORT_CONTEXT_H_
#define   _TRANSPORT_CONTEXT_H_

#include "madara/MADARA_export.h"
#include "madara/utility/stdint.h"

namespace Madara
{
  namespace Transport
  {
    /**
     * @class Transport_Context
     * @brief Provides context about the transport
     **/
    
    class MADARA_Export Transport_Context
    {
    public:

      enum {
        IDLE_OPERATION = 0,
        SENDING_OPERATION = 1,
        RECEIVING_OPERATION = 2,
        REBROADCASTING_OPERATION = 3
      };

      /**
       * Constructor
       **/
      Transport_Context (int64_t operation = IDLE_OPERATION,
        uint32_t receive_bandwidth = 0, uint32_t send_bandwidth = 0);

      /**
       * Copy constructor
       * @param  rhs   the value to be copied into this class
       **/
      Transport_Context (const Transport_Context & rhs);

      /**
       * Destructor
       **/
      ~Transport_Context ();
      
      /**
       * Assignment operator
       * @param  rhs   the value to be copied into this class
       **/
      void operator= (const Transport_Context & rhs);
      
      /**
       * Get operation that the context is performing
       * @return     operation being performed. See enums
       *             in this class.
       **/
      int64_t get_operation (void);

      /**
       * Sets the operation that the context is/should be performing
       * @param  operation   operation the context is undertaking
       **/
      void set_operation (int64_t operation);

      /**
       * Gets the receive bandwidth in bytes per second
       * @return     the bandwidth taken up in receive operations
       **/
      uint32_t  get_receive_bandwidth (void);
      
      /**
       * Gets the send/rebroadcast bandwidth in bytes per second
       * @return     the bandwidth taken up in send/rebroadcast operations
       **/
      uint32_t  get_send_bandwidth (void);

      
      /**
       * Sets the bandwidth used for receives in bytes per second
       * @param  bandwidth   bandwidth being used by the transport
       **/
      void set_receive_bandwidth (uint32_t bandwidth);
      
      /**
       * Sets the bandwidth used for sends/rebroadcast in bytes per second
       * @param  bandwidth   bandwidth being used by the transport
       **/
      void set_send_bandwidth (uint32_t bandwidth);

    private:

      /**
       * Operation being performed
       **/
      int64_t operation_;

      /**
       * Bandwidth being utilized for receive operations
       **/
      uint32_t receive_bandwidth_;

      /**
       * Bandwidth being utilized for send operations
       **/
      uint32_t send_bandwidth_;
    };
  }
}

#include "Transport_Context.inl"

#endif