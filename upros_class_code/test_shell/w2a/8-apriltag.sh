gnome-terminal --window -e 'bash -c "roscore; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch upros_bringup bringup_w2a.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; rosrun upros_cv upros_apriltag_detect.py; exec bash"' \
--tab -e 'bash -c "sleep 3; rosrun rqt_image_view rqt_image_view; exec bash"' \