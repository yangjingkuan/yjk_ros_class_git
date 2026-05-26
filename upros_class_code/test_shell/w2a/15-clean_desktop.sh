gnome-terminal --window -e 'bash -c "roscore; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch clean_desktop_robot clean_desktop_robot_w2a.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch clean_desktop_robot view_grab.launch; exec bash"' \
--tab -e 'bash -c "sleep 15; rosrun clean_desktop_robot task_flow_node; exec bash"' \