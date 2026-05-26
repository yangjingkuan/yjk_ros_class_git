gnome-terminal --window -e 'bash -c "roscore; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_bringup bringup_sensor.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_depth_vision view_sensor.launch; exec bash"' \

