#pragma once

#include "service_client.hpp"

#include "error.hpp"

namespace rclc_cppb
{
  template<typename _RequestMessageType, typename _ResponseMessageType>
  ServiceClient<_RequestMessageType, _ResponseMessageType>::ServiceClient(
    const Node* node,
    const char* service_name,
    CallbackType callback,
    RequestDataType default_request_data
  ) noexcept:
    Handle(node, 2),
    service_name(service_name),
    _callback(callback),
    _request_message(Message<RequestMessageType>::from_data(default_request_data))
  {

  }
  
  template<typename _RequestMessageType, typename _ResponseMessageType>
  ServiceClient<_RequestMessageType, _ResponseMessageType>::~ServiceClient() noexcept
  {
    rclc_cppb::error::handled_call<
      decltype(&rcl_client_fini),
      &rcl_client_fini
    >(
      &this->_client,
      this->get_node_handle()
    );
  }

  template<typename _RequestMessageType, typename _ResponseMessageType>
  bool ServiceClient<_RequestMessageType, _ResponseMessageType>::attach(void) noexcept
  {
    static_assert(InitStage::NEW < InitStage::INIT_DONE);
    if(this->_init_stage < InitStage::INIT_DONE)
    {
      const String full_name = this->_node->append_namespace_to_token(this->service_name);
      // Initialize client with default configuration
      if(
        !rclc_cppb::error::handled_call<
          decltype(&rclc_client_init_default),
          &rclc_client_init_default
        >(
          &this->_client,
          this->get_node_handle(),
          Service<RequestMessageType, ResponseMessageType>::get_type_support(),
          full_name.c_str()
        )
      )
      {
        rclc_cppb::error::handled_call<
          decltype(&rcl_client_fini),
          &rcl_client_fini
        >(
          &this->_client,
          this->get_node_handle_mut()
        );
        return false;
      }
      this->_init_stage = InitStage::INIT_DONE;
    }
    static_assert(InitStage::INIT_DONE < InitStage::EXECUTOR_DONE);
    if(this->_init_stage < InitStage::EXECUTOR_DONE)
    {
      // Add client callback to the executor
      if(
        !rclc_cppb::error::handled_call<
          decltype(&rclc_executor_add_client),
          &rclc_executor_add_client
        >(
          Handle::get_executor_mut(),
          &this->_client,
          &this->_response_message,
          (rclc_client_callback_t)this->_callback
        )
      )
      {
        return false;
      }
      this->_init_stage = InitStage::EXECUTOR_DONE;
      Node::spin_once();
    }
    return true;
  }

  template<typename _RequestMessageType, typename _ResponseMessageType>
  void ServiceClient<_RequestMessageType, _ResponseMessageType>::set_request_data(
    RequestDataType request_data
  ) noexcept
  {
    Message<RequestMessageType>::set_data(this->_request_message, request_data);
  }
  template<typename _RequestMessageType, typename _ResponseMessageType>
  typename Message<_RequestMessageType>::DataRef
    ServiceClient<_RequestMessageType, _ResponseMessageType>::get_last_request_data(void) noexcept
  {
    return Message<RequestMessageType>::get_data(this->_request_message);
  }
  template<typename _RequestMessageType, typename _ResponseMessageType>
  typename Message<_ResponseMessageType>::DataRef
    ServiceClient<_RequestMessageType, _ResponseMessageType>::get_last_response_data(void) noexcept
  {
    return Message<ResponseMessageType>::get_data(this->_response_message);
  }
  
  template<typename _RequestMessageType, typename _ResponseMessageType>
  bool ServiceClient<_RequestMessageType, _ResponseMessageType>::call(void) noexcept
  {
    if(this->_init_stage < InitStage::EXECUTOR_DONE)
    {
      return false;
    }
    if(
      !rclc_cppb::error::handled_call<
        decltype(&rcl_send_request),
        &rcl_send_request
      >(
        &this->_client,
        &this->_request_message,
        &this->_sequence_number
      )
    )
    {
      return false;
    }
    Node::spin_once();
    return true;
  }
  
  template<typename _RequestMessageType, typename _ResponseMessageType>
  bool ServiceClient<_RequestMessageType, _ResponseMessageType>::call(
    RequestDataType request_data
  ) noexcept
  {
    this->set_request_data(request_data);
    return this->call();
  }
}