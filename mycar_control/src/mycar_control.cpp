#include <mycar_control.h>

BalanceControl::BalanceControl(double r1, double r2)
{

    ImuSub = n.subscribe("imu", 10, &BalanceControl::GetImuValue, this);
    ROS_INFO("Now start to listening the IMU");
    std::string temp = LeftWheelTopic;
    Lwheel_ctl = n.advertise<std_msgs::Float64>(temp,5);
    temp = RightWheelTopic;
    Rwheel_ctl = n.advertise<std_msgs::Float64>(temp,5);
    
    T1 = n.createTimer(ros::Duration(r1), &BalanceControl::T1trigger, this, false, false);
    T2 = n.createTimer(ros::Duration(r2), &BalanceControl::T2trigger, this, false, false);

    /*debug section*/
    anglepub = n.advertise<std_msgs::Float64>("angle",5);
    statepub = n.advertise<geometry_msgs::Vector3>("status",5);
}

BalanceControl::~BalanceControl()
{
}

void BalanceControl::GetImuValue(const sensor_msgs::Imu::ConstPtr& msg)
{
    a_x = msg->linear_acceleration.x;
    a_y = msg->linear_acceleration.y;
    a_z = msg->linear_acceleration.z;
    w_x = msg->angular_velocity.x;
    w_y = msg->angular_velocity.y;
    w_z = msg->angular_velocity.z;

    //got imu's roll, pitch, yaw
    tf::Quaternion q;
    tf::quaternionMsgToTF(msg->orientation, q);
    tf::Matrix3x3(q).getRPY(roll, pitch, yaw);
    
    // ROS_INFO("RPY is:%f, %f, %f", roll, pitch, yaw);
}


void BalanceControl::Start()
{

    ROS_INFO("Let's go!!!");
    SetPID();   //PID parameter loading

    //return to zero
    Lwheel_msg.data = 0;
    Rwheel_msg.data = 0;
    speed = 0;
    acceleration_s = 0;
    acceleration_r = 0;
    I_r = 0;

    T1.start();
    T2.start();

}

void BalanceControl::stopAll()
{
    ROS_INFO("Stop!!!");
    T1.stop();
    T2.stop();

    Lwheel_msg.data = 0;
    Rwheel_msg.data = 0;

    Lwheel_ctl.publish(Lwheel_msg);
    Rwheel_ctl.publish(Rwheel_msg);
}

void BalanceControl::SetPID()
{
    //get param from rosparam
    ros::param::get("coefficient/A_s", A_s);
    ros::param::get("coefficient/B_s", B_s);
    ros::param::get("coefficient/A_r", A_r);
    ros::param::get("coefficient/B_r", B_r);
    ros::param::get("coefficient/A_t", A_t);

    ROS_INFO("A_s: %f, B_s :%f, A_r: %f, B_r: %f, A_t: %f!!!", A_s, B_s, A_r, B_r, A_t);
}

void BalanceControl::T1trigger(const ros::TimerEvent&)
{
    speed = (Lwheel_msg.data + Rwheel_msg.data)/2;    //update speed value(now)
    
    //stand up
    acceleration_s = A_s*roll + B_s*w_x;
    acceleration_s *= 0.90; //weighting
    acceleration_s /= 5;

    //speed control
    I_r += LinearSpeed - speed; //integral

    acceleration_r = A_r*(LinearSpeed - speed) + B_r*I_r;
    acceleration_r *= 1 - 0.90; //weighting
    acceleration_r /= -5;
}

void BalanceControl::T2trigger(const ros::TimerEvent&)
{
    speed_ctl += acceleration_s + acceleration_r;
    Lwheel_msg.data = speed_ctl + A_t*CorneringSpeed;
    Rwheel_msg.data = speed_ctl - A_t*CorneringSpeed;

    Lwheel_ctl.publish(Lwheel_msg);
    Rwheel_ctl.publish(Rwheel_msg);

   /*debug section*/   
    angle_msg.data = roll;
    status_msgs.x = speed;
    status_msgs.y = acceleration_r;
    status_msgs.z = acceleration_s;

    anglepub.publish(angle_msg);
    statepub.publish(status_msgs);
}

