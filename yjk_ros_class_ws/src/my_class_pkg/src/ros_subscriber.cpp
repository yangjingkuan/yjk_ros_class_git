#include <ros/ros.h>
#include <std_msgs/String.h>
// 消息回调函数，处理接收到的消息
void callback(const std_msgs::String::ConstPtr& msg){
    ROS_INFO("I heard: [%s]", msg->data.c_str());
}
int main(int argc, char** argv){
    // 初始化ROS节点，命名为my_subscriber
    ros::init(argc, argv, "my_subscriber");
    // 创建节点句柄
    ros::NodeHandle nh;
    // 定义订阅者，订阅主题my_topic，队列大小10，绑定回调函数
    ros::Subscriber sub = nh.subscribe("my_topic", 10, callback);
    // 循环等待回调函数触发
    ros::spin();
    return 0;
}
