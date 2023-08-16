#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>	// getch
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define _1_Sec_Send_Msg_

int getch(void);
int main(int argc, char *argv[])
{
	char data;
	int sock;
	struct sockaddr_in my_addr;

	/* args */
	if(argc != 3)
	{
		printf("Usage: %s <Group ip> <Port> \n", argv[0]);
		exit(1);
	}
	/* create a socket */
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock == -1)
	{
		printf("socket() error\n");
		exit(1);
	}

	/* bind my address */
	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = inet_addr(argv[1]);
	my_addr.sin_port = htons(atoi(argv[2]));
	
	while(1)
	{
		Queue_Clr();
#ifdef _1_Sec_Send_Msg_
		for (int i=0; i<10; i++) 
		{
			printf("> Send to reciver: ");
			sprintf(queue.buf, "MSG%02d", i);
			printf("%s\n", queue.buf);
			sendto(sock, queue.buf, strlen(queue.buf), 0, (struct sockaddr *)&my_addr, sizeof(my_addr));
			sleep(1);
		}
		Queue_Clr();
		queue.buf[0] = 'T';
		sendto(sock, queue.buf, strlen(queue.buf), 0, (struct sockaddr *)&my_addr, sizeof(my_addr));
		break;
#else
		printf("> Send to reciver: ");
		while(1)
		{
			data = getch();
			printf("%c", data);
			if(data == '\n')	break;
			queue.buf[queue.in++] = data;
			if(queue.in == BUF_MAX)	queue.in = 0;
		}
		sendto(sock, queue.buf, strlen(queue.buf), 0, (struct sockaddr *)&my_addr, sizeof(my_addr));
		if(queue.buf[0] == 't' || queue.buf[0] == 'T')		break;
#endif
	}

	close(sock);
	return 0;
}

int getch(void)
{
	int c;
	struct termios oldattr, newattr;
    tcgetattr(STDIN_FILENO, &oldattr);           // 현재 터미널 설정 읽음
	newattr = oldattr;
	newattr.c_lflag &= ~(ICANON | ECHO);         	// CANONICAL과 ECHO 끔
	newattr.c_cc[VMIN] = 1;                      	// 최소 입력 문자 수를 1로 설정
	newattr.c_cc[VTIME] = 0;                    	// 최소 읽기 대기 시간을 0으로 설정
	tcsetattr(STDIN_FILENO, TCSANOW, &newattr);  	// 터미널에 설정 입력
	c = getchar();                               	// 키보드 입력 읽음
	tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);  	// 원래의 설정으로 복구
	return c;
}