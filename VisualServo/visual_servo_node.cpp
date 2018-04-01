#include <iostream>
#include "ros/ros.h"
#include <std_msgs/String.h>
#include <icra_firefly/ArmorDetection.h>
#include <cstring>

#include "MoveControl.h"
#include "AutoPID.h"

#define epsilon 0.1

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
    ROS_INFO("%s", cmd.c_str());

    // 处理图片大小
    int width, height;
    ros::param::param("~width", width, 300);
    ros::param::param("~height", height, 300);
    ROS_INFO("witdh: %d height: %d", width, height);
    // PID 参数
    double Up_Down_P, Up_Down_I, Up_Down_D;
    double Right_Left_P, Right_Left_I, Right_Left_D;
    double ts;

    ros::param::param("~ud_p", Up_Down_P, 0.05);
    ros::param::param("~ud_i", Up_Down_I, 0.005);
    ros::param::param("~ud_d", Up_Down_D, 0.00005);

    ros::param::param("~rl_p", Right_Left_P, 0.05);
    ros::param::param("~rl_i", Right_Left_I, 0.005);
    ros::param::param("~rl_d", Right_Left_D, 0.00005);

    AutoPID PID_u_d = AutoPID(Up_Down_P, Up_Down_I, Up_Down_D, ts/1000.0, height/2);
    AutoPID PID_r_l = AutoPID(Right_Left_P, Right_Left_I, Right_Left_D, ts/1000.0, width/2);

    MoveControl MV;
    PID_u_d.uk = PID_u_d.pid_control(y*width);
	PID_r_l.uk = PID_r_l.pid_control(x*height);
	MV.Up_Down(PID_u_d.uk);
	MV.Right_Left_Rotation(-PID_r_l.uk);
    if(MV.Send_Message() == false) //xia mian de bu fen xu yao xiong huan
	{
		ROS_ERROR("Port Lost");
	}
}

int main(int argc, char *argv[])
{
    printf("move control node start\n");
    // 初始化当前结点visual_servo_node
    ros::init(argc, argv, "visual_servo_node");
    // 创建一个结点句柄
    ros::NodeHandle nh;
    // 订阅 armor_detection 
    ros::Subscriber sub = nh.subscribe("armor_detection", 10, armor_detection_Callback);
    // 进入循环
    ros::spin();
    
    return 0;
}