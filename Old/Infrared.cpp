
// 9/25/14 - Goobertron
// 10/4/14
	//Tested through the arduino sketch and sensor is about 3-5 cm off
	//for closer calculations. Farther calculations will cause an
	//'inf' error and the accuracy of the sensor is around ~80 cm.
	//Anything out of range (10cm-80cm), the output will be 0.
	//I changed the reading from centimeters to meters.
	//Sensor - Sharp 2Y0A21
	// - Khoun

#include "Infrared.h"
#include <math.h>
#include <Arduino.h>

Infrared *_infrared; 


Infrared::Infrared(){
}

float Infrared::Get(){
	return DistFilt();
}

float Infrared::DistFilt(){ 
	float newDist;
	//distance (cm) = 27.86 (voltage reading)^-1.15
	newDist = 27.86*pow(analogRead(pin)*(5.0/1023.0),-1.15); //where does the 5.0/1023.0 come from?
	dist = Filter(newDist, dist);
	if(dist > 80 || dist < 10) dist = 0; //range of 10 cm to 80 cm
	dist = dist*.01; //converts centimeter into meter
	return dist;
}

float Infrared::Filter(float current, float prev){
	//taking 90% of the current distance and 10% of the previous distance to smooth the data
	return current * 0.9f + prev * 0.1f;
}

