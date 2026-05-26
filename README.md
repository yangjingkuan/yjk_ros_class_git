第一周

实验一：主题与消息实验
一、标准消息
cd ~/yjk_ros_class_ws
source devel/setup.bash

roscore

cd ~/yjk_ros_class_ws
source devel/setup.bash
rosrun my_class_pkg ros_publisher_node

cd ~/yjk_ros_class_ws
source devel/setup.bash
rosrun my_class_pkg ros_subscriber_node

二、自定义消息
roscore

cd ~/yjk_ros_class_ws
source devel/setup.bash
rosrun my_class_pkg msg_publisher_node

cd ~/yjk_ros_class_ws
source devel/setup.bash
rosrun my_class_pkg msg_subscriber_node

三、使用launch文件启动节点
cd ~/yjk_ros_class_ws
source devel/setup.bash
roslaunch my_class_pkg bringup_topic.launch


实验二：机器人行走1x1m
roscore

roslaunch upros_bringup bringup_w2a.launch

cd ~/ros_class_ws
source devel/setup.bash
rosrun clas_pkg odom_square



第二周



实验一：
1、创建工作空间
cd ~/
mkdir -p ros_class_ws/src
cd ros_class_ws/src
catkin_init_workspace
cd ~/ros_class_ws
catkin_make

2、配置环境变量
gedit ~/.bashrc
source ~/.bashrc
rospack profile

3、创建功能包
cd ~/ros_class_ws/src
catkin_create_pkg my_class_pkg roscpp rospy std_msgs
cd ~/ros_class_ws/
catkin_make

实验二：
一、主题与消息实验
1、标准消息
cd ~/yjk_ros_class_ws
source devel/setup.bash

roscore

cd ~/yjk_ros_class_ws
source devel/setup.bash
rosrun my_class_pkg ros_publisher_node

cd ~/yjk_ros_class_ws
source devel/setup.bash
rosrun my_class_pkg ros_subscriber_node

2、自定义消息
roscore

cd ~/yjk_ros_class_ws
source devel/setup.bash
rosrun my_class_pkg msg_publisher_node

cd ~/yjk_ros_class_ws
source devel/setup.bash
rosrun my_class_pkg msg_subscriber_node

3、查看自定义消息
rosmsg show my_class_pkg/MyMessage

4、使用launch文件启动节点
cd ~/yjk_ros_class_ws
source devel/setup.bash
roslaunch my_class_pkg bringup_topic.launch

二、服务实验
1、自定义服务
cd ~/yjk_ros_class_ws/
source devel/setup.bash
rossrv show my_class_pkg/MyServiceMsg

2、使用 C++ 实现一个 ROS 服务的传递
roscore

cd ~/yjk_ros_class_ws
source devel/setup.bash
rosrun my_class_pkg ros_server_node

cd ~/yjk_ros_class_ws
source devel/setup.bash
rosrun my_class_pkg ros_client_node

三、使用 C++ 实现一个 ROS 动作的传递
roscore

cd ~/yjk_ros_class_ws
source devel/setup.bash
rosrun my_class_pkg ros_action_server

cd ~/yjk_ros_class_ws
source devel/setup.bash
rosrun my_class_pkg ros_action_client

实验三：传感器实验
0、循环检测碰撞有没有触发
roslaunch upros_bringup bringup_w2a.launch

cd ~/yjk_ros_class_ws/
source devel/setup.bash
rosrun my_class_pkg ros_bump_node

1、启动碰撞传感器节点
roslaunch upros_bringup bringup_w2a.launch

cd ~/yjk_ros_class_ws/
source devel/setup.bash
rosrun my_class_pkg bump_avoid_node

2、启动超声波TOF 传感器节点
roslaunch upros_bringup bringup_w2a.launch

cd ~/yjk_ros_class_ws/
source devel/setup.bash
rosrun my_class_pkg tof2_avoid_node

3、启动 IMU 自旋控制节点
roslaunch upros_bringup bringup_w2a.launch

