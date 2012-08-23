#include <iostream>
#include "main.h"
#include <stdio.h>

CPort::CPort():CSerial(DEVICE_GPS)
{
    
}

void CPort::OnConnect()
{
	printf("onConnect: %s %d\n",GetPortName(),GetBaudRate());
}

void CPort::OnData()
{
	printf("%s",GetBuffer());
}

void CPort::OnInvalid()
{
	printf ("\nINVALID: %d\n",GetDeviceType());
}

void CPort::OnValid()
{
	printf ("\nVALID: %d\n",this->GetDeviceType());
}

void CPort::OnNewScan()
{
	printf ("\n NeW SCAN\n");
}
/*
void CPort::FoldLine( unsigned char *Buffer, int BufferLength )
{
    unsigned char *ptr_Buffer, *ptr1;
    ptr1 = ptr_Buffer = Buffer;
    
    while ( *ptr_Buffer )
    {
        ptr_Buffer++;
        if ( !*ptr_Buffer )
        {
            _LineBufLen = POS_IN_BUF( ptr1, ptr_Buffer );
            memset( _LineBuffer, 0, BUFFER_LENGTH );
            memcpy( _LineBuffer, ptr1, _LineBufLen );
            return;
        }

        if ( *ptr_Buffer == 13 )
            ptr_Buffer++;
        if ( *ptr_Buffer == 10 )
        {
            ptr_Buffer++;
            memcpy( ( char *)_LineBuffer + _LineBufLen, ( const char *)ptr1, ( ptr_Buffer-ptr1 ) );
            OnLine(((char*)_LineBuffer, ( ptr_Buffer - ptr1 ) );
            memset( _LineBuffer, 0, BUFFER_LENGTH );
            _LineBufLen = 0;
            ptr1 = ptr_Buffer;
        }
    }

}
*/


int main()
{
    CPort *Serial1 = new CPort();
	
	if (Serial1->GetPortInfoLength() < 1)
	{
		printf("ERROR: no ports in system\n");
		system("pause");
		return 0;
	}
	for(size_t i = 0; i < Serial1->GetPortInfoLength(); i++)
	{
		printf("[%d]: %s\n",i,Serial1->GetPortInfo(i).port_name);
	}
	
	int idport, idbaud;
	printf("Port id ?");
	scanf("%d",&idport);

	for(size_t i = 0; i < Serial1->GetBaudInfoLength(); i++)
	{
		printf("[%d]: %d\n",i,Serial1->GetBaudInfo(i));
	}

	
	
	printf("baud id ?");
	scanf("%d",&idbaud);
	Serial1->SetPortIndex(idport);
	Serial1->SetBaudIndex(idbaud); 
			
	
	Serial1->Start();
	Sleep(300000);
    
	Serial1->Stop();
	delete Serial1;

	_CrtDumpMemoryLeaks();
}
