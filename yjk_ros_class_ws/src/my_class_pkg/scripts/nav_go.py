#!/usr/bin/env python3
import rospy
import math
import threading
import actionlib
from std_msgs.msg import String
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
from geometry_msgs.msg import Quaternion

# ===================== 你的坐标 =====================
POINT_A = (1.344299, 0.015167, 0.020438)
POINT_B = (1.702754, -1.515871, -0.115095)
POINT_HOME = (0.058173, -1.711935, -0.006233)

def yaw2quat(yaw):
    q = Quaternion()
    q.z = math.sin(yaw / 2)
    q.w = math.cos(yaw / 2)
    return q

def goto(x, y, yaw):
    # 每次导航前新建客户端，避免锁死
    client = actionlib.SimpleActionClient('move_base', MoveBaseAction)
    client.wait_for_server()
    
    goal = MoveBaseGoal()
    goal.target_pose.header.frame_id = "map"
    goal.target_pose.header.stamp = rospy.Time.now()
    goal.target_pose.pose.position.x = x
    goal.target_pose.pose.position.y = y
    goal.target_pose.pose.orientation = yaw2quat(yaw)
    
    client.send_goal(goal)
    rospy.loginfo(f"🚗 导航中：x={x}, y={y}")
    client.wait_for_result()
    rospy.loginfo("✅ 到达目标点")

def callback(msg):
    data = msg.data
    if data == "A":
        threading.Thread(target=goto, args=POINT_A, daemon=True).start()
    elif data == "B":
        threading.Thread(target=goto, args=POINT_B, daemon=True).start()
    elif data == "HOME":
        threading.Thread(target=goto, args=POINT_HOME, daemon=True).start()

if __name__ == "__main__":
    rospy.init_node("nav_go_node", anonymous=True)
    rospy.Subscriber("/nav/goal", String, callback)
    rospy.loginfo("✅ 语音导航执行节点已启动")
    rospy.spin()
