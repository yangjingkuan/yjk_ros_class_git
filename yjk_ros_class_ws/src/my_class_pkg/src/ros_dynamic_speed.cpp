#include <ros/ros.h>
#include <dynamic_reconfigure/server.h>
// 1. 头文件包名替换：dynamic_tutorials → my_class_pkg
#include <my_class_pkg/TutorialsConfig.h>
#include <geometry_msgs/Twist.h>

// 定义全局速度变量，先赋初始值（比如0.1m/s，小车启动就走）
double robot_speed = 0.1; 

// 2. 回调函数参数包名替换：dynamic_tutorials → my_class_pkg
void callback(my_class_pkg::TutorialsConfig &config, uint32_t level) {
    ROS_INFO("Reconfigure Request: %f", config.double_param);
    // 滑块调节的值覆盖初始速度
    robot_speed = config.double_param;
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "dynamic_tutorials");
    ros::NodeHandle nh;

    // 创建/cmd_vel发布者（机器人速度主题）
    ros::Publisher cmd_pub_ = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    // 设置发布频率（10Hz，每秒发10次速度指令）
    ros::Rate rate(10);

    // 3. 动态参数服务器包名替换：dynamic_tutorials → my_class_pkg（两处）
    dynamic_reconfigure::Server<my_class_pkg::TutorialsConfig> server;
    dynamic_reconfigure::Server<my_class_pkg::TutorialsConfig>::CallbackType f;
    f = boost::bind(&callback, _1, _2);
    server.setCallback(f);

    ROS_INFO("Spinning node, initial speed: 0.1 m/s");
    while(ros::ok()) {
        // 构造速度指令消息
        geometry_msgs::Twist cmd_vel;
        // 给线速度赋值（初始值0.1，滑块调节后会变）
        cmd_vel.linear.x = robot_speed; 
        cmd_vel.linear.y = 0.0;
        cmd_vel.linear.z = 0.0;
        cmd_vel.angular.x = 0.0;
        cmd_vel.angular.y = 0.0;
        cmd_vel.angular.z = 0.0;

        // 发布速度指令（关键：启动后就会持续发初始速度）
        cmd_pub_.publish(cmd_vel);

        ros::spinOnce();
        rate.sleep();
    }
    return 0;
}
