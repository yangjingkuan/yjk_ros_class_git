#include "simple_dataframe.h"
#include "data_holder.h"
#include <ros/ros.h>
#include <stdio.h>

DataFrame::DataFrame()
{
    recv_state = STATE_RECV_FIX;
    keep_running = false;
}

DataFrame::~DataFrame()
{
    keep_running = false; // 通知线程退出
    if (recv_thread.joinable())
    {
        recv_thread.join(); // 等待线程结束
    }
    if (send_thread.joinable())
    {
        send_thread.join(); // 等待线程结束
    }
    // 关闭串口
    if (serialPtr->isOpen())
        serialPtr->close();
}

bool DataFrame::init(std::string port_name, int32_t baudrate)
{
    // 初始化一个串口
    serialPtr = std::make_shared<serial::Serial>(port_name, baudrate, serial::Timeout::simpleTimeout(1000));
    return true;
}

bool DataFrame::data_recv(std::vector<uint8_t> data)
{
    recv_state = STATE_RECV_FIX;
    for (int i = 0; i < data.size(); i++)
    {
        unsigned char c = data[i];
        if (recv_state == STATE_RECV_FIX)
        {
            if (c == FIX_HEAD1)
            {
                // 帧头1校验成功，准备校验帧头2
                memset(&active_rx_msg, 0, sizeof(active_rx_msg));
                active_rx_msg.head.head1 = c;
                active_rx_msg.check += c;
                recv_state = STATE_RECV_FIX2;
            }
            else
            {
                // 帧头1校验失败，准备校验帧头1
                recv_state = STATE_RECV_FIX;
            }
            continue;
        }

        // 校验帧头head2
        if (recv_state == STATE_RECV_FIX2)
        {
            if (c == FIX_HEAD2)
            {
                // 帧头2校验成功，准备校验设备id
                active_rx_msg.head.head2 = c;
                active_rx_msg.check += c;
                recv_state = STATE_RECV_ID;
            }
            else
            {
                recv_state = STATE_RECV_FIX;
            }
            continue;
        }

        // 校验设备id
        if (recv_state == STATE_RECV_ID)
        {
            if (c < ID_MESSGAE_MAX)
            {
                // 设备id校验成功，准备校验数据长度
                active_rx_msg.head.msg_id = c;
                active_rx_msg.check += c;
                recv_state = STATE_RECV_LEN;
            }
            else
            {
                recv_state = STATE_RECV_FIX;
            }
            continue;
        }

        // 校验数据长度
        if (recv_state == STATE_RECV_LEN)
        {
            int len = (int)c;
            if (len == data.size() - 5)
            {
                // 数据长度校验成功，准备校验指令
                active_rx_msg.head.length = c;
                active_rx_msg.check += c;
                if (active_rx_msg.head.length == 0)
                {
                    recv_state = STATE_RECV_CHECK;
                }
                else
                {
                    recv_state = STATE_RECV_DATA;
                }
            }
            else
            {
                recv_state = STATE_RECV_FIX;
            }
            continue;
        }

        // 校验数据，复制到接收结构体内
        if (recv_state == STATE_RECV_DATA)
        {
            active_rx_msg.data[active_rx_msg.recv_count++] = c;
            active_rx_msg.check += c;
            if (active_rx_msg.recv_count >= active_rx_msg.head.length)
            {
                recv_state = STATE_RECV_CHECK;
            }
            continue;
        }

        // 校验位
        if (recv_state == STATE_RECV_CHECK)
        {
            recv_state = STATE_RECV_FIX;
            if (active_rx_msg.check == c)
            {
                return true;
            }
        }
    }
    return false;
}

// 将接收到的数据赋值给DataHolder
bool DataFrame::data_parse()
{
    MESSAGE_ID id = (MESSAGE_ID)active_rx_msg.head.msg_id;
    Data_holder *dh = Data_holder::get();
    switch (id)
    {
    case ID_SET_ROBOT_CHASSIS_TYPE:
        break;
    case ID_CLEAR_ODOM:
        break;
    case ID_SET_VELOCITY:
        break;
    case ID_GET_ODOM:
        memcpy(&dh->odom, active_rx_msg.data, sizeof(dh->odom));
        break;
    case ID_SINGLE_SERVO:
        break;
    case ID_MULTPLE_SERVO:
        break;
    case ID_UL_SENSOR:
        memcpy(&dh->ul_sensor, active_rx_msg.data, sizeof(dh->ul_sensor));
        break;
    case ID_SENSRO_STATUS:
        memcpy(&dh->sensor_status, active_rx_msg.data, sizeof(dh->sensor_status));
        break;
    case ID_GET_MOTOR_ENCODER:
        memcpy(&dh->encoder, active_rx_msg.data, sizeof(dh->encoder));
        break;
    case ID_GET_SERVO_INFO:
        memcpy(&dh->servo_pos, active_rx_msg.data, sizeof(dh->servo_pos));
        break;
    case ID_GET_IMU:
        memcpy(&dh->imu, active_rx_msg.data, sizeof(dh->imu));
        break;
    default:
        break;
    }
    return true;
}

// 只有ID的消息
bool DataFrame::send_message(const MESSAGE_ID id)
{
    Message msg(id);
    send_message(&msg);
    return true;
}

// 有ID的数据的消息
bool DataFrame::send_message(const MESSAGE_ID id, unsigned char *data, unsigned char len)
{
    Message msg(id, data, len);
    send_message(&msg);
    return true;
}

