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
