#pragma once

#include "service_server.hpp"

#include "error.hpp"

namespace rclc_cppb
{
  template<typename _RequestMessageType, typename _ResponseMessageType>
  ServiceServer<_RequestMessageType, _ResponseMessageType>::ServiceServer(
    Node* node,
    const char* service_name,
    CallbackType callback
  ) noexcept:
    Handle(node, 2),
    service_name(service_name),
    _callback(callback)
  {
    
  }
  
  template<typename _RequestMessageType, typename _ResponseMessageType>
  ServiceServer<_RequestMessageType, _ResponseMessageType>::~ServiceServer() noexcept
  {
    rclc_cppb::error::handled_call<
      decltype(&rcl_service_fini),
      &rcl_service_fini
    >(
      &this->_service,
      this->_node->get_handle_mut()
    );
  }

  template<typename _RequestMessageType, typename _ResponseMessageType>
  bool ServiceServer<_RequestMessageType, _ResponseMessageType>::advertise(void) noexcept
  {
    static_assert(InitStage::NEW < InitStage::INIT_DONE);
    if(this->_init_stage < InitStage::INIT_DONE)
    {
      const String full_name = this->_node->append_namespace_to_token(this->service_name);
      // Initialize server with default configuration
      if(
        !rclc_cppb::error::handled_call<
          decltype(&rclc_service_init_default),
          &rclc_service_init_default
        >(
          &this->_service,
          this->_node->get_handle(),
          Service<RequestMessageType, ResponseMessageType>::get_type_support(),
          full_name.c_str()
        )
      )
      {
        rclc_cppb::error::handled_call<
          decltype(&rcl_service_fini),
          &rcl_service_fini
        >(
          &this->_service,
          this->get_node_handle_mut()
        );
        return false;
      }
      this->_init_stage = InitStage::INIT_DONE;
    }
    static_assert(InitStage::INIT_DONE < InitStage::EXECUTOR_DONE);
    if(this->_init_stage < InitStage::EXECUTOR_DONE)
    {
      if(
        !rclc_cppb::error::handled_call<
          decltype(&rclc_executor_add_service),
          &rclc_executor_add_service
        >(
          Handle::get_executor_mut(),
          &this->_service,
          &this->_request_message,
          &this->_response_message,
          (rclc_service_callback_t)this->_callback
        )
      )
      {
        // remove service from executor?
        return false;
      }
      this->_init_stage = InitStage::EXECUTOR_DONE;
      
      Node::spin_once();
    }
    return true;
  }
  
  template<typename _RequestMessageType, typename _ResponseMessageType>
  typename Message<_RequestMessageType>::DataRef
    ServiceServer<_RequestMessageType, _ResponseMessageType>::get_last_request_data(void) noexcept
  {
    return Message<RequestMessageType>::get_data(this->_request_message);
  }
  template<typename _RequestMessageType, typename _ResponseMessageType>
  typename Message<_ResponseMessageType>::DataRef
    ServiceServer<_RequestMessageType, _ResponseMessageType>::get_last_response_data(void) noexcept
  {
    return Message<ResponseMessageType>::get_data(this->_response_message);
  }
}