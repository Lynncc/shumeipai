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

int get_ip(unsigned char *__ip_buf);

int main (void)
{


	unsigned char buf [32] ;

	sleep(5);
	I2C_Configuration();


	// OLED_Init();
	OLED_ON();
	OLED_Fill(0x00);
	// sleep(20);
	get_ip(buf);
	OLED_ShowStr(0,0,(unsigned char*)"Raspberry Pi",2);
	OLED_ShowStr(0,2,(unsigned char*)"IP:",2);
	OLED_ShowStr(3*8,2,buf,2);
	sleep(60);
	OLED_CLS();
	OLED_OFF();
	// sleep(2);
	// OLED_ON();
	// sleep(2);
	// OLED_ShowStr(0,4,(unsigned char*)"wake up 2",2);
  	return 0;
}


int get_ip(unsigned char *__ip_buf)
{
	int inet_sock;
	struct ifreq ifr;
	inet_sock = socket(AF_INET, SOCK_DGRAM, 0);

	strcpy(ifr.ifr_name, "eth0");
	if (ioctl(inet_sock, SIOCGIFADDR, &ifr) < 0)
		perror("ioctl");
	printf("%s\n", inet_ntoa(((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr));
	sprintf(__ip_buf,"%s", inet_ntoa(((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr));
	return 0;
}
