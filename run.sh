echo "FUCK WangXiong!"

sleep 1
gnome-terminal --window -e 'bash -c "roslaunch icra_firelfy visual_servo.launch; exec bash" '
sleep 10
gnome-terminal --window -e 'bash -c "roslaunch icra_firefly armor_detection.launch; exec bash" ‘


echo "WangXiong Funcked!"