cd ~/yjk_ros_class_ws/
source devel/setup.bash
rosrun my_class_pkg ros_imu_spin_node



第三周：



实验上：
1、参数服务器基本操作
roscore

cd ~/yjk_ros_class_ws/
source devel/setup.bash
rosrun my_class_pkg ros_param

2、Python 参数服务器操作
roscore

cd ~/yjk_ros_class_ws/
source devel/setup.bash
rosrun my_class_pkg ros_param.py

启动指令
roslaunch my_class_pkg parameter.launch

3、动态参数配置服务端
roscore

cd ~/yjk_ros_class_ws/
source devel/setup.bash
rosrun my_class_pkg dynamic_reconfigure_node

cd ~/yjk_ros_class_ws/
source devel/setup.bash
rosrun rqt_reconfigure rqt_reconfigure

4、动态参数控制小乌龟速度
roscore

rosrun turtlesim turtlesim_node

cd ~/yjk_ros_class_ws/
source devel/setup.bash
rosrun my_class_pkg ros_dynamic_speed_node

cd ~/yjk_ros_class_ws/
source devel/setup.bash
rosrun rqt_reconfigure rqt_reconfigure

5、动态参数控制真实机器人 W2A 速度
source ~/yjk_ros_class_ws/devel/setup.bash

roscore

roslaunch upros_bringup bringup_w2a.launch

rosrun my_class_pkg ros_dynamic_speed_node

rosrun rqt_reconfigure rqt_reconfigure

实验下：
1、使用 C++ 实现一个 LOG 节点
roscore

cd ~/yjk_ros_class_ws/
source devel/setup.bash
rosrun my_class_pkg ros_log

2、使用 Python实现一个 LOG 节点
roscore

cd ~/yjk_ros_class_ws/
source devel/setup.bash
rosrun my_class_pkg ros_log.py

3、使用 rqt 工具调试机器人信息
roslaunch upros_bringup bringup_w2a.launch

rosrun rqt_image_view rqt_image_view

rosrun rqt_tf_tree rqt_tf_tree

4、使用 rviz 工具调试机器人信息
roslaunch upros_bringup bringup_w2a.launch

rviz
修改 Fixed Frame 为 laser_link 并订阅激光雷达topic

rosbag record /scan
ctrl+c按键停止录制

关闭所有节点和roscore之后再执行以下操作：
roscore

rosbag play filename.bag

rviz
手动在Fixed Frame栏输入laser_link，并订阅雷达话题

5、综合练习
roslaunch upros_bringup bringup_w2a.launch

rviz
IMU 的主题为 /imu/data
IMU 的 frame_id: imu_link
点击 Add -> By display type -> rviz imu plugins
晃动机器人或通过键盘控制机器人，可以观察到 IMU 数据随着姿态变化而实时更新



第四周：



实验一：配置Gazebo仿真环境
预先将 Gazebo 模型下载到本地
cd ~/.gazebo/
mkdir -p models
cd ~/.gazebo/models/
git clone https://gitee.com/song_gang/gazebo_models.git

安装依赖
sudo apt-get install ros-noetic-teleop-twist-keyboard
sudo apt-get install ros-noetic-joint-state-controller
sudo apt-get install ros-noetic-effort-controllers
sudo apt-get install ros-noetic-position-controllers
sudo apt-get install ros-noetic-joint-trajectory-controller
sudo apt-get install ros-noetic-controller-manager
sudo apt-get install ros-noetic-gazebo-ros-control
sudo apt-get install ros-noetic-ros-controllers
sudo apt-get install ros-noetic-rqt-joint-trajectory-controller
sudo apt-get install ros-noetic-rqt-controller-manager
sudo apt-get install ros-noetic-gazebo-*
sudo apt-get install ros-noetic-gmapping
sudo apt-get install ros-noetic-navigation
sudo apt-get install ros-noetic-moveit-*

下载教具机器人仿真工作空间：
cd ~/
git clone https://gitee.com/song_gang/upros_sim.git

cd upros_sim
catkin_make 

