#!/usr/bin/env python3

import rospy
import socket
import json
import threading
from upros_message.msg import YoloDetection  # 替换为您的自定义消息包名

class UDPtoROSNode:
    def __init__(self):
        # 初始化ROS节点
        rospy.init_node('udp_to_ros_bridge', anonymous=True)
        # 参数配置
        self.udp_ip = "localhost"
        self.udp_port = 12345
        # 创建ROS发布者
        self.pub = rospy.Publisher('/yolo_detections', YoloDetection, queue_size=10)
        # 初始化UDP套接字
        self.udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.udp_socket.bind((self.udp_ip, self.udp_port))
        rospy.loginfo(f"UDP监听启动: {self.udp_ip}:{self.udp_port}")
        # 创建后台线程处理UDP数据
        self.thread = threading.Thread(target=self.udp_listener)
        self.thread.daemon = True
        self.thread.start()

    """持续监听UDP数据并处理"""
    def udp_listener(self):
        while not rospy.is_shutdown():
            try:
                # 接收UDP数据（缓冲区大小可调整）
                data, addr = self.udp_socket.recvfrom(4096)
                rospy.logdebug(f"收到来自 {addr} 的UDP数据")                
                # 解析JSON数据
                json_data = json.loads(data.decode('utf-8'))                
                # 处理每个检测结果
                for detection in json_data:
                    self.publish_detection(detection)                        
            except Exception as e:
                rospy.logerr(f"数据处理错误: {str(e)}")

    """发布检测结果到ROS话题"""
    def publish_detection(self, detection):
        try:
            # 创建自定义消息对象
            msg = YoloDetection()
            msg.header.stamp = rospy.Time.now()
            # 解析中心点坐标（元组→独立坐标）
            msg.class_id = int(detection['class'])
            msg.center_x = int(detection['center'][0])
            msg.center_y = int(detection['center'][1])
            # 发布消息
            self.pub.publish(msg)
            rospy.loginfo(f"发布检测: 类别={msg.class_id}, 中心点=({msg.center_x}, {msg.center_y})")            
        except KeyError as ke:
            rospy.logwarn(f"JSON字段缺失: {str(ke)}")
        except ValueError as ve:
            rospy.logwarn(f"坐标转换错误: {str(ve)}")

if __name__ == '__main__':
    try:
        node = UDPtoROSNode()
        rospy.spin()  # 保持节点运行
    except rospy.ROSInterruptException:
        pass