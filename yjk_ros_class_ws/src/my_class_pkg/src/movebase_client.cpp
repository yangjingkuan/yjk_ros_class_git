#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <iostream>
#include <tf2/LinearMath/Quaternion.h>
#include <geometry_msgs/Quaternion.h>
#include <cmath>

using namespace std;
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "send_goals_node");
    MoveBaseClient ac("move_base", true);
    ac.waitForServer();

    move_base_msgs::MoveBaseGoal goal1;
    move_base_msgs::MoveBaseGoal goal2;
    move_base_msgs::MoveBaseGoal goal3;
    tf2::Quaternion quaternion;

    // 导航点1: x=1.584947, y=-1.195707, yaw=-1.558030
    quaternion.setRPY(0.0, 0.0, -1.558030);
    goal1.target_pose.pose.position.x = 1.584947;
    goal1.target_pose.pose.position.y = -1.195707;
    goal1.target_pose.pose.position.z = 0.0;
    goal1.target_pose.pose.orientation.z = quaternion.z();
    goal1.target_pose.pose.orientation.w = quaternion.w();
    goal1.target_pose.header.frame_id = "map";
    goal1.target_pose.header.stamp = ros::Time::now();

    ac.sendGoal(goal1);
    ROS_INFO("Send Goal 1 !!!");
    ac.waitForResult();
    if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
        ROS_INFO("The Goal 1 Reached Successfully!!!");
    }
    else
    {
        ROS_WARN("The Goal Planning Failed for some reason");
    }

    // 导航点2: x=0.309973, y=-1.602660, yaw=-0.892550
    quaternion.setRPY(0.0, 0.0, -0.892550);
    goal2.target_pose.pose.position.x = 0.309973;
    goal2.target_pose.pose.position.y = -1.602660;
    goal2.target_pose.pose.position.z = 0.0;
    goal2.target_pose.pose.orientation.z = quaternion.z();
    goal2.target_pose.pose.orientation.w = quaternion.w();
    goal2.target_pose.header.frame_id = "map";
    goal2.target_pose.header.stamp = ros::Time::now();

    ac.sendGoal(goal2);
    ROS_INFO("Send Goal 2 !!!");
    ac.waitForResult();
    if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
        ROS_INFO("The Goal 2 Reached Successfully!!!");
    }
    else
    {
        ROS_WARN("The Goal Planning Failed for some reason");
    }

    // 导航点3: x=0.080611, y=-0.071459, yaw=0.020522
    quaternion.setRPY(0.0, 0.0, 0.020522);
    goal3.target_pose.pose.position.x = 0.080611;
    goal3.target_pose.pose.position.y = -0.071459;
    goal3.target_pose.pose.position.z = 0.0;
    goal3.target_pose.pose.orientation.z = quaternion.z();
    goal3.target_pose.pose.orientation.w = quaternion.w();
    goal3.target_pose.header.frame_id = "map";
    goal3.target_pose.header.stamp = ros::Time::now();

    ac.sendGoal(goal3);
    ROS_INFO("Send Goal Home !!!");
    ac.waitForResult();
    if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
        ROS_INFO("Back !!!!");
    }
    else
    {
        ROS_WARN("The Goal Planning Failed for some reason");
    }

    return 0;
}
