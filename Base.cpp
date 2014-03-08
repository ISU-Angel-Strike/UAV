#include <Base.h>

Base::Base()
{
}

//Checks the state for incoming data
Base::State(byte currentState, byte id, byte length, byte data)
{
	currentState = ST_IDLE;
    switch (currentState) {
        case ST_IDLE:
			//Sets the currentState to the next state 
			currentState = ST_ID;
            break;
        case ST_ID:
			if(Serial.read() > -1){
			currentState = ST_LENGTH;
			}else{
				currentState = ST_IDLE;
			}
            break;
        case ST_LENGTH:
			//checks the length of the incoming data
			if(length == 128){
				currentState = ST_DATA;
				break;
			}else{
				currentState = ST_IDLE;
				break;
			}
        case ST_DATA:
			GetData(byte id, byte* l);
			currentState = ST_IDLE;
            break;
        default:
            break;
    }
}



//Sends the data and also sends a checksum to make sure data is not corrupt
Base::Send(byte id,byte length,byte *data)
{
   byte checksum = 0;
   Serial.write(id);
   Serial.write(length);
   for(int i=0; i<length; i++){
	Serial.write(data[i]);
	checksum ^= data[i];
   }
   Serial.write(checksum);
}
//copies data from data packages and moves the data data package to the front of the qeue 
Base::GetData(byte id, byte* l)
{
		byte d[128]
for (int i = 0; i < numberOfPackages; i++){
	if(id[i] == id){
		*l = length[i];
		memcpy(d,data[i],128);
		for(int j = 0; j < 8; j++){
			length[j]=length[j+1];
			data[j]=data[j+1];
			id[j]=id[j+1];
		}
		break;
}
return d;
}
