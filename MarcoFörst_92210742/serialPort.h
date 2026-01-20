#pragma once

#include <Windows.h>


class SerialPort {
public:
	SerialPort();
	~SerialPort();



	bool open(const char* port);
	void close();
	bool isOpen() const;

	bool configure(	DWORD baudRate = 115200, 
					BYTE byteSize = 8, 
					BYTE stopBits = ONESTOPBIT, 
					BYTE parity = NOPARITY);



	DWORD getLastError() const;



private:
	HANDLE hCom; 
	DWORD lastError




};
