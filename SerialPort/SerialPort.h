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
class SerialPort
{
  public:
	SerialPort();
	~SerialPort();
	bool init_8n1(const char portName[]);
	bool openPort(int speed);
	bool closePort();
	int writeData(const char *send_buf, int data_len);
	int readData(char *rxbuff, int maxlength);
	void clearPort();
	bool isOpen();
	char getlow(int str);
	char gethigh(int str);

  private:
	int fd;
	char *portname;
};
