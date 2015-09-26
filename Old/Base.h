#define		ST_IDLE		1
#define		ST_ID		2
#define		ST_LENGTH	3
#define		ST_DATA		4
#define     	ST_CHECK    	5
#define     	packStart	55
class Base
{
public:
		Base();
		byte* GetData(byte id, byte* l);
		Send(byte id, byte length,byte data);
        	init(byte id, byte length);
        	State(byte currentState, byte id, byte length, byte data);
		void Stock(byte id, byte* data);
        	GetStock(byte id);
private:
		byte data[8][128];
		byte dataStock[8];
		byte tempStock[1][128];
		byte currentState;
		byte stData[1][128];
		byte idNum;
		byte idCheck;
		int numberOfPackages;
        int checkSumState;
        int step;
		
		
};
