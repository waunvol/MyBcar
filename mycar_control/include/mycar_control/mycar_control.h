#ifndef MYCCAR_CONTROL_H
#define MYCCAR_CONTROL_H

#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <karman.h>
#include <sensor_msgs/Imu.h>
#include <tf/transform_datatypes.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Vector3.h>

//change your wheels'controllers topic name here
#define LeftWheelTopic "Lwheel_controller/command";
#define RightWheelTopic "Rwheel_controller/command";


class BalanceControl
{
private:
    /* data */
    double A_s=1, B_s=0; //stand up pid
    double A_r=1, B_r=0,I_r=0;
    double A_t=1;
    double a_x,a_y,a_z; //three directions of the acceleration
    double w_x,w_y,w_z; //three directions of the angular velocity
    double roll, pitch, yaw;
    double speed, speed_ctl;//speed out(now)
    double acceleration_s, acceleration_r;  //acceleration for stand up loops(_s) and speed loops(_r)
    double LinearSpeed = 0, CorneringSpeed = 0; //target speed

    //geometry_msgs::Twist Ctrl_msgs;             //external control messages
    std_msgs::Float64 Lwheel_msg, Rwheel_msg, angle_msg;   //left wheel control message& right wheel control message

    ros::NodeHandle n;
    ros::Subscriber ImuSub;     
    ros::Timer T1, T2;

    ros::Publisher Lwheel_ctl;  //left wheel control topic
    ros::Publisher Rwheel_ctl;  //right wheel control topic

    void GetImuValue(const sensor_msgs::Imu::ConstPtr& msg); //call back funtion of IMU subscriber
    void T1trigger(const ros::TimerEvent&);
    void T2trigger(const ros::TimerEvent&);

    /*====================*/
    //debug section
    /*====================*/
    geometry_msgs::Vector3 status_msgs;
    ros::Publisher anglepub;
    ros::Publisher statepub;


public:
    BalanceControl(double r1=0.01, double r2=0.002);
    ~BalanceControl();

    void Start();
    void SetComand(double Linear, double Cornering=0)
    {
        LinearSpeed = -Linear;
        CorneringSpeed = Cornering;
    };
    void ShowRPY(){ROS_INFO("RPY is:%f, %f, %f", roll, pitch, yaw);};  
    void SetPID();
    void stopAll(); 

};


















#endif