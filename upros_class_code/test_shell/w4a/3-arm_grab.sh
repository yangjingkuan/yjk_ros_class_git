gnome-terminal --window -e 'bash -c "roscore; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_bringup bringup_w4a.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch zoo_arm apriltag_grab.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch zoo_arm show_grab.launch; exec bash"' \
--tab -e 'bash -c "sleep 13; rosrun zoo_arm arm_grab_node; exec bash"' \

