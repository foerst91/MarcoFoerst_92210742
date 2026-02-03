#pragma once											//Schutz vor mehrfachen Einbindungen von Bibliotheken

#include <fstream>										//Einbinden Standardbibliotheken
#include <string>

class Log {												//Klasse zur Protokollierung deklarieren
	std::ofstream log;									//Datei zum Schreiben öffnen

public:
	Log(const std::string& newText);					//Konstruktor mit Dateinamen

	~Log();												//Destruktor zum Schließen der Datei

	void write(const std::string& logging);				//Schreiben in die Datei

	bool isOpen();										//Überprüfen ob die Datei geöffnet ist
};
