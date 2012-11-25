#include "serial_port.h"


typedef struct 
{
	unsigned char *name;
	int count;

}SLine;

class CPort:public CSerial
{
	int data_len;
	void FoldLine( unsigned char *Buffer, int BufferLength );
	bool CheckChecksum(const char *nmea_line);

public:
    CPort();
    virtual void OnConnect();
    virtual void OnData(unsigned char* buffer, int length);
	virtual void OnLine(unsigned char* line);
	virtual void OnStop();
	virtual void OnNewSignal();
};
