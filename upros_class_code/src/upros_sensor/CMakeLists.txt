cmake_minimum_required(VERSION 3.0.2)
project(upros_sensor)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  image_transport
  cv_bridge
  pcl_ros
  tf2
  tf2_ros
)

find_package(OpenCV REQUIRED)
find_package(PCL REQUIRED)
find_package(Eigen3 REQUIRED)

catkin_package()

include_directories(include ${catkin_INCLUDE_DIRS}  ${OpenCV_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIR})

add_executable(ros_bump_node src/ros_bump.cpp)
target_link_libraries(ros_bump_node ${catkin_LIBRARIES})

add_executable(bump_avoid_node src/bump_avoid.cpp)
target_link_libraries(bump_avoid_node ${catkin_LIBRARIES})

add_executable(ros_sonic_node src/ros_sonic.cpp)
target_link_libraries(ros_sonic_node ${catkin_LIBRARIES})

add_executable(tof_avoid_node src/tof_avoid.cpp)
target_link_libraries(tof_avoid_node ${catkin_LIBRARIES})

add_executable(distance_lidar_filter src/distance_lidar_filter.cpp)
target_link_libraries(distance_lidar_filter ${catkin_LIBRARIES})

add_executable(angle_lidar_filter src/angle_lidar_filter.cpp)
target_link_libraries(angle_lidar_filter ${catkin_LIBRARIES})

add_executable(blob_lidar_filter src/blob_lidar_filter.cpp)
target_link_libraries(blob_lidar_filter ${catkin_LIBRARIES})

add_executable(ros_lidar src/ros_lidar.cpp)
target_link_libraries(ros_lidar ${catkin_LIBRARIES})

add_executable(move_lidar src/move_lidar.cpp)
target_link_libraries(move_lidar ${catkin_LIBRARIES})

add_executable(upros_image_node src/upros_image.cpp)
target_link_libraries(upros_image_node ${catkin_LIBRARIES} ${OpenCV_LIBS})

add_executable(upros_red_stop_node src/upros_red_stop.cpp)
target_link_libraries(upros_red_stop_node ${catkin_LIBRARIES} ${OpenCV_LIBS})

add_executable(upros_point_filter_node src/upros_point_filter.cpp)
target_link_libraries(upros_point_filter_node ${catkin_LIBRARIES})

add_executable(ros_odom_node src/ros_odom.cpp)
target_link_libraries(ros_odom_node ${catkin_LIBRARIES})

add_executable(odom_move_node src/odom_move.cpp)
target_link_libraries(odom_move_node ${catkin_LIBRARIES})