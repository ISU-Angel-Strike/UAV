#ifndef SCHEDULER_H
#define	SCHEDULER_H

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "TaskQueue.h"

#define MAXTASKS		16

class Scheduler
{
public:
	Scheduler();
	
	void Init(TaskQueue *que);
	
	void Start();
	void Stop();
	
	bool IsRunning();
	
	bool AddTask(Task t, void *c, unsigned short time);
	bool RemoveTask(Task t);
	
	static void Interrupt();
	void ProcessInterrupt() volatile;

private:
	TaskQueue *que;
	Task schedule[MAXTASKS];
	void *cList[MAXTASKS];
	unsigned short times[MAXTASKS];
	uint32_t offset[MAXTASKS];
	volatile uint32_t ticks;
	byte length;
};

extern Scheduler *_schedule;

#endif
