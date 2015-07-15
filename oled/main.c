#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <wiringPi.h>
#include "oled.h"


int main (void)
{
	struct tm *t ;
	time_t tim ;

	char buf [64] ;


	printf ("Raspberry Pi OLCD test program\n") ;

	// fd_oled = wiringPiI2CSetup (0x3c);
	// if (fd_oled < 0)
	// {
	// 	printf("wiringPiI2CSetup errno\n");
	// 	return 1;
	// }
	I2C_Configuration();
	printf("wiringPiI2CSetup success.\n");

	OLED_Init();
	printf("oled init success.\n");
	// OLED_Fill(0xFF);
	// sleep(1);
	OLED_Fill(0x00);
	// OLED_ShowString(0, 0, "你好，Mr.赵");
	// OLED_ShowString(0, 2, "你好 Mr.赵");
	// cycle_show(0,0,"01234567890abcdefghigklmnopq");
	cycle_show(0,2,"采用絮凝法降低炼油厂循环水中油含量");

	// sleep(20);
	// OLED_ShowStr(0,0,(unsigned char*)"Raspberry Pi",2);
	// OLED_ShowStr(0,4,(unsigned char*)"Hello Mr.zhao",2);
	// sleep(2);
	// OLED_CLS();
	// OLED_OFF();
	// sleep(2);
	// OLED_ON();
	// sleep(2);
	// OLED_ShowStr(0,4,(unsigned char*)"wake up 2",2);
  	return 0;
}
