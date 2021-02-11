# MyBcar
## A self-balancing vehicle simulation by GAZEBO.
  Enter `roslaunch mycar_control Mycar.launch` to launch the model with control program.
  
    > Edit `MyBcar/mycar_control/config/PID.yaml` to setup default PID.
  
  Enter `roslaunch key_board keyboard.launch` to launch the Node to control the vehicle in GAZEBO(forward, backward, turn, speed up).
  
    > This package was provide by [here](https://github.com/wanghuohuo0716/key_board) . I made some changes on this basis.
  
  You can also control the car in other ways.
  
    > Control message should be `geometry_msgs::Twist`.
    
    > Control topic name should be `cmd_vel`.
    
