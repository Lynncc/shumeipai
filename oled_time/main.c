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

/*  struct tm
   {
      int tm_sec;   //秒值
      int tm_min;   //分钟值
      int tm_hour;  //小时值
      int tm_mday;  //本月第几日
      int tm_mon;   //本年第几月
      int tm_year;  //tm_year+1900=哪一年
      int tm_wday;  //本周第几日
      int tm_yday;  //本年第几日
      int tm_isdst; //日光节约时间
   }*/

int main(int argc, char const *argv[])
{


	unsigned char str_buf [BUF_LENTH];
	unsigned char x_pos,y_pos;
	int current_min = 0;
	if (argc != 3)
		return -1;

	struct tm *local;
	time_t t;


	x_pos = atoi(argv[1]);
	y_pos = atoi(argv[2]);
	// printf("%s\n", argv[3]);

	// printf("%s\n", str_buf);
	I2C_Configuration();


	// OLED_Init();
	OLED_ON();
	// OLED_Fill(0x00);
	OLED_Line(y_pos,0);
	OLED_Line(y_pos+1,0);
	while(1)
	{
		t = time(NULL);   //获取日历时间
		local = localtime(&t);  //将日历时间转化为本地时间，并保存在struct tm结构中
		if (current_min != local->tm_min)
		{
			current_min = local->tm_min;
			sprintf(str_buf,"%d-%d-%d %d:%02d",
			local->tm_year+1900, local->tm_mon, local->tm_mday, local->tm_hour, local->tm_min/*, local->tm_sec*/);
			OLED_Line(y_pos,0);
			OLED_Line(y_pos+1,0);
			cycle_show(x_pos, y_pos, str_buf);
			// usleep(500*1000);
			sleep(50);
		}
		else
			sleep(2);

	}

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
