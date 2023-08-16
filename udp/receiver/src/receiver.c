#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>	// getch
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>


int main(int argc, char *argv[])
{
	int sock;
	socklen_t str_len;
	struct sockaddr_in send_addr;

	/* args */
	if(argc != 3)
	{
		printf("Usage: %s <Group IP> <Port> \n", argv[0]);
		exit(1);
	}

	/* create a socket */
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock == -1)
	{
		printf("socket() error\n");
		exit(1);
	}

	/* server address */
	memset(&send_addr, 0, sizeof(send_addr));
	send_addr.sin_family = AF_INET;
	send_addr.sin_addr.s_addr = inet_addr(argv[1]);
	send_addr.sin_port = htons(atoi(argv[2]));

	if (bind(sock, (struct sockaddr *)&send_addr, sizeof(send_addr)) == -1)
	{
		printf("bind() error\n");
		exit(1);
	}

	while (1) 
	{
		Queue_Clr();
		str_len = recvfrom(sock, queue.buf, sizeof(queue.buf), 0, NULL, 0);
		printf("> Receive from sender: %s\n", queue.buf);
		if(queue.buf[0] == 't' || queue.buf[0] == 'T')		break;
	}
	close(sock);
	return 0;
}
