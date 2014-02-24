#include "TaskQueue.h"

//Global pointer to the Task Queue
TaskQueue *_taskQue;

TaskQueue::TaskQueue() {
}

//Initialize the Task Queue
void TaskQueue::Init() {
	length = 0;
	_taskQue = this;
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
		tQue[0](cQue[0]);									//Run the first task
		
		Serial.print("TaskQueue: Finished running ");		//Print that the task is done
		Serial.println(idQue[0]);							//For debugging purposes
		
		length--;
		
		for(int i = 0; i < length; i++) {					//Remove the finished task
			tQue[i] = tQue[i + 1];
			cQue[i] = cQue[i + 1];
			idQue[i] = idQue[i + 1];
		}
	}
}

//Clears the queue
bool TaskQueue::Clear() {
	length = 0;
}

//Adds a new task to the queue
//Task t:	A pointer to a void function(void*)
//void *c:	A void pointer that will be passed to the function
//char *id:	Task identifier string (optional)
byte TaskQueue::Add(Task t, void *c, char *id) {
	if(length >= MAXQUEUE)
		return 0;
	else {
		tQue[length] = t;
		cQue[length] = c;
		idQue[length] = id;
		
		return (length++);
	}
}
