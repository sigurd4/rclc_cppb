#ifndef RCLC_CPPB__SERVICE_H_
  #define RCLC_CPPB__SERVICE_H_
  
  #include <rcl/rcl.h>
  #include <rcl/error_handling.h>
  #include <rclc/rclc.h>
  #include <rclc/executor.h>
  
  #define INCLUDE_ALL_STD_SRVS false
  #if INCLUDE_ALL_STD_SRVS
    #include <std_srvs/srv/empty.h>
  #endif

  namespace rclc_cppb
  {
    /**
     * Service trait.
     * 
     * Implementation is mandatory for service types used with publishers, subscribers, etc.
     * 
     * Implement by specializing the struct with your chosen message type pair as generic arguments.
     * 
     * @param <_RequestMessageType> The request message-type for which the trait is implemented
     * @param <_ResponseMessageType> The response message-type for which the trait is implemented
    */
    template<typename _RequestMessageType, typename _ResponseMessageType>
    struct Service
    {
      /**
       * Public export of _RequestMessageType
      */
      using RequestMessageType = typename Message<_RequestMessageType>::MessageType;
      /**
       * Internal data-type of request message-type
      */
      using RequestDataType = typename Message<_RequestMessageType>::DataType;
      
      /**
       * Public export of _ResponseMessageType
      */
      using ResponseMessageType = typename Message<_ResponseMessageType>::MessageType;
      /**
       * Internal data-type of response message-type
      */
      using ResponseDataType = typename Message<_ResponseMessageType>::DataType;
      
      /**
       * true if trait is implemented
       * 
       * Please override this and set to true in implementation
      */
      constexpr static const bool IS_IMPL = false;
      
      /**
       * Retrieves pointer to rclc type-support-struct of this service-type
      */
      static const rosidl_service_type_support_t* get_type_support(void) noexcept;
    };
  }
  
  #define SRV_IMPL(REQUEST_MESSAGE_TYPE, RESPONSE_MESSAGE_TYPE, TYPE_SUPPORT) \
  namespace rclc_cppb \
  { \
    static_assert(Message<REQUEST_MESSAGE_TYPE>::IS_IMPL, "Trait Message must be implemented for request!"); \
    static_assert(Message<RESPONSE_MESSAGE_TYPE>::IS_IMPL, "Trait Message must be implemented for response!"); \
    \
    template<> \
    struct Service<REQUEST_MESSAGE_TYPE, RESPONSE_MESSAGE_TYPE> \
    { \
      using RequestMessageType = typename Message<REQUEST_MESSAGE_TYPE>::MessageType; \
      using RequestDataType = typename Message<REQUEST_MESSAGE_TYPE>::DataType; \
      \
      using ResponseMessageType = typename Message<RESPONSE_MESSAGE_TYPE>::MessageType; \
      using ResponseDataType = typename Message<RESPONSE_MESSAGE_TYPE>::DataType; \
      \
      constexpr static const bool IS_IMPL = true; \
      \
      static const rosidl_service_type_support_t* get_type_support(void) noexcept \
      { \
        return TYPE_SUPPORT; \
      } \
    }; \
  }
#endif

#if defined(STD_SRVS__SRV__EMPTY_H_) && !defined(RCLC_CPPB__SERVICE_H__EMPTY_)
  #define RCLC_CPPB__SERVICE_H__EMPTY_
  #include <std_srvs/srv/empty.h>
  #include <std_msgs/msg/empty.h>
  #include "message.hpp"
  
  SRV_IMPL(std_msgs__msg__Empty, std_msgs__msg__Empty, ROSIDL_GET_SRV_TYPE_SUPPORT(std_srvs, srv, Empty))
#endif