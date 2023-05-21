#ifndef RCLC_CPPB__MESSAGE_H_
  #define RCLC_CPPB__MESSAGE_H_

  #include <rcl/rcl.h>
  #include <rcl/error_handling.h>
  #include <rclc/rclc.h>
  #include <rclc/executor.h>

  #define INCLUDE_ALL_STD_MSGS false
  #if INCLUDE_ALL_STD_MSGS
    #include <std_msgs/msg/empty.h>
    #include <std_msgs/msg/bool.h>
    #include <std_msgs/msg/u_int8.h>
    #include <std_msgs/msg/u_int16.h>
    #include <std_msgs/msg/u_int32.h>
    #include <std_msgs/msg/u_int64.h>
    #include <std_msgs/msg/byte.h>
    #include <std_msgs/msg/char.h>
    #include <std_msgs/msg/int8.h>
    #include <std_msgs/msg/int16.h>
    #include <std_msgs/msg/int32.h>
    #include <std_msgs/msg/int64.h>
    #include <std_msgs/msg/float32.h>
    #include <std_msgs/msg/float64.h>
    #include <std_msgs/msg/byte_multi_array.h>
    #include <std_msgs/msg/u_int8_multi_array.h>
    #include <std_msgs/msg/u_int16_multi_array.h>
    #include <std_msgs/msg/u_int32_multi_array.h>
    #include <std_msgs/msg/u_int64_multi_array.h>
    #include <std_msgs/msg/int8_multi_array.h>
    #include <std_msgs/msg/int16_multi_array.h>
    #include <std_msgs/msg/int32_multi_array.h>
    #include <std_msgs/msg/int64_multi_array.h>
    #include <std_msgs/msg/float32_multi_array.h>
    #include <std_msgs/msg/float64_multi_array.h>
    #include <std_msgs/msg/multi_array_dimension.h>
    #include <std_msgs/msg/multi_array_layout.h>
    #include <std_msgs/msg/header.h>
    #include <std_msgs/msg/string.h>
    #include <std_msgs/msg/color_rgba.h>
  #endif

  namespace rclc_cppb
  {
    /**
     * Message trait.
     * 
     * Implementation is mandatory for message types used with publishers, subscribers, etc.
     * 
     * Implement by specializing the struct with your chosen type as generic argument.
     * 
     * @param <_MessageType> The message-type for which the trait is implemented.
    */
    template<typename _MessageType>
    struct Message
    {
      /**
       * Public export of _MessageType
      */
      using MessageType = _MessageType;
      /**
       * Internal data-type of message-type
      */
      using DataType = MessageType;
      /**
       * Internal data-type of message-type as reference
      */
      using DataRef = const MessageType&;

      /**
       * true if trait is implemented
       * 
       * Please override this and set to true in implementation
      */
      constexpr static const bool IS_IMPL = false;

      /**
       * Converts message into internal data
       * @param message Message
       * @return Internal data of message
      */
      static constexpr DataType into_data(MessageType message) noexcept;
      /**
       * Creates message out of internal data
       * @param data Internal data
       * @return New message
      */
      static constexpr MessageType from_data(DataType data) noexcept;
      /**
       * Sets internal data of message
       * @param message Message
       * @param data Data to insert into message
      */
      static void set_data(MessageType& message, DataType data) noexcept;
      /**
       * Retrieves reference to internal data in message
      */
      static constexpr DataRef get_data(const MessageType& message) noexcept;
      /**
       * Retrieves pointer to rclc type-support-struct of this message-type
      */
      static const rosidl_message_type_support_t* get_type_support(void) noexcept;
    };
  }

  #define MSG_IMPL_INTERNAL_DATA(MESSAGE_TYPE, TYPE_SUPPORT, DATA_TYPE) \
  template<> \
  struct rclc_cppb::Message<MESSAGE_TYPE> \
  { \
    using MessageType = MESSAGE_TYPE; \
    using DataType = DATA_TYPE; \
    using DataRef = const DATA_TYPE&; \
     \
    constexpr static const bool IS_IMPL = true; \
     \
    static constexpr DataType into_data(MessageType message) noexcept \
    { \
      return message.data; \
    } \
    static constexpr MessageType from_data(DataType data) noexcept \
    { \
      MessageType message = MessageType(); \
      message.data = data; \
      return message; \
    } \
    static void set_data(MessageType& message, DataType data) noexcept \
    { \
      message.data = data; \
    } \
    static constexpr DataRef get_data(const MessageType& message) noexcept \
    { \
      return message.data; \
    } \
    static const rosidl_message_type_support_t* get_type_support(void) noexcept \
    { \
      return TYPE_SUPPORT; \
    } \
  };
  #define MSG_IMPL_SELF(MESSAGE_TYPE, TYPE_SUPPORT) \
  template<> \
  struct rclc_cppb::Message<MESSAGE_TYPE> \
  { \
    using MessageType = MESSAGE_TYPE; \
    using DataType = MessageType; \
    using DataRef = const DataType&; \
     \
    constexpr static const bool IS_IMPL = true; \
     \
    static constexpr DataType into_data(MessageType message) noexcept \
    { \
      return message; \
    } \
    static constexpr MessageType from_data(DataType data) noexcept \
    { \
      return data; \
    } \
    static void set_data(MessageType& message, DataType data) noexcept \
    { \
      message = data; \
    } \
    static constexpr DataRef get_data(const MessageType& message) noexcept \
    { \
      return message; \
    } \
    static const rosidl_message_type_support_t* get_type_support(void) noexcept \
    { \
      return TYPE_SUPPORT; \
    } \
  };
  #define _GET_MSG_IMPL_MACRO(_1, _2, _3, NAME, ...) NAME
  #define MSG_IMPL(...) _GET_MSG_IMPL_MACRO(__VA_ARGS__, MSG_IMPL_INTERNAL_DATA, MSG_IMPL_SELF)(__VA_ARGS__)
