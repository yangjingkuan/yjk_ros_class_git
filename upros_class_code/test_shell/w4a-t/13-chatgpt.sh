gnome-terminal --window -e 'bash -c "roscore; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_bringup bringup_w4a_t.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_chat robot_speaker.launch; exec bash"' \
