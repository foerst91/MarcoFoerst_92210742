#include <filesystem>									//Einbinden Standardbibliotheken
#include "log.h"										//Einbinden der Header Datei für die Log Klasse

Log::Log(const std::string& newText) {					//Konstruktor mit Dateinamen als Parameter
	log.open(newText, std::ios::app);					//Datei öffnen, im Modus append (anhängen)
}


Log::~Log() {											//Destruktor zum Schließen der Datei, wenn geöffnet
	if (log.is_open()) {								
		log.close();								
	}
}

void Log::write(const std::string& logging) {			//Funktion zum Schreiben in die Datei
	if (log.is_open()) {
		log << logging << std::endl;					//logging String in Datei schreiben
	}
}

bool Log::isOpen() {
	return log.is_open();
}