// 9/25/14 - Goobertron

#include <math.h>
#include <arduino.h>

#ifndef INFRARED_H
#define INFRARED_H
#define INFRARED_PIN 0

//Infrared *_infrared;

class Infrared
{
public:
	Infrared();
	
	float Get();
	
	float DistFilt();


private:
	float Filter(float current, float prev);
	
	byte id, pin;
	bool running;
	unsigned long volatile start;
	float dist;	
};

extern Infrared *_infrared;

#endif
