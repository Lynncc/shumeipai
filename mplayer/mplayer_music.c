#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>

/**********************全局变量定义区*****************/
int fd_fifo;                    //创建有名管道，用于向mplayer发送命令
int fd_pipe[2];                 //创建无名管道,用于从mplayer读取命令


int main(int argc, char *argv[])
{
	int fd;
	char buf[100];
	pid_t pid;

	unlink("/tmp/fifo_mplayer");                 //如果明明管道存在，则先删除
	mkfifo("/tmp/fifo_mplayer",O_CREAT|0666);
	perror("mk fifo_mplayer error");

	if (pipe(fd_pipe)<0 )                    //创建无名管道
	{
		perror("pipe error\n");
		exit(-1);
	}


    pid=fork();
    if(pid<0)
    {
        perror("fork");
    }
    if(pid==0)                              //子进程播放mplayer
    {
        close(fd_pipe[0]);
        dup2(fd_pipe[1],1);                 //将子进程的标准输出重定向到管道的写端
        dup2(fd_pipe[1],2);                 //将子进程的标准输出重定向到管道的写端
        fd_fifo=open("/tmp/fifo_mplayer",O_RDWR);
        system("sudo mplayer -slave -quiet -shuffle -input file=/tmp/fifo_mplayer /home/zhaowei/music/*  NULL");
        // execlp("mplayer","mplayer","-slave","-quiet","-input","file=/tmp/fifo_mplayer","juhuatai.mpg",NULL);
    }
    else
    {

        // system("irexec");
        while(1)
            sleep(5);
    }

	return 0;
}
