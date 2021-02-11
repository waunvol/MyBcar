#include <ros/ros.h>
#include <mycar_control.h>
#include <std_msgs/Int8.h>


using namespace std;
geometry_msgs::Twist cmdvel_;
ros::Subscriber cmdsub;
double L_speed, C_speed;

void cmdCallback(const geometry_msgs::Twist msg)
{
    L_speed = msg.linear.x;
    C_speed = msg.angular.z;
}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "MasterControl");
    ros::NodeHandle n;

    
    cmdsub = n.subscribe("cmd_vel", 10, cmdCallback);

    ros::AsyncSpinner spinner(5);
    BalanceControl mycar;

    ros::Rate r(10);//delay
    int j = 50;     
    while (j)
    {
        j--;
        r.sleep();
    }
    

    mycar.Start();
    spinner.start();

    int count=0;

    while(ros::ok)
    {    
        r.sleep();
        mycar.SetComand(L_speed, C_speed);


        /*//debug section
        cin>>count;
        if(count==1)
        {
            mycar.stopAll();
            count = 0;
        }
        else if(count==2)
        {
            mycar.Start();
            count = 0;
        }
        // mycar.SetComand(count, 1);
        */
    }

    return 0;

}
