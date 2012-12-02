#include <iostream>
#include "main.h"
#include <stdio.h>
#include <string.h>

unsigned char _LineBuffer[BUFFER_LENGTH];
std::vector<char*> vLines;

CPort::CPort():CSerial()
{

}

void CPort::OnConnect()
{
	printf("onConnect: %s %d\n",GetPortName(),GetBaudRate());
}

void CPort::OnData(unsigned char *buffer, int length)
{
	data_len+=length;
	//printf("%s",buffer);
}

bool CPort::CheckChecksum(const char *nmea_line) {

	const char *xor_chars = "0123456789ABCDEF";
	size_t chcount = 1;
	int _xor = 0;

	size_t len_nmea = strlen(nmea_line);
	while ( (chcount < len_nmea) && ( nmea_line[chcount] != '*') )
	_xor = _xor ^ (unsigned short int)(nmea_line[chcount++]);

	if ( nmea_line[chcount] != '*' ) return false;

	if( (nmea_line[chcount + 1] == xor_chars[_xor >> 4]) && (nmea_line[chcount + 2] == xor_chars[_xor & 0x0F]) )
	    return true;
	else
	    return false;
 
}

void CPort::OnLine(unsigned char *line)
{
	printf("LINE:%d%s\n",strlen((char*)line),line);
	//return;
	//lines++;
	//if(!CheckChecksum((char*)line))
	//{
	//	printf("Bad crc: %d\n",bad_crc);
//		printf("%s",line);
		//Sleep(5);
		//bad_crc++;
		//return;
	//}
	
	//unsigned char *ptr = (unsigned char*)memchr(line,',',strlen((char*)line));
	//unsigned char *buf = (unsigned char*)malloc( ptr - line + 1 );
	//memset(buf,0,ptr - line + 1);
	//memcpy(buf,line, ptr - line);
	//bool add = true;

	//if(vList.size() != 0)
	//{
		//for( int i = 0; i < vList.size();i++)
		//{	
			//if( memcmp(vList[i].name,buf,ptr - line) == 0)
			//{
				//add = false;
				//vList[i].count++;
				//break;
			//}
		//}
		
	//}

	//if(add)
	//{	
		
		//SLine Line;
		//Line.name = buf;
		//Line.count = 1;

		//vList.push_back(Line);
		
	//}else{
	
		//free(buf);
	//}

	//system("cls");
	//for( int i = 0; i < vList.size();i++)
		//printf("%d : \t%s -> %d\n",i,vList[i].name,vList[i].count);
	
	//printf("Bad crc: %d\n",bad_crc);
	//printf("NMEA Lines: %d\n",lines);
	//printf("x potem ENTER - koniec programu. log zapisany w pliku nmealog.txt ");
	//line_len += strlen((char*)line);
	//printf("Data: %d Line %d",data_len,line_len);

}

void CPort::OnStop()
{
	//FILE *f = fopen("nmealog.txt","w");
	
	//for( int i = 0; i < vList.size();i++)
		//fprintf(f,"%d : \t%s -> %d\n",i,vList[i].name,vList[i].count);
	
	//fprintf(f,"Bad crc: %d\n",bad_crc);
	//fprintf(f,"NMEA Lines: %d\n",lines);
	//fclose(f);
}

void CPort::OnNewSignal()
{
	system("cls");
	
	//for( int i = 0; i < vList.size();i++)
		//printf("%d : \t%s -> %d\n",i,vList[i].name,vList[i].count);
	
}

int main()
{
    CPort *Serial1 = new CPort();
	
	Serial1->ScanPorts();
	
	if (Serial1->GetPortInfoLength() < 1)
	{
		printf("ERROR: no ports in system\n");
		system("pause");
		return 0;
	}
	
	for(size_t i = 0; i < Serial1->GetPortInfoLength(); i++)
		printf("%s\n",Serial1->GetPortInfo(i).port_name);

	char idport[8]; 
	int	idbaud;
	
	scanf("%s",idport);

	for(size_t i = 0; i < Serial1->GetBaudInfoLength(); i++)
		printf("%d\n",Serial1->GetBaudInfo(i));
	
		

	scanf("%d",&idbaud);
	Serial1->SetPort(idport);
	Serial1->SetBaud(idbaud); 
	

	Serial1->Start();
	//system("pause");
	system("cls");

	int key = 0;
	for(;;)
	{
		key = getchar();
		if (key == 'x')
		{
			printf("x pressed exiting");
			break;
		}
	}

	Serial1->Stop();
    
	delete Serial1;
	system("pause");
	_CrtDumpMemoryLeaks();
}
