#include "serial_port.h"

class CPort:public CSerial
{
    int numer;
	void FoldLine( unsigned char *Buffer, int BufferLength );
public:
    CPort();
    virtual void OnConnect();
    virtual void OnData();
	virtual void OnInvalid();
	virtual void OnValid();
	virtual void OnNewScan();
};
