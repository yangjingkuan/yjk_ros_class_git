#!/bin/bash

# 强制删除旧规则文件（避免不存在时报错）
sudo rm -f /etc/udev/rules.d/w4a.rules /etc/udev/rules.d/zyzx.rules

# 复制新规则文件到目标目录
sudo cp /home/bcsh/upros_class_code/src/upros_bringup/rules/w4a.rules /etc/udev/rules.d/

# 重新加载udev规则（确保新规则生效）
sudo udevadm control --reload-rules

# 触发设备事件处理（立即应用新规则）
sudo udevadm trigger