source ~/upros_sim/devel/setup.bash

gedit ~/.bashrc
将source ~/upros_sim/devel/setup.bash这句话加到文件最后

启动Gazebo仿真环境和教具机器人
roslaunch zx_description w2a.launch

实验二：
在仿真中进行定位导航
1、仿真测试机械臂控制
roslaunch zx_description w2a.launch

rosrun rqt_joint_trajectory_controller rqt_joint_trajectory_controller

2、导航仿真
roslaunch zx_description w2a.launch

roslaunch zx_description gmapping.launch

切换终端到Rviz可观察

rosrun teleop_twist_keyboard teleop_twist_keyboard.py
按照按键输入控制机器人移动

roslaunch zx_description save_map.launch

然后关闭所有终端，重新打开终端输入
roslaunch zx_description w2a.launch

roslaunch zx_description navigation.launch

在Rviz中通过2D Nav Goal 按钮选择目标点即可实现自主导航

实验三：让自己的urdf小车动起来
export LIBGL_ALWAYS_SOFTWARE=1
cd ~/yjk_ros_class_ws
source devel/setup.bash
roslaunch yjk_robot_description gazebo.launch

 cd ~/yjk_ros_class_ws
 source devel/setup.bash
 rosrun teleop_twist_keyboard teleop_twist_keyboard.py _repeat_rate:=30



第五周：



实验一：深度相机驱动实验
roslaunch orbbec_camera dabai_dcw2.launch

rosrun rqt_image_view rqt_image_view

观察到摄像头的图像信息，图像的主题为 /camera/color/image_raw

cd ~/yjk_ros_class_ws
source devel/setup.bash

roslaunch upros_bringup bringup_w2a.launch

rosrun my_class_pkg get_ros_image.py

rosrun rqt_image_view rqt_image_view

图像的主题为 /image_result

实验二：基于颜色识别的自主巡线实验
（颜色标记rosrun upros_cv color_choose.py）

cd ~/yjk_ros_class_ws
source devel/setup.bash

roslaunch upros_bringup bringup_w2a.launch

rosrun my_class_pkg follow_line.py

rostopic pub -1 /enable_move std_msgs/Int16 "data: 1"

实验三：基于手势识别的机器人控制实验
cd ~/yjk_ros_class_ws
source devel/setup.bash

roslaunch upros_bringup bringup_w2a.launch

rosrun my_class_pkg gesture_movement.py

rosrun rqt_image_view rqt_image_view

实验四：视觉跟踪实验
cd ~/yjk_ros_class_ws
source devel/setup.bash

roslaunch upros_bringup bringup_w2a.launch

rosrun my_class_pkg apriltag_follow.py

rosrun rqt_image_view rqt_image_view

实验五：视觉抓取实验
cd ~/yjk_ros_class_ws 
source devel/setup.bash

roslaunch upros_bringup bringup_w2a.launch

roslaunch upros_arm recognize_apriltag.launch

rosrun my_class_pkg tag_grab_node



第六周：



实验一：惯性测量单元传感器实验
1、编写节点订阅IMU传感器信息
roslaunch upros_bringup bringup_w2a.launch

cd ~/yjk_ros_class_ws
source devel/setup.bash
rosrun my_class_pkg ros_imu_node

2、编写节点实现基于IMU的自旋控制
roslaunch upros_bringup bringup_w2a.launch

cd ~/yjk_ros_class_ws
source devel/setup.bash
rosrun my_class_pkg ros_imu_rotate_node

实验二：激光雷达驱动实验
1、激光雷达驱动卸载
cd ~/upros_class_code && catkin_make

2、激光雷达驱动安装
cd ~/upros_class_code/src/upros_hardware
git clone https://gitee.com/song_gang/bluesea.git
cd ~/upros_class_code
catkin_make
source ~/.bashrc
rospack profile

3、激光雷达驱动启动
roslaunch upros_bringup bringup_w2a.launch

