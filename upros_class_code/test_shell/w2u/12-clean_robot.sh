gnome-terminal --window -e 'bash -c "roscore; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_bringup bringup_w2u.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch w2u_navigation navigation.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_core_move core_move.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_navigation view_nav.launch; exec bash"' \
--tab -e 'bash -c "sleep 10; rosrun clean_robot core_move_client; exec bash"' \