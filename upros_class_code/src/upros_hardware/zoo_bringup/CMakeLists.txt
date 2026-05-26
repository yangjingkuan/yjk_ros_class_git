cmake_minimum_required(VERSION 2.8.3)
project(zoo_bringup)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  tf
  dynamic_reconfigure
  serial
  upros_message
)

catkin_package()

include_directories(
  include ${catkin_INCLUDE_DIRS}
  include ${catkin_INCLUDE_DIRS}
  include include/zoo_bringup/
)

add_executable(zoo_driver
  src/main.cpp
  src/base_driver_config.cpp
  src/base_driver.cpp
  src/simple_dataframe.cpp
)
add_dependencies(zoo_driver ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(zoo_driver ${catkin_LIBRARIES})