4、rviz观察激光雷达数据
rviz
首先将 Global Option 下的 Fixed Frame 设置为激光雷达的 frame_id:laser_link
然后，添加激光雷达数据源 /scan
在 RViz 的左下角，点击 “Add”，选择 “By topic”，然后选择 /scan 下的 LaserScan 类型

5、激光雷达信息ROS获取
roslaunch upros_bringup bringup_w2a.launch

cd ~/yjk_ros_class_ws
source devel/setup.bash
rosrun my_class_pkg ros_scan_node

6、激光雷达角度屏蔽
找到upros_class_code/src/upros_hardware/bluesea/launch
修改以下参数
<rosparam param="mask1" >[0.52,0.98]</rosparam>
<rosparam param="mask2" >[-0.98,-0.52]</rosparam>
<rosparam param="mask3" >[2.09,2.62]</rosparam>
<rosparam param="mask4" >[-2.62,-2.09]</rosparam>  

7、激光雷达避障
roslaunch upros_bringup bringup_w2a.launch

cd ~/yjk_ros_class_ws
source devel/setup.bash
rosrun my_class_pkg ros_avoid_node

实验三：建图导航实验
1、机器人建图实验
roslaunch upros_bringup bringup_w2a.launch

roslaunch upros_navigation gmapping.launch

roslaunch upros_navigation view_nav.launch

rosrun upros_move_linear teleop_twist_keyboard.py

roslaunch upros_navigation save_map.launch

2、机器人定位导航实验
回到建图起点
roslaunch upros_bringup bringup_w2a.launch

roslaunch upros_navigation navigation.launch

roslaunch upros_navigation view_nav.launch

使用 rviz 中的 2D Nav Goal 按钮设置目标点，机器人将根据设定的目标点进行自主导航

3、机器人标记导航点
将机器人关闭一切节点，搬回建图导航起点
roslaunch upros_bringup bringup_w2a.launch

roslaunch upros_navigation navigation.launch

roslaunch upros_navigation view_nav.launch

rosrun upros_move_linear teleop_twist_keyboard.py

rosrun upros_transform tf_echo_node

4、机器人发送导航点
将智行-W2A机器人搬回建图起点
roslaunch upros_bringup bringup_w2a.launch

roslaunch upros_navigation navigation.launch

roslaunch upros_navigation view_nav.launch

rosrun my_class_pkg movebase_client_node



第七周：



实验一：语音交互与大模型实验
1、麦克风音频获取
打开系统桌面右上角的 settings 设置
选择 sound 标签页
首先选择输出源，点击语音模块的音量加键
选择完成后，一直触碰麦克风右侧增大音量的按钮，直到出现上图，调整到最大音量。然后点击右侧的 test
点击左侧右侧的两个按钮，可以听到模块发出 Front Left 和 Front Right 的语音。如此则证明语音模块输出正常
选择麦克风输入为下图源，并发出声音测试响应
发出声音时，输入源下面的线条有明显反应，则麦克风输入正常

2、离线语音识别
roslaunch upros_chat speech_to_word.launch

rostopic echo /speech/result

3、大模型调用
roslaunch upros_chat speech_to_word.launch

cd ~/yjk_ros_class_ws
source devel/setup.bash
rosrun my_class_pkg llm_chat.py

4、离线语音合成
roslaunch upros_chat word_to_speech.launch

rostopic pub -1 /talk std_msgs/String “您好我是智行”

实验二：语音控制小车前往A/B点
source ~/upros_class_code/devel/setup.bash
roslaunch upros_bringup bringup_w2a.launch

source ~/upros_class_code/devel/setup.bash
roslaunch upros_navigation navigation.launch

rosrun rviz rviz -d /home/bcsh/upros_class_code/src/upros_navigation/rviz/show.rviz

source ~/upros_class_code/devel/setup.bash
roslaunch upros_chat speech_to_word.launch

source ~/yjk_ros_class_ws/devel/setup.bash
rosrun my_class_pkg voice_to_point.py

source ~/yjk_ros_class_ws/devel/setup.bash
rosrun my_class_pkg car_go_ab.py










