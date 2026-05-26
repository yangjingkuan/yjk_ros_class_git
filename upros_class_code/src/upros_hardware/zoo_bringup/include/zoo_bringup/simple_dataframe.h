#ifndef ZOO_SIMPLE_DATAFRAME_H_
#define ZOO_SIMPLE_DATAFRAME_H_

#include "msg_queue.h"
#include "serial/serial.h"
#include <string.h>
#include <vector>
#include <iostream>
#include <inttypes.h>
#include <deque>
#include <queue>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

static const unsigned short MESSAGE_BUFFER_SIZE = 255;

typedef int int32;
typedef short int16;
typedef unsigned short uint16;
typedef std::vector<uint8_t> Buffer;

enum MESSAGE_ID
{
    ID_SET_ROBOT_CHASSIS_TYPE = 0x01,
    ID_INIT_ODOM = 0x03,
    ID_SET_VELOCITY = 0x04,
    ID_GET_ODOM = 0x05,
    ID_CLEAR_ODOM = 0x06,
    ID_GET_IMU = 0X07,
    ID_SINGLE_SERVO = 0x08,
    ID_MULTPLE_SERVO = 0x09,
    ID_UL_SENSOR = 0X0C,
    ID_SENSRO_STATUS = 0X0D,
    ID_GET_MOTOR_ENCODER = 0X0F,
    ID_GET_SERVO_INFO = 0X10,
    ID_MESSGAE_MAX
};

// 字头
#define FIX_HEAD1 0x5A
#define FIX_HEAD2 0xA5

struct Head
{
    unsigned char head1;  // 头部标记,固定值:0X5A
    unsigned char head2;  // 头部标记,固定值:0XA5
    unsigned char msg_id; // 消息ID,表示消息具体作用,决定消息体具体格式
    unsigned char length; // 消息体长度
};

struct Message
{
    struct Head head;
    unsigned char data[MESSAGE_BUFFER_SIZE];
    unsigned char check;
    unsigned char recv_count; // 已经接收的字节数

    Message() {}
    Message(unsigned char msg_id, unsigned char *data = 0, unsigned char len = 0)
    {
        head.head1 = FIX_HEAD1;
        head.head2 = FIX_HEAD2;
        head.msg_id = msg_id;
        head.length = recv_count = len;
        check = 0;

        if (data != 0 && len != 0)
            memcpy(this->data, data, len);

        unsigned char *_send_buffer = (unsigned char *)this;

        unsigned int i = 0;
        for (i = 0; i < sizeof(head) + head.length; i++)
            check += _send_buffer[i];

        _send_buffer[sizeof(head) + head.length] = check;
    }
};

/*
若干枚举变量，用于校验反馈的数据
*/
enum RECEIVE_STATE
{
    STATE_RECV_FIX = 0,
    STATE_RECV_FIX2,
    STATE_RECV_ID,
    STATE_RECV_LEN,
    STATE_RECV_DATA,
    STATE_RECV_CHECK,
};

class DataFrame
{

public:
    DataFrame();                                        // 构造函数
    ~DataFrame();                                       // 析构函数，关闭串口
    bool init(std::string port_name, int32_t baudrate); // 初始化函数，配置并打开串口
    void start_thread();                                // 启动线程函数

private:
    std::string DecIntToHexStr(char c);        // 将 char 转化为16进制字符串的函数，调试用
    void recv_thread_func();                   // 接收线程主循环
    void send_thread_func();                   // 发送线程主循环
    void process_buffer();                     // 数据提包函数
    bool data_recv(std::vector<uint8_t> data); // 数据校验函数，校验帧头，ID，数据长度
    bool data_parse();                         // 解析函数，将数据解析到结构体内

public:
    bool send_message(const MESSAGE_ID id);
    bool send_message(const MESSAGE_ID id, unsigned char *data, unsigned char len);
    bool send_message(Message *msg);

private:
    std::shared_ptr<serial::Serial> serialPtr; // 串口通信类

    Message active_rx_msg;    // 解析完成的数据暂存
    RECEIVE_STATE recv_state; // 数据接收状态
    MsgQueue<Buffer> queue;   // 数据发送队列

    std::thread recv_thread; // 接收线程对象
    std::thread send_thread; // 发送线程对象

    std::atomic<bool> keep_running{false}; // 线程退出标志

    std::mutex serial_mutex; // 串口操作互斥锁

    std::deque<uint8_t> buffer_queue;   // 串口回调的数据队列
    std::vector<uint8_t> current_frame; // 当前解析的帧数据
};

#endif
