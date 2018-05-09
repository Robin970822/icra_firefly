#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include <icra_firefly/GimbalControl.h>
#include "MoveControl.h"

#define PI 3.1415926
MoveControl MV;

void CmdVelCallback(const geometry_msgs::Twist& cmd_vel)
{
    if(cmd_vel.linear.z == 0)
    {
        double linear_x = cmd_vel.linear.x;
        double linear_y = cmd_vel.linear.y;
        double angular_z = cmd_vel.angular.z;

        double linear_x_coef;
        double linear_y_coef;
        double angular_z_coef;

        ros::param::param("~linear_x_coef", linear_x_coef, 1.0);
        ros::param::param("~linear_y_coef", linear_y_coef, 0.0);
        ros::param::param("~angular_z_coef", angular_z_coef, 1.0);

        double x = linear_x * linear_x_coef * 1000;
        double y = linear_y * linear_y_coef * 1000;
        double z = angular_z * angular_z_coef * 180.0 / PI;
        ROS_INFO("linear x: %lf linear y: %lf angular z: %lf", x, y, z);

        MV.Forward_Back(x);
        MV.Right_Left(y);
        MV.Right_Left_Rotation(z);
    }

    if(MV.Send_Message() == false)
    {
        ROS_ERROR("Port Lost");
    }
}

void GimbalCallback(const icra_firefly::GimbalControl& gimbal)
{
    double u_d = gimbal.u_d;
    double r_l = gimbal.r_l;

    MV.Up_Down(u_d);
    MV.Right_Left(r_l);

    if(MV.Send_Message() == false)
    {
        ROS_ERROR("Gimbal Lost");
    }    
}

int main(int argc, char **argv)
{
    printf("move control node statr\n");
    //初始化ROS当前节点为car_contr
    ros::init(argc, argv, "move_control_node");
    ros::NodeHandle nh;
    //订阅的是/cmd_vel话题，如果速度不够快的话就会缓存1000条消息，当有新消息到来时，会调用cmd_vel_Callback函数，控制底盘运动
    ros::Subscriber sub_cmd_vel_ = nh.subscribe("cmd_vel", 1000, CmdVelCallback);

    ros::Subscriber sub_gimbal_ctrl = nh.subscribe("gimbal", 1000, GimbalCallback);
    //进入循环，可以尽快调用消息的回调函数，并且可以退出
    ros::spin();

    return 0;

}
