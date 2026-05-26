#include <ros/ros.h>
#include <my_class_pkg/MyMessage.h>
#include <string>

int main(int argc, char** argv) {
    // 初始化ROS节点
    ros::init(argc, argv, "my_message_publisher");
    ros::NodeHandle nh;

    // 创建发布者，发布到 /my_msg_topic 话题，队列大小10
    ros::Publisher pub = nh.advertise<my_class_pkg::MyMessage>("/my_msg_topic", 10);

    // 设置发布频率：1Hz（每秒发1次）
    ros::Rate rate(1);

    // 初始化自定义消息
    my_class_pkg::MyMessage msg;
    int count = 0;

    ROS_INFO("Custom message publisher started!");

    // 循环发布
    while (ros::ok()) {
        // 填充消息内容
        msg.key = count;
        msg.value = "Hello from C++ custom publisher - count: " + std::to_string(count);

        // 发布消息
        pub.publish(msg);
        ROS_INFO("Published: key=%d, value='%s'", msg.key, msg.value.c_str());

        // 计数自增
        count++;

        // 按频率休眠
        rate.sleep();
    }

    return 0;
}
