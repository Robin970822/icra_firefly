#include <iostream>
#include "ros/ros.h"
#include <std_msgs/String.h>
#include <icra_firefly/ArmorDetection.h>
#include <cstring>


#include "MoveControl.h"

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
    std::cout << cmd;
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