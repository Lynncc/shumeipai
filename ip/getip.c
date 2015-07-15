#include <stdio.h>
#include <netdb.h>
int main()
{
struct hostent *he;
char hostname[20] = {0};

gethostname(hostname,sizeof(hostname));
he = gethostbyname(hostname);
printf("hostname=%s\n",hostname);
printf("%s\n",inet_ntoa(*(struct in_addr*)(he->h_addr)));
}
