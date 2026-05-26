#include <ros/ros.h>
#include <math.h>
#include <boost/assign/list_of.hpp>
#include <boost/shared_ptr.hpp>
#include <geometry_msgs/Twist.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int32.h>
#include <upros_message/SingleServo.h>
#include <upros_message/MultipleServo.h>
#include <upros_message/Adc.h>
#include <sensor_msgs/Range.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/JointState.h>
#include <std_msgs/Int16MultiArray.h>
#include <std_msgs/Float32MultiArray.h>

#include "base_driver_config.h"
#include "simple_dataframe.h"
#include "data_holder.h"

class BaseDriver
{
private:
  BaseDriver();

public:
  static BaseDriver *Instance()
  {
    if (instance == NULL)
      instance = new BaseDriver();

    return instance;
  }
  ~BaseDriver();
  void work_loop();

private:
  void cmd_vel_callback(const geometry_msgs::Twist &vel_cmd);
  void cmd_single_servo_callback(const upros_message::SingleServo &servoData);
  void cmd_multiple_servo_callback(const upros_message::MultipleServo &servoData);
  void imu_data_callback(const sensor_msgs::Imu &imu_data);
  void init_chassis();
  void init_cmd_odom();
  void init_sensor();
  void update_speed();
  void update_sensor();
  void update_mpu6500();
  void update_sensor_status();
  void update_motor_encoder();
  void update_diff_odom(int32 motor1_encoder, int32 motor2_encoder);
  void update_onmi_odom(int32 motor1_encoder, int32 motor2_encoder, int32 motor3_encoder);
  void update_mec_odom(int32 motor1_encoder, int32 motor2_encoder, int32 motor3_encoder, int32 motor4_encoder);

public:
  BaseDriverConfig &getBaseDriverConfig()
  {
    return bdg;
  }

  ros::NodeHandle *getNodeHandle()
  {
    return &nh;
  }

  ros::NodeHandle *getPrivateNodeHandle()
  {
    return &pn;
  }

private:

  static BaseDriver *instance;

  // 里程计累加
  float pos_x;
  float pos_y;

  // 电机编码器最后一帧数据（两轮）
  float last_left_encoder;
  float last_right_encoder;
  
  // 电机编码器最后一帧数据（三轮、四轮）
  float last_encoder1;
  float last_encoder2;
  float last_encoder3;
  float last_encoder4;

  // imu最后一帧航向角
  float last_yaw;
  // imu当前航向角
  float current_yaw;

  // imu零偏误差
  float first_error_yaw;
  ros::Time last_time;
  bool is_first_measurement;

  // 上一帧舵机位置缓存
  float last_joint1_pos;
  float last_joint2_pos;
  float last_joint3_pos;
  float last_joint4_pos;
  float last_joint5_pos;
  float last_claw_pos;
  ros::Time last_joint_time;

  // 配置文件读取
  BaseDriverConfig bdg;

  // 通信交互类封装
  boost::shared_ptr<DataFrame> frame;

  // 底盘控制topic接收
  ros::Subscriber cmd_vel_sub;
  // 里程计发布
  ros::Publisher odom_pub;
  // 超声发布
  ros::Publisher ul_sensor_pub1, ul_sensor_pub2, ul_sensor_pub3, ul_sensor_pub4;
  // tof发布
  ros::Publisher tof_pub1, tof_pub2, tof_pub3, tof_pub4;
  // 碰撞传感器发布
  ros::Publisher bump_sensor_pub;
  // 12路ADC发布
  ros::Publisher adc_value_pub;
  // MPU6500发布
  ros::Publisher mpu_6500_pub;
  // 机械臂位置信息发布
  ros::Publisher joint_pub;
  // 单个舵机数据回调
  ros::Subscriber cmd_single_servo_sub;
  // 多个舵机的数据回调
  ros::Subscriber cmd_multiple_servo_sub;
  // imu数据接收
  ros::Subscriber imu_sub;
  // 里程计msg
  nav_msgs::Odometry odom;
  geometry_msgs::TransformStamped odom_trans;
  tf::TransformBroadcaster odom_broadcaster;
  // 超声传感器消息
  sensor_msgs::Range ul_sensor1, ul_sensor2, ul_sensor3, ul_sensor4;
  // tof传感器消息
  sensor_msgs::Range tof1, tof2, tof3, tof4;
  // 碰撞传感器消息
  std_msgs::Int16MultiArray bump_sensor_array;
  // 12路ADC传感器消息
  upros_message::Adc adc_value;
  // MPU6500消息
  sensor_msgs::Imu mpu_value;
  // 机械臂位置信息
  sensor_msgs::JointState joint_info;

  int devide_clock_senser = 200;
  int devide_clock_senser_status = 200;
  int devide_clock_odom = 50;
  int device_clock_joint_info = 100;
  int devide_clock_speed = 100;
  int devide_clock_mpu = 100;

  int clock = 0;

  ros::NodeHandle nh;
  ros::NodeHandle pn;

  #define MAX_MOTOR_COUNT 4

  bool need_update_speed;
};
