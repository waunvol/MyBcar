# This package (c + +) can send the specified topic by reading the keyboard keys "W", "a", "s" and "d". This package can be used to simply control the robot movement

Topic name is ` CMD_vel ` The data type is ` geometry '_ Msgs:: twist `, which controls the **speed of the robot**

# 1. Compile and install

After clone this warehouse to `catkin_ws/src`, return to ` catkin_ Ws ` working path, execute the compile command




Note: if it prompts that the 'boost' Library cannot be found and needs to be installed. Just `sudo apt-get install boost`

# 2. Operation

Run: ` rosrun keyboard_ node`

# 3. Use

Select the terminal of the running node with the mouse and press the "W", "a", "s" and "d" keys to control the forward and backward left and right movement (Shift + W "will accelerate)



**If you want the robot to move all the time, you can send the command circularly by pressing the key, and if you release it, the robot will stop**

# 4. Code mechanism

**When the key is pressed, the topic will be published. When the key is not pressed, the topic will not be published**




After pressing the button, the topic will be sent twice. The first time is the set speed, and the second time is to set the speed to 0 and send it again, so as to avoid that the robot is always at the set speed
