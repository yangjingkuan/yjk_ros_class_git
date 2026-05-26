
git clone https://gitee.com/song_gang/upros_class_code.git

cd upros_class_code

rosdepc install --from-paths src --ignore-src --rosdistro=noetic

catkin_make --pkg upros_message

catkin_make