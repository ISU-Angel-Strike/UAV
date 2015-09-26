#include <Scheduler.h>

Scheduler *_schedule;

Scheduler::Scheduler() {
}

void Scheduler::Init(TaskQueue *que) {
	noInterrupts();
	
	//Setting hardware timer registers for a 1ms interrupt
	TCCR2A = 0;
	TCCR2B = 0;
	TCNT2 = 0;
	
	OCR2A = 249; // 16 Mhz / (1KHz * 64(prescaler))
	TCCR2A |= (1 << WGM21); //Turn on CTC mode
	TCCR2B |= (1 << CS22); //64 prescaler
	TIMSK2 &= ~(1 << OCIE2A); //Disable interrupt
	
	interrupts();
	
	length = 0;
	ticks = 0;
	
	this->que = que;
	
	_schedule = this;
}

void Scheduler::Start() {
	TIMSK2 |= (1<<OCIE2A);
}

void Scheduler::Stop() {
	TIMSK2 &= ~(1<<OCIE2A);
}

bool Scheduler::IsRunning() {
	return TIMSK2 & (1<<OCIE2A);
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
	}
	
	return 1;
}

bool Scheduler::RemoveTask(Task t) {
	bool found = 0;
	
	for(int i = 0; i < length; i++) {
		if(t == schedule[i]) {
			found = 1;
			for(int j = i; j < length; j++) {
				schedule[j] = schedule[j+1];
				times[j] = times[j+1];
				offset[j] = offset[j+1];
				cList[j] = cList[j+1];
			}
			break;
		}
	}
	length -= found;
	return found;
}

void Scheduler::Interrupt() {
	_schedule->ProcessInterrupt();
}

void Scheduler::ProcessInterrupt() volatile {
	ticks++;
	
	for(int i = 0; i < length; i++) {
		if((ticks - offset[i]) % times[i] == 0) {
			que->Add(schedule[i], cList[i]);
		}
	}
}

ISR(TIMER2_COMPA_vect) {
	Scheduler::Interrupt();
}
