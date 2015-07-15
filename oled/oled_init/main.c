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


int main (void)
{


	I2C_Configuration();

	OLED_Init();

	OLED_Fill(0x00);
	// sleep(20);
	OLED_ShowStr(0,0,(unsigned char*)"  Raspberry Pi",2);
	OLED_ShowStr(0,2,(unsigned char*)" Hello Mr.zhao",2);
	sleep(1);
	OLED_CLS();
	OLED_OFF();
	// sleep(2);
	// OLED_ON();
	// sleep(2);
	// OLED_ShowStr(0,4,(unsigned char*)"wake up 2",2);
  	return 0;
}



