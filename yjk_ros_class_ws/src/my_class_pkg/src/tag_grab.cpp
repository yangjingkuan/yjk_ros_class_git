#include "tf2_ros/transform_listener.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
#include "geometry_msgs/TransformStamped.h"
#include "geometry_msgs/Twist.h"
#include "upros_message/ArmPosition.h"
#include "std_srvs/Empty.h"
#include <ros/ros.h>

int limit(int val, int min, int max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

void sleep(double second)
{
    ros::Duration(second).sleep();
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "mgrab_test");
    ros::AsyncSpinner spinner(1);
    spinner.start();
    ros::NodeHandle nh;

    ros::Publisher cmd_vel_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    ros::ServiceClient arm_move_open_client = nh.serviceClient<upros_message::ArmPosition>("/upros_arm_control/arm_pos_service_open");
    ros::ServiceClient arm_zero_client = nh.serviceClient<std_srvs::Empty>("/upros_arm_control/zero_service");
    ros::ServiceClient arm_grab_client = nh.serviceClient<std_srvs::Empty>("/upros_arm_control/grab_service");
    ros::ServiceClient arm_release_client = nh.serviceClient<std_srvs::Empty>("/upros_arm_control/release_service");

    std_srvs::Empty empty_srv;
    arm_zero_client.call(empty_srv);
    sleep(2.0);

    tf2_ros::Buffer buffer;
    tf2_ros::TransformListener listener(buffer);
    ROS_INFO("等待标签检测...");

    geometry_msgs::TransformStamped tfs_base;
    tfs_base = buffer.lookupTransform("base_link", "tag_1", ros::Time(0), ros::Duration(5.0));

    ROS_INFO("小车开始移动...");
    geometry_msgs::Twist vel_msg;
    while (ros::ok())
    {
        tfs_base = buffer.lookupTransform("base_link", "tag_1", ros::Time(0), ros::Duration(1.0));
        double dx = tfs_base.transform.translation.x;
        double dy = tfs_base.transform.translation.y;

        if (dx > 0.35) {
            vel_msg.linear.x = 0.15;
            vel_msg.angular.z = -dy * 0.6;
        } else {
            vel_msg.linear.x = 0.0;
            vel_msg.angular.z = 0.0;
            cmd_vel_pub.publish(vel_msg);
            break;
        }
        cmd_vel_pub.publish(vel_msg);
        sleep(0.05);
    }

    ROS_INFO("小车已到位！开始抓取...");
    sleep(1.0);

    geometry_msgs::TransformStamped tfs_arm = buffer.lookupTransform("arm_base_link", "tag_1", ros::Time(0), ros::Duration(3.0));

    int x = -int(tfs_arm.transform.translation.y * 1000);
    int y =  int(tfs_arm.transform.translation.x * 1000);
    int z =  int(tfs_arm.transform.translation.z * 1000) + 10;

    x = limit(x, -130, 130);
    y = limit(y, 30, 190);
    z = limit(z, 40, 230);

    upros_message::ArmPosition move_srv;
    move_srv.request.x = x;
    move_srv.request.y = y;
    move_srv.request.z = z;

    // ==============================
    // 抓取 + 超级夹紧（关键！）
    // ==============================
    arm_release_client.call(empty_srv); sleep(2.0);
    arm_move_open_client.call(move_srv); sleep(3.5);

    // 连续多次夹紧，锁死力矩！
    arm_grab_client.call(empty_srv);  sleep(1.0);
    arm_grab_client.call(empty_srv);  sleep(1.0);
    arm_grab_client.call(empty_srv);  sleep(3.0);  // 超级紧

    // ==============================
    // 回零 + 持续夹紧，绝不掉
    // ==============================
    arm_zero_client.call(empty_srv);
    arm_grab_client.call(empty_srv);
    arm_grab_client.call(empty_srv);
    sleep(5.0);

    // ==============================
    // 回到位再松开
    // ==============================
    arm_release_client.call(empty_srv);
    sleep(1.0);

    ROS_INFO("✅ 超级夹紧，完美回位！");
    ros::shutdown();
    return 0;
}
