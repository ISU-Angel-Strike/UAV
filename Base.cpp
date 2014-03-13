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
			//Checks to see if id packages is in the queue
            int step = 0;
			int checkSumState = 0;
			idCheck = Serial.read();
			if(idCheck == packStart){
				currentState = ST_ID;
				break;
			}else{
				break;
			}
        case ST_ID:
			//checks the id package number
			idNum = Serial.read();
			if(idNum > -1){
				currentState = ST_LENGTH;
				checkSumState ^= idNum;
				break;
			}else{
				break;
			}
        case ST_LENGTH:
			//checks the length of the incoming data
			length = Serial.read();
			if(length > -1){
				currentState = ST_DATA;
				checkSumState ^= length;
				break;
			}else{
				break;
			}
        case ST_DATA:
			//gets the data from the package
            stData[idNum][step] = data;
            checkSumState ^= data;
            step++;
            break;
            if ( step > length)
			currentState = ST_CHECK;
            break;
		case ST_CHECK:
			if(checkSumState == checkSum){
				currentState = ST_IDLE;
				break;
			}else{
				break;
			}
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
Base::Stock(byte id, byte* data)
{
	for(int i =0; i < 8; i++){
		dataStock[i] = memcpy(ds,data[i],128);
	}
}
Base::GetStock(byte id)
{
    byte tempStock[1][128];
	for (int = i; i < 8; i++){
		if(id == dataStock[i][0]){
            memcpy(tempStock[1],dataStock[i])
		}
		for(int j = i; j < 8; j++){
			memcpy(dataStock[j],dataStock[j+1],128);
		}
        return tempStock;
	}
}
