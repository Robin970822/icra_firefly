#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <termios.h>
#include <sys/time.h>
#include <linux/serial.h>
#include <sys/ioctl.h>
class AutoPID
{
	private:
		float uk_1;
		float ek;
		float ek_1;
		float ek_2;
	public:
		float kp;
		float ki;
		float kd;
		float ts;
		float _center;
		float uk;
		AutoPID();
		AutoPID(float _kp, float _ki, float _kd, float _ts, float __center):kp(_kp),ki(_ki),kd(_kd),ts(_ts),_center(__center){};
		~AutoPID();
		float pid_control(float y);
};
