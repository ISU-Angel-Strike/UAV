// 9/25/14 - Goobertron

#include "Infrared.h"
#include <math.h>
#include <Arduino.h>

Infrared::Infrared(){

}

float NewInfrared::Get(){
	return DistFilt();
}

float Infrared::DistFilt(){ 
	float newDist;
	//distance (cm) = 27.86 (voltage reading)^-1.15
	newDist = 27.86*pow(analogRead(pin)*(5.0/1023.0),-1.15); //where does the 5.0/1023.0 come from?
	dist = Infrared::Filter(newDist, dist);
	return dist;
}

float Infrared::Filter(float current, float prev){
	//taking 90% of the current distance and 10% of the previous distance to smooth the data
	return current * 0.9f + prev * 0.1f;
}

