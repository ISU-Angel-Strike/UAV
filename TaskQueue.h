#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <Arduino.h>

typedef void (*Task)(void*);

#define 	MAXQUEUE		128

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
	Task tQue[MAXQUEUE];
	void *cQue[MAXQUEUE];
	
	byte length;
};

extern TaskQueue *_taskQue;
#endif
