#include "Infrared.h"
#include <math.h>
#include <Arduino.h>

Infrared::Infrared(){

}

bool Infrared::Init(byte id){
	bool retval = true;
	
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
	float newDist;
	//distance (cm) = 27.86 (voltage reading)^-1.15
	newDist = 27.86*pow(analogRead(pin)*(5.0/1023.0),-1.15);
	dist = Infrared::Filter(newDist, dist);
}

float Infrared::Filter(float current, float prev){
	//taking 90% of the current distance and 10% of the previous distance to smooth the data
	return current * 0.9f + prev * 0.1f;
}

