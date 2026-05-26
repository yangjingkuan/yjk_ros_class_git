#include "ros/ros.h"
#include "sensor_msgs/Range.h"

// 正前方TOF2主题名
#define TOF_TOPIC "/us/tof2"

// 只处理Range类型（你的TOF2实际类型）
void tof2Callback(const sensor_msgs::Range::ConstPtr& msg)
{
    // 获取正前方距离（单位：米）
    float distance = msg->range;
    ROS_INFO("正前方TOF2传感器检测距离：%.2f 米", distance);

    // 避障阈值：<0.4米触发警告
    if (distance < 0.4) {
        ROS_WARN("⚠️ 正前方障碍物过近！距离：%.2f米 → 建议后退", distance);
    } else if (distance < 1.0) {
        ROS_INFO("🔶 正前方有障碍物（中等距离）：%.2f米", distance);
    } else {
        ROS_INFO("✅ 正前方无近距离障碍物：%.2f米", distance);
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "tof2_subscriber_node");
    ros::NodeHandle nh;

    // 只订阅Range类型，避免冲突
    ros::Subscriber tof_sub = nh.subscribe(TOF_TOPIC, 1000, tof2Callback);

    ROS_INFO("正前方TOF2传感器订阅节点已启动 → 监听主题：%s", TOF_TOPIC);
    ROS_INFO("等待TOF2数据...（请靠近正前方传感器）");

    ros::spin();
    return 0;
}
