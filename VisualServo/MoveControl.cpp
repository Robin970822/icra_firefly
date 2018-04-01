#include "MoveControl.h"
#define UP 1
#define DOWN 2
#define MIDDLE 3

MoveControl::MoveControl()
{
	flag=false;
	char portName[20]="/dev/ttyUSB0";
	int baud=100000;
	if(SP.init_8n1(portName)&&SP.openPort(baud)==true)
	{
		printf("\nSystem is Online\n" );	
		flag=true;
	}
	setbuf(stdin,NULL);
	CH0=1024;
	CH1=1024;
	CH2=1024;
	CH3=1024;
	S1=3;
	S2=1;
}

MoveControl::~MoveControl()
{
	SP.closePort();
}


bool MoveControl::Attack_Mode_Change()
{
	for(int i=1;i<=3;i++)
	{
		S1=MIDDLE;
		Send_Message();
		usleep(14*1000);
	}	
	for(int i=1;i<=3;i++)
	{
		S1=UP;
		Send_Message();
		usleep(14*1000);
	}
	for(int i=1;i<=3;i++)
	{
		S1=MIDDLE;
		Send_Message();
		usleep(14*1000);
	}
}

bool MoveControl::Attack(int cnt)
{
	Attack_Mode_Change(); 
	for(int j=1;j<=cnt;j++)
	{	
		for(int i=1;i<=3;i++)
		{
			S1=MIDDLE;
			Send_Message();
			usleep(14*1000);
		}
		for(int i=1;i<=10;i++)
		{
			S1=DOWN;
			Send_Message();
			usleep(14*1000);
		}
		for(int i=1;i<=3;i++)
		{
			S1=MIDDLE;
			Send_Message();
			usleep(30*1000);
		}
	}
	Attack_Mode_Change(); 
}


bool MoveControl::Forward_Back(float f_b)
{
	if(f_b>=100.0) f_b=100.0;
	if(f_b<=-100.0) f_b=-100.0;
	CH1=1024+(f_b/100.0)*660;
}

bool MoveControl::Right_Left(float r_l)
{
	if(r_l>=100.0) r_l=100.0;
	if(r_l<=-100.0) r_l=-100.0;
	CH0=1024+(r_l/100.0)*660;
}

bool MoveControl::Up_Down(float u_d)
{
	if(u_d>=100.0) u_d=100.0;
	if(u_d<=-100.0) u_d=-100.0;
	CH3=1024+(u_d/100.0)*660;
}

bool MoveControl::Right_Left_Rotation(float r_l_R)
{
	if(r_l_R>=100.0) r_l_R=100.0;
	if(r_l_R<=-100.0) r_l_R=-100.0;
	CH2=1024+(r_l_R/100.0)*660;
}

bool MoveControl::Send_Message()
{
	char sendstr[18];
	for (int i=6;i<=17;i++)
	{
	sendstr[i] = 0b00000000;	
	}
	sendstr[0]=CH0;
	sendstr[1]=(CH0>>8)+(CH1<<3);
	sendstr[2]=(CH1>>5)+(CH2<<6);
	sendstr[3]=CH2>>2;
	sendstr[4]=(CH2>>10)+(CH3<<1);
	sendstr[5]=(CH3>>7)+(S1<<6)+(S2<<4);
	SP.writeData(sendstr,18);
}
