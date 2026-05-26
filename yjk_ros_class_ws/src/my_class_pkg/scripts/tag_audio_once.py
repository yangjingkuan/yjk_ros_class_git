#!/usr/bin/env python3
import rospy
import cv2
import apriltag
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import os

class TagAudioOnce:
    def __init__(self):
        rospy.init_node("tag_audio_once")
        self.bridge = CvBridge()
        self.detector = apriltag.Detector(apriltag.DetectorOptions(families='tag36h11'))
        self.image = None
        self.sub = rospy.Subscriber("/camera/color/image_raw", Image, self.callback)
        print("✅ 已订阅相机话题")

    def callback(self, msg):
        try:
            self.image = self.bridge.imgmsg_to_cv2(msg, "bgr8")
        except Exception as e:
            print("图像转换错误:", e)

    def check_tag1(self):
        if self.image is None:
            print("❌ 没有收到相机图像！")
            return False

        # 显示画面（你能直接看到相机在拍什么）
        cv2.imshow("Camera", self.image)
        cv2.waitKey(1)

        gray = cv2.cvtColor(self.image, cv2.COLOR_BGR2GRAY)
        tags = self.detector.detect(gray)

        print(f"🔍 识别到标签数量: {len(tags)}")
        for tag in tags:
            print(f"发现标签 ID: {tag.tag_id}")
            if tag.tag_id == 1:
                return True
        return False

if __name__ == "__main__":
    print("按回车开始检测...")
    input()

    ta = TagAudioOnce()
    rospy.sleep(2.0)  # 等2秒，保证图像进来

    if ta.check_tag1():
        print("✅ 识别到 tag1")
        os.system("aplay /home/bcsh/targetfound.wav")
    else:
        print("❌ 未识别到 tag1")
        os.system("aplay /home/bcsh/targetnotfound.wav")

    cv2.destroyAllWindows()
