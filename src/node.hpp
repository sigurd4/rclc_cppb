#pragma once

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <Arduino.h>

/**
 * C++ OOP bindings for Arduino microROS rclc.
 * This library does not contain all features of rcl.
 * 
 * It does however include:
 * - Nodes
 * - Publishers
 * - Subscribers
 * - Service servers
 * - Service clients
 * 
 * As well as traits for message and service types.
 * To introduce your own message or service type,
 * please manually implement the traits for your custom type.
 * All messages and services in std_msgs and std_srvs are already implemented.
 * To use them, please include the needed messages and service types before including this library.
 * 
 * Macro usage (optional):
 * - Implement the ERROR_ONCE macro to define behaviour upon runtime errors
 * - Implement the ERROR_LOOP macro to define behaviour upon fatal runtime errors
 * 
 * If an ordinary runtime error occurs, you can retry the offending action again safely.
 * If a fatal runtime error occurs, there is something wrong with your setup and you should
 * suspend your program in an infinite loop in the ERROR_LOOP macro.
 * If macros are undefined, the methods will simply return false, and the program will continue
 * whenever a runtime error occurs.
 * 
 * This library is NOT threadsafe.
*/
namespace rclc_cppb
{
  /**
   * ROS2 node designed to be used similarily to the Node class in rclcpp.
   * It is recommended to extend this class with your own custom node, but not mandatory.
   * The class contains some virtual methods which can be extended optionally.
   * 
   * Usage instructions:
   * - The object can be instantiated immediately with no preparation.
   * - The setup method must be called on startup, until it returns true.
   * - When setup returns true, the node is running and connected to the host.
   * - Then in the main loop of the program, call the loop method once.
   * - Call the spin_once method once or more every loop cycle.
  */
  class Node
  {
    private:
      /**
       * Default timeout for spinning in nanoseconds
      */
      static constexpr uint64_t DEFAULT_SPIN_TIMEOUT_NS = RCL_MS_TO_NS(100);

    public:
      /**
       * Node name
      */
      const char* const node_name;
      /**
       * Node namespace
      */
      const char* const node_namespace;
    private:
      /**
       * rclc node struct
      */
      rcl_node_t _node;
      /**
       * true if node initialization is done
      */
      bool _is_node_init = false;

    public:
      /**
       * ROS2 node designed to be used similarily to the Node class in rclcpp.
       * It is recommended to extend this class with your own custom node, but not mandatory.
       * The class contains some virtual methods which can be extended optionally.
       * 
       * Usage instructions:
       * - All nodes in program must be instantiated before any are setup.
       * - When setup returns true, the node is running and connected to the host.
       * - Then in the main loop of the program, call the loop method once.
       * - Call the spin_once method once or more every loop cycle.
       * 
       * @param node_name Node name
       * @param node_namespace Node namespace
      */
      Node(const char *node_name, const char *node_namespace = "") noexcept;
      /**
       * First calls @see{on_kill}, then safely destroys rclc entities.
       * To add your own destructor behaviour, override @see{on_kill}.
      */
      ~Node() noexcept;

      /**
       * Attempts to up node via rclc.
       * Returns false upon any errors.
       * If error is non-fatal you can safely try again.
       * Calls @see{on_setup} when done, which may also return false if it encounters any errors.
       * To add your own setup behaviour, override @see{on_setup}.
       * @return true if success
      */
      bool setup(void) noexcept;
      /**
       * Virtual loop method to be called every loop-cycle of your program.
       * Does nothing unless overrided.
       * Add your own loop behaviour by extending this class.
      */
      virtual void loop(void) noexcept;
      
      /**
       * Returns true if a namespace is set
      */
      bool has_namespace(void) const noexcept;
      /**
       * Appends namespace to a string token in the following formats:
       * /{ @code{node_name} }/{ @code{token} }
       * or
       * /{ @code{node_namespace} }/{ @code{node_name} }/{ @code{token} }
       * @param token Token cstring
       * @return Formatted string
      */
      String append_namespace_to_token(const char* token) const noexcept;
      /**
       * Retrieves full node name with namespace appended if given.
       * See @see{append_namespace_to_token} for format.
       * A forward slash is added at the beginning.
       * @return Formatted full node name
      */
      String get_full_node_name(void) const noexcept;
      
      /**
       * Spins the node once, similar to spinOnce in rclcpp.
       * @param timeout_ns Timeout in nanoseconds
       * @return true if successful
      */
      static bool spin_once(uint64_t timeout_ns = Node::DEFAULT_SPIN_TIMEOUT_NS) noexcept;
    protected:
      /**
       * Called after the node has been successfully set up, see @see{setup}.
       * This method does nothing then returns true unless overrided.
       * This is where you want to set up your topics and services.
       * @return true if success
      */
      virtual bool on_setup(void) noexcept;
      /**
       * Called at the beginning if destruction, see @see{~Node}.
       * This method does nothing unless overrided.
      */
      virtual void on_kill(void) noexcept;
    private:
      /**
       * Initializes rclc before Node initialization. Called at beginning of @see{setup}.
       * @return true if success
      */
      static bool init(void) noexcept;
      /**
       * Returns the number of handles needed for executor to manage.
      */
      static unsigned int get_num_handles() noexcept;
      /**
       * Adds an executor handle.
      */
      static void add_handle() noexcept;
      /**
       * Adds a given amount of executor handles.
       * @param count Amount of handles to add
      */
      static void add_handles(unsigned int count) noexcept;
      
      /**
       * Retrieves pointer to rcl node entity
      */
      const rcl_node_t* get_handle(void) const noexcept;
      /**
       * Retrieves mutable pointer to rcl node entity
      */
      rcl_node_t* get_handle_mut(void) noexcept;
      /**
       * Retrieves pointer to rcl executor entity
      */
      static const rclc_executor_t* get_executor(void) noexcept;
      /**
       * Retrieves mutable pointer to rcl executor entity
      */
      static rclc_executor_t* get_executor_mut(void) noexcept;

      friend class Handle;
  };
}