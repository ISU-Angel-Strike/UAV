// 9/25/14 - Goobertron

#ifndef INFRARED_H
#def INFRARED_H
#define INFRARED_PIN 0

Infrared *_infrared;

class NewInfrared
{
public:	
	Infrared();
	
	float Get();
	
	float DistFilt();


private:
	float Filter(float reading);
	
	byte id, pin;
	bool running;
	unsigned long volatile start;
	float dist;	
};

#endif
