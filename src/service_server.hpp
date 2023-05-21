#pragma once

#include "message.hpp"
#include "service.hpp"
#include "node.hpp"
#include "handle.hpp"

namespace rclc_cppb
{
  /**
   * ROS2 service server.
   * 
   * A service server is used to handle service calls in a callback, then send back a response.
   * 
   * Usage instructions:
   * - Instantiate before any node is setup.
   * - Call advertise() in on_setup-method of node or after node setup is completed.
   * - Message types must have Message trait implemented on them.
   * - Message type pair must have Service trait implemented on them.
   * @param <_RequestMessageType> Request message type handled by service server
   * @param <_ResponseMessageType> Response message type handled by service server
  */
  template<typename _RequestMessageType, typename _ResponseMessageType>
  class ServiceServer: Handle
  {
    static_assert(Message<_RequestMessageType>::IS_IMPL, "Trait Message must be implemented for request!");
    static_assert(Message<_ResponseMessageType>::IS_IMPL, "Trait Message must be implemented for response!");
    static_assert(Service<_RequestMessageType, _ResponseMessageType>::IS_IMPL, "Trait Service must be implemented!");

    public:
      /**
       * Request message type handled by service server
      */
      using RequestMessageType = _RequestMessageType;
      /**
       * Internal data type of request message
      */
      using RequestDataType = typename Message<RequestMessageType>::DataType;
      /**
       * Reference to internal data type of request message
      */
      using RequestDataRef = typename Message<RequestMessageType>::DataRef;

      /**
       * Response message type handled by service server
      */
      using ResponseMessageType = _ResponseMessageType;
      /**
       * Internal data type of response message
      */
      using ResponseDataType = typename Message<ResponseMessageType>::DataType;
      /**
       * Reference to internal data type of response message
      */
      using ResponseDataRef = typename Message<ResponseMessageType>::DataRef;

      /**
       * Function-pointer type of callback function used by this service server
      */
      using CallbackType = void(*)(
        const RequestMessageType* request_message,
        ResponseMessageType* response_message
      );
    public:
      /**
       * Service name
      */
      const char* const service_name;
    private:
      /**
       * rclc service entity
      */
      rcl_service_t _service;
      /**
       * Request message
      */
      RequestMessageType _request_message;
      /**
       * Response message
      */
      ResponseMessageType _response_message;
      /**
       * Callback function used by this service server
      */
      CallbackType _callback;
      /**
       * Stage of initialization for this service server
      */
      InitStage _init_stage = InitStage::NEW;

    public:
      /**
       * ROS2 service server.
       * 
       * A service server is used to handle service calls in a callback, then send back a response.
       * 
       * Usage instructions:
       * - Instantiate before any node is setup.
       * - Call advertise() in on_setup-method of node or after node setup is completed.
       * - Message types must have Message trait implemented on them.
       * - Message type pair must have Service trait implemented on them.
       * @param <_RequestMessageType> Request message type handled by service server
       * @param <_ResponseMessageType> Response message type handled by service server
       * @param node Pointer to node owning the service server
       * @param service_name Service name (slash and namespace of node is appended later)
       * @param callback Pointer to callback-function used by this service server
      */
      ServiceServer(
        Node* node,
        const char* service_name,
        CallbackType callback
      ) noexcept;

      ~ServiceServer() noexcept;

      /**
       * Initializes the service server, and then advertises the service onto the ROS2 network.
       * Node must be successfully initialized for this to succeed.
       * @return true if success
      */
      bool advertise(void) noexcept;
      
      /**
       * Retrieves last received request message data as reference
       * @return Reference to request message data
      */
      RequestDataRef get_last_request_data(void) noexcept;
      /**
       * Retrieves last sent response message data as reference
       * @return Reference to response message data
      */
      ResponseDataRef get_last_response_data(void) noexcept;
  };
}

#include "service_server_impl.hpp"