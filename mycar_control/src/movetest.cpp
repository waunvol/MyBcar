#include <ros/ros.h>
#include <mycar_control.h>
#include <std_msgs/Int8.h>


using namespace std;
std_msgs::Int8 cmd;
ros::Publisher cmdpub;

int main(int argc, char** argv)
{
    ros::init(argc, argv, "setup");
    ros::NodeHandle nh;
    ROS_INFO("Node has been launched!");

    cmdpub = nh.advertise<std_msgs::Int8>("cmd", 5);
    
    

    while (ros::ok())
    {
        ROS_INFO("Please enter you command!");
        //1 for stop, 2 for set PID, 3 for restart
        int temp;
        cin >> temp;
        cmd.data = temp;
        cmdpub.publish(cmd);
    }
    

    // ros::waitForShutdown();
    // ros::shutdown();
    return 0;

}