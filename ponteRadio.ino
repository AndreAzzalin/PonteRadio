#include "SoftwareSerial.h";
#include <Sim800l.h>

SoftwareSerial mySerial(3,2);  // (Rx,Tx  > Tx,Rx) 

//defiine libreria
Sim800l Sim800l;

float voltaggio;
float temp_radio1;
int temp_radio2;
char incomingByte; 
String inputString;
int relay_ponte = 6; // Output for Relay Control
int relay_ventole = 8;
int statorelay_ponte;

bool tempAlert=false;

void setup() 
{
      pinMode(relay_ponte, OUTPUT);
      digitalWrite(relay_ponte, LOW);
      pinMode(relay_ventole, OUTPUT);
      digitalWrite(relay_ventole, LOW); 
      Serial.begin(9600);
      mySerial.begin(9600); 

     while(!mySerial.available()){
        mySerial.println("AT");
        delay(1000); 
        Serial.println("Connessione in corso...");
        }
      Serial.println("Connesso !");  
      
      mySerial.println("AT+CMGF=1");  //Set SMS to Text Mode 
      delay(1000);  
      mySerial.println("AT+CNMI=1,2,0,0,0");  //Procedure to handle newly arrived messages(command name in text: new message indications to TE) 
      delay(1000);
      mySerial.println("AT+CMGL=\"REC UNREAD\""); // Read Unread Messages
     }

void loop(){  
  
  //msg arrivato
  if(mySerial.available()){
      delay(100);

      // Serial Buffer
      while(mySerial.available()){
        incomingByte = mySerial.read();
        inputString += incomingByte; 
        }

        delay(10);      

        Serial.println(inputString);
        inputString.toUpperCase(); // Uppercase the Received Message



        //turn RELAY ON or OFF
        if (inputString.indexOf("ON") > -1){
          digitalWrite(relay_ponte, HIGH);
        }
          
        if (inputString.indexOf("OFF") > -1){
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
          
          statorelay_ponte= digitalRead(relay_ponte);
        
            
          if(statorelay_ponte){
            Serial.println("PONTE ATTIVO");
          } else {
            Serial.println("PONTE NON ATTIVO");
          }
         
          delay(1000);
          statorelay_ponte = "";
          
          
        }
        
        //Delete Messages & Save Memory
        if (inputString.indexOf("OK") == -1){
        mySerial.println("AT+CMGDA=\"DEL ALL\"");
        delay(1000);
          
        }

        inputString = "";
  }
  
  voltaggio = analogRead(A0)*0.01368 ;
  temp_radio1= analogRead(A1)/2.046;
      
  //temp_radio2= map(analogRead(A2),0,1023,0,5000)/10;
  //if(temp_radio1 >35 || temp_radio2>35)
  
  
  
  if(temp_radio1>35 || temp_radio2>35){
    //accendi ventole
  }else if(temp_radio1<25 || temp_radio2<25){
    //spegni ventole
  } 

       
  }//end loop
