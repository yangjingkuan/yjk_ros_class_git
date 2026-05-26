gnome-terminal --window -e 'bash -c "roscore; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_bringup bringup_w4a_t.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch clean_desktop_robot recognize_apriltag.launch; exec bash"' \
--tab -e 'bash -c "sleep 10; roslaunch clean_desktop_robot arm_grab.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch clean_desktop_robot view_grab.launch; exec bash"' \
