#ifndef SCHEDULER_H
#define	SCHEDULER_H

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "TaskQueue.h"

#define MAXTASKS		100

class Scheduler
{
public:
	Scheduler();
	
	void Init();
	
	void Start();
	void Stop();
	
	bool IsRunning();
	
	bool AddTask(Task t, void *c, unsigned short time);
	bool RemoveTask(Task t);
	
	static void Interrupt();
	void ProcessInterrupt();

private:
	Task schedule[MAXTASKS];
	void *cList[MAXTASKS];
	unsigned short times[MAXTASKS], offset[MAXTASKS];
	unsigned long ticks;
	byte length, tInit;
};

extern Scheduler *_schedule;

#endif
