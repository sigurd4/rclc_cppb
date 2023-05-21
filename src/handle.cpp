#include "handle.hpp"

namespace rclc_cppb
{
  Handle::Handle(Node* node) noexcept:
    _node(node)
  {
    Node::add_handle();
  }

  Handle::Handle(Node* node, unsigned int handle_count) noexcept:
    _node(node)
  {
    Node::add_handles(handle_count);
  }

  const Node* Handle::get_node(void) const noexcept
  {
    return this->_node;
  }
  Node* Handle::get_node_mut(void) const noexcept
  {
    return this->_node;
  }
  
  const rcl_node_t* Handle::get_node_handle(void) const noexcept
  {
    return this->get_node()->get_handle();
  }
  rcl_node_t* Handle::get_node_handle_mut(void) noexcept
  {
    return this->get_node_mut()->get_handle_mut();
  }
  const rclc_executor_t* Handle::get_executor(void) noexcept
  {
    return Node::get_executor();
  }
  rclc_executor_t* Handle::get_executor_mut(void) noexcept
  {
    return Node::get_executor_mut();
  }
}