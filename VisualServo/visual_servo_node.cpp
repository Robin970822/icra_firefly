#include <iostream>
#include "ros/ros.h"
#include <std_msgs/String.h>
#include <icra_firefly/ArmorDetection.h>
#include <cstring>

#include "MoveControl.h"
#include "AutoPID.h"

#define epsilon 0.1

MoveControl MV;
AutoPID PID_u_d = AutoPID(0.05, 0.005, 0.00005, 1.0/1000.0, 240);
AutoPID PID_r_l = AutoPID(0.05, 0.005, 0.00005, 1.0/1000.0, 320);

// 回调函数
void armor_detection_Callback(const icra_firefly::ArmorDetection& armor_detection)
{
    std::string cmd = "";
    cmd += armor_detection.kind;
    float x = (armor_detection.x1 + armor_detection.x2) / 2;
    float y = (armor_detection.y1 + armor_detection.y2) / 2;
    if (x < 0.5 - epsilon)
    {
        cmd += "\tleft\n";
    }
    if (x > 0.5 + epsilon)
    {
        cmd += "\tright\n";
    }
    if (0.5 - epsilon <= x && x <= 0.5 + epsilon)
    {
        cmd += "\tshoot\n";
    }
    std::cout << cmd;
    PID_u_d.uk=PID_u_d.pid_control(y*240);
	PID_r_l.uk=PID_r_l.pid_control(x*320);
	MV.Up_Down(PID_u_d.uk);
	MV.Right_Left_Rotation(-PID_r_l.uk);
    if(MV.Send_Message()==false) //xia mian de bu fen xu yao xiong huan
	{
		printf("\nPort Lost \n");
	}
}

int main(int argc, char *argv[])
{
    printf("move control node start\n");
    // 初始化当前结点visual_servo_node
    ros::init(argc, argv, "visual_servo");
    // 创建一个结点句柄
    ros::NodeHandle nh;
    // 订阅 armor_detection 
    ros::Subscriber sub = nh.subscribe("armor_detection", 10, armor_detection_Callback);
    // 进入循环
    ros::spin();
    
    return 0;
}