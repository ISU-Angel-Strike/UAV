#include <Scheduler.h>

Scheduler *_schedule;

Scheduler::Scheduler() {
}

void Scheduler::Init() {
	//Setting hardware timer registers for a 1ms interrupt
	TIMSK2 &= ~(1<<TOIE2);
	TCCR2A &= ~((1<<WGM21) | (1<<WGM20));
	TCCR2B &= ~(1<<WGM22);
	ASSR &= ~(1<<AS2);
	TIMSK2 &= ~(1<<OCIE2A);
	TCCR2B |= (1<<CS22) | (1<<CS20);
	TCCR2B &= ~(1<<CS21);
	
	tInit = 131;
	
	length = 0;
	ticks = 0;
	
	_schedule = this;
}

void Scheduler::Start() {
	TCNT2 = tInit;
	TIMSK2 |= (1<<TOIE2);
}

void Scheduler::Stop() {
	TIMSK2 &= ~(1<<TOIE2);
}

bool Scheduler::IsRunning() {
	return TIMSK2 & (1<<TOIE2);
}

bool Scheduler::AddTask(Task t, void *c, unsigned short time) {
	if(length >= MAXTASKS)
		return 0;
	else {
		//Add the task
		schedule[length] = t;
		times[length] = time;
		offset[length] = ticks % time;
		cList[length++] = c;
		
		//Run it for the first time
		_taskQue->Add(t, c);
	}
	
	return 1;
}

bool Scheduler::RemoveTask(Task t) {
	bool found = 0;
	
	for(int i = 0; i < length; i++) {
		if(t == schedule[i]) {
			found = 1;
			for(int j = i; j < (length--); j++) {
				schedule[j] = schedule[j+1];
				times[j] = times[j+1];
				offset[j] = offset[j+1];
				cList[j] = cList[j+1];
			}
			break;
		}
	}
	
	return found;
}

void Scheduler::Interrupt() {
	_schedule->ProcessInterrupt();
}

void Scheduler::ProcessInterrupt() {
	ticks++;
	
	for(int i = 0; i < length; i++) {
		if((ticks - offset[i]) % times[i] == 0) {
			_taskQue->Add(schedule[i], cList[i]);
		}
	}
	
	Start();
}

ISR(TIMER2_OVF_vect) {
	Scheduler::Interrupt();
}
