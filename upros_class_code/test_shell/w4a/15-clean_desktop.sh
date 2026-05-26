gnome-terminal --window -e 'bash -c "roscore; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch carry_robot carry_robot_w4a.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch carry_robot view_grab.launch; exec bash"' \
--tab -e 'bash -c "sleep 15; rosrun carry_robot w4a_task_flow_node; exec bash"' \