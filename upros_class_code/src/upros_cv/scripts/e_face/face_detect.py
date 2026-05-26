#!/usr/bin/env python3

import rospy
import dlib                     
import numpy as np             
import cv2           
import sys

from sensor_msgs.msg import Image  
from cv_bridge import CvBridge, CvBridgeError

class Face_Emotion():

    def __init__(self):
        rospy.init_node('face_emotion_node', anonymous=True) 
        self.detector = dlib.get_frontal_face_detector()
        modelPath = "/home/bcsh/Documents/Models/shape_predictor_68_face_landmarks.dat"
        self.predictor = dlib.shape_predictor(modelPath)
        self.bridge = CvBridge()  
        self.image_sub = rospy.Subscriber('/camera/color/image_raw', Image, self.image_callback)  
        self.image_pub = rospy.Publisher('/image_result', Image, queue_size=10)
        
    def update_frame(self, frame):
        result = frame.copy()
        img_gray = cv2.cvtColor(frame, cv2.COLOR_RGB2GRAY)  # 取灰度
        faces = self.detector(img_gray, 0)  # 使用人脸检测器检测每一帧图像中的人脸。并返回人脸数rects
        font = cv2.FONT_HERSHEY_SIMPLEX  # 要显示在屏幕上的字体
        # 如果只检测到1张人脸
        if len(faces) == 1:
            # 对每个人脸都标出68个特征点
            for i in range(len(faces)):
                # enumerate方法同时返回数据对象的索引和数据，k为索引，d为faces中的对象
                for k, d in enumerate(faces):
                    cv2.rectangle(result, (d.left(), d.top()), (d.right(), d.bottom()), (0, 0, 255))   # 用红色矩形框出人脸
                    self.face_width = d.right() - d.left()  # 计算人脸框边长
                    shape = self.predictor(result, d)  # 使用预测器得到68点数据的坐标
                    # 圆圈显示每个特征点
                    for i in range(68):
                        cv2.circle(result, (shape.part(i).x, shape.part(i).y), 2, (0, 255, 0), -1, 8)
        else:
            cv2.putText(result, "Not 1 Face", (20, 50), font, 1, (0, 0, 255), 1, cv2.LINE_AA)   # 没有检测到人脸,直接写出
        return result

    def image_callback(self, msg):  
        try:  
            cv_image = self.bridge.imgmsg_to_cv2(msg, "bgr8")  
            src = cv_image.copy()
            result = self.update_frame(src)
            ros_image = self.bridge.cv2_to_imgmsg(result, "bgr8")
            self.image_pub.publish(ros_image)         
        except CvBridgeError as e:  
            rospy.logerr(e)  
            return     

    def spin(self):  
        rospy.spin()          # 让ROS节点保持运行，直到被关闭  
  

if __name__ == "__main__":
    my_face = Face_Emotion()
    my_face.spin()