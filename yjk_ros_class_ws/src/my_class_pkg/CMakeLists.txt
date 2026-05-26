cmake_minimum_required(VERSION 3.0.2)
project(my_class_pkg)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  tf
  std_msgs
  sensor_msgs
  message_generation
  dynamic_reconfigure
  actionlib          
  actionlib_msgs  
  tf2_ros  
  upros_message
)
generate_dynamic_reconfigure_options(
  cfg/Tutorials.cfg
)
## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)

## Uncomment this if the package has a setup.py.
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################
add_message_files(
  DIRECTORY msg
  FILES MyMessage.msg
)

# 自定义服务-添加编译规则
add_service_files(
  DIRECTORY srv
  FILES MyServiceMsg.srv
)

# 自定义动作-添加编译规则（新增，放在服务之后）
add_action_files(
  DIRECTORY action
  FILES MyAction.action
)

# 生成消息/服务/动作（关键：添加actionlib_msgs依赖）
generate_messages(
  DEPENDENCIES
  std_msgs
  actionlib_msgs  # 新增：动作消息依赖
)

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )

###################################
## catkin specific configuration ##
###################################
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES my_class_pkg
  CATKIN_DEPENDS actionlib actionlib_msgs message_runtime roscpp rospy std_msgs roscpp std_msgs tf2_ros upros_message  # 新增actionlib/actionlib_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

## 所有节点编译规则统一放在include_directories之后（修复顺序错误）
# 原有发布订阅节点
add_executable(ros_publisher_node src/ros_publisher.cpp)
target_link_libraries(ros_publisher_node ${catkin_LIBRARIES})

add_executable(ros_subscriber_node src/ros_subscriber.cpp)
target_link_libraries(ros_subscriber_node ${catkin_LIBRARIES})

## 自定义消息订阅/发布节点
add_executable(msg_subscriber_node src/msg_subscriber.cpp)
target_link_libraries(msg_subscriber_node ${catkin_LIBRARIES})
add_dependencies(msg_subscriber_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

add_executable(msg_publisher_node src/msg_publisher.cpp)
target_link_libraries(msg_publisher_node ${catkin_LIBRARIES})
add_dependencies(msg_publisher_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

# 碰撞/TOF/IMU相关节点
add_executable(ros_bump_node src/ros_bump.cpp)
target_link_libraries(ros_bump_node ${catkin_LIBRARIES})

add_executable(bump_avoid_node src/bump_avoid.cpp)
target_link_libraries(bump_avoid_node ${catkin_LIBRARIES})

add_executable(tof_sub_node src/tof_sub.cpp)
target_link_libraries(tof_sub_node ${catkin_LIBRARIES})

add_executable(tof2_avoid_node src/tof2_avoid.cpp)
target_link_libraries(tof2_avoid_node ${catkin_LIBRARIES})

add_executable(ros_imu_node src/ros_imu.cpp)
target_link_libraries(ros_imu_node ${catkin_LIBRARIES})

add_executable(ros_imu_spin_node src/ros_imu_spin.cpp)
target_link_libraries(ros_imu_spin_node ${catkin_LIBRARIES})

add_executable(ros_imu_rotate_node src/imu_rotate.cpp)
target_link_libraries(ros_imu_rotate_node ${catkin_LIBRARIES})

# C++服务端/客户端
add_executable(ros_server_node src/ros_server.cpp)
target_link_libraries(ros_server_node ${catkin_LIBRARIES})
add_dependencies(ros_server_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

add_executable(ros_client_node src/ros_client.cpp)
target_link_libraries(ros_client_node ${catkin_LIBRARIES})
add_dependencies(ros_client_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

# C++动作服务端/客户端（新增，放在最后，确保依赖正确）
add_executable(ros_action_server_node src/ros_action_server.cpp)
target_link_libraries(ros_action_server_node ${catkin_LIBRARIES})
add_dependencies(ros_action_server_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

add_executable(ros_action_client_node src/ros_action_client.cpp)
target_link_libraries(ros_action_client_node ${catkin_LIBRARIES})
add_dependencies(ros_action_client_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

#############
## Install ##
#############
# catkin_install_python(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

# install(TARGETS ${PROJECT_NAME}_node
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

# install(TARGETS ${PROJECT_NAME}
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
# )

# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############
# catkin_add_gtest(${PROJECT_NAME}-test test/test_my_class_pkg.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

# catkin_add_nosetests(test)
add_executable(ros_action_server src/ros_action_server.cpp)
target_link_libraries(ros_action_server ${catkin_LIBRARIES})

add_executable(ros_action_client src/ros_action_client.cpp)
target_link_libraries(ros_action_client ${catkin_LIBRARIES})


add_executable(ros_param src/ros_param.cpp)
target_link_libraries(ros_param ${catkin_LIBRARIES})

add_executable(dynamic_reconfigure_node src/dynamic_reconfigure.cpp)
add_dependencies(dynamic_reconfigure_node ${PROJECT_NAME}_gencfg)
target_link_libraries(dynamic_reconfigure_node ${catkin_LIBRARIES})

# 编译 ros_dynamic_speed_node 节点
add_executable(ros_dynamic_speed_node src/ros_dynamic_speed.cpp)
# 链接依赖库
target_link_libraries(ros_dynamic_speed_node ${catkin_LIBRARIES})
# 确保依赖顺序正确（可选，但避免编译错）
add_dependencies(ros_dynamic_speed_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

# 编译tag_grab.cpp，生成可执行文件tag_grab_node
add_executable(tag_grab_node src/tag_grab.cpp)
# 将可执行文件与catkin库链接
target_link_libraries(tag_grab_node ${catkin_LIBRARIES})

add_executable(ros_log src/ros_log.cpp)
target_link_libraries(ros_log ${catkin_LIBRARIES})

add_executable(ros_scan_node src/ros_scan.cpp)
target_link_libraries(ros_scan_node ${catkin_LIBRARIES})

add_executable(ros_avoid_node src/ros_avoid.cpp)
target_link_libraries(ros_avoid_node ${catkin_LIBRARIES})

add_executable(movebase_client_node src/movebase_client.cpp)
target_link_libraries(movebase_client_node ${catkin_LIBRARIES})
