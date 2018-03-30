#include "SerialPort.h"

SerialPort::SerialPort()
{
	fd=-1;
	portname=NULL;
}

SerialPort::~SerialPort()
{
	free(portname);
}

bool SerialPort::init_8n1(const char portName[])
{
	int flag=1;
	fd=-1;
 	if(portname!=NULL){
        	free(portname);
        	portName=NULL;
    	}

	portname=(char*)malloc((strlen(portName)+1)*sizeof(char));
	strcpy(portname,portName);

	fd=open(portName,O_RDWR|O_NOCTTY|O_NDELAY) ;
    	if(fd==-1)
    		flag=-1;

    	struct  termios options;
    	tcgetattr(fd,&options);
	options.c_cflag = B38400|CLOCAL|CREAD;
	options.c_cflag &=~CSIZE;
 	options.c_cflag |=CS8;
	options.c_cflag |=PARENB;
	options.c_cflag &=~PARODD;
	options.c_cflag |=INPCK;
	options.c_cflag &=~CSTOPB;
	cfsetispeed(&options,B38400);
    	cfsetospeed(&options,B38400);
	tcflush(fd,TCIFLUSH);
	tcsetattr(fd,TCSANOW,&options);
	
 	options.c_iflag=IGNPAR;
	options.c_oflag=0;
	options.c_lflag=0;
	options.c_cc[VTIME]=0;
	options.c_cc[VMIN]=0;

    	close(fd);
	if(flag==1){
    		return true;
	}
	else
	{
		return false;
	}
    		
}

bool SerialPort::openPort(int speed)
{
    	fd=-1;
	int flag=1;
    	fd=open(portname,O_RDWR|O_NOCTTY|O_NDELAY) ;
	
	struct serial_struct ss,ss_set;
	if(ioctl(fd,TIOCGSERIAL,&ss)!=0)
		flag=-1;
	ss.flags |=ASYNC_SPD_CUST;
	//ss.flags &=~ASYNC_SPD_MASK;
	ss.custom_divisor=ss.baud_base/speed;
	if(ioctl(fd,TIOCSSERIAL,&ss)!=0)
		flag=-1;
	if(ioctl(fd,TIOCGSERIAL,&ss_set)!=0)
		flag=-1;  
    	if(fd==-1)
    	{
        	return false;
    	}
    	if(flag==1)
    	{
    		fprintf(stderr,"\nPort %s Open Succeed and Baudrate is Set at %d!\n","USB0",ss_set.baud_base/ss_set.custom_divisor);
    		return true;
    	}
    	else
    	{	
    		fprintf(stderr, "\nOpen Faild, Please Check Hardware Connections\n");
    		return false;
    	}
}

bool SerialPort::closePort()
{
	if(fd==-1) return false;
    	close(fd);
    	return true;
}

void SerialPort::clearPort()
{
	tcflush(fd,TCIFLUSH);
}

int SerialPort::readData(char* rxbuff, int maxlength)
{
	int rx_length=0;
    	if(fd==-1)
    	{
		perror("Port is not open !\n") ;
		return 0;
    	}
	rx_length=read(fd,rxbuff,maxlength);
	if(rx_length<0)   perror("read failed!\n");
	return rx_length;
}

int SerialPort::writeData(const char *send_buf, int data_len)
{
	if(fd==-1)
	{
		perror("Port is not open !\n") ;
		return 0;
	}
	int ret;
	ret = write(fd,send_buf,data_len);
	if (data_len == ret ){
		return 1;
		} 
	else {
		tcflush(fd,TCOFLUSH);
		return -1;
	}
}

bool SerialPort::isOpen()
{
	if(fd==-1) return false;
	else return true;
}

char SerialPort::getlow(int str)
{
	char low;
	low=str;
	return low;
}

char SerialPort::gethigh(int str)
{
	char high;
	high=(str>>8);
	return high;
}
