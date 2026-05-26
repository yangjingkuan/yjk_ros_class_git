gnome-terminal --window -e 'bash -c "roscore; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_bringup bringup_w4a.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; rosrun upros_odometry ros_odom_node; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_odometry view_odom.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; rosrun upros_move_linear teleop_twist_keyboard.py; exec bash"' \
