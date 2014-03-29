#include "Ranger.h"

Ranger::Ranger() {
}

bool Ranger::Init(byte id) {
	bool retval = true;

	us = 0;
	running = false;
	this->id = id;
	
	if(id == 0)
		pin = 2;
	else if(id == 1)
		pin = 3;
	else {
		//ERROR! There are only 2 interrupts!
		this->id = 0; //Set it to interrupt 0 instead
		pin = 2;
		
		retval = false;	//Return false to indicate defaults are being used
	}
	
	_ranger = this;
	
	return retval;
}

void Ranger::Start() {
	if(running == false) {
		running = true;
		
		attachInterrupt(id, Ranger::Interrupt, CHANGE);	//Start the interrupt service routine
	}
}

float Ranger::Get() {
	return (float)us / 5787.4f;							//Convert pulse width to meters
}														//From datasheet

void Ranger::Stop() {
	if(running == true) {
		running = false;
	
		detachInterrupt(id);
	}
}

bool Ranger::IsRunning() {
	return running;
}


//This is called automatically when the interrupt pin switches between high and low
void Ranger::Interrupt() {
	_ranger->ProcessInterrupt(digitalRead(pin) == HIGH);
}

void Ranger::ProcessInterrupt(bool high) {
	if(high)
		start = micros();								//Pin changed from low to high
	else
		us = Filter(start - micros());					//Pin changed from high to low
}

float Ranger::Filter(float reading) {
	return reading*0.9f + us*0.1f;						//Simple low-pass filter
}
