#pragma once

#include "message.hpp"
#include "service.hpp"
#include "node.hpp"
#include "handle.hpp"

namespace rclc_cppb
{
  /**
   * ROS2 service client.
   * 
   * A service client is used to perform service calls, and then receive the response in a callback.
   * 
   * Usage instructions:
   * - Instantiate before any node is setup.
   * - Call attach() in on_setup-method of node or after node setup is completed.
   * - Message types must have Message trait implemented on them.
   * - Message type pair must have Service trait implemented on them.
   * @param <_RequestMessageType> Request message type handled by service client
   * @param <_ResponseMessageType> Response message type handled by service client
  */
  template<typename _RequestMessageType, typename _ResponseMessageType>
  class ServiceClient: Handle
  {
    static_assert(Message<_RequestMessageType>::IS_IMPL, "Trait Message must be implemented for request!");
    static_assert(Message<_ResponseMessageType>::IS_IMPL, "Trait Message must be implemented for response!");
    static_assert(Service<_RequestMessageType, _ResponseMessageType>::IS_IMPL, "Trait Service must be implemented!");

    public:
      /**
       * Request message type handled by service client
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
       * Response message type handled by service client
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
       * Function-pointer type of callback function used by this service client
      */
      using CallbackType = void(*)(
        const ResponseMessageType* response_message
      );

    public:
      /**
       * Service name
      */
      const char* const service_name;

    private:
      /**
       * rclc client entity
      */
      rcl_client_t _client;
      /**
       * Request message
      */
      RequestMessageType _request_message;
      /**
       * Response message
      */
      ResponseMessageType _response_message;
      /**
       * Given by the executor to keep track of the clients it manages
      */
      int64_t _sequence_number;
      /**
       * Callback function used by this service client
      */
      CallbackType _callback;
      /**
       * Stage of initialization for this service client
      */
      InitStage _init_stage = InitStage::NEW;
      
    public:
      /**
       * ROS2 service client.
       * 
       * A service client is used to perform service calls, then receive the response in a callback.
       * 
       * Usage instructions:
       * - Instantiate before any node is setup.
       * - Call attach() in on_setup-method of node or after node setup is completed.
       * - Message types must have Message trait implemented on them.
       * - Message type pair must have Service trait implemented on them.
       * @param <_RequestMessageType> Request message type handled by service client
       * @param <_ResponseMessageType> Response message type handled by service client
       * @param node Pointer to node owning the service client
       * @param service_name Service name (slash and namespace of node is appended later)
       * @param callback Pointer to callback-function used by this service client
       * @param default_request_data Initial request message data
      */
      ServiceClient(
        const Node* node,
        const char* service_name,
        CallbackType callback,
        RequestDataType default_request_data
      ) noexcept;

      ~ServiceClient() noexcept;

      /**
       * Initializes the service client, and then attaches to the service on the ROS2 network.
       * Node must be successfully initialized for this to succeed.
       * @return true if success
      */
      bool attach(void) noexcept;

      /**
       * Sets request message data.
       * @param request_data Request message data
      */
      void set_request_data(RequestDataType request_data) noexcept;
      /**
       * Retrieves last sent request message data as reference
       * @return Reference to request message data
      */
      RequestDataRef get_last_request_data(void) noexcept;
      /**
       * Retrieves last received response message data as reference
       * @return Reference to response message data
      */
      ResponseDataRef get_last_response_data(void) noexcept;

      /**
       * Calls the service with the current request message.
       * Service client must be successfully attached for this to succeed.
       * @return true if success
      */
      bool call(void) noexcept;
      /**
       * Calls the service with given data in request message.
       * Service client must be successfully attached for this to succeed.
       * @param request_data Request message data
       * @return true if success
      */
      bool call(RequestDataType request_data) noexcept;
  };
}

#include "service_client_impl.hpp"