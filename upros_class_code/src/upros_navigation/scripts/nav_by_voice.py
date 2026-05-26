#!/usr/bin/env python3

import rospy
import actionlib
import time
from actionlib_msgs.msg import GoalStatus
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
from std_msgs.msg import Int32
from tf_conversions import transformations
from math import pi

# ======== 导航点坐标：建完图后用 rviz 点 2D Nav Goal 读坐标填在这里 ========
# 格式：(x, y, yaw角度)，yaw 单位是"度"
WAYPOINT_1 = (0.0,  0.0,  0.0)   # TODO: 导航点 1
WAYPOINT_2 = (0.0,  0.0,  0.0)   # TODO: 导航点 2
HOME       = (0.0,  0.0,  0.0)   # TODO: 起点（机器人出发位置）
# =========================================================================


class NavByVoice:
    def __init__(self):
        self.move_base = actionlib.SimpleActionClient("move_base", MoveBaseAction)
        rospy.loginfo("等待 move_base 服务器...")
        self.move_base.wait_for_server(rospy.Duration(60))
        rospy.loginfo("move_base 已连接")

        self.voice_id = None
        rospy.Subscriber("/voice_cmd", Int32, self._voice_cb)
        rospy.loginfo("等待语音指令（话题 /voice_cmd，发布 1 或 2）...")

    def _voice_cb(self, msg):
        if self.voice_id is None:       # 只响应第一条，防止重复触发
            rospy.loginfo("收到语音指令：ID = %d" % msg.data)
            self.voice_id = msg.data

    def goto(self, point, name="目标点"):
        goal = MoveBaseGoal()
        goal.target_pose.header.frame_id = "map"
        goal.target_pose.header.stamp = rospy.Time.now()
        goal.target_pose.pose.position.x = point[0]
        goal.target_pose.pose.position.y = point[1]
        q = transformations.quaternion_from_euler(0.0, 0.0, point[2] / 180.0 * pi)
        goal.target_pose.pose.orientation.x = q[0]
        goal.target_pose.pose.orientation.y = q[1]
        goal.target_pose.pose.orientation.z = q[2]
        goal.target_pose.pose.orientation.w = q[3]

        rospy.loginfo("前往 %s  x=%.2f  y=%.2f  yaw=%.1f°" % (name, point[0], point[1], point[2]))
        self.move_base.send_goal(goal)
        result = self.move_base.wait_for_result(rospy.Duration(120))

        if not result:
            self.move_base.cancel_goal()
            rospy.logwarn("%s 超时，已取消目标" % name)
            return False

        if self.move_base.get_state() == GoalStatus.SUCCEEDED:
            rospy.loginfo("%s 到达！" % name)
            return True
        else:
            rospy.logwarn("%s 导航失败，状态码=%d" % (name, self.move_base.get_state()))
            return False

    def run(self):
        rate = rospy.Rate(10)
        while not rospy.is_shutdown():
            if self.voice_id is not None:
                break
            rate.sleep()

        vid = self.voice_id

        if vid == 1:
            rospy.loginfo("执行顺序：导航点1 → 导航点2 → 起点")
            self.goto(WAYPOINT_1, "导航点1")
            time.sleep(1)
            self.goto(WAYPOINT_2, "导航点2")
        elif vid == 2:
            rospy.loginfo("执行顺序：导航点2 → 导航点1 → 起点")
            self.goto(WAYPOINT_2, "导航点2")
            time.sleep(1)
            self.goto(WAYPOINT_1, "导航点1")
        else:
            rospy.logwarn("未知 ID：%d，退出" % vid)
            return

        time.sleep(1)
        self.goto(HOME, "起点")
        rospy.loginfo("任务完成，已返回起点")


if __name__ == "__main__":
    rospy.init_node("nav_by_voice", anonymous=False)
    node = NavByVoice()
    node.run()
