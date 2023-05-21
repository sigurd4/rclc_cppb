#pragma once

#include "error.hpp"

namespace rclc_cppb::error
{
  template<typename Fn, Fn FUNCTION, typename... Args>
  bool handled_call(Args... args) noexcept
  {
    return handle<Fn, FUNCTION>(FUNCTION(args...));
  }

  #define CASE_OK(CODENAME, BEFORE) \
    case CODENAME: \
    { \
      BEFORE \
      return true; \
    }
  #ifdef ERROR_ONCE
    #define CASE_ERROR_ONCE(CODENAME, BEFORE) \
      case CODENAME: \
      { \
        BEFORE \
        ERROR_ONCE \
        return false; \
      }
  #else
    #define CASE_ERROR_ONCE(CODENAME, BEFORE) \
      case CODENAME: \
      { \
        BEFORE \
        return false; \
      }
  #endif
  #ifdef ERROR_LOOP
    #define CASE_ERROR_LOOP(CODENAME, BEFORE) \
      case CODENAME: \
      { \
        BEFORE \
        ERROR_LOOP \
        return false; \
      }
    #define DEFAULT_ERROR_LOOP(BEFORE) \
      default: \
      { \
        BEFORE \
        ERROR_LOOP \
        return false; \
      }
  #else
    #define CASE_ERROR_LOOP(CODENAME, BEFORE) \
      case CODENAME: \
      { \
        BEFORE \
        return false; \
      }
    #define DEFAULT_ERROR_LOOP(BEFORE) \
      default: \
      { \
        BEFORE \
        return false; \
      }
  #endif
  
  template<typename Fn, Fn FUNCTION>
  bool handle(rcl_ret_t return_code) noexcept
  {
    switch(return_code)
    {
      CASE_OK(RCL_RET_OK, )
      CASE_OK(RCL_RET_ALREADY_INIT, )
      CASE_ERROR_ONCE(RCL_RET_ERROR, )
      CASE_ERROR_ONCE(RCL_RET_TIMEOUT, )
      CASE_ERROR_LOOP(RCL_RET_INVALID_ARGUMENT, )
      CASE_ERROR_LOOP(RCL_RET_PUBLISHER_INVALID, )
      CASE_ERROR_LOOP(RCL_RET_CLIENT_INVALID, )
      CASE_ERROR_LOOP(RCL_RET_SERVICE_INVALID, )
      CASE_ERROR_LOOP(RCL_RET_SUBSCRIPTION_INVALID, )
      CASE_ERROR_LOOP(RCL_RET_NODE_INVALID, )
      CASE_ERROR_LOOP(RCL_RET_BAD_ALLOC, )
      // Add more if needed
      DEFAULT_ERROR_LOOP()
    }
  }

