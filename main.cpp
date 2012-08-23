#include <iostream>
#include "main.h"
#include <stdio.h>
#include <string.h>

#define POS_IN_BUF( ptr1, ptr_Buffer ) ( ((size_t)ptr_Buffer)-((size_t)ptr1) )
#define BUFFER_LENGTH 1024
unsigned char _LineBuffer[BUFFER_LENGTH];
std::vector<char*> vLines;

CPort::CPort():CSerial(DEVICE_GPS)
{
    
}

void CPort::OnConnect()
{
	printf("onConnect: %s %d\n",GetPortName(),GetBaudRate());
}

void CPort::OnData(unsigned char *buffer, int length)
{
	//printf("%s",buffer);
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

void CPort::OnLine(unsigned char *line)
{
	//printf("LINE:%d%s\n",strlen((char*)line),line);
	//return;
	unsigned char *ptr = (unsigned char*)memchr(line,',',strlen((char*)line));
	unsigned char *buf = (unsigned char*)malloc( ptr - line + 1 );
	memset(buf,0,ptr - line + 1);
	memcpy(buf,line, ptr - line);
	bool add = true;

	if(vList.size() == 0)
	{
		vList.push_back(buf);
	
	}else{
		
		for( int i = 0; i < vList.size();i++)
		{	
			if( memcmp(vList[i],buf,ptr - line + 1) == 0)
			{
				add = false;
				break;
			}
		}
		
	}

	if(add)
	{	
		system("cls");
		printf("FOUND NEW %s\n",buf);
		vList.push_back(buf);
		for( int i = 0; i < vList.size();i++)
		{
			
			printf("%d : \t%s\n",i,vList[i]);
		}
	}else{
	
		free(buf);
	}

}


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
	system("pause");
	Serial1->Stop();
    
	delete Serial1;

	_CrtDumpMemoryLeaks();
}
