#include <iostream>
#include "ros/ros.h"
#include <std_msgs/String.h>
#include <icra_firefly/ArmorDetection.h>
#include <cstring>

#include "MoveControl.h"
#include "AutoPID.h"

#define epsilon 0.1

class VisualServo{
    public:
    VisualServo();
    ~VisualServo();
    void armor_detection_Callback(const icra_firefly::ArmorDetection& armor_detection);

    private:
    MoveControl MV;
    AutoPID PID_u_d;
    AutoPID PID_r_l;
    double width;
    double height;
};