  // The following specializations will not compile. Instead using default above.
  /*#define HANDLE_IMPL(FUNCTION, RETURN_CODE_VARIABLE_NAME) \
    template<> \
    bool handle< \
      decltype(&FUNCTION), \
      &FUNCTION \
    >(rcl_ret_t RETURN_CODE_VARIABLE_NAME) noexcept
  
  HANDLE_IMPL(rclc_publisher_init_default, return_code)
  {
    switch(return_code)
    {
      CASE_OK(RCL_RET_OK)
      CASE_ERROR_ONCE(RCL_RET_ERROR)
      DEFAULT_ERROR_LOOP
    }
  }
  HANDLE_IMPL(rcl_publish, return_code)
  {
    switch(return_code)
    {
      CASE_OK(RCL_RET_OK)
      CASE_ERROR_ONCE(RCL_RET_ERROR)
      CASE_ERROR_LOOP(RCL_RET_INVALID_ARGUMENT)
      CASE_ERROR_LOOP(RCL_RET_PUBLISHER_INVALID)
      DEFAULT_ERROR_LOOP
    }
  }
  HANDLE_IMPL(rcl_publisher_fini, return_code)
  {
    switch(return_code)
    {
      CASE_OK(RCL_RET_OK)
      CASE_ERROR_LOOP(RCL_RET_INVALID_ARGUMENT)
      CASE_ERROR_LOOP(RCL_RET_PUBLISHER_INVALID)
      CASE_ERROR_LOOP(RCL_RET_NODE_INVALID)
      CASE_ERROR_LOOP(RCL_RET_ERROR)
      DEFAULT_ERROR_LOOP
    }
  }
  
  HANDLE_IMPL(rcl_client_fini, return_code)
  {
    switch(return_code)
    {
      CASE_OK(RCL_RET_OK)
      CASE_ERROR_LOOP(RCL_RET_INVALID_ARGUMENT)
      CASE_ERROR_LOOP(RCL_RET_CLIENT_INVALID)
      CASE_ERROR_LOOP(RCL_RET_NODE_INVALID)
      CASE_ERROR_LOOP(RCL_RET_ERROR)
      DEFAULT_ERROR_LOOP
    }
  }
  HANDLE_IMPL(rclc_client_init_default, return_code)
  {
    switch(return_code)
    {
      CASE_OK(RCL_RET_OK)
      CASE_ERROR_ONCE(RCL_RET_ERROR)
      DEFAULT_ERROR_LOOP
    }
  }
  HANDLE_IMPL(rclc_executor_add_client, return_code)
  {
    switch(return_code)
    {
      CASE_OK(RCL_RET_OK)
      CASE_ERROR_ONCE(RCL_RET_ERROR)
      DEFAULT_ERROR_LOOP
    }
  }
  HANDLE_IMPL(rcl_send_request, return_code)
  {
    switch(return_code)
    {
      CASE_OK(RCL_RET_OK)
      CASE_ERROR_ONCE(RCL_RET_ERROR)
      CASE_ERROR_LOOP(RCL_RET_INVALID_ARGUMENT)
      CASE_ERROR_LOOP(RCL_RET_CLIENT_INVALID)
      DEFAULT_ERROR_LOOP
    }
  }
  
  HANDLE_IMPL(rcl_service_fini, return_code)
  {
    switch(return_code)
    {
      CASE_OK(RCL_RET_OK)
      CASE_ERROR_LOOP(RCL_RET_INVALID_ARGUMENT)
      CASE_ERROR_LOOP(RCL_RET_SERVICE_INVALID)
      CASE_ERROR_LOOP(RCL_RET_NODE_INVALID)
      CASE_ERROR_LOOP(RCL_RET_ERROR)
      DEFAULT_ERROR_LOOP
    }
  }
  HANDLE_IMPL(rclc_service_init_default, return_code)
  {
    switch(return_code)
    {
      CASE_OK(RCL_RET_OK)
      CASE_ERROR_ONCE(RCL_RET_ERROR)
      DEFAULT_ERROR_LOOP
    }
  }
  HANDLE_IMPL(rclc_executor_add_service, return_code)
  {
      switch(return_code)
      {
        CASE_OK(RCL_RET_OK)
        CASE_ERROR_ONCE(RCL_RET_ERROR)
        CASE_ERROR_LOOP(RCL_RET_INVALID_ARGUMENT)
        DEFAULT_ERROR_LOOP
      }
  }

  HANDLE_IMPL(rcl_subscription_fini, return_code)
  {
      switch(return_code)
      {
        CASE_OK(RCL_RET_OK)
        CASE_ERROR_ONCE(RCL_RET_ERROR)
        CASE_ERROR_LOOP(RCL_RET_INVALID_ARGUMENT)
        CASE_ERROR_LOOP(RCL_RET_PUBLISHER_INVALID)
        CASE_ERROR_LOOP(RCL_RET_NODE_INVALID)
        DEFAULT_ERROR_LOOP
      }
  }
  HANDLE_IMPL(rclc_subscription_init_default, return_code)
  {
      switch(return_code)
      {
        CASE_OK(RCL_RET_OK)
        CASE_ERROR_ONCE(RCL_RET_ERROR)
        DEFAULT_ERROR_LOOP
      }
  }
  HANDLE_IMPL(rclc_executor_add_subscription, return_code)
  {
      switch(return_code)
      {
        CASE_OK(RCL_RET_OK)
        CASE_ERROR_ONCE(RCL_RET_ERROR)
        CASE_ERROR_LOOP(RCL_RET_INVALID_ARGUMENT)
        DEFAULT_ERROR_LOOP
      }
  }
  HANDLE_IMPL(rcl_node_fini, return_code)
  {
      switch(return_code)
      {
        CASE_OK(RCL_RET_OK)
        CASE_ERROR_LOOP(RCL_RET_NODE_INVALID)
        CASE_ERROR_LOOP(RCL_RET_ERROR)
        DEFAULT_ERROR_LOOP
      }
  }
  HANDLE_IMPL(rclc_node_init_default, return_code)
  {
      switch(return_code)
      {
        CASE_OK(RCL_RET_OK)
        DEFAULT_ERROR_LOOP
      }
  }
  HANDLE_IMPL(rclc_executor_spin_some, return_code)
  {
      switch(return_code)
      {
        CASE_OK(RCL_RET_OK)
        CASE_ERROR_ONCE(RCL_RET_TIMEOUT)
        CASE_ERROR_ONCE(RCL_RET_ERROR)
        CASE_ERROR_LOOP(RCL_RET_INVALID_ARGUMENT)
        DEFAULT_ERROR_LOOP
      }
  }
  HANDLE_IMPL(rclc_support_init, return_code)
  {
      switch(return_code)
      {
        CASE_OK(RCL_RET_OK)
        CASE_OK(RCL_RET_ALREADY_INIT)
        CASE_ERROR_ONCE(RCL_RET_ERROR)
        CASE_ERROR_LOOP(RCL_RET_BAD_ALLOC)
        CASE_ERROR_LOOP(RCL_RET_INVALID_ARGUMENT)
        DEFAULT_ERROR_LOOP
      }
  }
  HANDLE_IMPL(rclc_executor_init, return_code)
  {
      switch(return_code)
      {
        CASE_OK(RCL_RET_OK)
        CASE_ERROR_ONCE(RCL_RET_ERROR)
        CASE_ERROR_LOOP(RCL_RET_BAD_ALLOC)
        CASE_ERROR_LOOP(RCL_RET_INVALID_ARGUMENT)
        DEFAULT_ERROR_LOOP
      }
  }*/
}

#undef HANDLE_IMPL
#undef CASE_OK
#undef CASE_ERROR_ONCE
#undef CASE_ERROR_LOOP
#undef DEFAULT_ERROR_LOOP