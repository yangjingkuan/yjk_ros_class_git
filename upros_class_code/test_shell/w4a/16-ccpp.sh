gnome-terminal --window -e 'bash -c "roscore; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_bringup bringup_w4a.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_navigation navigation.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_navigation area_path.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_navigation view_nav.launch; exec bash"' \
--tab -e 'bash -c "sleep 13; roslaunch upros_navigation next_goal.launch; exec bash"' \