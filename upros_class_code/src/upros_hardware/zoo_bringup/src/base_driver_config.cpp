#include "base_driver_config.h"

#include "data_holder.h"
#define PI 3.1415926f

BaseDriverConfig::BaseDriverConfig(ros::NodeHandle &p) : pn(p)
{
}

BaseDriverConfig::~BaseDriverConfig()
{
}

void BaseDriverConfig::init()
{
  pn.param<std::string>("port", port, "/dev/zoo");
  pn.param<int>("buadrate", buadrate, 115200);
  pn.param<float>("motor_encoder", motor_encoder, 64.0);
  pn.param<float>("motor_ratio", motor_ratio, 90.0);
  pn.param<float>("diff_wheel_radius", diff_wheel_radius, 0.099);
  pn.param<float>("diff_wheel_track", diff_wheel_track, 0.216);
  pn.param<float>("onmi_wheel_radius", onmi_wheel_radius, 0.075);
  pn.param<float>("onmi_wheel_track", onmi_wheel_track, 0.1466);
  pn.param<float>("mec_wheel_radius", mec_wheel_radius, 0.075);
  pn.param<float>("mec_wheel_track", mec_wheel_track, 0.1466);

  ROS_INFO("port: %s  buadrate: %d", port.c_str(), buadrate);
  ROS_INFO("motor_encoder: %f  motor_ratio: %f", motor_encoder, motor_ratio);
  ROS_INFO("diff_wheel_radius: %f  diff_wheel_track: %f", diff_wheel_radius, diff_wheel_track);

  pn.param<int>("chassis_type", chassis_type, 1);
  pn.param<std::string>("base_frame", base_frame, "base_link");
  pn.param<std::string>("odom_frame", odom_frame, "odom");
  pn.param<bool>("publish_tf", publish_tf, true);

  pn.param<std::string>("imu_topic", imu_topic, "ros/imu");
  pn.param<std::string>("cmd_vel_topic", cmd_vel_topic, "cmd_vel");
  pn.param<std::string>("odom_topic", odom_topic, "odom");
  pn.param<std::string>("cmd_single_servo_topic", cmd_single_servo_topic, "single_servo_topic");
  pn.param<std::string>("cmd_multiple_servo_topic", cmd_multiple_servo_topic, "multiple_servo_topic");

}