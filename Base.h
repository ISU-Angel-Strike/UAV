
#define		ST_IDLE		1
#define		ST_ID		2
#define		ST_LENGTH	3
#define		ST_DATA		4
#define     	idValue		55
class Base
{
public:
		Base();
		byte* GetData(byte id, byte* l);
		Send(byte id, byte length,byte data);
        init(byte id, byte length);
        State(byte currentState, byte id, byte length, byte data);
private:
		byte length[8];
		byte data[8][128];
		byte id[8];
		byte currentState;
		byte idNum;
		byte idCheck;
		int numberOfPackages;
		
};
