#include "ros/ros.h"
#include "sensor_msgs/Range.h"
#include "geometry_msgs/Twist.h"

// 核心配置参数（可根据需求调整）
#define TOF2_TOPIC "/us/tof2"       // 正前方TOF2主题名
const float OBSTACLE_THRESHOLD = 0.4;  // 避障阈值（米）
const float FORWARD_SPEED = 0.2;       // 前进速度（米/秒）
const float BACKWARD_SPEED = -0.3;     // 后退速度（米/秒）
const float ANGULAR_SPEED = 0.0;       // 角速度（0=不转弯）

// 全局速度发布器（用于向机器人发布运动指令）
ros::Publisher vel_pub;

// TOF2数据回调函数（核心避障逻辑）
void tof2AvoidCallback(const sensor_msgs::Range::ConstPtr& msg)
{
    // 1. 获取正前方TOF2检测距离
    float distance = msg->range;
    ROS_INFO("正前方TOF2检测距离：%.2f 米", distance);

    // 2. 创建速度指令对象（ROS标准运动指令）
    geometry_msgs::Twist vel_cmd;
    vel_cmd.angular.z = ANGULAR_SPEED;  // 不转弯

    // 3. 避障逻辑判断
    if (distance < OBSTACLE_THRESHOLD) {
        // 距离过近 → 后退
        vel_cmd.linear.x = BACKWARD_SPEED;
        ROS_WARN("⚠️ 正前方遇障！后退（距离：%.2fm < %.2fm）", distance, OBSTACLE_THRESHOLD);
    } else {
        // 距离安全 → 前进
        vel_cmd.linear.x = FORWARD_SPEED;
        ROS_INFO("🟢 正前方无近距离障碍 → 前进（速度：%.1fm/s）", FORWARD_SPEED);
    }

    // 4. 发布速度指令（控制机器人运动）
    vel_pub.publish(vel_cmd);
}

int main(int argc, char **argv)
{
    // 初始化ROS节点（节点名：tof2_avoid_node）
    ros::init(argc, argv, "tof2_avoid_node");
    ros::NodeHandle nh;

    // 初始化速度发布器（发布到/cmd_vel主题，ROS机器人通用运动指令主题）
    vel_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    // 订阅TOF2传感器数据
    ros::Subscriber tof2_sub = nh.subscribe(TOF2_TOPIC, 1000, tof2AvoidCallback);

    // 打印启动信息
    ROS_INFO("========================");
    ROS_INFO("TOF2正前方避障节点已启动");
    ROS_INFO("避障阈值：%.1f米 | 前进速度：%.1fm/s | 后退速度：%.1fm/s",
             OBSTACLE_THRESHOLD, FORWARD_SPEED, -BACKWARD_SPEED);
    ROS_INFO("监听主题：%s | 运动指令主题：/cmd_vel", TOF2_TOPIC);
    ROS_INFO("========================");

    // 循环等待数据（保持节点运行）
    ros::spin();

    return 0;
}
