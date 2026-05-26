gnome-terminal --window -e 'bash -c "roscore; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_bringup bringup_w2a.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_chat robot_speaker.launch; exec bash"' \
