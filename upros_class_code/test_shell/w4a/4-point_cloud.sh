gnome-terminal --window -e 'bash -c "roscore; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_bringup bringup_w4a.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; rosrun upros_depth_vision upros_point_filter_node; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_depth_vision view_pcl.launch; exec bash"' \

