#ifndef INFRARED_H
#def INFRARED_H

Infrared *_infrared;

class Infrared
{
public:	
	Infrared();
	bool Init(byte id = 0);
	
	void Start();
	float Get();
	void Stop();
	
	bool IsRunning();

	static void InfraredInterrupt();
	void ProcessInfraredInterrupt(bool high);

private:
	float Filter(float reading);
	
	byte id, pin;
	bool running;
	unsigned long volatile start;
	float us;	
};

#endif