#endif

#include <std_msgs/msg/string.h>
#if defined(STD_MSGS__MSG__STRING_H_) && !defined(RCLC_CPPB__MESSAGE_H__STRING_)
  //https://micro.ros.org/docs/tutorials/advanced/handling_type_memory/
  #define RCLC_CPPB__MESSAGE_H__STRING_
  #include <std_msgs/msg/string.h>
  #include <micro_ros_arduino.h>
  template<>
  struct rclc_cppb::Message<std_msgs__msg__String>
  {
    using MessageType = std_msgs__msg__String;
    using DataType = const char*;
    using DataRef = const char*;
    
    constexpr static const bool IS_IMPL = true;
    
    static constexpr DataType into_data(MessageType message) noexcept
    {
      return message.data.data;
    }
    static constexpr MessageType from_data(DataType data) noexcept
    {
      MessageType message = MessageType();

      message.data.capacity = strlen(data) + 1;
      message.data.data = (char*)malloc(sizeof(char)*message.data.capacity);
      
      strcpy(message.data.data, data);
      message.data.size = strlen(message.data.data);

      return message;
    }
    static void set_data(MessageType& message, DataType data) noexcept
    {
      size_t new_capacity = strlen(data) + 1;
      if(new_capacity > message.data.capacity)
      {
        free(message.data.data);
        message.data.capacity = new_capacity;
        message.data.data = (char*)malloc(sizeof(char)*message.data.capacity);
      }
      strcpy(message.data.data, data);
      message.data.size = strlen(message.data.data);
    }
    static constexpr DataRef get_data(const MessageType& message) noexcept
    {
      return message.data.data;
    }
    static const rosidl_message_type_support_t* get_type_support(void) noexcept
    {
      return ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String);
    }
  };
#endif

