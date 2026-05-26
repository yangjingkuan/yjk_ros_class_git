#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <my_class_pkg/MyActionAction.h>

int main(int argc, char **argv) {
    ros::init(argc, argv, "my_action_client");

    actionlib::SimpleActionClient<my_class_pkg::MyActionAction> client("my_action", true);

    ROS_INFO("Waiting for action server...");
    client.waitForServer();
    ROS_INFO("Server connected!");

    my_class_pkg::MyActionGoal goal;
    goal.object_name = "test_object";

    ROS_INFO("Sending goal...");
    client.sendGoal(goal);

    bool finished = client.waitForResult(ros::Duration(30.0));

    if (finished) {
        actionlib::SimpleClientGoalState state = client.getState();
        ROS_INFO("Action finished: %s", state.toString().c_str());
    } else {
        ROS_INFO("Action timeout!");
    }

    return 0;
}
