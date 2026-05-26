#include "tf2_ros/transform_listener.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
#include "geometry_msgs/TransformStamped.h"
#include "geometry_msgs/PointStamped.h"
#include "geometry_msgs/Twist.h"
#include "upros_message/ArmPosition.h"
#include "std_srvs/Empty.h"
#include <ros/ros.h>

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

    ros::ServiceClient arm_move_open_client = nh.serviceClient<upros_message::ArmPosition>("/upros_arm_control/arm_pos_service_open");
    ros::ServiceClient arm_move_close_client = nh.serviceClient<upros_message::ArmPosition>("/upros_arm_control/arm_pos_service_close");
    ros::ServiceClient arm_zero_client = nh.serviceClient<std_srvs::Empty>("/upros_arm_control/zero_service");
    ros::ServiceClient arm_grab_client = nh.serviceClient<std_srvs::Empty>("/upros_arm_control/grab_service");
    ros::ServiceClient arm_release_client = nh.serviceClient<std_srvs::Empty>("/upros_arm_control/release_service");

    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    geometry_msgs::Twist vel_msg;
    ros::Rate loop_rate(10);

    // 打开夹爪
    std_srvs::Empty empty_srv;
    arm_release_client.call(empty_srv);
    sleep(5.0);

    // 运行到观测点，平视，300长，200高
    upros_message::ArmPosition move_srv;
    move_srv.request.x = 0;
    move_srv.request.y = 300.0;
    move_srv.request.z = 200.0;
    arm_move_open_client.call(move_srv);
    sleep(3.0);

    tf2_ros::Buffer buffer;
    tf2_ros::TransformListener listener(buffer);
    ROS_INFO("tf coordinate transformaing....");

    // 获取待抓取目标到相机的坐标变换
    geometry_msgs::TransformStamped tfs_1 = buffer.lookupTransform("camera_link", "yolo_pose_link", ros::Time(0), ros::Duration(100));

    float grab_pos_x = tfs_1.transform.translation.x + 0.25; // 相机与爪子中心位置差约5cm
    float grab_pos_y = tfs_1.transform.translation.y;
    float grab_pos_z = tfs_1.transform.translation.z + 0.24; // 相机与爪子中心高度差

    std::cout << "grab_pos_x: " << grab_pos_x << "  grab_pos_y: " << grab_pos_y << "  grab_pos_z: " << grab_pos_z << std::endl;

    // 单位转换，ros坐标系到逆运算坐标系
    int x = -int(grab_pos_y * 1000);
    int y = 250.0;
    int z = 200.0;
    
    // 运动到抓取位置
    move_srv.request.x = x;
    move_srv.request.y = y;
    move_srv.request.z = z;
    arm_move_open_client.call(move_srv);
    sleep(5.0);

    //前进一段距离
    vel_msg.linear.x = 0.1;
    int count = 0;
    while (ros::ok() && count < 32)
    {
        pub.publish(vel_msg);
        ros::spinOnce();
        loop_rate.sleep();
        count++;
    }
    vel_msg.linear.x = 0.0;
    pub.publish(vel_msg);
    sleep(5.0);

    // 实施抓取
    arm_grab_client.call(empty_srv);
    sleep(5.0);

    //houtui一段距离
    vel_msg.linear.x = -0.1;
    count = 0;
    while (ros::ok() && count < 32)
    {
        pub.publish(vel_msg);
        ros::spinOnce();
        loop_rate.sleep();
        count++;
    }
    vel_msg.linear.x = 0.0;
    pub.publish(vel_msg);
    sleep(5.0);

    // 回中
    arm_zero_client.call(empty_srv);
    sleep(5.0);

    ros::shutdown();

    return 0;
}
