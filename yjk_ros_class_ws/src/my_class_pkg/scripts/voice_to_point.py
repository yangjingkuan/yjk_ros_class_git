#!/usr/bin/env python3
# 语音接收+大模型解析，持续监听不关闭订阅
import rospy
from std_msgs.msg import String
from openai import OpenAI
import threading  # 我只加了这一行

# 大模型密钥配置
api_key = "sk-N5tqbaHsfumGmlViYGy8TmihgzLL9xfbzyYm8giyxbemQUTB"
base_url = "https://api.moonshot.cn/v1"
client = OpenAI(api_key=api_key, base_url=base_url)

pub = None

# 大模型语义解析函数
def llm_analysis(text):
    prompt = f"""
分析用户说出的话语，严格按规则只输出对应字符
话语内容：{text}
1.话语里包含A，只输出字符：A
2.话语里包含B，只输出字符：B
3.话语里包含起点、出发点，只输出字符：HOME
4.无有效导航指令，只输出字符：NONE
不要输出多余汉字、标点符号
"""
    res = client.chat.completions.create(
        model="moonshot-v1-8k",
        messages=[{"role":"user","content":prompt}],
        temperature=0.1
    )
    return res.choices[0].message.content.strip()

# 单独开线程处理解析，避免阻塞主回调
def deal_speech(text):
    result = llm_analysis(text)
    if result == "A":
        rospy.loginfo("解析完成，下发前往A点指令")
        pub.publish("GO_A")
    elif result == "B":
        rospy.loginfo("解析完成，下发前往B点指令")
        pub.publish("GO_B")
    elif result == "HOME":
        rospy.loginfo("解析完成，下发返回起点指令")
        pub.publish("GO_HOME")
    else:
        rospy.loginfo("未识别有效导航指令")

# 语音回调函数
def speech_callback(msg):
    speech_text = msg.data
    rospy.loginfo(f"识别到语音内容：{speech_text}")

    # ======================
    # 我只改了这里！！！
    # 开子线程，不阻塞监听
    # ======================
    threading.Thread(target=deal_speech, args=(speech_text,), daemon=True).start()

if __name__ == "__main__":
    rospy.init_node("voice_analyze_node")
    # 创建指令发布话题
    pub = rospy.Publisher("/nav_control",String,queue_size=10)
    # 持续订阅语音话题
    rospy.Subscriber("/speech/result",String,speech_callback)
    rospy.loginfo("✅语音解析节点启动完毕，持续等待语音输入")
    rospy.spin()
