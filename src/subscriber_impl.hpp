#pragma once

#include "subscriber.hpp"

#include "error.hpp"

//https://micro.ros.org/docs/tutorials/programming_rcl_rclc/pub_sub/

namespace rclc_cppb
{
  template<typename _MessageType>
  Subscriber<_MessageType>::Subscriber(
    Node* node,
    const char* const topic_name,
    CallbackType callback
  ) noexcept:
    Handle(node),
    topic_name(topic_name),
    _callback(callback)
  {
    
  }

  template<typename _MessageType>
  Subscriber<_MessageType>::~Subscriber() noexcept
  {
    rclc_cppb::error::handled_call<
      decltype(&rcl_subscription_fini),
      &rcl_subscription_fini
    >(
      &this->_subscription,
      this->get_node_handle_mut()
    );
  }

  template<typename _MessageType>
  bool Subscriber<_MessageType>::subscribe(
    rclc_executor_handle_invocation_t invocation
  ) noexcept
  {
    using InitStage = Subscriber<MessageType>::InitStage;

    static_assert(InitStage::NEW < InitStage::INIT_DONE);
    if(this->_init_stage < InitStage::INIT_DONE)
    {
      if(
        !rclc_cppb::error::handled_call<
          decltype(&rclc_subscription_init_default),
          &rclc_subscription_init_default
        >(
          &this->_subscription,
          this->get_node_handle_mut(),
          Message<MessageType>::get_type_support(),
          this->topic_name
        )
      )
      {
        rclc_cppb::error::handled_call<
          decltype(&rcl_subscription_fini),
          &rcl_subscription_fini
        >(
          &this->_subscription,
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
          decltype(&rclc_executor_add_subscription),
          &rclc_executor_add_subscription
        >(
          Handle::get_executor_mut(),
          &this->_subscription,
          &this->_message,
          (rclc_subscription_callback_t)this->_callback,
          invocation
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
  
  template<typename _MessageType>
  typename Message<_MessageType>::DataRef
    Subscriber<_MessageType>::get_last_data(void) noexcept
  {
    return Message<MessageType>::get_data(this->_message);
  }
}