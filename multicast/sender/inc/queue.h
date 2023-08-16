#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdio.h>

#define BUF_MAX	(256)

#pragma pack(push, 1)
typedef struct {
	char buf[BUF_MAX];
	int in;
	int out;
} Queue;
#pragma pack(pop)

extern Queue queue;

extern void Queue_Clr(void);

#endif
