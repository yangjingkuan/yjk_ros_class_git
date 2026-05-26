gnome-terminal --window -e 'bash -c "roscore; exec bash"' \
--tab -e 'bash -c "sleep 3; rosrun upros_slam clear_area_node; exec bash"' \

