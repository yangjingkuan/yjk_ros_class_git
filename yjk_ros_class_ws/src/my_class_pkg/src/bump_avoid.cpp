#include "ros/ros.h"
#include "std_msgs/Int16MultiArray.h"
#include "geometry_msgs/Twist.h"

ros::Publisher vel_publisher;
// 新增：定义初始前进速度（可自行调整，0.2m/s为适中速度）
const float FORWARD_SPEED = 0.2;  

void bumpCallback(const std_msgs::Int16MultiArray::ConstPtr& msg)
{
    geometry_msgs::Twist vel_msg;
    // 修改1：默认设置为前进速度（不再是0）
    vel_msg.linear.x = FORWARD_SPEED;  
    vel_msg.angular.z = 0.0;

    bool is_collision = false;
    for (int i = 0; i < msg->data.size(); ++i)
    {
        if (msg->data[i] == 1)
        {
            is_collision = true;
            ROS_INFO("检测到碰撞！传感器编号：%d → 机器人后退", i);
            break;
        }
    }

    // 修改2：碰撞时设置后退速度，无碰撞则保持前进
    if (is_collision)
    {
        vel_msg.linear.x = -0.3;  // 后退速度略快于前进，避障更明显
    }

    vel_publisher.publish(vel_msg);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "bump_avoid_node");
    ros::NodeHandle nh;

    vel_publisher = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    ros::Subscriber bump_sub = nh.subscribe("/robot/bump_sensor", 1000, bumpCallback);

    // 新增：提示初始前进速度
    ROS_INFO("避障节点已启动 → 机器人以%.1fm/s前进，碰撞后自动后退！", FORWARD_SPEED);

    ros::spin();
    return 0;
}
