#include "node.hpp"

#include <Arduino.h>
#include <micro_ros_arduino.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include "error.hpp"

// https://micro.ros.org/docs/tutorials/programming_rcl_rclc/node/
// https://micro.ros.org/docs/tutorials/programming_rcl_rclc/executor/#example-1-hello-world

namespace rclc_cppb
{
  static constexpr size_t ARGUMENTS_COUNT =
    #ifdef ARGC
      ARGC
    #else
      0
    #endif
  ;
  #if !defined(ARGC) || ARGC == 0
    static constexpr const char* const* ARGUMENTS_VALUES = NULL;
  #else
    static constexpr char const* ARGUMENTS_VALUES[ARGUMENTS_COUNT] = {
      #ifdef ARGV
        ARGV
      #endif
    };
  #endif

  unsigned int _num_handles = 0;
  rcl_allocator_t _allocator;
  rclc_support_t _support;
  rclc_executor_t _executor;

  enum class InitStage: uint8_t
  {
    NEW = 0,
    ALLOCATOR_DONE = 1,
    SUPPORT_DONE = 2,
    EXECUTOR_DONE = 3
  };
  InitStage _init_stage = InitStage::NEW;

  Node::Node(const char *node_name, const char *node_namespace) noexcept:
    node_name(node_name),
    node_namespace(node_namespace)
  {

  }
  Node::~Node() noexcept
  {
    this->on_kill();

    this->_is_node_init = false;
    rclc_cppb::_init_stage = InitStage::NEW;

    rclc_cppb::error::handled_call<
      decltype(&rcl_node_fini),
      &rcl_node_fini
    >(
      &this->_node
    );
  }

  bool Node::setup(void) noexcept
  {
    if(!Node::init())
    {
      return false;
    }
    
    if(!this->_is_node_init)
    {
      if(
        !rclc_cppb::error::handled_call<
          decltype(&rclc_node_init_default),
          &rclc_node_init_default
        >(
          &this->_node,
          this->node_name,
          this->node_namespace,
          &rclc_cppb::_support
        )
      )
      {
        return false;
      }
      this->_is_node_init = true;
    }

    return this->on_setup();
  }

  void Node::loop(void) noexcept {}

  bool Node::spin_once(uint64_t timeout_ns) noexcept
  {
    if(rclc_cppb::_init_stage < InitStage::EXECUTOR_DONE)
    {
      return false;
    }
    if(
      !rclc_cppb::error::handled_call<
        decltype(&rclc_executor_spin_some),
        &rclc_executor_spin_some
      >(
        Node::get_executor_mut(),
        timeout_ns
      )
    )
    {
      return false;
    }
    return true;
  }

  const rcl_node_t* Node::get_handle(void) const noexcept
  {
    return &this->_node;
  }
  rcl_node_t* Node::get_handle_mut(void) noexcept
  {
    return &this->_node;
  }
  const rclc_executor_t* Node::get_executor(void) noexcept
  {
    return &rclc_cppb::_executor;
  }
  rclc_executor_t* Node::get_executor_mut(void) noexcept
  {
    return &rclc_cppb::_executor;
  }
  bool Node::has_namespace(void) const noexcept
  {
    return this->node_namespace != NULL && strlen(this->node_namespace) != 0;
  }
  String Node::append_namespace_to_token(const char* token) const noexcept
  {
    if(this->has_namespace())
    {
      return String("/") + String(this->node_namespace) + String("/") + String(token);
    }
    return String("/") + String(token);
  }
  String Node::get_full_node_name(void) const noexcept
  {
    return this->append_namespace_to_token(this->node_name);
  }

  bool Node::on_setup(void) noexcept
  {
    return true;
  }
  void Node::on_kill(void) noexcept {}

  bool Node::init(void) noexcept
  {
    static_assert(InitStage::NEW < InitStage::ALLOCATOR_DONE);
    if(rclc_cppb::_init_stage < InitStage::ALLOCATOR_DONE)
    {
      set_microros_transports();
      rclc_cppb::_allocator = rcl_get_default_allocator();

      rclc_cppb::_init_stage = InitStage::ALLOCATOR_DONE;
    }
    static_assert(InitStage::ALLOCATOR_DONE < InitStage::SUPPORT_DONE);
    if(rclc_cppb::_init_stage < InitStage::SUPPORT_DONE)
    {
      if(
        !rclc_cppb::error::handled_call<
          decltype(&rclc_support_init),
          &rclc_support_init
        >(
          &rclc_cppb::_support,
          rclc_cppb::ARGUMENTS_COUNT,
          rclc_cppb::ARGUMENTS_VALUES,
          &rclc_cppb::_allocator
        )
      )
      {
        rclc_cppb::error::handled_call<
          decltype(&rclc_support_fini),
          &rclc_support_fini
        >(
          &rclc_cppb::_support
        );
        return false;
      }
      rclc_cppb::_init_stage = InitStage::SUPPORT_DONE;
    }
    static_assert(InitStage::SUPPORT_DONE < InitStage::EXECUTOR_DONE);
    if(rclc_cppb::_init_stage < InitStage::EXECUTOR_DONE)
    {
      if(rclc_cppb::_executor.handles != NULL || rclc_cppb::_executor.allocator != NULL || rclc_cppb::_executor.max_handles != 0)
      {
        rclc_cppb::error::handled_call<
          decltype(&rclc_executor_fini),
          &rclc_executor_fini
        >(
          &rclc_cppb::_executor
        );
      }
      rclc_cppb::_executor = rclc_executor_get_zero_initialized_executor();
      
      if(
        !rclc_cppb::error::handled_call<
          decltype(&rclc_executor_init),
          &rclc_executor_init
        >(
          &rclc_cppb::_executor,
          &rclc_cppb::_support.context,
          Node::get_num_handles(),
          &rclc_cppb::_allocator
        )
      )
      {
        return false;
      }
      rclc_cppb::_init_stage = InitStage::EXECUTOR_DONE;
    }

    return true;
  }
  
  unsigned int Node::get_num_handles() noexcept
  {
    return _num_handles;
  }
  void Node::add_handle() noexcept
  {
    _num_handles++;
  }
  void Node::add_handles(unsigned int count) noexcept
  {
    _num_handles += count;
  }
}