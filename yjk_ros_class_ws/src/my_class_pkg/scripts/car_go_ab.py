#!/usr/bin/env python3
# 导航执行节点：A→B→HOME 或 B→A→HOME（状态机 + 线程安全）
import rospy
import math
import actionlib
import threading
from enum import Enum
from std_msgs.msg import String
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
from geometry_msgs.msg import Quaternion
from actionlib_msgs.msg import GoalStatus

# 坐标
POINT_A = (1.344299, 0, 0.020438)
POINT_B = (0.661, 1.485875, -0.115095)
POINT_HOME = (0.05817, 0.0173, -0.006233)

# 状态机定义（标准状态机格式）
class NavState(Enum):
    IDLE          = 0  # 空闲
    GO_TO_START   = 1  # 去起点
    GO_TO_MID     = 2  # 去中间点
    GO_TO_HOME    = 3  # 回HOME
    ERROR         = 4  # 失败

# ==================== 全局变量 ====================
client = actionlib.SimpleActionClient('move_base', MoveBaseAction)

# 线程安全锁（修复 GO_B 失败的核心！）
state_lock = threading.Lock()
current_state = NavState.IDLE

# ==================== 工具函数 ====================
def yaw_to_quat(yaw):
    q = Quaternion()
    q.z = math.sin(yaw / 2.0)
    q.w = math.cos(yaw / 2.0)
    return q

def go_to_point(x, y, yaw, point_name):
    goal = MoveBaseGoal()
    goal.target_pose.header.frame_id = "map"
    goal.target_pose.header.stamp = rospy.Time.now()
    goal.target_pose.pose.position.x = x
    goal.target_pose.pose.position.y = y
    goal.target_pose.pose.orientation = yaw_to_quat(yaw)

    client.send_goal(goal)
    client.wait_for_result()
    return client.get_state() == GoalStatus.SUCCEEDED

# ==================== 状态机执行函数 ====================
def nav_state_machine(start_p, start_name, mid_p, mid_name):
    global current_state

    # 上锁：防止状态错乱
    with state_lock:
        if current_state != NavState.IDLE:
            return
        current_state = NavState.GO_TO_START

    try:
        # ===== 第一步：去起点 =====
        rospy.loginfo(f"正在前往 {start_name}")
        success = go_to_point(*start_p, start_name)
        
        if not success:
            rospy.logerr(f"导航至 {start_name} 失败")
            with state_lock:
                current_state = NavState.ERROR
            return

        rospy.loginfo(f"已到达 {start_name}")
        with state_lock:
            current_state = NavState.GO_TO_MID

        # ===== 第二步：去中间点 =====
        rospy.loginfo(f"正在前往 {mid_name}")
        success = go_to_point(*mid_p, mid_name)
        
        if not success:
            rospy.logerr(f"导航至 {mid_name} 失败")
            with state_lock:
                current_state = NavState.ERROR
            return

        rospy.loginfo(f"已到达 {mid_name}")
        with state_lock:
            current_state = NavState.GO_TO_HOME

        # ===== 第三步：回HOME =====
        rospy.loginfo("正在前往起点")
        success = go_to_point(*POINT_HOME, "起点")
        
        if success:
            rospy.loginfo("已到达起点，任务完成")
        else:
            rospy.logerr("返回起点失败")

    except Exception as e:
        rospy.logerr(f"导航异常: {str(e)}")
    finally:
        with state_lock:
            current_state = NavState.IDLE
        rospy.loginfo("导航任务结束，可接收新指令")

# ==================== 指令回调 ====================
def callback(msg):
    cmd = msg.data.strip()
    rospy.loginfo(f"收到指令: {cmd}")

    # 线程安全判断状态
    with state_lock:
        if current_state != NavState.IDLE:
            rospy.loginfo("任务正在执行，忽略新指令")
            return

    if cmd == "GO_A":
        threading.Thread(target=nav_state_machine, args=(POINT_A, "A点", POINT_B, "B点"), daemon=True).start()

    elif cmd == "GO_B":
        threading.Thread(target=nav_state_machine, args=(POINT_B, "B点", POINT_A, "A点"), daemon=True).start()

    else:
        rospy.logwarn(f"不支持的指令: {cmd}")

# ==================== 主函数 ====================
if __name__ == "__main__":
    rospy.init_node("nav_executor_node")
    rospy.loginfo("等待 move_base 服务...")
    client.wait_for_server()
    rospy.loginfo("move_base 已连接")

    rospy.Subscriber("/nav_control", String, callback)
    rospy.loginfo("导航节点已启动")
    rospy.loginfo("支持指令：GO_A  /  GO_B")
    
    rospy.spin()