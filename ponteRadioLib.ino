/*
*
*    PINOUT: 
*        _____________________________
*       |  ARDUINO UNO >>>   SIM800L  |
*        -----------------------------
*            GND      >>>   GND
*        RX  10       >>>   TX    
*        TX  11       >>>   RX
*       RESET 2       >>>   RST 
*
*/

#include <Sim800l.h>
#include <SoftwareSerial.h> //is necesary for the library!! 

Sim800l Sim800l;  //to declare the library
String text;     // to storage the text of the sms
uint8_t index;   // to indicate the message to read.

//variabili 
float voltaggio;
float temp_radio1;
int temp_radio2;
char incomingByte; 
String inputString;
int relay_ponte = 6; // Output for Relay Control
int relay_ventole = 8;
int statorelay_ponte;


void setup(){
  
	Serial.begin(9600); // only for debug the results . 
	Sim800l.begin(); // initializate the library. 
  Sim800l.reset();
  //don't forget to catch the return of the function delAllSms! 
  error=Sim800l.delAllSms(); //clean memory of sms;
  
  //config relay
  pinMode(relay_ponte, OUTPUT);
  pinMode(relay_ventole, OUTPUT);
  digitalWrite(relay_ponte, LOW);
  digitalWrite(relay_ventole, LOW); 
}


void loop(){

  
    //esegui azioni in base all'SMS ricevuto
    if (textSms.indexOf("ON") > -1){
      Serial.println("RELAY PONTE ON");
      digitalWrite(relay_ponte, HIGH);
    }
          
    if (textSms.indexOf("OFF") > -1){
      Serial.println("RELAY PONTE OFF");
      digitalWrite(relay_ponte, LOW);
    }
  
     if (inputString.indexOf("STATUS") > -1){
          delay(100);
        
          voltaggio = analogRead(A0)*0.01368 ;
          temp_radio1= map(analogRead(A1),0,1023,0,5000)/10;
          temp_radio2= map(analogRead(A2),0,1023,0,5000)/10;
          Serial.print("TEMP_RADIO 1: ");
          Serial.print(temp_radio1);
          Serial.println("°C");
          Serial.print("TEMP_RADIO 2: ");
          Serial.print(temp_radio2);
          Serial.println("°C");
          Serial.print("TENSIONE BATTERIA: ");
          Serial.print(voltaggio);
          Serial.println("V");
          delay(100);
          
          //in teoria prende solo i valori 0 e 1 non so se considera il tipo nell'IF      
          statorelay_ponte= digitalRead(relay_ponte);
        
            
          if(statorelay_ponte){
            Serial.println("PONTE ATTIVO");
          } else {
            Serial.println("PONTE NON ATTIVO");
          }
         
          delay(1000);
        }
  
        //non sono sicuro vada qua 
        Sim800l.delAllSms(); //do only if the message is not empty,in other case is not necesary
        //delete all sms..so when receive a new sms always will be in first position
  
  
}
