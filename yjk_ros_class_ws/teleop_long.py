#!/usr/bin/env python3
import rospy
from geometry_msgs.msg import Twist
import sys, select, termios, tty

msg = """
长按控制小车：
   i
j  k  l
   ,

i : 前进
, : 后退
j : 左转
l : 右转
k : 停止
CTRL+C 退出
"""

def getKey():
    tty.setraw(sys.stdin.fileno())
    rlist, _, _ = select.select([sys.stdin], [], [], 0.1)
    if rlist:
        key = sys.stdin.read(1)
    else:
        key = ''
    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
    return key

if __name__ == "__main__":
    settings = termios.tcgetattr(sys.stdin)
    rospy.init_node('teleop_long')
    pub = rospy.Publisher('cmd_vel', Twist, queue_size=10)
    rate = rospy.Rate(50)

    speed = 0.8
    turn = 0.6

    try:
        print(msg)
        while not rospy.is_shutdown():
            key = getKey()
            twist = Twist()

            if key == 'i':
                twist.linear.x = speed
                twist.angular.z = 0
            elif key == ',':
                twist.linear.x = -speed
                twist.angular.z = 0
            elif key == 'j':
                twist.linear.x = speed * 0.5
                twist.angular.z = turn
            elif key == 'l':
                twist.linear.x = speed * 0.5
                twist.angular.z = -turn
            elif key == 'k':
                twist.linear.x = 0
                twist.angular.z = 0
            elif key == '\x03':
                break

            pub.publish(twist)
            rate.sleep()

    except Exception as e:
        print(e)

    finally:
        twist = Twist()
        pub.publish(twist)
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)