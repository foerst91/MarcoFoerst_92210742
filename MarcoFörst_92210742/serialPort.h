#pragma once															//Schutz vor mehrfachen Einbindungen von Bibliotheken

#include <string>														//Einbinden Standardbibliotheken
#include <iostream>
#include <Windows.h>


class SerialPort {														//Klasse für Serielle Verbindung deklarieren
private:
	HANDLE hCom;														//Handle für serielle Schnittstelle	
	std::string portName;												//Name der seriellen Schnittstelle
	bool isConnected;													//Verbindungsstatus

public:																	
	SerialPort(const std::string& port);								//Konstruktor mit Portname

	~SerialPort();														//Destruktor zum Schließen der Schnittstelle

	bool openPort();													//Öffnen der seriellen Schnittstelle

	std::string sendAndReceive(const std::string& message);				//Senden und Empfangen von Daten

	void closePort();													//Schließen der seriellen Schnittstelle

};
