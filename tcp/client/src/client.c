#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


int main(int argc, char *argv[])
{
	int clnt_sock;
	int str_len;
	struct sockaddr_in serv_addr;

	/* args */
	if(argc != 3)
	{
		printf("Usage: %s <Group ip> <Port> \n", argv[0]);
		exit(1);
	}

	/* 소켓 생성 */
	clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(clnt_sock == -1)
	{
		printf("socket() error\n");
		exit(1);
	}
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	/* 서버에 연결 요청 */
	if(connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		printf("connect() error\n");
		exit(1);
	}

	while(1)
	{
		Queue_Clr();
		str_len = read(clnt_sock, queue.buf, sizeof(queue.buf));
		if(str_len == 0)	break;
		queue.buf[str_len] = 0;
		printf("> Receive from sender: %s\n", queue.buf);
		if(queue.buf[0] == 't' || queue.buf[0] == 'T')		break;
	}
	close(clnt_sock);
	return 0;
}

