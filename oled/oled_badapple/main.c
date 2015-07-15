#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <wiringPi.h>
#include "../src/oled.h"
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

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
	fd =fopen(IMAGE_PATH,"rb");
	if (fd == NULL)
		return -1;
play:
	for(i=0;i<6566;i++)
	{

		while(GetImage(fd, str_buf, i) != 0)
			;
		OLED_Draw(str_buf);
		usleep(29000);

	}

	goto play;
	fclose(fd);
  	return 0;
}
