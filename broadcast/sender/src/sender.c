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
	int send_sock;
	struct sockaddr_in broad_adr;
	int bcast = 1;
	char data;

	if(argc != 3)
	{
		printf("Usage: %s <Group ip> <Port> \n", argv[0]);
		exit(1);
	}

	// UDP 소켓 생성
	send_sock = socket(PF_INET, SOCK_DGRAM, 0);	
	if(send_sock == -1)
	{
		printf("socket() error\n");	exit(1);
	}

	// Broadcast 주소 정보 초기화
	memset(&broad_adr, 0, sizeof(broad_adr));
	broad_adr.sin_family = AF_INET;
	broad_adr.sin_addr.s_addr = inet_addr(argv[1]);
	broad_adr.sin_port = htons(atoi(argv[2]));

	setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void*)&bcast, sizeof(bcast));
	
	while(1)
	{
		Queue_Clr();
#ifdef _1_Sec_Send_Msg_
		for (int i=0; i<10; i++) 
		{
			printf("> Send to reciver: ");
			sprintf(queue.buf, "MSG%02d", i);
			printf("%s\n", queue.buf);
			sendto(send_sock, queue.buf, strlen(queue.buf), 0, (struct sockaddr*)&broad_adr, sizeof(broad_adr));
			sleep(1);
		}
		Queue_Clr();
		queue.buf[0] = 'T';
		sendto(send_sock, queue.buf, strlen(queue.buf), 0, (struct sockaddr *)&broad_adr, sizeof(broad_adr));
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
		sendto(send_sock, queue.buf, strlen(queue.buf), 0, (struct sockaddr*)&broad_adr, sizeof(broad_adr));
		if(queue.buf[0] == 't' || queue.buf[0] == 'T')		break;
#endif
	}

	close(send_sock);
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