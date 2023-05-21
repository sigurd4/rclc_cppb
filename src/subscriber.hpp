#pragma once

#include <micro_ros_arduino.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include "message.hpp"
#include "node.hpp"
#include "handle.hpp"

namespace rclc_cppb
{
  /**
   * ROS2 subscriber designed to be similar to the Subscriber class in rclcpp.
   * 
   * Subscribers are used to subscribe to topics and receive messages in a callback.
   * 
   * Usage instructions:
   * - Instantiate before any node is setup.
   * - Call subscribe() in on_setup-method of node or after node setup is completed.
   * - Message type must have Message trait implemented on it.
   * @param <_MessageType> Message type handled by subscriber
  */
  template<typename _MessageType>
  class Subscriber: Handle
  {
    static_assert(Message<_MessageType>::IS_IMPL, "Trait Message must be implemented!");

    public:
      /**
       * Message type handled by subscriber
      */
      using MessageType = _MessageType;
      /**
       * Internal data type of message
      */
      using DataType = typename Message<MessageType>::DataType;
      /**
       * Reference to internal data type of message
      */
      using DataRef = typename Message<MessageType>::DataRef;
      
      /**
       * Function-pointer type of callback function used by this subscriber
      */
      using CallbackType = void(*)(
        const MessageType* message
      );
    public:
      /**
       * Topic name
      */
      const char* const topic_name;
    private:
      /**
       * Callback function used by this subscriber
      */
      CallbackType _callback;
      /**
       * Message
      */
      MessageType _message;
      /**
       * rclc subscription entity
      */
      rcl_subscription_t _subscription;
      /**
       * Stage of initialization for this subscriber
      */
      InitStage _init_stage = InitStage::NEW;

    public:
      /**
       * ROS2 subscriber designed to be similar to the Subscriber class in rclcpp.
       * 
       * Usage instructions:
       * - Instantiate before any node is setup
       * - Call subscribe() in on_setup-method of node or after node setup is completed
       * - Message type must have Message trait implemented on it
       * @param <_MessageType> Message type handled by subscriber
       * @param node Pointer to node owning the subscriber
       * @param topic_name Topic name (slash and namespace of node is appended later)
       * @param callback Pointer to callback-function used by this subscriber
      */
      Subscriber(Node* node, const char* topic_name, CallbackType callback) noexcept;

      ~Subscriber() noexcept;

      /**
       * Initializes the subscriber, and then subscribes to the topic on the ROS2 network.
       * Node must be successfully initialized for this to succeed.
       * @return true if success
      */
      bool subscribe(rclc_executor_handle_invocation_t invocation = ON_NEW_DATA) noexcept;
      
      /**
       * Retrieves last received message data as reference
       * @return Reference to message data
      */
      DataRef get_last_data(void) noexcept;
  };
}

#include "subscriber_impl.hpp"