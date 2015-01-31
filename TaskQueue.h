#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <Arduino.h>

typedef void (*Task)(void*);

#define 	MAXQUEUE		64

class TaskQueue
{
public:
	TaskQueue();
	
	void Init();
	
	byte GetLength();
	bool DoNext();
	bool Clear();
	
	byte Add(Task t, void *c);
	
private:
	volatile Task tQue[MAXQUEUE];
	volatile void *cQue[MAXQUEUE];
	
	volatile byte length;
};
#endif
