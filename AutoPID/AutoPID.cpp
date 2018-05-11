#include "AutoPID.h"
AutoPID::AutoPID()
{
	kp = 0.0;
	ki = 0.0;
	kd = 0.0;
	ts = 0.0;
	uk = 0.0;
	uk_1 = 0.0;
	ek = 0.0;
	ek_1 = 0.0;
	ek_2 = 0.0;
	_center = 0.0;
}
AutoPID::~AutoPID()
{
}
float AutoPID::pid_control(float y)
{
	ek = 0.5 - y;
	uk = uk_1 + (kp + ki * ts + kd / ts) * ek - (kp + 2 * kd / ts) * ek_1 + kd * ek_2 / ts;
	uk_1 = uk;
	ek_1 = ek;
	ek_2 = ek_1;
	return uk;
}
