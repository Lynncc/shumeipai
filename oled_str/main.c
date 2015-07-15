#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <wiringPi.h>
#include "oled.h"
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_LENTH 128

int get_ip(unsigned char *__ip_buf);

// unsigned char x_pos;
// unsigned char y_pos;
// unsigned char str_buf[BUF_LENTH];


int main(int argc, char const *argv[])
{


	// unsigned char str_buf [BUF_LENTH];
	unsigned char x_pos,y_pos;
	if (argc != 4)
		return -1;
	if (strlen(argv[3]) > BUF_LENTH)
		return -1;

	x_pos = atoi(argv[1]);
	y_pos = atoi(argv[2]);
	printf("%s\n", argv[3]);

	I2C_Configuration();


	// OLED_Init();
	OLED_ON();
	// OLED_Fill(0x00);
	OLED_Line(y_pos,0);
	OLED_Line(y_pos+1,0);
	cycle_show(x_pos, y_pos, (unsigned char *)argv[3]);

  	return 0;
}



int get_opt(int argc, char *argv[])
{
	int c;
	int ch_time = 0;

	// signal(SIGINT, sig_handler);
	if (argc == 1)
	{
		// usage();
		goto out;
	}


out:
	return -1;
}
