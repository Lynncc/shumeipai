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

#define BUF_LENTH 1024


int main(int argc, char const *argv[])
{


	unsigned char str_buf [BUF_LENTH];
	int i;
	FILE *fd;

	I2C_Configuration();


	// OLED_Init();
	OLED_ON();
	OLED_Fill(0x00);
	fd =file_init(IMAGE_PATH);
play:
	for(i=0;i<6566;i++)
	{

		while(GetImage(fd, str_buf, i) != 0)
			;
		OLED_DrawBMP(0,0,127,7,str_buf);
		usleep(29000);

	}

	goto play;
	file_close(fd);
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
