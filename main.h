#include "serial_port.h"

class CPort:public CSerial
{
    std::vector<unsigned char*> vList;
	void FoldLine( unsigned char *Buffer, int BufferLength );

public:
    CPort();
    virtual void OnConnect();
    virtual void OnData(unsigned char* buffer, int length);
	virtual void OnLine(unsigned char* line);
	virtual void OnInvalid();
	virtual void OnValid();
	virtual void OnNewScan();
};
