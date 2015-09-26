#include "SerialPort.h"

SerialPort::SerialPort() {
	ready = false;
}

int SerialPort::Connect(byte id) {
	Serial.begin(57600);
	ready = true;
}

void SerialPort::Disconnect() {
	Serial.end();
	ready = false;
}

bool SerialPort::IsReady() {
	return ready;
}

int SerialPort::WriteBytes(int length, byte *data) {
	if(!ready)
		return ERROR_PORT_NOT_READY;
	if(data == NULL)
		return ERROR_GIVEN_NULL_POINTER;

	return Serial.write(data, length);
}

int SerialPort::ReadBytes(int length, byte *data) {
	int i;
	int ch;
	
	for(i = 0; (i < length) && ( (ch = Serial.read()) >= 0); i++)
		data[i] = ch;
	
	return i;
}

SerialPort::~SerialPort() {
	Serial.end();
}
