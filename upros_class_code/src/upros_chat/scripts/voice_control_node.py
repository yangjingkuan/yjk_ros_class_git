#!/usr/bin/env python3

from tokenizer import Tokenizer
import rospy
from upros_message.msg import TagCommand
from std_msgs.msg import String

class VoiceControlNode:
    def __init__(self):
        rospy.init_node('tokenizer_publisher')
        self.tokenizer = Tokenizer()
        self.tag_cmd_pub = rospy.Publisher('/voice_control', TagCommand, queue_size=10)
        self.talker_sub = rospy.Subscriber('/speech/result', String, self.speech_result_callback)
    
    def speech_result_callback(self, msg):
        user_input = msg.data # 语音指令中提取字符串
        filtered_input = self.tokenizer.pre_process(user_input) # 预处理
        intent_string = self.tokenizer.extract_intent(filtered_input) # 提取意图加指令组合
        # 先只把第一个命令发布出去
        cmd = TagCommand()
        cmd.intent = intent_string[0]['intent']
        cmd.target = intent_string[0]['target']
        self.tag_cmd_pub.publish(cmd)

if __name__ == "__main__":
    try:
        ttsplayer = VoiceControlNode()
        rospy.spin()
    except KeyboardInterrupt:
        print("\nCaught Ctrl + C. Exiting")