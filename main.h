#include "serial_port.h"


typedef struct 
{
	unsigned char *name;
	int count;

}SLine;

class CPort:public CSerial
{
	int bad_crc;
	int lines;
    std::vector<SLine> vList;
	void FoldLine( unsigned char *Buffer, int BufferLength );
	bool CheckChecksum(const char *nmea_line);

public:
    CPort();
    virtual void OnConnect();
    virtual void OnData(unsigned char* buffer, int length);
	virtual void OnLine(unsigned char* line);
	virtual void OnInvalid();
	virtual void OnValid();
	virtual void OnNewScan();
};
