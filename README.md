# rclc_cppb
C++ bindings for Micro-ROS implementation of rclc

Officially supported hardware:
- Portenta H7

Currently available features:
- Nodes
- Publishers
- Subscribers
- Service Servers
- Service Clients (not tested)
- Message Trait
  - Already implemented for std_msgs
  - Macro for easy implementation for custom message types
- Service Trait
  - Already implemented for std_srvs
  - Macro for easy implementation for custom service types

Planned features:
- Timers
- Alternative to rosidl code generation for custom messages and services (if possible)
- Other features of rclcpp
