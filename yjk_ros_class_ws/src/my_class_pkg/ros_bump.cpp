// ros_bump.cpp - PDF原生碰撞传感器避障代码
#include "ros/ros.h"
#include "std_msgs/Int16MultiArray.h"
#include "geometry_msgs/Twist.h"

ros::Publisher vel_pub;
bool bump_flag = false;

// PDF原生回调函数（无修改）
void bumpCallback(const std_msgs::Int16MultiArray::ConstPtr& msg)
{
    for(int i=0; i<msg->data.size(); i++)
    {
        if(msg->data[i] == 1)
        {
            bump_flag = true;
            ROS_INFO("Bump sensor %d is triggered!", i);
            break;
        }
        else
        {
            bump_flag = false;
        }
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "bump_avoid_node");
    ros::NodeHandle n;

    // PDF原生话题订阅/发布（无修改）
    ros::Subscriber bump_sub = n.subscribe("/robot/bump_sensor", 10, bumpCallback);
    vel_pub = n.advertise<geometry_msgs/Twist>("/cmd_vel", 10);

    ros::Rate loop_rate(10);
    geometry_msgs::Twist vel_msg;

    while (ros::ok())
    {
        if(bump_flag)
        {
            // PDF原生避障逻辑：后退+左转
            vel_msg.linear.x = -0.2;
            vel_msg.angular.z = 0.5;
            vel_pub.publish(vel_msg);
            ros::Duration(0.5).sleep();
            
            // 停止运动
            vel_msg.linear.x = 0;
            vel_msg.angular.z = 0;
            vel_pub.publish(vel_msg);
            bump_flag = false;
        }
        else
        {
            // PDF原生直行逻辑
            vel_msg.linear.x = 0.2;
            vel_msg.angular.z = 0;
            vel_pub.publish(vel_msg);
        }

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
