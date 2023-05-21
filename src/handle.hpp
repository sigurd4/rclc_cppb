#pragma once

#include "node.hpp"

namespace rclc_cppb
{
  /**
   * A ROS2 entity which requires handling by the executor
  */
  class Handle
  {
    private:
      /**
       * State of initialization
      */
      enum class InitStage: uint8_t
      {
        NEW = 0,
        INIT_DONE = 1,
        EXECUTOR_DONE = 2
      };
      
      /**
       * A mutable pointer to the node that owns this object
      */
      Node* const _node;

    protected:
      /**
       * A ROS2 entity which requires handling by the executor
       * One handle will be added to the handle-counter
       * 
       * Must be instantiated before node is initialized
       * 
       * @param node Mutable pointer to node that owns this object
      */
      Handle(Node* node) noexcept;
      /**
       * A ROS2 entity which requires handling by the executor
       * 
       * Must be instantiated before node is initialized
       * 
       * @param node Mutable pointer to node that owns this object
       * @param handle_count Amount of handles to add to handle-counter
      */
      Handle(Node* node, unsigned int handle_count) noexcept;

      /**
       * Retrieves a pointer to the node that owns this object
       * @return Pointer to the node that owns this object
      */
      const Node* get_node(void) const noexcept;
      /**
       * Retrieves a mutable pointer to the node that owns this object
       * @return Mutable pointer to the node that owns this object
      */
      Node* get_node_mut(void) const noexcept;
      
      /**
       * Retrieves a pointer to the rclc node entity of the node that owns this object
       * @return Pointer to rclc node entity
      */
      const rcl_node_t* get_node_handle(void) const noexcept;
      /**
       * Retrieves a mutable pointer to the rclc node entity of the node that owns this object
       * @return Mutable pointer to rclc node entity
      */
      rcl_node_t* get_node_handle_mut(void) noexcept;
      /**
       * Retrieves a pointer to the rclc executor struct
       * @return Pointer to the rclc executor struct
      */
      static const rclc_executor_t* get_executor(void) noexcept;
      /**
       * Retrieves a mutable pointer to the rclc executor struct
       * @return Mutable pointer to the rclc executor struct
      */
      static rclc_executor_t* get_executor_mut(void) noexcept;
  };
}