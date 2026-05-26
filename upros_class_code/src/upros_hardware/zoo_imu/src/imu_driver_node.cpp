#include <ros/ros.h>
#include <serial/serial.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/MagneticField.h>
#include <tf2/LinearMath/Quaternion.h>

// 数据包类型标识
enum PacketType
{
    ACCEL = 0x51,
    GYRO = 0x52,
    ANGLE = 0x53
};

class IMUNode
{
private:
    size_t key = 0;
    int flag = 0;
    std::array<uint8_t, 11> buffer;                    // 串口数据
    std::array<float, 3> angular_velocity = {0, 0, 0}; // 角速度
    std::array<float, 3> acceleration = {0, 0, 0};     // 线加速度
    std::array<float, 3> angle_degree = {0, 0, 0};     // 姿态角度
    std::array<bool, 3> pub_flag = {true, true, true}; // 发布标志位

    // ROS接口
    ros::Publisher imu_pub_;
    serial::Serial ser_;
    sensor_msgs::Imu imu_msg_;

    // 数据包接收标志
    std::array<bool, 4> recv_flags_{true, true, true, true};

public:
    IMUNode(ros::NodeHandle &nh)
    {
        // 参数配置
        std::string port;
        int baudrate;
        nh.param<std::string>("port", port, "/dev/imu");
        nh.param("baudrate", baudrate, 921600);

        // 初始化串口
        try
        {
            ser_.setPort(port);
            ser_.setBaudrate(baudrate);
            serial::Timeout to = serial::Timeout::simpleTimeout(1000);
            ser_.setTimeout(to);
            ser_.open();
            ROS_INFO("Serial port %s opened at %d baud", port.c_str(), baudrate);
        }
        catch (const std::exception &e)
        {
            ROS_FATAL("Failed to open serial port: %s", e.what());
            exit(1);
        }

        // 初始化ROS发布者
        imu_pub_ = nh.advertise<sensor_msgs::Imu>("/ros/imu", 10);
        // 初始化消息头
        imu_msg_.header.frame_id = "imu_link";
    }

    void run()
    {
        ros::Rate rate(200); // 200Hz
        while (ros::ok())
        {
            size_t avail = ser_.available();
            if (avail > 0)
            {
                std::vector<uint8_t> data;
                ser_.read(data, avail);
                for (uint8_t byte : data)
                {
                    handle_serial_data(byte);
                }
            }
            rate.sleep();
        }
    }

    void handle_serial_data(uint8_t raw_data)
    {
        buffer[key] = raw_data;
        key++;
        // 帧头不正确，丢弃
        if (buffer[0] != 0x55)
        {
            key = 0;
            return;
        }
        // 没到1帧数据不处理
        if (key < 11)
            return;
        // 一整帧提取出来
        std::vector<uint8_t> data_buffer(buffer.begin(), buffer.begin() + 11);
        // 加速度解析 (0x51)
        if (buffer[1] == 0x51 && pub_flag[0])
        {
            if (checksum(data_buffer, data_buffer[10]))
            {
                auto accel_data = hex_to_short({data_buffer.begin() + 2, data_buffer.begin() + 8});
                for (int i = 0; i < 3; ++i)
                {
                    acceleration[i] = accel_data[i] / 32768.0f * 16.0f * 9.8f;
                }
            }
            pub_flag[0] = false;
        }
        // 角速度解析 (0x52)
        else if (buffer[1] == 0x52 && pub_flag[1])
        {
            if (checksum(data_buffer, data_buffer[10]))
            {
                auto gyro_data = hex_to_short({data_buffer.begin() + 2, data_buffer.begin() + 8});
                for (int i = 0; i < 3; ++i)
                {
                    angular_velocity[i] = gyro_data[i] / 32768.0f * 2000.0f * M_PI / 180.0f;
                }
            }
            pub_flag[1] = false;
        }
        // 姿态解析 (0x53)
        else if (buffer[1] == 0x53 && pub_flag[2])
        {
            if (checksum(data_buffer, data_buffer[10]))
            {
                auto angle_data = hex_to_short({data_buffer.begin() + 2, data_buffer.begin() + 8});
                for (int i = 0; i < 3; ++i)
                {
                    angle_degree[i] = angle_data[i] / 32768.0f * 180.0f;
                }
            }
            pub_flag[2] = false;
        }
        else
        {
            key = 0;
            return;
        }
        // 校验完成重新开始
        key = 0;
        if (pub_flag[0] || pub_flag[1] || pub_flag[2])
            return;
        pub_flag = {true, true, true};
        publish_imu_data();
    }

    // 校验和
    bool checksum(const std::vector<uint8_t> &data, uint8_t check_data)
    {
        uint8_t sum = 0;
        for (size_t i = 0; i < 10; ++i)
            sum += data[i];
        return (sum & 0xFF) == check_data;
    }

    // 两个8位数据合成16进制数据
    std::vector<int16_t> hex_to_short(const std::vector<uint8_t> &raw_data)
    {
        std::vector<int16_t> result;
        for (size_t i = 0; i < raw_data.size(); i += 2)
        {
            int16_t value = (raw_data[i + 1] << 8) | raw_data[i];
            result.push_back(value);
        }
        return result;
    }

    void publish_imu_data()
    {
        ros::Time now = ros::Time::now();
        // 填充IMU消息
        imu_msg_.header.stamp = now;
        tf2::Quaternion q;
        q.setRPY(angle_degree[0] * M_PI / 180.0f, angle_degree[1] * M_PI / 180.0f, angle_degree[2] * M_PI / 180.0f);
        imu_msg_.orientation.x = q.x();
        imu_msg_.orientation.y = q.y();
        imu_msg_.orientation.z = q.z();
        imu_msg_.orientation.w = q.w();
        imu_msg_.angular_velocity.x = angular_velocity[0];
        imu_msg_.angular_velocity.y = angular_velocity[1];
        imu_msg_.angular_velocity.z = angular_velocity[2];
        imu_msg_.linear_acceleration.x = acceleration[0];
        imu_msg_.linear_acceleration.y = acceleration[1];
        imu_msg_.linear_acceleration.z = acceleration[2];
        // 发布
        imu_pub_.publish(imu_msg_);
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "imu_node");
    ros::NodeHandle nh("~");
    IMUNode node(nh);
    node.run();
    return 0;
}