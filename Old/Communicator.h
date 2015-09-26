#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include "utility/SerialPort.h"
#include "PacketDefines.h"
#include <stdio.h>

typedef void (*DispatchFunction)(void*, byte, byte*);


class Communicator
{
public:
	static const int ERROR_PACKET_TOO_BIG = -1;
	static const int ERROR_NO_MATCHING_PACKET = -2;
	static const int ERROR_NO_PACKETS = -3;
	static const int ERROR_GIVEN_NULL_PTR = -4;
	static const int ERROR_INVALID_ID = -5;
	static const int ERROR_NOT_READY_YET = -6;
	static const int ERROR_GENERIC_WRITE_FAILURE = -7;
	static const int ERROR_DISPATCH_BUFFER_FULL = -8;
	static const int ERROR_DISPATCH_ALREADY_EXISTS = -9;
	static const int ERROR_DISPATCH_NOT_EXIST = -10;

	Communicator();
	Communicator(byte port);

	bool Init(byte port);

	bool Ready();

	byte Update();
	
	byte CountPackets(byte id = 0);
	byte NextPacketLength(byte id = 0);

	int SendPacket(byte id, byte length, byte *data);
	int GetPacket(byte *id, byte *length, byte *data);

	byte GetOverflowCount();

	int AddWatch(byte id, DispatchFunction f, void *arg, byte length, byte *data);
	bool RemoveWatch(byte id);

	~Communicator();
private:
	static const byte PACKET_STORAGE_COUNT = 4;
	static const byte PACKET_START_VALUE = 0xAA;
	static const byte DISPATCH_BUFFER_SIZE = 8;
	static const byte PACKET_BUFFER_SIZE = 32;
	static const byte SERIAL_BUFFER_SIZE = 128;

	static const byte STATE_READY = 0;
	static const byte STATE_ID = 2;
	static const byte STATE_LENGTH = 3;
	static const byte STATE_DATA = 4;
	static const byte STATE_CHECK = 5;

	struct Packet {
		byte id, length, data[PACKET_BUFFER_SIZE];
	} curPacket, packets[PACKET_STORAGE_COUNT];

	struct Dispatcher {
		DispatchFunction f;
		void *arg;
		byte id, bufferSize, *data;
	} dispatchers[DISPATCH_BUFFER_SIZE];

	SerialPort comm;
	byte packetCount, dispatchCount, overflowCount, state, ptr, checksum;
	int badPacketCount;
	bool ready;

	void ProcessByte(byte b);
	bool DispatchPacket(Packet p);
};
#endif