#if defined(STD_MSGS__MSG__EMPTY_H_) && !defined(RCLC_CPPB__MESSAGE_H__EMPTY_)
  #define RCLC_CPPB__MESSAGE_H__EMPTY_
  #include <std_msgs/msg/empty.h>
  MSG_IMPL(std_msgs__msg__Empty, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Empty))
#endif

#if defined(STD_MSGS__MSG__BOOL_H_) && !defined(RCLC_CPPB__MESSAGE_H__BOOL_)
  #define RCLC_CPPB__MESSAGE_H__BOOL_
  #include <std_msgs/msg/bool.h>
  MSG_IMPL(std_msgs__msg__Bool, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool), bool)
#endif

#if defined(STD_MSGS__MSG__U_INT8_H_) && !defined(RCLC_CPPB__MESSAGE_H__U_INT8_)
  #define RCLC_CPPB__MESSAGE_H__U_INT8_
  #include <std_msgs/msg/u_int8.h>
  MSG_IMPL(std_msgs__msg__UInt8, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, UInt8), uint8_t)
#endif
#if defined(STD_MSGS__MSG__U_INT16_H_) && !defined(RCLC_CPPB__MESSAGE_H__U_INT16_)
  #define RCLC_CPPB__MESSAGE_H__U_INT16_
  #include <std_msgs/msg/u_int16.h>
  MSG_IMPL(std_msgs__msg__UInt16, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, UInt16), uint16_t)
#endif
#if defined(STD_MSGS__MSG__U_INT32_H_) && !defined(RCLC_CPPB__MESSAGE_H__U_INT32_)
  #define RCLC_CPPB__MESSAGE_H__U_INT32_
  #include <std_msgs/msg/u_int32.h>
  MSG_IMPL(std_msgs__msg__UInt32, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, UInt32), uint32_t)
#endif
#if defined(STD_MSGS__MSG__U_INT64_H_) && !defined(RCLC_CPPB__MESSAGE_H__U_INT64_)
  #define RCLC_CPPB__MESSAGE_H__U_INT64_
  #include <std_msgs/msg/u_int64.h>
  MSG_IMPL(std_msgs__msg__UInt64, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, UInt64), uint64_t)
#endif
#if defined(STD_MSGS__MSG__BYTE_H_) && !defined(RCLC_CPPB__MESSAGE_H__BYTE_)
  #define RCLC_CPPB__MESSAGE_H__BYTE_
  #include <std_msgs/msg/byte.h>
  MSG_IMPL(std_msgs__msg__Byte, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Byte), uint8_t)
#endif
#if defined(STD_MSGS__MSG__CHAR_H_) && !defined(RCLC_CPPB__MESSAGE_H__CHAR_)
  #define RCLC_CPPB__MESSAGE_H__CHAR_
  #include <std_msgs/msg/char.h>
  MSG_IMPL(std_msgs__msg__Char, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Char), uint8_t)
#endif

#if defined(STD_MSGS__MSG__INT8_H_) && !defined(RCLC_CPPB__MESSAGE_H__INT8_)
  #define RCLC_CPPB__MESSAGE_H__INT8_
  #include <std_msgs/msg/int8.h>
  MSG_IMPL(std_msgs__msg__Int8, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int8), int8_t)
#endif
#if defined(STD_MSGS__MSG__INT16_H_) && !defined(RCLC_CPPB__MESSAGE_H__INT16_)
  #define RCLC_CPPB__MESSAGE_H__INT16_
  #include <std_msgs/msg/int16.h>
  MSG_IMPL(std_msgs__msg__Int16, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int16), int16_t)
#endif
#if defined(STD_MSGS__MSG__INT32_H_) && !defined(RCLC_CPPB__MESSAGE_H__INT32_)
  #define RCLC_CPPB__MESSAGE_H__INT32_
  #include <std_msgs/msg/int32.h>
  MSG_IMPL(std_msgs__msg__Int32, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32), int32_t)
