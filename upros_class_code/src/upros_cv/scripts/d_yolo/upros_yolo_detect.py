#!/usr/bin/env python3

import cv2
import time
import socket
import json

from upros_yolo.rknnpool import rknnPoolExecutor
from upros_yolo.func import myFunc

cap = cv2.VideoCapture('http://0.0.0.0:8080/stream?topic=/camera/color/image_raw')

modelPath = "/home/bcsh/Documents/Models/rknnModel/yolov5s_relu_tk2_RK3588_i8.rknn"

cv2.namedWindow("Yolo", cv2.WINDOW_NORMAL)

TPEs = 4  # 线程数, 增大可提高帧率

pool = rknnPoolExecutor(rknnModel=modelPath, TPEs=TPEs, func=myFunc) # 初始化rknn池

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# 自定义词典，可以添加特定词汇以便更好地分词
json_template = {
    "class": -1,
    "center": (0, 0)
}

def send_data_to_server(server_ip, server_port, yolo_data):
    json_data = json.dumps(yolo_data, indent=4, ensure_ascii=False)
    print("\n转换为 JSON 并发送到机器人:\n")
    print(json_data)
    try:
        s.sendto(json_data.encode(), (server_ip, server_port))
    except Exception as e:
        print(f"发送数据时出错: {e}")
           
if __name__ == "__main__":
    server_ip = "localhost" 
    server_port = 12345     

    # 初始化异步所需要的帧
    if (cap.isOpened()):
        for i in range(TPEs + 1):
            ret, frame = cap.read()
            if not ret:
                cap.release()
                del pool
                exit(-1)
            pool.put(frame)
    frames, loopTime, initTime = 0, time.time(), time.time()
    while (True):
        ret, frame = cap.read()
        if not ret:
            break

        pool.put(frame)

        (frame, centers, cls), flag = pool.get()

        if cls is None:
            continue

        yolo_data = [] # 识别结果数组

        # 打包识别结果到json
        for center, cl in zip(centers, cls):
            json_obj = json_template.copy()
            json_obj['class'] = int(cl)
            json_obj['center'] = center
            yolo_data.append(json_obj)            
        
        # 发送到udp服务器
        send_data_to_server(server_ip, server_port, yolo_data)    
        
        if flag == False:
            break
        cv2.imshow("Yolo", frame)
        if cv2.waitKey(30) & 0xFF == ord('q'):
            break
    cap.release()
    pool.release()
