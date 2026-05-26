#!/usr/bin/env python3
import rospy
import os

def play_sound():
    rospy.init_node('play_sound')
    file_path = "/home/bcsh/targetfound.wav"
    rospy.loginfo("播放：targetfound.wav")
    os.system("aplay " + file_path)

if __name__ == '__main__':
    try:
        play_sound()
    except rospy.ROSInterruptException:
        pass
