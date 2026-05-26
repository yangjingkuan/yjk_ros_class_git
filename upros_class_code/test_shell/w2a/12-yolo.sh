gnome-terminal --window -e 'bash -c "roscore; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_bringup bringup_w2a.launch; exec bash"' \
--tab -e 'bash -c "sleep 10; roslaunch upros_cv yolo_detect.launch; exec bash"' \
