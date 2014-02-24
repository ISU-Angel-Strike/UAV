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
	
	byte Add(Task t, void *c, char *id = "Generic Task");
	
private:
	Task tQue[MAXQUEUE];
	void *cQue[MAXQUEUE];
	char *idQue[MAXQUEUE];
	
	byte length;
};

extern TaskQueue *_taskQue;
#endif
