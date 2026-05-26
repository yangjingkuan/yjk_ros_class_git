#!/usr/bin/env python3

import jieba
import re

class Tokenizer:
        
    # 定义停用词表
    stopwords = set(['的', '是', '啊'])

    # 自定义词典，可以添加特定词汇以便更好地分词
    json_template = {
        "intent": "",
        "target": -1
    }

    # 中文文字对应数字的字典
    chinese_to_arabic = {
        '零': 0,
        '一': 1,
        '二': 2,
        '三': 3,
        '四': 4,
        '五': 5,
        '六': 6,
        '七': 7,
        '八': 8,
        '九': 9,
        '十': 10,
        '百': 100,
        '千': 1000,
        '万': 10000,
        '亿': 100000000
    }

    # 水果中英文对照字典
    fruit_name_mapping = {
        '苹果': 'apple',
        '香蕉': 'banana',
        '橙子': 'orange',
        '橘子': 'orange',
        '草莓': 'strawberry',
        '西瓜': 'watermelon',
        '菠萝': 'pineapple',
    }

    def __init__(self):
        print("Init Tokenizer!!!!")
        
    # 通过字典从汉字中提取数字
    def chinese_to_arabic_number(self, chinese_num):
        total = 0
        r = 1
        for i in range(len(chinese_num) - 1, -1, -1):
            val = self.chinese_to_arabic[chinese_num[i]]
            if val == 10000 or val == 100000000:
                if r < val:
                    r *= val
                else:
                    r //= val
            else:
                total += r * val
        return total

    # 将代表数字的中文汉字替换成阿拉伯数字方便程序处理
    def replace_chinese_numbers(self, text):
        # 查找所有包含中文大写数字的子字符串
        pattern = re.compile(r'零?[一二三四五六七八九十百千万亿]+')
        matches = pattern.findall(text)
        for match in matches:
            arabic_number = self.chinese_to_arabic_number(match)
            text = text.replace(match, str(arabic_number))   
        return text

    # 命令字符串预处理，去除无用的字符和数字
    def pre_process(self, text):
        # 去除无用字符和数字
        text = re.sub(r'\s+', '', text)
        text = self.replace_chinese_numbers(text)
        text = text.lower()
        tokens = list(jieba.lcut(text))
        # 去除停用词
        filtered_tokens = [token for token in tokens if token not in self.stopwords]
        # 替换中文水果名称到对应的英文
        for i, token in enumerate(filtered_tokens):
            if token in self.fruit_name_mapping:
                filtered_tokens[i] = self.fruit_name_mapping[token]      
        return filtered_tokens

    # 从字符串命令中提取意图和ID，打包成Json命令
    def extract_intent(self, tokens):
        intents = {
            'go_to': ['移动', '去', '前往'],
            'pick': ['抓取', '拿起', '抓', '拿', '取'],
            'release': ['放下', '放到', '放', '放置']
        }
        result = []
        i = 0
        while i < len(tokens):
            for intent, triggers in intents.items():
                # 检查当前词元是否是某个意图的触发词
                if tokens[i] in triggers:
                    # 跳过非数字和“号”的词元，寻找目标编号
                    j = i + 1
                    while j < len(tokens) and not (tokens[j].isdigit() or tokens[j] == '号'):
                        j += 1
                    # 提取目标编号（第一个遇到的数字）
                    target_index = -1
                    if j < len(tokens):
                        for k in range(j, len(tokens)):
                            if tokens[k].isdigit():
                                target_index = int(tokens[k])
                                break
                    # 生成 JSON 对象并添加到结果
                    json_obj = self.json_template.copy()
                    json_obj['intent'] = intent
                    json_obj['target'] = target_index
                    result.append(json_obj)
                    # 跳过已处理的词元，继续后续处理
                    i = j  
                    break
            else:
                i += 1
        return result

    # 从字符串命令中提取意图和数字
    def get_intent_from_text(self, text):
        filtered_input = self.pre_process(text)
        intents = self.extract_intent(filtered_input)
        return intents


if __name__ == '__main__':
    tokenizer = Tokenizer()
    user_input = input("请输入指令: ")
    filtered_input = tokenizer.pre_process(user_input) # 预处理
    intent_string = tokenizer.extract_intent(filtered_input) # 提取意图加指令组合
    print("Output:", intent_string)