#endif
#if defined(STD_MSGS__MSG__INT64_H_) && !defined(RCLC_CPPB__MESSAGE_H__INT64_)
  #define RCLC_CPPB__MESSAGE_H__INT64_
  #include <std_msgs/msg/int64.h>
  MSG_IMPL(std_msgs__msg__Int64, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int64), int64_t)
#endif

#if defined(STD_MSGS__MSG__FLOAT32_H_) && !defined(RCLC_CPPB__MESSAGE_H__FLOAT32_)
  #define RCLC_CPPB__MESSAGE_H__FLOAT32_
  #include <std_msgs/msg/float32.h>
  MSG_IMPL(std_msgs__msg__Float32, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32), float)
#endif
#if defined(STD_MSGS__MSG__FLOAT64_H_) && !defined(RCLC_CPPB__MESSAGE_H__FLOAT64_)
  #define RCLC_CPPB__MESSAGE_H__FLOAT64_
  #include <std_msgs/msg/float64.h>
  MSG_IMPL(std_msgs__msg__Float64, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float64), double)
#endif

#if defined(STD_MSGS__MSG__BYTE_MULTI_ARRAY_H_) && !defined(RCLC_CPPB__MESSAGE_H__BYTE_MULTI_ARRAY_)
  #define RCLC_CPPB__MESSAGE_H__BYTE_MULTI_ARRAY_
  #include <std_msgs/msg/byte_multi_array.h>
  MSG_IMPL(std_msgs__msg__ByteMultiArray, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, ByteMultiArray))
#endif
#if defined(STD_MSGS__MSG__U_INT8_MULTI_ARRAY_H_) && !defined(RCLC_CPPB__MESSAGE_H__U_INT8_MULTI_ARRAY_)
  #define RCLC_CPPB__MESSAGE_H__U_INT8_MULTI_ARRAY_
  #include <std_msgs/msg/u_int8_multi_array.h>
  MSG_IMPL(std_msgs__msg__UInt8MultiArray, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, UInt8MultiArray))
#endif
#if defined(STD_MSGS__MSG__U_INT16_MULTI_ARRAY_H_) && !defined(RCLC_CPPB__MESSAGE_H__U_INT16_MULTI_ARRAY_)
  #define RCLC_CPPB__MESSAGE_H__U_INT16_MULTI_ARRAY_
  #include <std_msgs/msg/u_int16_multi_array.h>
  MSG_IMPL(std_msgs__msg__UInt16MultiArray, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, UInt16MultiArray))
#endif
#if defined(STD_MSGS__MSG__U_INT32_MULTI_ARRAY_H_) && !defined(RCLC_CPPB__MESSAGE_H__U_INT32_MULTI_ARRAY_)
  #define RCLC_CPPB__MESSAGE_H__U_INT32_MULTI_ARRAY_
  #include <std_msgs/msg/u_int32_multi_array.h>
  MSG_IMPL(std_msgs__msg__UInt32MultiArray, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, UInt32MultiArray))
#endif
#if defined(STD_MSGS__MSG__U_INT64_MULTI_ARRAY_H_) && !defined(RCLC_CPPB__MESSAGE_H__U_INT64_MULTI_ARRAY_)
  #define RCLC_CPPB__MESSAGE_H__U_INT64_MULTI_ARRAY_
  #include <std_msgs/msg/u_int64_multi_array.h>
  MSG_IMPL(std_msgs__msg__UInt64MultiArray, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, UInt64MultiArray))
#endif

#if defined(STD_MSGS__MSG__INT8_MULTI_ARRAY_H_) && !defined(RCLC_CPPB__MESSAGE_H__INT8_MULTI_ARRAY_)
  #define RCLC_CPPB__MESSAGE_H__INT8_MULTI_ARRAY_
  #include <std_msgs/msg/int8_multi_array.h>
  MSG_IMPL(std_msgs__msg__Int8MultiArray, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int8MultiArray))
