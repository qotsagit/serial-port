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

#define _GPS				"$GP"
#define _GPSAIS				"$AI"
#define _AIS				"!AI"

#define DEVICE_GPS			0
#define DEVICE_AIS			1

#define BUFFER				4096

#define NUMBER_OF_PORTS		256
#define BAUD_LENGTH			5

#define PORT_NAME_LENGTH    16
#define PORT_STRING_LENGTH  16

typedef struct
{
	int port_number;
	char port_name[PORT_NAME_LENGTH];
	char port_string[PORT_STRING_LENGTH];
} SPorts;

class CSerial
{
	unsigned char m_SerialBuffer[BUFFER];
#ifdef _WIN32
	static DWORD WINAPI _W32Thread(void *Param);
#endif

	HANDLE m_ComPort;
	std::vector <SPorts> vPorts;
	std::vector <SPorts>::iterator itvPorts;

	int m_emptyCount;			// flaga jest decrementowana kiedy bufor odczytu z pliku jest rowny 0
	int m_DeviceType;
	int m_NumberOfPorts;		// iloœæ portów do skanowania
	int m_BaudIndex;			// index baudrate w tablicy baud rate
	int m_PortIndex;			// index portu w tablicy portów
	bool m_OpenPort;			// otwarty port pomyœlnie
	int m_BaudRate;
	int m_BufferLength;
	bool m_Stop;
	bool m_Connected;
	int m_nErrors;			// ilosc blednych polaczen
	bool m_ValidDevice;
	bool m_Working;
	char m_PortName[PORT_NAME_LENGTH];        // nazwa portu aktualnie otwieranego/uzywanego
	bool m_FirstTime;
	DWORD threadID;

#if defined(_WIN32) || defined(_WIN64)
	HANDLE m_ThreadHANDLE;
#endif
	void BuildPorts();
	
	void StartThread();
	void OpenPort(char*, int);
	int ReadPort(HANDLE port, unsigned char *, int);
	
public:

		CSerial(int device_type);
		virtual ~CSerial();
		int GetBaudInfo(int id);
		size_t GetBaudInfoLength();
		size_t GetPortInfoLength();
		SPorts GetPortInfo(int id);
		void ShowError();
		int GetDeviceType();
		bool Connect(char port_string[PORT_STRING_LENGTH], int baud_rate);
		void Disconnect();
		int GetLength();				// buffer length
		bool GetStop();
		int GetBaudRate();
		unsigned char *GetBuffer();
		bool GetIsConnected();
		int GetnErrors();				//retrun number of connection try errors
		void SetnErrors(int n);			// set number of connection tries
		bool IsValidDevice();			// serial port data is really device data
		bool GetWorkingFlag();
		void SetWorkingFlag(bool stop);
		char *GetPortName();            // nazwa portu
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
        void SetNumberOfPorts(int val);
		void SetPortIndex(int id);
		void SetBaudIndex(int id);
		int GetPortIndex();
		int GetBaudIndex();
				
		virtual void OnConnect();
		virtual void OnDisconnect();
		virtual void OnData();
		virtual void OnExit();			// no gps found plugin ends working
		virtual void OnValid();			// valid gps
		virtual void OnInvalid();		// not valid gps
		virtual void OnStop();			// stop pressed or stopped
		virtual void OnStart();			// start
		virtual void OnNewScan();		// new scan begins
		virtual void OnAfterMainLoop();
		virtual void OnBeforeMainLoop();
		virtual void OnReconnect();
};

