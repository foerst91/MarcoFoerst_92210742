void setup() {
  Serial.begin(115200);                                                         //Serielle Verbindung Baudrate setzen. Bytesize, Parity und Stopbits im Default auf 8N1
  while(!Serial);                                                               //Schleife bis Serielle Verbindung besteht
  delay(1000);                                                                  //Verzögerung bis nächste Funktion aufgerufen
}

void loop() {                                                                   //Schleife zum lesen eines String in der Seriellen Verbindung
  if (Serial.available() > 0) {                                                 //prüft ob Daten zum lesen verfügbar sind
    String input = Serial.readStringUntil('\n');                                //lesen des String
    input.trim();                                                               //entfernt Lerzeichen, Tabs und Zeilenumbrüche am Anfang und Ende

    calculation(input);                                                         //Funktion deklarieren
      
  }
}

void calculation(const String& input){                                          //Funktion definieren
  int zahl1, zahl2;
  char op;

  int parsen = sscanf(input.c_str()," %d %c %d", &zahl1, &op, &zahl2);          //String Parsen und Zahlen als int, sowie Operand als Char speichern
  if(parsen != 3){
    Serial.print("Fehler beim parsen");                                         //Fehlermeldung an PC übermitteln
    return;
  }

  if (zahl2 == 0 && op == '/'){                                                 //Prüfen auf Fehler Division durch 0
    Serial.print("Fehler: Division durch 0 nicht moeglich");                    //Fehlermeldung an PC übermitteln
    return;
  }

  String result;                                                                //String für das Ergebnis erstellen

  switch(op){                                                                   //switch-case erstellen abfrage der Rechenoperation, enstprechend berechnen und als String in result speichern
    case '+':{
      result = String(zahl1 + zahl2);
      break;
    }

    case '-':{
      result = String(zahl1 - zahl2);
      break;
    }

    case '*':{
      result = String(zahl1 * zahl2);
      break;
    }

    case '/':{                                                                 
      float division = (float)zahl1 / (float)zahl2;                              //Bei Division als Float aufgrund Nachkommastellen
      result = String(division, 5);                                              //Float als String mit 5 Nachkommastellen in result speichern
      break;
    }
  }
  Serial.println(result);                                                        //result an PC übermitteln
}