#endif
#if defined(STD_MSGS__MSG__INT16_MULTI_ARRAY_H_) && !defined(RCLC_CPPB__MESSAGE_H__INT16_MULTI_ARRAY_)
  #define RCLC_CPPB__MESSAGE_H__INT16_MULTI_ARRAY_
  #include <std_msgs/msg/int16_multi_array.h>
  MSG_IMPL(std_msgs__msg__Int16MultiArray, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int16MultiArray))
#endif
#if defined(STD_MSGS__MSG__INT32_MULTI_ARRAY_H_) && !defined(RCLC_CPPB__MESSAGE_H__INT32_MULTI_ARRAY_)
  #define RCLC_CPPB__MESSAGE_H__INT32_MULTI_ARRAY_
  #include <std_msgs/msg/int32_multi_array.h>
  MSG_IMPL(std_msgs__msg__Int32MultiArray, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32MultiArray))
#endif
#if defined(STD_MSGS__MSG__INT64_MULTI_ARRAY_H_) && !defined(RCLC_CPPB__MESSAGE_H__INT64_MULTI_ARRAY_)
  #define RCLC_CPPB__MESSAGE_H__INT64_MULTI_ARRAY_
  #include <std_msgs/msg/int64_multi_array.h>
  MSG_IMPL(std_msgs__msg__Int64MultiArray, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int64MultiArray))
#endif

#if defined(STD_MSGS__MSG__FLOAT32_MULTI_ARRAY_H_) && !defined(RCLC_CPPB__MESSAGE_H__FLOAT32_MULTI_ARRAY_)
  #define RCLC_CPPB__MESSAGE_H__FLOAT32_MULTI_ARRAY_
  #include <std_msgs/msg/float32_multi_array.h>
  MSG_IMPL(std_msgs__msg__Float32MultiArray, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32MultiArray))
#endif
#if defined(STD_MSGS__MSG__FLOAT64_MULTI_ARRAY_H_) && !defined(RCLC_CPPB__MESSAGE_H__FLOAT64_MULTI_ARRAY_)
  #define RCLC_CPPB__MESSAGE_H__FLOAT64_MULTI_ARRAY_
  #include <std_msgs/msg/float64_multi_array.h>
  MSG_IMPL(std_msgs__msg__Float64MultiArray, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float64MultiArray))
#endif

#if defined(STD_MSGS__MSG__MULTI_ARRAY_DIMENSION_H_) && !defined(RCLC_CPPB__MESSAGE_H__MULTI_ARRAY_DIMENSION_)
  #define RCLC_CPPB__MESSAGE_H__MULTI_ARRAY_DIMENSION_
  #include <std_msgs/msg/multi_array_dimension.h>
  MSG_IMPL(std_msgs__msg__MultiArrayDimension, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, MultiArrayDimension))
#endif
#if defined(STD_MSGS__MSG__MULTI_ARRAY_LAYOUT_H_) && !defined(RCLC_CPPB__MESSAGE_H__MULTI_ARRAY_LAYOUT_)
  #define RCLC_CPPB__MESSAGE_H__MULTI_ARRAY_LAYOUT_
  #include <std_msgs/msg/multi_array_layout.h>
  MSG_IMPL(std_msgs__msg__MultiArrayLayout, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, MultiArrayLayout))
#endif

#if defined(STD_MSGS__MSG__HEADER_H_) && !defined(RCLC_CPPB__MESSAGE_H__HEADER_)
  #define RCLC_CPPB__MESSAGE_H__HEADER_
  #include <std_msgs/msg/header.h>
  MSG_IMPL(std_msgs__msg__Header, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Header))
#endif

#if defined(STD_MSGS__MSG__COLOR_RGBA_H_) && !defined(RCLC_CPPB__MESSAGE_H__COLOR_RGBA_)
  #define RCLC_CPPB__MESSAGE_H__COLOR_RGBA_
  #include <std_msgs/msg/color_rgba.h>
  MSG_IMPL(std_msgs__msg__ColorRGBA, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, ColorRGBA))
#endif