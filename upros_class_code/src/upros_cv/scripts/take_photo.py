#!/usr/bin/env python3

import rospy
import cv2
import os
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

class ImageSubscriberNode:
    def __init__(self):
        rospy.init_node('take_photo_node', anonymous=True)
        self.bridge = CvBridge()
        self.cv_image = None  # 存储当前图像帧
        self.save_path = "/home/bcsh/image.jpg"  # 保存路径
        self.image_sub = rospy.Subscriber('/camera/color/image_raw', Image, self.image_callback)

    def image_callback(self, msg):
        try:
            self.cv_image = self.bridge.imgmsg_to_cv2(msg, "bgr8")
            cv2.imshow("Image", self.cv_image)
            key = cv2.waitKey(1) 
            if(key == ord('p')):
                cv2.imwrite(self.save_path, self.cv_image)
                rospy.loginfo(f"Image saved to {self.save_path}")
        except CvBridgeError as e:
            rospy.logerr(e)

if __name__ == '__main__':
    try:
        node = ImageSubscriberNode()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass