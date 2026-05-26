#!/usr/bin/env python3
import rospy
from std_msgs.msg import String

# 消息回调函数
def callback(data):
    rospy.loginfo(rospy.get_caller_id() + "I heard %s", data.data)

def my_subscriber():
    # 初始化节点，anonymous=True保证节点名唯一
    rospy.init_node('my_subscriber', anonymous=True)
    # 订阅主题my_topic，绑定回调函数
    rospy.Subscriber("my_topic", String, callback)
    # 循环等待回调
    rospy.spin()

if __name__ == '__main__':
    try:
        my_subscriber()
    except rospy.ROSInterruptException:
        pass
