cmake_minimum_required(VERSION 2.8.3)
project(icra_firefly)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  geometry_msgs
  move_base
  nav_msgs
  roscpp
  rospy
  serial
  std_msgs
  tf

  message_generation
)

## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## Generate messages in the 'msg' folder
 add_message_files(
   FILES
   ArmorDetection.msg
   GimbalControl.msg
 )

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
# )

## Generate added messages and services with any dependencies listed here
generate_messages(
#   DEPENDENCIES
#   geometry_msgs
#   nav_msgs
#   std_msgs
)

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

## Generate dynamic reconfigure parameters in the 'cfg' folder
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )

###################################
## catkin specific configuration ##
###################################

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES icra_firefly
   CATKIN_DEPENDS 
   cv_bridge 
   geometry_msgs 
   move_base 
   nav_msgs 
   roscpp 
   rospy 
   serial 
   std_msgs 
   tf 
   message_runtime
#  DEPENDS system_lib
)

# add_subdirectory(VisualServo)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  SerialPort
  MoveControl
  AutoPID
  ${catkin_INCLUDE_DIRS}
)

add_executable(visual_servo_node 
    VisualServo/visual_servo_node.cpp
    MoveControl/MoveControl.cpp
    SerialPort/SerialPort.cpp
    AutoPID/AutoPID.cpp
    )

target_link_libraries(visual_servo_node 
    ${catkin_LIBRARIES}
    )

add_executable(move_control_node 
    MoveControl/move_control_node.cpp
    MoveControl/MoveControl.cpp
    SerialPort/SerialPort.cpp
    )

target_link_libraries(move_control_node 
    ${catkin_LIBRARIES}
    )