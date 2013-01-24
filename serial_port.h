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


#define BUFFER				4096
#define EOL_LENGTH			1

#define NUMBER_OF_PORTS		256
#define BAUD_LENGTH			7

#define PORT_NAME_LENGTH    16
#define PORT_STRING_LENGTH  16


#define BUFFER_LENGTH 1024

typedef struct
{
	int port_number;
	char port_name[PORT_NAME_LENGTH];
	char port_string[PORT_STRING_LENGTH];
} SPorts;

typedef struct 
{
	unsigned char *name;
	unsigned char *nmea;
	int count;

}SSignal;

class CSerial
{
	unsigned char m_SerialBuffer[BUFFER + 1];
#ifdef _WIN32
	static DWORD WINAPI _W32Thread(void *Param);
#endif

	HANDLE m_ComPort;
	std::vector <SPorts> vPorts;
	std::vector <SPorts>::iterator itvPorts;
	std::vector <SSignal> vSignals;				//sygnaly NMEA
		
	bool m_CheckCRC;
	int m_BadCrc,m_LinesCount;
	int m_EOLen;
	char m_LineBuffer[BUFFER_LENGTH];
	char *m_OldLineBuffer;
	int m_OldLineLength;
	unsigned char _LineBuffer[BUFFER_LENGTH];
	int m_emptyCount;			// flaga jest decrementowana kiedy bufor odczytu z pliku jest rowny 0
	int m_NumberOfPorts;		// iloœæ portów do skanowania
	//int m_BaudIndex;			// index baudrate w tablicy baud rate
	//int m_PortIndex;			// index portu w tablicy portów
	bool m_OpenPort;			// otwarty port pomyœlnie
	int m_Baud;
	char m_Port[PORT_NAME_LENGTH];
	int m_BufferLength;
	bool m_Stop;
	bool m_Connected;
	int m_nErrors;			// ilosc blednych polaczen
	bool m_ValidDevice;
	bool m_Working;
	bool m_FirstTime;
	DWORD threadID;

#if defined(_WIN32) || defined(_WIN64)
	HANDLE m_ThreadHANDLE;
#endif
	
	
	void StartThread();
	void OpenPort(const char*, int);
	int ReadPort(HANDLE port, unsigned char *, int);
	void FoldLine( unsigned char *Buffer, int BufferLength );
	void PharseLine( char *_Data, int _DataLen );
	void NMEASignal(unsigned char *Line);
	void ClearLineBuffer(void);
	bool CheckChecksum(const char *nmea_line);
	void ClearSignals();
	
public:

	CSerial();
	virtual ~CSerial();
	int GetBaudInfo(int id);
	size_t GetBaudInfoLength();
	size_t GetPortInfoLength();
	SPorts GetPortInfo(int id);
	void ShowError();
	//int GetDeviceType();
	bool Connect(const char *port, int baud);
	void Disconnect();
	int GetLength();				// buffer length
	bool GetStop();
	int GetBaudRate();
	unsigned char *GetBuffer();
	bool IsConnected();
	int GetnErrors();				//retrun number of connection try errors
	void SetnErrors(int n);			// set number of connection tries
	bool GetWorkingFlag();
	void SetWorkingFlag(bool stop);
	const char *GetPortName();            // nazwa portu
	void ScanPorts();
#if defined(_LINUX32) || defined(_LINUX64)
	bool Scan(char port_name[PORT_NAME_LENGTH]);
#endif
#if defined(_WIN32) || defined(_WIN64)
	HANDLE GetHandle();
#endif

	std::vector <SPorts>GetPorts();
	void Stop();					// set stop flag
	void Start();					// starts the connect thread
    int Read();
    void SetLength(int size);
    bool Reconnect();
    //void SetNumberOfPorts(int val);
	void SetPort(char *port);
	void SetBaud(int baud);
	size_t GetSignalCount();
	SSignal *GetSignal(int idx);
	void SetCheckCRC(bool val);
	size_t GetBadCRC();
	size_t GetLinesCount();
	size_t GetSignalQuality();
				
	virtual void OnConnect();
	virtual void OnDisconnect();
	virtual void OnData(unsigned char *buffer, int length);
	virtual void OnExit();			
	virtual void OnLine(unsigned char *buffer);
	virtual void OnStop();			// stop pressed or stopped
	virtual void OnStart();			// start
	virtual void OnAfterMainLoop();
	virtual void OnBeforeMainLoop();
	virtual void OnReconnect();
	virtual void OnNewSignal(); // nowy znaleziony typ danych w sygnale
	virtual void OnNoSignal();
};

