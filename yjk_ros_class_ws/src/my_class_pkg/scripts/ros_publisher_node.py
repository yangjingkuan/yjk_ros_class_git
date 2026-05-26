#!/usr/bin/env python3
import rospy
from std_msgs.msg import String

if __name__ == '__main__':
    # 初始化ROS节点
    rospy.init_node('my_publisher')
    # 定义发布者，主题my_topic，消息类型String，队列大小10
    pub = rospy.Publisher('my_topic', String, queue_size=10)
    # 发布频率1Hz
    rate = rospy.Rate(1)
    while not rospy.is_shutdown():
        # 创建消息并赋值
        msg = String()
        msg.data = 'Hello, world!'
        # 发布消息
        pub.publish(msg)
        # 按频率休眠
        rate.sleep()
