#include <ros/ros.h>
#include <std_msgs/String.h>
int main(int argc, char **argv)
{
    // 初始化ROS节点，命名为my_publisher
    ros::init(argc, argv, "my_publisher");
    // 创建节点句柄
    ros::NodeHandle nh;
    // 定义发布者，发布主题my_topic，队列大小10
    ros::Publisher pub = nh.advertise<std_msgs::String>("my_topic", 10);
    // 定义发布频率：1Hz（每秒1次）
    ros::Rate rate(1.0);
    while (ros::ok())
    {
        // 创建消息对象并赋值
        std_msgs::String msg;
        msg.data = "Hello, world!";
        // 发布消息
        pub.publish(msg);
        // 按频率休眠
        rate.sleep();
    }
    return 0;
}
