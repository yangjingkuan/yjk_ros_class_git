cmake_minimum_required(VERSION 3.0.2)
project(upros_chat)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  upros_message
  tf
)

catkin_package(
  CATKIN_DEPENDS 
)

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(voice_grab_node src/voice_grab.cpp)
target_link_libraries(voice_grab_node ${catkin_LIBRARIES})