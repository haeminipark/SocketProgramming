#include "queue.h"
#include <stdio.h>
#include <string.h>	// memset

Queue queue;

void Queue_Clr(void)
{
	memset(&queue, 0, sizeof(Queue));
}
