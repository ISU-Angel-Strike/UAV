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
		int numberOfPackages;
		
};
