#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <cv_bridge/cv_bridge.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Pose.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2_ros/transform_broadcaster.h>
#include <thread>
#include "std_srvs/Empty.h"
#include <image_transport/image_transport.h>
#include <upros_message/YoloDetection.h>

class Yolo_Detect
{
private:

    cv::Mat camera_matrix;
    cv::Mat camera_dis;
    bool camera_info = 0;
    float x, y, dis;

    int target_id = 0;

    ros::Subscriber camera_info_sub; // 内参矩阵回调

    ros::NodeHandle nh; // 设置一个句柄

    std::string rgb_result_pub_topic; // 识别结果图像话题

    geometry_msgs::TransformStamped obg_msg; // 目标位置

    tf2_ros::TransformBroadcaster tf_pub; // tf 变换

    message_filters::Subscriber<upros_message::YoloDetection> *detection_sub; // 识别结果回调

    message_filters::Subscriber<sensor_msgs::Image> *depth_sub; // 深度图像回调

    typedef message_filters::sync_policies::ApproximateTime<upros_message::YoloDetection, sensor_msgs::Image> yoloSyncPolicy; // 过滤器，同时订阅识别回调与深度图，保证时序一致

    message_filters::Synchronizer<yoloSyncPolicy> *sync_; // 同步回调

    void camera_info_callback(const sensor_msgs::CameraInfo::ConstPtr &msg); // 内参矩阵回调函数

    void detectCallback(const upros_message::YoloDetection::ConstPtr &detect_msg, const sensor_msgs::Image::ConstPtr &depth_msg); // 联合回调函数

public:
    Yolo_Detect();
    ~Yolo_Detect();
};


Yolo_Detect::Yolo_Detect()
{
    camera_info_sub = nh.subscribe("/camera/color/camera_info", 100, &Yolo_Detect::camera_info_callback, this);

    detection_sub = new message_filters::Subscriber<upros_message::YoloDetection>(nh, "/yolo_detections", 1);

    depth_sub = new message_filters::Subscriber<sensor_msgs::Image>(nh, "/camera/depth/image_raw", 1);

    sync_ = new message_filters::Synchronizer<yoloSyncPolicy>(yoloSyncPolicy(10), *detection_sub, *depth_sub);

    sync_->registerCallback(std::bind(&Yolo_Detect::detectCallback, this, std::placeholders::_1, std::placeholders::_2));
}

Yolo_Detect::~Yolo_Detect()
{
}

void Yolo_Detect::detectCallback(const upros_message::YoloDetection::ConstPtr &detect_msg, const sensor_msgs::Image::ConstPtr &depth_msg)
{
    if (camera_info)
    {
        int detect_id = detect_msg->class_id;
        int detect_center_x = detect_msg->center_x;
        int detect_center_y = detect_msg->center_y;

        cv_bridge::CvImagePtr cv_depth_ptr = cv_bridge::toCvCopy(depth_msg, sensor_msgs::image_encodings::TYPE_16UC1);
        cv::Mat depimage=cv_depth_ptr->image;

        // 发布坐标位置
        if (detect_id == target_id)
        {
            cv::Point newpos(detect_center_x, detect_center_y);
            dis = (depimage.at<ushort>(newpos.y, newpos.x)) / 1000.0;
            x = (newpos.x - camera_matrix.at<double>(0, 2)) / camera_matrix.at<double>(0, 0) * dis;
            y = (newpos.y - camera_matrix.at<double>(1, 2)) / camera_matrix.at<double>(1, 1) * dis;
            obg_msg.transform.translation.x = x;
            obg_msg.transform.translation.y = y;
            obg_msg.transform.translation.z = dis;
            obg_msg.transform.rotation.x = 0;
            obg_msg.transform.rotation.y = 0;
            obg_msg.transform.rotation.z = 0;
            obg_msg.transform.rotation.w = 1;

            obg_msg.header.stamp = ros::Time::now();
            obg_msg.header.frame_id = "camera_color_optical_frame";
            obg_msg.child_frame_id = "yolo_pose_link";
            tf_pub.sendTransform(obg_msg);
        }
    }
    else
    {
        ROS_ERROR("invalid camera info!");
    }
}

// 内参矩阵回调
void Yolo_Detect::camera_info_callback(const sensor_msgs::CameraInfo::ConstPtr &msg)
{
    bool K_valid = 0;
    bool D_valid = 0;
    if (!camera_info)
    {
        for (uint8_t i = 0; i < msg->K.size(); i++)
        {
            if (msg->K.at(i) != 0)
            {
                K_valid = 1;
                break;
            }
        }
        for (uint8_t i = 0; i < msg->D.size(); i++)
        {
            if (msg->D.at(i) != 0)
            {
                D_valid = 1;
                break;
            }
        }
        if (K_valid && D_valid)
        {
            camera_matrix = cv::Mat::zeros(3, 3, CV_64F);
            camera_dis = cv::Mat::zeros(1, 5, CV_64F);
            for (uint8_t i = 0; i < 3; i++)
            {
                for (uint8_t j = 0; j < 3; j++)
                {
                    camera_matrix.at<double>(i, j) = msg->K[i * 3 + j];
                }
            }
            for (uint8_t i = 0; i < 5; i++)
            {
                camera_dis.at<double>(0, i) = msg->D[i];
            }
            camera_info = 1;
        }
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "yolo_tf_publisher");
    Yolo_Detect yolo_detect;
    ros::spin();
    return 0;
}