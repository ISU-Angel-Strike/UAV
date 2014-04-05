#include "Infrared.h"
#include <math.h>

Infrared::Infrared(){

}

bool Infrared::Init(byte id){
	bool retval = true;
	
	us = 0;
	running = false;
	this->id = id;
	
	if(id == 0)
		pin = 2;
	else if(id == 1)
		pin = 3;
	else{
		this->id = 0;
		pin = 2;
		
		retval = false;
	}
	
	_infrared = this;
	
	return retval;
}

void Infrared::Start(){
	if(running == false){
		running = true;
		
		attachInterrupt(id, Infrared::InfraredInterrupt, CHANGE);
	}
}

float Infrared::Get(){
	return dist;
}

void Infrared::Stop(){
	if(running == true){
		running = false;
		
		detachInterrupt(id);
	}
}

bool Infrared::IsRunning(){
	return running;
}

void Infrared::InfraredInterrupt(){
	_infrared->ProcessInfraredInterrupt();
}

void Infrared::ProcessInfraredInterrupt(){
	//distance (cm) = 27.86 (voltage reading)^-1.15
	dist = 27.86*pow(analogeRead(pin)*(5.0/1023.0),-1.15);
}

float Infrared::Filter(float reading){
	return reading * 0.9f + us * 0.1f;
}

