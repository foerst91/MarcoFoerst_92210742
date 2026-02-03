#include <string>																					//Einbinden von Standardbibliotheken sowie Header Dateien
#include <iostream>
#include <limits> 
#include "serialPort.h"
#include "log.h"
																									//Sammlung der globalen Variablen
char newCalc;
int number1, number2;
std::string op;
std::string userInput;
std::string calculation;
std::string port;
std::string result;


void menu() {																						//Funktion zur Anzeige des Menüs
	std::cout << "\nMikrocontroller Kalkulator \n" << std::endl;
	std::cout << "1 = Addieren + \n"
		"2 = Subtrahieren - \n"
		"3 = Multiplizieren * \n"
		"4 = Dividieren / \n"
		"5 = Hilfe H \n"
		"6 = Beenden B/Q \n" << std::endl;

	std::cout << "Geben Sie die gewuenschte Rechenoperation ein: ";
	std::cin >> op;																					//Eingabe der gewünschten Rechenoperation
}

void input() {																						//Funktion zur Eingabe der Zahlen, mit Validierung
	while (true) {																					//Endlosschleife bis gültige Eingabe
		std::cout << "Eingabe erste Zahl: ";
		std::cin >> userInput;
		size_t pos;																					//Variable zur Überprüfung der gesamten Eingabe
		try {
			number1 = std::stoi(userInput, &pos);													//Funktion zur Umwandlung String in Integer
		}
		catch (...) {																				//Catch Block für ungültige Eingaben
			std::cout << "Ungueltige Eingabe. Bitte ganze Zahl eingeben.\n";
			continue;
		}

		if (pos != userInput.size()) {																//Vergleich der Position um sicherzustellen, dass die gesamte Eingabe eine Zahl ist
			std::cout << "Ungueltige Eingabe. Bitte ganze Zahl eingeben.\n";
			continue;
		}
		if (number1 < -32768 || number1 > 32767) {													//Überprüfung ob die Zahl im gültigen Bereich von Integer liegt
			std::cout << "Ungueltige Eingabe. Zahl muss zwischen -32768 und 32767 liegen.\n";
			continue;
		}
		std::cout << "Eingabe zweite Zahl: ";														//Eingabe Zahl 2 analog zu Zahl 1
		std::cin >> userInput;
		try {
			number2 = std::stoi(userInput, &pos);
		}
		catch (...) {
			std::cout << "Ungueltige Eingabe. Bitte ganze Zahl eingeben.\n";
			continue;
		}
		if (pos != userInput.size()) {
			std::cout << "Ungueltige Eingabe. Bitte ganze Zahl eingeben.\n";
			continue;
		}
		if (number2 < -32768 || number2 > 32767) {
			std::cout << "Ungueltige Eingabe. Zahl muss zwischen -32768 und 32767 liegen.\n";
			continue;
		}
		break;
	}
}


	
int main() {																																				//Hauptprogramm
	std::cout << "Geben Sie den COM-Port des Mikrocontrollers in Form von COMx ein: ";
	std::cin >> port;																																		//Eingabe des COM Ports
	SerialPort serial(port);																																//Objekt der SerialPort Klasse erstellen
	if (!serial.openPort()) {																																//Öffnen der seriellen Schnittstelle
		std::cout << "Fehler beim Oeffnen des Ports, pruefe ob COMx korrekt eingegeben und der Mikrocontroller verbunden ist " << port << std::endl;		//Fehlermeldung bei Verbindungsfehler
		return 1;																																			//Programm beenden
		}

	Log log("KalkulatorLog.txt");																															//Objekt der Log Klasse erstellen, Text Datei zum Protokollieren erstellen/öffnen
	if (!log.isOpen()) {																																	//Überprüfen ob die Datei geöffnet ist
		std::cout << "Fehler beim oeffnen der Textdatei!" << std::endl;																						//Fehlermeldung bei Fehler
		return 1;																																			//Programm beenden
	}

	while (true) {																																			//Endlosschleife für das Hauptmenü
		system("cls");																																		//Konsole wird gelöscht
		menu();																																				//Menü Funktion aufrufen
		if (op == "1" || op == "+") {																														//if Abfragen für die Auswahl der Rechenoperation
			op = "+";																																		//Setzen des Operators
			input();																																		//Aufruf der Eingabefunktion
			}
			else if (op == "2" || op == "-") {		
			op = "-";	
			input();		
			}
			else if (op == "3" || op == "*") {					
			op = "*";
			input();
			}
			else if (op == "4" || op == "/") {
			op = "/";
			input();
			}
			else if (op == "5" || op == "H" || op == "h") {																													//Hilfe Anzeige
			std::cout << "\nDer Mikrocontroller Kalkulator unterstuetzt Ganzzahlen von -32768 bis 32767 sowie die vier Grundrechenarten + , - , * , /  \n" << std::endl;	//Hilfe Text ausgeben
			system("pause");																																				//Warten auf Benutzereingabe
			continue;																																						//Menü wird erneut angezeigt
			}
			else if (op == "6" || op == "B" || op == "b" || op == "Q" || op == "q") {																						//Programm beenden
			break;																																							//Schleife verlassen
			}
			else {																																									
			std::cout << "\nungueltige Eingabe\n" << std::endl;																												//Ungültige Eingabe, Menü wird erneut angezeigt
			system("pause");
			continue;
			}

		calculation = std::to_string(number1) + " " + op + " " + std::to_string(number2);																					//Rechenoperation als String zusammenfügen																			
		std::cout << "\n" << calculation << "         (zum berechnen an Mikrocontroller gesendet)" << std::endl;															//Anzeige der Rechenoperation
			

		result = serial.sendAndReceive(calculation + "\n");																													//Rechenoperation an Mikrocontroller senden und Ergebnis empfangen
		if (result.find("Fehler") != std::string::npos) {																													//Überprüfen ob eine Fehlermeldung empfangen wurde
			std::cout << "\n" << result << std::endl;																														//Fehlermeldung ausgeben
			log.write("Rechenoperation: " + calculation + "\n" + result);																									//Fehlermeldung in die Log Datei schreiben
			system("pause");																																				//Warten auf Benutzereingabe
			continue;
			}
		std::cout << "\nErgebnis: " << result << std::endl;																													//Ergebnis Ausgabe
		log.write("Rechenoperation: " + calculation + " = " + result);																										//Rechenoperation und Ergebnis in die Log Datei schreiben



			while (true) {																																					//Schleife für Abfrage ob weitere Berechnung ausgeführt werden soll
				std::cout << "\n\nWeitere Berechnung ausfuehren? J/N: ";																									
				std::cin >> newCalc;
				if (newCalc == 'J' || newCalc == 'j') {
					system("cls");																																			//Konsole wird gelöscht und Menü erneut angezeigt
					break;
				}
				else if (newCalc == 'N' || newCalc == 'n') {
					return 1;
				}
				else {
					std::cout << "ungueltige Eingabe" << std::endl;																											//Ungültige Eingabe, Abfrage wird erneut gestellt
					continue;
				}
			}
		}

	serial.closePort();																																						//Serielle Schnittstelle schließen

	return 0;																																								//Programm erfolgreich beenden
	}	
