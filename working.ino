#include "SoftwareSerial.h";


SoftwareSerial mySerial(3,2);  // (Rx,Tx  > Tx,Rx) 


float voltaggio;
float temp_radio1;
int temp_radio2;
char incomingByte; 
String inputString;
int relay_ponte = 6; 
int relay_ventole = 5;
int statorelay_ponte;
String Data_SMS;



void setup() 
{
      pinMode(relay_ponte, OUTPUT);
      digitalWrite(relay_ponte, HIGH);
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
          // temp_radio1= (analogRead(A1)/2.046; // togliere commento quando si attivano i sensori di temperatura
          //temp_radio1= (analogRead(A2)/2.046;  // togliere commento quando si attivano i sensori di temperatura
          temp_radio1 = 22.00; //Commentare quando si attivano i sensori di temperatura
          temp_radio2 = 26.00; //Commentare quando si attivano i sensori di temperatura
          statorelay_ponte= digitalRead(relay_ponte);
          mySerial.print("AT+CMGS=\"+393319943523\"\r");  
          delay(500);
          Data_SMS = "Batt: "+String(voltaggio)+" V" + "\nPonte: "+String(statorelay_ponte) + "\nRadio TX: "+String(temp_radio1)+" C"+ "\nRadio RX: "+String(temp_radio2)+" C";   
          mySerial.print(Data_SMS);  //This string is sent as SMS
          delay(500);
          mySerial.print((char)26);//(required according to the datasheet)
          delay(500);
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
     // temp_radio1= (analogRead(A1)/2.046; // togliere commento quando si attivano i sensori di temperatura
     //temp_radio1= (analogRead(A2)/2.046;  // togliere commento quando si attivano i sensori di temperatura
     temp_radio1 = 22.00; //Commentare quando si attivano i sensori di temperatura
     temp_radio2 = 28.00; //Commentare quando si attivano i sensori di temperatura
      
  

  
  
  if(temp_radio1>35 || temp_radio2>35){
    digitalWrite(relay_ventole, HIGH);
  }else if(temp_radio1<25 || temp_radio2<25){
    digitalWrite(relay_ventole, LOW);
  } 

       
  }//end loop
