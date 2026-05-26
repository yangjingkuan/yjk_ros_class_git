#!/usr/bin/env python3
import rospy
from std_msgs.msg import String

class VoiceCommand:
    def __init__(self):
        rospy.init_node("voice_cmd_node")
        self.pub = rospy.Publisher("/nav/goal", String, queue_size=10)
        rospy.Subscriber("/voice_words", String, self.callback)
        rospy.loginfo("✅ 语音导航指令节点已启动")

    def callback(self, msg):
        text = msg.data
        rospy.loginfo(f"听到：{text}")
        text_clean = text.replace(" ", "")

        # 支持各种中文说法
        if "A点" in text_clean or "去A" in text_clean or "到A" in text_clean:
            rospy.loginfo("👉 指令：去A点")
            self.pub.publish("A")

        elif "B点" in text_clean or "去B" in text_clean or "到B" in text_clean:
            rospy.loginfo("👉 指令：去B点")
            self.pub.publish("B")

        elif "起点" in text_clean or "回起点" in text_clean or "回到起点" in text_clean:
            rospy.loginfo("👉 指令：回到起点")
            self.pub.publish("HOME")

if __name__ == "__main__":
    VoiceCommand()
    rospy.spin()
