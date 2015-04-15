#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H
#include <stdio.h>
#include <string.h>

#ifdef __linux__
	#include <termios.h>
	#include <sys/ioctl.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <limits.h>
#endif

#ifdef _WIN32
	#include <windows.h>
#endif
#include "vector"


#define THREAD_SLEEP			20
#define EOL_LENGTH				1
#define NUMBER_OF_PORTS			256
#define BAUD_LENGTH				7
#define PORT_NAME_LENGTH    	16
#define PORT_STRING_LENGTH  	16
#define BUFFER_LENGTH			1024
#define _MAX_ZERO_COUNTER 		3000/THREAD_SLEEP
#define RECCONNECT_COUNTER		5000/THREAD_SLEEP
#define TALKER_LENGTH			2
#define MAX_TALKER_COUNTER		20			// przez 50 lini brak nowego talkera wtedy OnNoNewTalker

typedef struct
{
	int port_number;
	char port_name[PORT_NAME_LENGTH];
	char port_string[PORT_STRING_LENGTH];
} SPorts;

typedef struct 
{
	char *name;
	char *nmea;
	int count;

}SSignal;

typedef struct 
{
	char *talker;

}STalker;



class CSerial
{
	char m_SerialBuffer[BUFFER_LENGTH];
#if defined(_WIN32) || defined(_WIN64)
	static DWORD WINAPI _W32Thread(void *Param);
	HANDLE m_ComPort;
	DWORD threadID;
	HANDLE m_ThreadHANDLE;
#endif

#if defined(_LINUX32) || defined(_LINUX64)
	int m_ComPort;
	struct termios m_OldPortSettings;
#endif
	std::vector <SPorts> vPorts;
	std::vector <SPorts>::iterator itvPorts;
	std::vector <SSignal> vSignals;				//sygnaly NMEA

	std::vector <STalker> vTalkers;
	
	size_t m_LineBufferLength;
	int m_NoNewSignalCounter;
	bool m_CheckCRC;
	int m_BadCrc,m_LinesCount,m_LinesWritten;
	int m_EOLen;
	char *m_LineBuffer;
	char *m_OldLineBuffer;
	int m_OldLineLength;
//	unsigned char _LineBuffer[BUFFER_LENGTH];
	int m_EmptyCount;			// flaga jest decrementowana kiedy bufor odczytu z pliku jest rowny 0
	int m_NumberOfPorts;		// iloœæ portów do skanowania
	//int m_BaudIndex;			// index baudrate w tablicy baud rate
	//int m_PortIndex;			// index portu w tablicy portów
	bool m_OpenPort;			// otwarty port pomyœlnie
	int m_Baud;
	char m_Port[PORT_NAME_LENGTH];
	int m_BufferLength;
	bool m_Stop;
	bool m_Connected;
	int m_Errors;
	bool m_ValidDevice;
	bool m_Working;
	bool m_FirstTime;
	bool m_ValidNMEA;
	bool m_Writer;
	int m_ReconnectCounter;
	bool m_PharseLine;
	bool m_NoNewSignal;
	int m_TalkerCounter;

	void StartThread();
	void OpenPort(const char*, int);
#if defined(_WIN32) || defined(_WIN64)
	int ReadPort(HANDLE port, char *, int);
	int WritePort(HANDLE port, char *buf, int size);
#endif

#if defined(_LINUX32) || defined(_LINUX64)
	int ReadPort(int port,unsigned char *buf, int size);
	int WritePort(int port,unsigned char *buf, int size);
#endif

	void FoldLine(  char *Buffer, int BufferLength );
	void PharseLine( char *_Data, int _DataLen );
	int NMEASignal( char *Line);
	void ClearLineBuffer(void);
	bool CheckChecksum(const char *nmea_line);
	void ClearSignals();
	bool Connect(const char *port, int baud);
	void AddSignal(char *data, char *to);
	void AddTalker(char *data);

public:

	CSerial();
	virtual ~CSerial();
	int GetBaudInfo(int id);
	size_t GetBaudInfoLength();
	size_t GetPortInfoLength();
	SPorts GetPortInfo(int id);
	void ShowError();
	//int GetDeviceType();
	bool Connect();
	void Disconnect();
	int GetLength();				// buffer length
	bool GetStop();
	int GetBaudRate();
	 char *GetBuffer();
	bool IsConnected();
	void SetIsConnected(bool val);
	void ResetErrors();				
	void IncrementErrors();
	int GetErrors();
	bool GetWorkingFlag();
	void SetWorkingFlag(bool stop);
	const char *GetPortName();            // nazwa portu
	void ScanPorts();
#if defined(_LINUX32) || defined(_LINUX64)
	bool Scan(char *port_name);
#endif
#if defined(_WIN32) || defined(_WIN64)
	HANDLE GetHandle();
#endif

	std::vector <SPorts>GetPorts();
	void Stop(bool wait = true);					// set stop flag
	void Start();					// starts the connect thread
	int Read();
	int Write(char *buffer, int length);
	void SetLength(int size);
	bool Reconnect();
    //void SetNumberOfPorts(int val);
	void _SetPort(const char *port);
	void SetBaud(int baud);
	size_t GetSignalCount();
	SSignal *GetSignal(int idx);
	void SetCheckCRC(bool val);
	size_t GetBadCRC();
	size_t GetLinesCount();
	size_t GetSignalQuality();
	bool GetValidNMEA();
	void SetWriter(bool val);
	bool GetWriter();
	int GetLinesWriten();
	void SetParseLine(bool value);
	void ClearTalkers();
	size_t GetTalkerCount();
	char *GetTalker(size_t id);
	char *GetLineBuffer();
	size_t GetLineBufferLength();
	void SetStop(bool v);
					
	virtual void OnConnect();
	virtual void OnConnected();
	virtual void OnDisconnect();
	virtual void OnData(char *buffer, int length);
	virtual void OnExit();
	virtual void OnLine(char *buffer, int length);
	virtual void OnNMEALine(char *buffer, int length);
	virtual void OnStop();			// stop pressed or stopped
	virtual void OnStart();			// start
	virtual void OnAfterMainLoop();
	virtual void OnBeforeMainLoop();
	virtual void OnReconnect();
	virtual void OnNewSignal(SSignal signal); // nowy znaleziony typ danych w sygnale
	virtual void OnNoSignal();
	virtual void OnValidNMEA();
	virtual void OnInvalidNMEA();
	virtual void OnBadCRC();
	virtual void OnNewTalker(STalker talker);
	virtual void OnNoNewTalker();

};

#endif
