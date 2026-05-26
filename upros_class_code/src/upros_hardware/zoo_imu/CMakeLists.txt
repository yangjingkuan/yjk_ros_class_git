cmake_minimum_required(VERSION 3.0.2)
project(zoo_imu)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  geometry_msgs
  tf
  tf2
  serial
)

catkin_package()

include_directories(
  include ${catkin_INCLUDE_DIRS}
  include include/zoo_imu/
)

add_executable(imu_calibrate src/imu_calibrate.cpp src/imu_calibrate_node.cpp)
add_dependencies(imu_calibrate ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(imu_calibrate ${catkin_LIBRARIES})

add_executable(imu_driver_node src/imu_driver_node.cpp)
add_dependencies(imu_driver_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(imu_driver_node ${catkin_LIBRARIES})


