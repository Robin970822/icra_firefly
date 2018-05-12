#include "visual_servo_node.h"

VisualServo::VisualServo()
{
    gim_ctrl_pub = nh.advertise<icra_firefly::GimbalControl>("gimbal", 30);

    // 处理图片大小
    ros::param::param("~width", width, 300.0);
    ros::param::param("~height", height, 300.0);
    ROS_INFO("width: %lf height: %lf", width, height);
    // PID 参数
    double Up_Down_P, Up_Down_I, Up_Down_D;
    double Right_Left_P, Right_Left_I, Right_Left_D;
    double ts;

    ros::param::param("~ud_p", Up_Down_P, 0.005);
    ros::param::param("~ud_i", Up_Down_I, 0.0005);
    ros::param::param("~ud_d", Up_Down_D, 0.000005);

    ros::param::param("~rl_p", Right_Left_P, 0.005);
    ros::param::param("~rl_i", Right_Left_I, 0.0005);
    ros::param::param("~rl_d", Right_Left_D, 0.000005);

    ros::param::param("~ts", ts, 100.0);
    ROS_INFO("ts: %lf", ts);

    PID_u_d = AutoPID(Up_Down_P, Up_Down_I, Up_Down_D, ts / 1000.0, 0.5);
    PID_r_l = AutoPID(Right_Left_P, Right_Left_I, Right_Left_D, ts / 1000.0, 0.5);

    std::string portName;
    int baud;

    ros::param::param("~baud", baud, 100000);
    if (!ros::param::get("~serial_name", portName))
    {
        portName = "/dev/ttyUSB0";
    }

    ROS_INFO("serial_name: %s baud: %d", portName.c_str(), baud);
    //MV = MoveControl(portName, baud);
}

VisualServo::~VisualServo() {}

// 回调函数
void VisualServo::ArmorDetectionCallback(const icra_firefly::ArmorDetection &armor_detection)
{
    std::string cmd = "";
    cmd += armor_detection.kind;
    float x = (armor_detection.x1 + armor_detection.x2) / 2.0;
    float y = (armor_detection.y1 + armor_detection.y2) / 2.0;
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
    //ROS_INFO("%s", cmd.c_str());

    if (armor_detection.kind == "Armor")
    {
        PID_u_d.uk = PID_u_d.pid_control(y);
        PID_r_l.uk = PID_r_l.pid_control(x);
        ROS_INFO("UD:%lf RL:%lf x:%lf y:%lf", PID_u_d.uk, PID_r_l.uk, x, y);
        ROS_INFO("Width:%lf Height:%lf", width, height);
        ROS_INFO("P:%lf I:%lf D:%lf", PID_u_d.kp, PID_u_d.ki, PID_u_d.kd);

        icra_firefly::GimbalControl gimbal;
        gimbal.pitch = PID_u_d.uk;
        gimbal.yaw = PID_r_l.uk;
        gim_ctrl_pub.publish(gimbal);
        // MV.Up_Down(PID_u_d.uk);
        // MV.Right_Left_Rotation(-PID_r_l.uk);
    }

    // if(MV.Send_Message() == false) //
    // {
    // 	ROS_ERROR("Port Lost");
    // }
}
// Run函数
void VisualServo::Run()
{
    // 订阅 armor_detection
    sub_armor_detection = nh.subscribe("armor_detection", 10, &VisualServo::ArmorDetectionCallback, this);
    // 开始循环
    ros::spin();
}

int main(int argc, char *argv[])
{
    printf("visual servo node start\n");
    // 初始化当前结点visual_servo_node
    ros::init(argc, argv, "visual_servo_node");

    VisualServo visualServo;

    visualServo.Run();

    return 0;
}