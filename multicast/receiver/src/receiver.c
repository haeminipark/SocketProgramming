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
	int recv_sock, option;
	socklen_t str_len;
	struct sockaddr_in mul_adr;
	struct ip_mreq join_adr;

	if(argc != 3)
	{
		printf("Usage: %s <Group IP> <Port> \n", argv[0]);
		exit(1);
	}

	// UDP ���� ����
	recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(recv_sock == -1)
	{
		printf("socket() error\n");
		exit(1);
	}

	option = 1;
	setsockopt(recv_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option) );

	// Multicast �ּ� ���� �ʱ�ȭ, �ݵ�� IP �ּҸ� ��Ƽĳ��Ʈ �ּҷ� ����
	memset(&mul_adr, 0, sizeof(mul_adr));
	mul_adr.sin_family = AF_INET;
	mul_adr.sin_addr.s_addr = inet_addr(argv[1]);
	mul_adr.sin_port = htons(atoi(argv[2]));

	// ���Ͽ� �ּ� �Ҵ�
	if(bind(recv_sock, (struct sockaddr*)&mul_adr, sizeof(mul_adr)) == -1)
	{
		printf("bind() error\n");	exit(1);
	}


	join_adr.imr_multiaddr.s_addr = inet_addr(argv[1]);	// ������ �׷��� IP �ּ�
	join_adr.imr_interface.s_addr = htonl(INADDR_ANY);	// �׷쿡 �����ϴ� ������ ���� ȣ��Ʈ�� IP �ּ�

	setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&join_adr, sizeof(join_adr));

	while(1)
	{
		Queue_Clr();
		str_len = recvfrom(recv_sock, queue.buf, sizeof(queue.buf), 0, NULL, 0);
		queue.buf[str_len] = 0;
		printf("> Receive from sender: %s", queue.buf);
		if(queue.buf[0] == 't' || queue.buf[0] == 'T')		break;
	}

	close(recv_sock);
	return 0;
}
