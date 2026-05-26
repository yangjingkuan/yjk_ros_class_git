cmake_minimum_required(VERSION 3.0.2)
project(upros_cv)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  sensor_msgs
  cv_bridge
  message_filters
  geometry_msgs
  tf2_geometry_msgs
  tf2_ros
  image_transport
  upros_message
)

find_package(OpenCV REQUIRED)

catkin_package(
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(yolo_tf_publisher_node src/yolo_tf_publisher.cpp)
target_link_libraries(yolo_tf_publisher_node ${catkin_LIBRARIES})
