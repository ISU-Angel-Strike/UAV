#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <string.h>
#include <Arduino.h>

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long uint32_t;
typedef uint8_t byte;

class SerialPort {
public:
	static const int ERROR_SETTING_COMM_SETTINGS = -1;
	static const int ERROR_SETTING_COMM_TIMEOUTS = -2;
	static const int ERROR_OPENING_COMM_PORT = -3;
	static const int ERROR_SENDING_COMM = -4;
	static const int ERROR_PORT_NOT_READY = -5;
	static const int ERROR_GENERIC_WRITE_FAIL = -6;
	static const int ERROR_GENERIC_READ_FAIL = -7;
	static const int ERROR_GIVEN_NULL_POINTER = -8;

	SerialPort();

	int Connect(byte id);
	void Disconnect();

	bool IsReady();

	int WriteBytes(int length, byte *data);
	int ReadBytes(int length, byte *data);

	~SerialPort();
private:
	bool ready;
};
#endif
