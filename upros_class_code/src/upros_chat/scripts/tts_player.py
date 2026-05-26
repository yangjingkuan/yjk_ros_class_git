#!/usr/bin/env python3
import argparse
import queue
import sys
import threading
import time
import subprocess
import sherpa_onnx
import soundfile as sf
import rospy
from std_msgs.msg import String

MODEL_ROOT = "/home/bcsh/Documents/Models/vits-zh-hf-fanchen-C"
VITS_MODEL = MODEL_ROOT + "/vits-zh-hf-fanchen-C.onnx"
VITS_LEXICON = MODEL_ROOT + "/lexicon.txt"
VITS_TOKENS = MODEL_ROOT + "/tokens.txt"
DICT_DIR = MODEL_ROOT + "/dict"
TTS_RULE_FSTS = MODEL_ROOT + "/number.fst"

current_path = sys.path[0]

defualt_args = argparse.Namespace(
    vits_model=VITS_MODEL,
    vits_lexicon=VITS_LEXICON,
    vits_tokens=VITS_TOKENS,
    vits_length_scale=1.0,
    vits_dict_dir=DICT_DIR,
    tts_rule_fsts=TTS_RULE_FSTS,
    output_filename=current_path + "generated.wav",
    sid=100,
    debug=False,
    provider="cpu",
    num_threads=1,
    speed=1,
)

class TTSPlayer:
    def __init__(self, args=defualt_args):
        # 初始化ros节点
        rospy.init_node('robot_voice_tts_node', anonymous=True)
        self.args = args
        self.buffer = queue.Queue()
        self.started = False
        self.stopped = False
        self.killed = False
        self.sample_rate = None
        self.event = threading.Event()
        self.first_message_time = None
        self.tts_config = self.generate_self_config()
        self.load_model()
        rospy.Subscriber("/talk", String, self.speech_result_callback) # 接收此主题的消息后，合成语音并播放

    def speech_result_callback(self, msg):
        result = msg.data
        if result:
            self.generate_and_play(result)

    def reset_flags_and_event(self):
        self.started = False
        self.stopped = False
        self.killed = False
        self.event.clear()

    def load_model(self):
        if not self.tts_config.validate():
            raise ValueError("Please check your config")
        print("Loading model ...")
        self.tts = sherpa_onnx.OfflineTts(self.tts_config)
        self.sample_rate = self.tts.sample_rate
        print("Model loaded.")

    def generate_self_config(self):
        tts_config = sherpa_onnx.OfflineTtsConfig(
            model=sherpa_onnx.OfflineTtsModelConfig(
                vits=sherpa_onnx.OfflineTtsVitsModelConfig(
                    model=self.args.vits_model,
                    lexicon=self.args.vits_lexicon,
                    dict_dir=self.args.vits_dict_dir,
                    tokens=self.args.vits_tokens,
                ),
                provider=self.args.provider,
                debug=self.args.debug,
                num_threads=self.args.num_threads,
            ),
            rule_fsts=self.args.tts_rule_fsts,
        )
        return tts_config

    def generate_and_play(self, text):
        self.reset_flags_and_event()
        if not self.tts_config.validate():
            raise ValueError("请检查你的配置文件")
        print("开始生成语音 ...")
        audio = self.tts.generate(text,sid=self.args.sid,speed=self.args.speed)
        print("生成语音结束 ...")
        self.stopped = True
        if len(audio.samples) == 0:
            print("语音生成错误")
            self.killed = True
            return
        sf.write(self.args.output_filename,audio.samples,samplerate=audio.sample_rate,subtype="PCM_16")
        print("保存文件完成")
        time.sleep(1.0)
        subprocess.run(["aplay", self.args.output_filename])
        print("可以继续了 ...")

if __name__ == "__main__":
    try:
        ttsplayer = TTSPlayer()
        rospy.spin()
    except KeyboardInterrupt:
        print("\nCaught Ctrl + C. Exiting")
    
    
