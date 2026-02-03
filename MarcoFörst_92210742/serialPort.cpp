#include "serialPort.h"											//Einbinden der Header Datei für die SerialPort Klasse


SerialPort::SerialPort(const std::string& port) {				//Konstruktor erstellt Objekt mit Portname als Parameter 
		portName = port;										//Portname aus Konstruktor zuweisen 										
		isConnected = false;									//Verbindungsstatus auf false setzen
		hCom = INVALID_HANDLE_VALUE;							//Handle auf ungültigen Wert setzen
		
		
		
	}

	SerialPort::~SerialPort() {									//Destruktor schließen der seriellen Schnittstelle
		closePort();
	}

	bool SerialPort::openPort() {								//Funktion zum Öffnen der seriellen Schnittstelle
		
		hCom = CreateFileA(										//CreateFileA Funktion zum Öffnen der seriellen Schnittstelle
			portName.c_str(),									//Port des ESP
			GENERIC_READ | GENERIC_WRITE,						//Lese- und Schreiberechte
			0,													//kein Teilen des Ports
			NULL,												//Standard Sicherheitsattribute
			OPEN_EXISTING,										//Port existiert
			0,													//Keine speziellen Attribute
			NULL												//Kein Template Handle
		);

		if (hCom == INVALID_HANDLE_VALUE) {						//Überprüfen ob der Port erfolgreich geöffnet wurde
			//lastError = GetLastError();
			isConnected = false;
			return false;
		}

		DCB dcb;												//Struktur für die Konfiguration der seriellen Schnittstelle
		SecureZeroMemory(&dcb, sizeof(DCB));					//Struktur mit Nullen füllen
		dcb.DCBlength = sizeof(DCB);							//Länge der DCB Struktur setzen

		if (!GetCommState(hCom, &dcb)) {						//wenn das Abrufen der aktuellen Port Parameter fehlschlägt, Handle schließen und false zurückgeben
			CloseHandle(hCom);									
			isConnected = false;
			return false;
		}
																//Standardkonfiguration der Port Parameter zur Seriellen Verbindung zum Mikrocontroller
		dcb.BaudRate = CBR_115200;								//Baudrate setzen
		dcb.ByteSize = 8;										//Größe Datenbits setzen
		dcb.StopBits = ONESTOPBIT;								//Stopbit setzen
		dcb.Parity = NOPARITY;									//Kein Paritätsbit setzen
		
		if (!SetCommState(hCom, &dcb)) {						//wenn das Setzen der Port Parameter fehlschlägt, Handle schließen und false zurückgeben
			CloseHandle(hCom);
			isConnected = false;
			return false;
		}

		COMMTIMEOUTS timeouts;									//Struktur für die Timeout Parameter der seriellen Schnittstelle
		if (!GetCommTimeouts(hCom, &timeouts)) {				//wenn das Abrufen der aktuellen Timeout Parameter fehlschlägt, Handle schließen und false zurückgeben
			CloseHandle(hCom);
			isConnected = false;
			return false;
		}

		timeouts.ReadIntervalTimeout = 50;						//Maximal zulässige Zeit bis zum Eintreffen des nächsten Byte
		timeouts.ReadTotalTimeoutMultiplier = 10;				//Multiplizierer für die Gesamtwartezeit beim Lesen
		timeouts.ReadTotalTimeoutConstant = 1200;				//Konstante für die Gesamtwartezeit beim Lesen
		timeouts.WriteTotalTimeoutConstant = 100;				//Konstante für die Gesamtwartezeit beim Schreiben	
		timeouts.WriteTotalTimeoutMultiplier = 10;				//Multiplizierer für die Gesamtwartezeit beim Schreiben

		if (!SetCommTimeouts(hCom, &timeouts)) {				//wenn das Setzen der Timeout Parameter fehlschlägt, Handle schließen und false zurückgeben
			CloseHandle(hCom);
			isConnected = false;
			return false;
		}

		isConnected = true;										//Wenn Port geöffnet, dcb und timeouts erfolgreich gesetzt wurden, Verbindungsstatus auf true sowie true zurückgeben
		return true;												
		
	}

	void SerialPort::closePort() {								//Funktion zum Schließen der seriellen Schnittstelle
		if (isConnected) {										//wenn verbunden, Handle schließen, hCom auf ungültigen Wert setzen und Verbindungsstatus auf false setzen
			CloseHandle(hCom);
			hCom = INVALID_HANDLE_VALUE;
			isConnected = false;
		}
	}

	std::string SerialPort::sendAndReceive(const std::string& message)									//Funktion zum Senden und Empfangen von Daten
	{
		if (!isConnected) {																				//Überprüfen ob der Port verbunden ist
			return "Port nicht verbunden.";																//Rückgabe bei nicht verbundener Schnittstelle
		}
		DWORD bytesWritten;
		if (!WriteFile(hCom, message.c_str(), (DWORD)message.length(), &bytesWritten, NULL)) {			//Daten an den Mikrocontroller senden
			return "Fehler beim Senden der Rechenoperation an den Mikrocontroller.";					//Rückgabe bei Fehler				
		}
		if (bytesWritten != (DWORD)message.length()) {													//Überprüfen ob alle Daten gesendet wurden
			return "Rechenoperation nicht vollständig übertragen.";										//Rückgabe bei unvollständiger Übertragung
		}

		char buffer[64] = { 0 };																		//Puffer für empfangene Daten
		DWORD bytesRead = 0;																			//Anzahl der gelesenen Bytes
		if (!ReadFile(hCom, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {							//Daten vom Mikrocontroller empfangen
			return "Fehler beim Empfangen des Ergebnis vom Mikrocontroller.";							//Rückgabe bei Fehler
			}
		if (bytesRead == 0)																				//Überprüfen ob Daten empfangen wurden
			return "Kein Ergebnis vom Mikrocontroller erhalten";										//Rückgabe bei keiner empfangenen Daten
			
		buffer[bytesRead] = '\0';																		//Nullterminator für den String
		return std::string(buffer);																		//Empfangenes Ergebnis als String zurückgeben
	}



