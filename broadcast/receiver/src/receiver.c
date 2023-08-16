#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>	// getch
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char* argv[])
{
	int recv_sock;
	socklen_t str_len;
	struct sockaddr_in broad_adr;

	if(argc != 2)
	{
		printf("Usage: %s <Port> \n", argv[0]);
		exit(1);
	}

	// 家南 积己
	recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(recv_sock == -1)
	{
		printf("socket() error\n");
		exit(1);
	}

	// Broadcast 林家 沥焊 檬扁拳
	memset(&broad_adr, 0, sizeof(broad_adr));
	broad_adr.sin_family = AF_INET;
	broad_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	broad_adr.sin_port = htons(atoi(argv[1]));

	// 家南 林家 且寸
	if(bind(recv_sock, (struct sockaddr*)&broad_adr, sizeof(broad_adr)) == -1)
	{
		printf("bind error\n");	exit(1);
	}	

	while(1)
	{
		Queue_Clr();
		str_len = recvfrom(recv_sock, queue.buf, sizeof(queue.buf), 0, NULL, 0);
		queue.buf[str_len] = 0;
		printf("> Receive from sender: %s\n", queue.buf);
		if(queue.buf[0] == 't' || queue.buf[0] == 'T')		break;
	}

	close(recv_sock);
	return 0;
}
