#include "TaskQueue.h"

TaskQueue::TaskQueue() {
}

//Initialize the Task Queue
void TaskQueue::Init() {
	length = 0;
	tQue[0] = NULL;
}

//Returns the length of the queue
byte TaskQueue::GetLength() {
	return length;
}

//Runs the next task in the queue, if available
bool TaskQueue::DoNext() {
	if(length == 0)											//If the queue is empty
		return 0;											//Return 0
	else {													//Otherwise
		noInterrupts();
		
		Task t = tQue[0];
		void *c = (void*)cQue[0];
		
		length--;
		
		for(int i = 0; i < length; i++) {					//Remove the finished task
			tQue[i] = tQue[i + 1];
			cQue[i] = cQue[i + 1];
		}
		tQue[length] = NULL;
		
		interrupts();

		t(c);
	}
}

//Clears the queue
bool TaskQueue::Clear() {
	length = 0;
}

//Adds a new task to the queue
//Task t:	A pointer to a void function(void*)
//void *c:	A void pointer that will be passed to the function
byte TaskQueue::Add(Task t, void *c) {
	if(length >= MAXQUEUE)
		return 0;
	else if(t == NULL) {
		return 0;
	}
	else {
		tQue[length] = t;
		cQue[length] = c;
		
		return (length++);
	}
}
