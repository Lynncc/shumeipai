#include "lewei.h"




int server_connect(char * __host, int __port)
{
	int sockfd;
	struct sockaddr_in c_addr;
	struct hostent *host_global = NULL;
	char buff[30];

	if(NULL == (host_global = gethostbyname(__host))){
		printf("gethostbyname error");
		return -1;
	}

	c_addr.sin_family=AF_INET;
	c_addr.sin_port=htons(__port);
	c_addr.sin_addr.s_addr=inet_addr(inet_ntop(host_global->h_addrtype, host_global->h_addr, buff, sizeof(buff)));
	memset(c_addr.sin_zero, 0, sizeof(c_addr.sin_zero));

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		return -1;

	if(-1 == (connect(sockfd, (struct sockaddr*)(&c_addr), sizeof(struct sockaddr)))){
		printf("connect error");
		return -1;
	}

	return sockfd;

}

int server_ping(int sockfd, int gateway)
{
	char msg[128];
	int msg_len = 0;
	int ret;

	sprintf(msg, "{\"method\":\"update\",\"gatewayNo\":\"%02d\",\"userkey\":\"%s\"}&^!", gateway, APIKEY);
	printf("msg:%s\n", msg);
	ret = send(sockfd, msg, strlen(msg), 0);
	if (ret != strlen(msg))
	{
		printf("ret:%d\n", ret);
		return -1;
	}
	return 0;

}

int update_sensor(int sockfd, char * sensor, int value)
{
	char data[128];
	int ret;



	sprintf(data, "{\"method\":\"upload\",\"data\":[{\"Name\":\"%s\",\"Value\":\"%d\"}]}&^!", sensor, value);
	printf("data:%s\n", data);



	ret = send(sockfd, data, strlen(data), 0);
	if (ret != strlen(data))
	{
		printf("ret:%d\n", ret);
		return -1;
	}
	return 0;
}


// void server_listing(int sockfd, int gateway, char * userkey)
// {
// 	int ret;
// 	char rcv_buf[MAXBUFF];
// 	struct timeval tv_out;

//     tv_out.tv_sec = 1;
//     tv_out.tv_usec = 0;

//     setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out));
//     server_ping(sockfd, 2);
//     while(1)
//     {
//     	ret = recv(sockfd, rcv_buf, MAXBUFF, 0);
//     	if (ret > 0)
//     	{
//     		printf("rcv:\n%s\n", rcv_buf);
//     		send_resp(sockfd, "xxxx");
//     	}
//     }
// }

// void send_resp(int sockfd, char * msg)
// {


// 	char data[256];
// 	int ret;

// 	sprintf(data, "{\"method\":\"response\",\"result\":{\"successful\":\"true\",\"message\":\"%s\",\"data\":{\"id\":\"%s\",\"value\":\"%d\"}}}&^!",
// 		msg, "Light", 0);

// 	printf("data:%s\n", data);

// 	ret = send(sockfd, data, strlen(data), 0);
// 	if (ret != strlen(data))
// 	{
// 		printf("ret:%d\n", ret);
// 	}
// }
