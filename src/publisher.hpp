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
   * ROS2 publisher designed to be similar to the Publisher class in rclcpp.
   * 
   * Publishers are used to publish messages on topics.
   * 
   * Usage instructions:
   * - Instantiate before any node is setup.
   * - Call advertise() in on_setup-method of node or after node setup is completed.
   * - Message type must have Message trait implemented on it.
   * @param <_MessageType> Message type handled by publisher
  */
  template<typename _MessageType>
  class Publisher: Handle
  {
    static_assert(Message<_MessageType>::IS_IMPL, "Trait Message must be implemented!");
    
    public:
      /**
       * Message type handled by publisher
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
    public:
      /**
       * Topic name
      */
      const char* const topic_name;
    private:
      /**
       * Message
      */
      MessageType _message;
      /**
       * rclc publisher entity
      */
      rcl_publisher_t _publisher;
      /**
       * true if publisher has been successfully initialized
      */
      bool _init_done;
    public:
      /**
       * ROS2 publisher designed to be similar to the Publisher class in rclcpp.
       * 
       * Usage instructions:
       * - Instantiate before any node is setup
       * - Call advertise() in on_setup-method of node or after node setup is completed
       * - Message type must have Message trait implemented on it
       * @param <_MessageType> Message type handled by publisher
       * @param node Pointer to node owning the publisher
       * @param topic_name Topic name (slash and namespace of node is appended later)
       * @param default Initial message data
      */
      Publisher(
        Node* node,
        const char* topic_name,
        DataType default_data
      ) noexcept;

      ~Publisher() noexcept;

      /**
       * Initializes the publisher, and then advertises the topic onto the ROS2 network.
       * Node must be successfully initialized for this to succeed.
       * @return true if success
      */
      bool advertise() noexcept;
      
      /**
       * Sets message data.
       * @param data Message data
      */
      void set_data(DataType data) noexcept;
      /**
       * Retrieves last sent message data as reference
       * @return Reference to message data
      */
      DataRef get_last_data(void) noexcept;

      /**
       * Publishes current message onto topic.
       * Publisher must be successfully advertised for this to succeed.
       * @return true if success
      */
      bool publish(void) const noexcept;
      /**
       * Publishes message with given data onto topic.
       * Publisher must be successfully advertised for this to succeed.
       * @param data Message data
       * @return true if success
      */
      bool publish(DataType data) noexcept;
  };
};

#include "publisher_impl.hpp"