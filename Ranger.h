#ifndef RANGER_H
#define RANGER_H

Ranger *_ranger;
typedef unsigned char byte;
class Ranger
{
public:
	Ranger();
	bool Init(byte id = 0);
	
	void Start();
	float Get();
	void Stop();

	bool IsRunning();
	
	static void Interrupt();

private:
	float Filter(float reading);
	void ProcessInterrupt(bool high);
	
	byte id, pin;
	bool running;
	unsigned long volatile start;
	float us;
};

#endif
