#ifndef RANGER_H
#define RANGER_H

Ranger *_ranger;

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
	void ProcessInterrupt(bool high);

private:
	float Filter(float reading);
	
	byte id, pin;
	bool running;
	unsigned long volatile start;
	float us;
};

#endif
