#include "queue.h"
#include <stdio.h>
#include <string.h>	// memset
#include <unistd.h>
#include <termios.h>	// getch

Queue queue;

int getch(void);

void Queue_Clr(void)
{
	memset(&queue, 0, sizeof(Queue));
}

void Queue_Get(void)
{
	char data;
	while(1)
	{
		data = getch();
		printf("%c", data);
		if(data == '\n')	break;
		queue.buf[queue.in++] = data;
		if(queue.in == BUF_MAX)	queue.in = 0;
	}
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
