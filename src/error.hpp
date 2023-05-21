#pragma once

#include <micro_ros_arduino.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

namespace rclc_cppb::error
{
  /**
   * Error handling for rclc functions.
   * 
   * May be overloaded for each possible function.
   * Otherwise uses default implementation.
   * 
   * Failure action should be defined with macros ERROR_ONCE and ERROR_LOOP outside of library.
   * If macros are undefined, just returns false upon failure.
   * 
   * @param <Fn> Type of function-pointer, put @code{decltype(&function_name)}
   * @param <FUNCTION> Pointer to function, put @code{&function_name}
   * @param return_code rclc return code received from function-call
   * @return true if success
  */
  template<typename Fn, Fn FUNCTION>
  bool handle(rcl_ret_t return_code) noexcept;
  
  /**
   * Calls given function with given arguments, then handles the resulting return-code.
   * 
   * Failure action should be defined with macros ERROR_ONCE and ERROR_LOOP outside of library.
   * If macros are undefined, just returns false upon failure.
   * 
   * @param <Fn> Type of function-pointer, put @code{decltype(&function_name)}
   * @param <FUNCTION> Pointer to function, put @code{&function_name}
   * @param args Arguments passed to function
   * @return true if success
  */
  template<typename Fn, Fn FUNCTION, typename... Args>
  bool handled_call(Args... args) noexcept;
}

#include "error_impl.hpp"