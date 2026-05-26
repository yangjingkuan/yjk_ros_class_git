#include "ros/ros.h"
#include "std_msgs/Int16MultiArray.h"

// 碰撞传感器回调函数，接收并解析数据
void bumpCallback(const std_msgs::Int16MultiArray::ConstPtr& msg)
{
    ROS_INFO("Bump Sensor Data Received: ");
    for (int i = 0; i < msg->data.size(); ++i)
    {
        // 打印每个传感器ID的状态：1=触发，0=未触发
        ROS_INFO("Sensor %d: %s", i, msg->data[i] ? "Triggered" : "Not Triggered");
    }
}

int main(int argc, char **argv)
{
    // 初始化ROS节点，节点名：bump_sensor_subscriber
    ros::init(argc, argv, "bump_sensor_subscriber");
    // 创建节点句柄
    ros::NodeHandle n;
    // 订阅碰撞传感器主题，队列大小1000，回调函数bumpCallback
    ros::Subscriber sub = n.subscribe("/robot/bump_sensor", 1000, bumpCallback);
    // 循环等待回调函数触发
    ros::spin();
    return 0;
}
