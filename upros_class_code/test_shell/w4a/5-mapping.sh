gnome-terminal --window -e 'bash -c "roscore; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_bringup bringup_w4a.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_navigation gmapping.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_navigation view_nav.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; rosrun upros_move_linear teleop_twist_keyboard.py; exec bash"' \
