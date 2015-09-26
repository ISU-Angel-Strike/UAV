#include "Communicator.h"

Communicator::Communicator() {
	ready = false;
}

Communicator::Communicator(byte port) {
	ready = Init(port);
}

bool Communicator::Init(byte port) {
	packetCount = overflowCount = badPacketCount = 0;
	state = STATE_READY;

	if(comm.Connect(port) >= 0)
		ready = true;

	return ready;
}

bool Communicator::Ready() {
	return ready;
}

byte Communicator::Update() {
	byte buffer[SERIAL_BUFFER_SIZE];
	int nRead;

	nRead = comm.ReadBytes(SERIAL_BUFFER_SIZE, buffer);

	for (int i = 0; i < nRead; i++)
		ProcessByte(buffer[i]);

	return packetCount;
}

byte Communicator::CountPackets(byte id) {
	if (id == 0)
		return packetCount;
	else {
		byte i, count = 0;
		
		for (i = 0; i < packetCount; i++)
			count += packets[i].id == id;

		return count;
	}
}

byte Communicator::NextPacketLength(byte id) {
	if (packetCount < 1)
		return 0;
	if (id == 0)
		return packets[0].length;
	else {
		byte i, length = 0;
		for (i = 0; i < packetCount; i++) {
			if (packets[i].id == id) {
				length = packets[i].length;
				break;
			}
		}

		return length;
	}
}

int Communicator::GetPacket(byte *id, byte *length, byte *data) {
	byte pick = 0, i;

	if (packetCount == 0)
		return ERROR_NO_PACKETS;

	if (data == NULL || id == NULL || length == NULL)
		return ERROR_GIVEN_NULL_PTR;

	if (*id != 0) {
		for (i = 0; i < packetCount; i++) {
			if (packets[i].id == *id) {
				pick = i;
				break;
			}
		}
		if (i == packetCount)
			return ERROR_NO_MATCHING_PACKET;
	}

	if (*length < packets[pick].length) {
		*length = packets[pick].length;
		return ERROR_PACKET_TOO_BIG;
	}

	*id = packets[pick].id;
	*length = packets[pick].length;
	memcpy(data, packets[pick].data, packets[pick].length);

	for (i = pick + 1; i < packetCount; i++)
		packets[i - 1] = packets[i];

	packetCount--;
}

int Communicator::SendPacket(byte id, byte length, byte *data) {
	byte i, check = 0, buffer[64];

	if (id == 0)
		return ERROR_INVALID_ID;
	if (data == NULL)
		return ERROR_GIVEN_NULL_PTR;
	if (!ready)
		return ERROR_NOT_READY_YET;

	for (i = 0; i < length; i++)
		check ^= data[i];

	buffer[0] = PACKET_START_VALUE;
	buffer[1] = id;
	buffer[2] = length;
	memcpy(buffer + 3, data, length);
	buffer[length + 3] = check;

	if (comm.WriteBytes(length + 4, buffer) <= 0)
		return ERROR_GENERIC_WRITE_FAILURE;
	
	return 0;
}

byte Communicator::GetOverflowCount() {
	return overflowCount;
}

int Communicator::AddWatch(byte id, DispatchFunction f, void *arg, byte length, byte *data) {
	if (id == 0)
		return ERROR_INVALID_ID;
	else if (f == NULL || length == NULL || data == NULL)
		return ERROR_GIVEN_NULL_PTR;
	else if (dispatchCount >= DISPATCH_BUFFER_SIZE)
		return ERROR_DISPATCH_BUFFER_FULL;

	for (int i = 0; i < dispatchCount; i++) {
		if (dispatchers[dispatchCount].id == id)
			return ERROR_DISPATCH_ALREADY_EXISTS;
	}

	dispatchers[dispatchCount].id = id;
	dispatchers[dispatchCount].bufferSize = length;
	dispatchers[dispatchCount].data = data;
	dispatchers[dispatchCount].arg = arg;
	dispatchers[dispatchCount].f = f;

	dispatchCount++;
}

bool Communicator::RemoveWatch(byte id) {
	byte found = dispatchCount;
	
	if (id == 0)
		return false;
	
	for (int i = 0; i < dispatchCount; i++) {
		if (dispatchers[i].id == id) {
			found = i;
			break;
		}
	}
	if (found == dispatchCount)
		return false;

	for (int i = found + 1; i < dispatchCount; i++)
		dispatchers[found - 1] = dispatchers[found];

	return true;
}

Communicator::~Communicator() {
}

void Communicator::ProcessByte(byte b) {
	switch (state) {
		case STATE_READY:
			if (b == PACKET_START_VALUE)
				state = STATE_ID;
		break;

		case STATE_ID:
			curPacket.id = b;
			state = STATE_LENGTH;
		break;

		case STATE_LENGTH:
			curPacket.length = b;

			//This should never happen
			if (b == 0) {
				state = STATE_READY;
			}
			else {
				ptr = 0;
				checksum = 0;

				state = STATE_DATA;
			}
		break;

		case STATE_DATA:
			curPacket.data[ptr++] = b;

			checksum = checksum ^ b;
			
			if (ptr >= curPacket.length)
				state = STATE_CHECK;
		break;

		case STATE_CHECK:
			checksum = checksum ^ b;
			if (checksum != 0) {
				SendPacket(PACKET_BAD_CHECKSUM, 1, &checksum);
				badPacketCount++;
			}
			else if (DispatchPacket(curPacket)) {
			}
			else if (packetCount >= PACKET_STORAGE_COUNT) {
				SendPacket(PACKET_RECEIVE_OVERFLOW, 1, &curPacket.id);
				overflowCount++;
			}
			else {
				packets[packetCount].id = curPacket.id;
				packets[packetCount].length = curPacket.length;
				memcpy(packets[packetCount].data, curPacket.data, curPacket.length);

				packetCount++;
			}
			state = STATE_READY;
		break;

		//This should never happen!
		default:
			state = STATE_READY;
		break;
	}
}

bool Communicator::DispatchPacket(Packet p) {
	if (dispatchCount < 1)
		return false;

	byte found = dispatchCount, length;

	for (int i = 0; i < dispatchCount; i++) {
		if (dispatchers[i].id == p.id) {
			found = i;
			break;
		}
	}

	if (found == dispatchCount)
		return false;

	if (dispatchers[found].bufferSize < p.length) {
		length = 0;
	}
	else {
		length = p.length;
		memcpy(dispatchers[found].data, p.data, p.length);
	}

	dispatchers[found].f(dispatchers[found].arg, length, dispatchers[found].data);

	return true;
}