// 整合为Message的消息
bool DataFrame::send_message(Message *msg)
{
    Buffer data((unsigned char *)msg, (unsigned char *)msg + sizeof(msg->head) + msg->head.length + 1);
    queue.push(data);
    return true;
}

// 启动发送数据包线程
void DataFrame::send_thread_func()
{
    while (keep_running)
    {
        // 如果发送缓存队列不为空，发送数据帧
        if (!queue.isEmpty())
        {
            // 从待发送队列中取出一个数据包，写入串口
            Buffer msg = queue.get_and_pop();
            size_t bytes_written = serialPtr->write(msg.data(), msg.size());
        }
        // 发送缓存区为空
        else
        {
            this_thread::sleep_for(std::chrono::milliseconds(10)); // 防止CPU过载
        }
    }
}

void DataFrame::start_thread()
{
    // 串口校验不通过，不启动线程
    if (!serialPtr || !serialPtr->isOpen())
    {
        throw std::runtime_error("Serial port not initialized");
    }
    keep_running = true;
    recv_thread = std::thread(&DataFrame::recv_thread_func, this); // 启动接收线程
    send_thread = std::thread(&DataFrame::send_thread_func, this); // 启动发送线程
}

void DataFrame::recv_thread_func()
{
    const size_t BUFFER_SIZE = 1024;
    std::vector<uint8_t> buffer(BUFFER_SIZE, 0);
    while (keep_running)
    {
        try
        {
            // 从串口中读取数据
            size_t bytes_available = 0;
            {
                std::lock_guard<std::mutex> lock(serial_mutex);
                bytes_available = serialPtr->available();
            }
            // 如果串口缓存区存在可获取数据
            if (bytes_available > 0)
            {
                size_t bytes_read = 0;
                {
                    std::lock_guard<std::mutex> lock(serial_mutex);
                    bytes_read = serialPtr->read(buffer.data(), bytes_available);
                }
                // 如果读取到的数据大字节数于0
                if (bytes_read > 0)
                {
                    // 读取串口缓存区的数据
                    std::vector<uint8_t> received_data(buffer.begin(), buffer.begin() + bytes_read);
                    // 将收到的数据塞入队列
                    buffer_queue.insert(buffer_queue.end(), received_data.begin(), received_data.end());
                    // 处理队列的数据
                    process_buffer();
                }
            }
            // 适当休眠避免CPU占用过高
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        catch (const std::exception &e)
        {
            // 处理异常（例如串口断开）
            std::cerr << "Serial thread error: " << e.what() << std::endl;
            break;
        }
    }
}

// 处理缓存队列数据避免粘包
void DataFrame::process_buffer()
{
    bool got = false;
    while (true)
    {
        // 1. 寻找帧头 0xA5 0x5A
        size_t start_pos = 0;
        bool found_start = false;
        for (; start_pos + 1 < buffer_queue.size(); ++start_pos)
        {
            if (buffer_queue[start_pos] == FIX_HEAD1 && buffer_queue[start_pos + 1] == FIX_HEAD2)
            {
                found_start = true;
                break;
            }
        }

        // 2. 删除帧头前的无效数据
        buffer_queue.erase(buffer_queue.begin(), buffer_queue.begin() + start_pos);

        // 3. 检查是否有足够数据包含帧头，id，length
        if (buffer_queue.size() < 4)
            return;

        size_t id = buffer_queue[2];

        // 4. 寻找帧长度
        size_t length = buffer_queue[3];

        // std::cout << "ID: " << id << " Length: " << length << std::endl;

        // 5. 提取有效数据（帧头到帧尾）
        current_frame.assign(buffer_queue.begin(), buffer_queue.begin() + length + 5);

        // 6. 调用解析逻辑
        if (data_recv(current_frame))
        {
            // 成功解析后，删除整个帧（包括头尾）
            buffer_queue.erase(buffer_queue.begin(), buffer_queue.begin() + length + 5);
            got = true;
        }
        else
        {
            // 解析失败，丢弃该帧
            buffer_queue.erase(buffer_queue.begin(), buffer_queue.begin() + length + 5);
            break;
        }

        // 堆积先清除试试
        if (buffer_queue.size() > 100)
        {
            buffer_queue.clear();
        }

        // 解析成功，将数据帧复制在结构体中
        if (got)
        {
            this->data_parse();
            break;
        }
    }
}

// 十进制转十六进制字符串的函数，返回字符串变量，显示调试的时候有用
std::string DataFrame::DecIntToHexStr(char c)
{
    // 处理符号问题：转无符号后再计算
    unsigned char uc = static_cast<unsigned char>(c);
    int decimalNumber = uc;
    std::vector<int> ivec;
    // 特判 0 的情况
    if (decimalNumber == 0)
    {
        return "00";
    }
    // 提取十六进制位（自动逆序）
    while (decimalNumber != 0)
    {
        ivec.push_back(decimalNumber % 16);
        decimalNumber /= 16;
    }
    // 逆序排列得到正确的高低位顺序
    std::reverse(ivec.begin(), ivec.end());
    std::string hexadecimal;
    for (int digit : ivec)
    {
        char ch;
        if (digit > 9)
        {
            ch = 'A' + (digit - 10); // 10→A, 11→B, ..., 15→F
        }
        else
        {
            ch = '0' + digit; // 0→0, 1→1, ..., 9→9
        }
        hexadecimal += ch;
    }
    // 补零到两位（如 "F" → "0F"）
    if (hexadecimal.size() < 2)
    {
        hexadecimal = std::string(2 - hexadecimal.size(), '0') + hexadecimal;
    }
    return hexadecimal;
}