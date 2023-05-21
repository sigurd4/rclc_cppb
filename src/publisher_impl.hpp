#pragma once

#include "publisher.hpp"

#include "error.hpp"

//https://micro.ros.org/docs/tutorials/programming_rcl_rclc/pub_sub/

namespace rclc_cppb
{
  template<typename _MessageType>
  Publisher<_MessageType>::Publisher(
    Node* const node,
    const char* const topic_name,
    const typename Publisher<MessageType>::DataType default_data
  ) noexcept:
    Handle(node),
    topic_name(topic_name),
    _message(Message<MessageType>::from_data(default_data))
  {
    
  }

  template<typename _MessageType>
  Publisher<_MessageType>::~Publisher() noexcept
  {
    this->_init_done = false;

    rclc_cppb::error::handled_call<
      decltype(&rcl_publisher_fini),
      &rcl_publisher_fini
    >(
      &this->_publisher,
      this->get_node_handle_mut()
    );
  }

  template<typename _MessageType>
  bool Publisher<_MessageType>::advertise() noexcept
  {
    if(!this->_init_done)
    {
      // Creates a reliable rcl publisher
      // For best effor, use: rclc_publisher_init_best_effort
      if(
        !rclc_cppb::error::handled_call<
          decltype(&rclc_publisher_init_default),
          &rclc_publisher_init_default
        >(
          &this->_publisher,
          this->get_node_handle(),
          Message<MessageType>::get_type_support(),
          this->topic_name
        )
      )
      {
        rclc_cppb::error::handled_call<
          decltype(&rcl_publisher_fini),
          &rcl_publisher_fini
        >(
          &this->_publisher,
          this->get_node_handle_mut()
        );
        return false;
      }
      this->_init_done = true;
      
      Node::spin_once();
    }
    return true;
  }

  template<typename _MessageType>
  void Publisher<_MessageType>::set_data(DataType data) noexcept
  {
    Message<MessageType>::set_data(this->_message, data);
  }
  template<typename _MessageType>
  typename Message<_MessageType>::DataRef
    Publisher<_MessageType>::get_last_data(void) noexcept
  {
    return Message<MessageType>::get_data(this->_message);
  }

  template<typename _MessageType>
  bool Publisher<_MessageType>::publish(void) const noexcept
  {
    if(
      !this->_init_done ||
      !rclc_cppb::error::handled_call<
        decltype(&rcl_publish),
        &rcl_publish
      >(
        &this->_publisher,
        &this->_message,
        (rmw_publisher_allocation_t*)NULL
      )
    )
    {
      return false;
    }
    Node::spin_once();
    return true;
  }

  template<typename _MessageType>
  bool Publisher<_MessageType>::publish(DataType data) noexcept
  {
    this->set_data(data);
    return this->publish();
  }
}