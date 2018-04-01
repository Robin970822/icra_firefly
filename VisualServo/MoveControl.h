#include "SerialPort.h"

class MoveControl
{
	public:
		MoveControl();
		~MoveControl();

		bool flag;
		int CH0;
		int CH1;
		int CH2;
		int CH3;
		int S1;
		int S2;

		bool Send_Message();
		bool Attack_Mode_Change();
		bool Attack(int n);
		bool Forward_Back(int f_b);
		bool Right_Left(int r_l);
		bool Up_Down(int u_d);
		bool Right_Left_Rotation(int r_l_R);
	private:
		SerialPort SP;		
};