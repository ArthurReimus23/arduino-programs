//Libraries
#include <SoftwareSerial.h>
#include <TinyStepper.h> 

String myAPIkey= "KN3D2M74HPEB38M7";   // Write API KEY

boolean relay1_st = false; 
boolean relay2_st = false; 
unsigned char check_connection=0;
unsigned char times_check=0;
boolean error;


SoftwareSerial ESP8266(2,3); //rx, tx

char btVal = 0; 

#define StepPin1 13
#define StepPin2 12 
#define StepPin3 11 
#define StepPin4 10 
#define HALFSTEPS 4096  // This is the numbers of step for a full 360 degree rotation
TinyStepper stepper(HALFSTEPS, StepPin1, StepPin2, StepPin3, StepPin4); 

//Pinouts of the LED
  const int ledPin=4;

//Status of the Appliances
int ledStatus = 0;
int stepperStatus = 0;

//Millis used as an alternative for delays
  unsigned long currentMillis = 0;
  unsigned long previousMillis =0;
  
void setup() {
  
  Serial.begin(9600); 
  
  pinMode(ledPin, OUTPUT);  
  stepper.Enable(); 
  
  ESP8266.begin(9600);  
  ESP8266.println("AT+RST");
  delay(2000);
  Serial.println("Connecting to Wifi");
   while(check_connection==0)
  {
    Serial.print(".");
  ESP8266.print("AT+CWJAP=\"HUAWEI-2.4G-U2Yg\",\"3kgbKX79\"\r\n");
  ESP8266.setTimeout(5000);
 if(ESP8266.find("WIFI CONNECTED\r\n")==1)
 {
 Serial.println("WIFI CONNECTED");
 break;
 }
 times_check++;
 if(times_check>3) 
 {
  times_check=0;
   Serial.println("Trying to Reconnect..");
  }
  }

}

void loop() {

  ledStatus = 1;
  stepperStatus = 1;
  Serial.print("LED Status: ");
  Serial.println(ledStatus);
  Serial.print("Servo Status: ");
  Serial.println(stepperStatus);
  delay(3000);
//  ledStatus = 0;
//  stepperStatus = 0;
//  Serial.print("LED Status: ");
//  Serial.println(ledStatus);
//  Serial.print("Servo Status: ");
//  Serial.println(stepperStatus);
//  delay(3000);
  

  if(Serial.available() > 0)   {
    btVal = Serial.read();  
    Serial.print("Bluetooth Value Received: ");    
    Serial.println(btVal); 

    if(btVal == '0') {
      digitalWrite(ledPin, LOW);    
      ledStatus = 0;
      delay(100);    
    }

    if(btVal == '0') {
      digitalWrite(ledPin, LOW);    
      ledStatus = 0;
      delay(100);    
    }
    
    if(btVal == '1') {
      digitalWrite(ledPin, HIGH); 
      ledStatus = 1;  
      delay(100);    
    }
    
    if(btVal == '2') {
      Serial.println("Stepper OFF");  
      stepperStatus = 0;
      delay(100);    
    }
    
    if(btVal == '3') {
      Serial.println("Stepper ON");
      stepperStatus = 1;   
      delay(100);    
    }
  } //bluetooth end

  if (ledStatus == 1) { 
    digitalWrite(ledPin, HIGH); 
  }
  if (stepperStatus == 1) { 
    stepperFunction();
  }
  
//Writing to thingspeak
  unsigned long currentMillis = millis();
  
  if ((unsigned long)(currentMillis - previousMillis) >= 20000) {
    writeThingSpeak();
    delay(100);
    previousMillis = currentMillis;
  }
}
  
//Functions ***************************************************************************************************************************************

void stepperFunction() { //Function for moving servo motor from 0 degrees to 180 degrees
  stepper.Move(60); 
  delay(200);
}

void writeThingSpeak(void) {
  startThingSpeakCmd();
  String getStr = "GET /update?api_key=";
  getStr += myAPIkey;
  getStr +="&field1=";
  getStr += String(ledStatus);
  getStr +="&field2=";
  getStr += String(stepperStatus);
  getStr += "\r\n\r\n";
  GetThingspeakcmd(getStr); 
}

void startThingSpeakCmd(void) {
  ESP8266.flush();
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com IP address
  cmd += "\",80";
  ESP8266.println(cmd);
  Serial.print("Start Commands: ");
  Serial.println(cmd);

  if(ESP8266.find("Error"))
  {
    Serial.println("AT+CIPSTART error");
    return;
  }
}

String GetThingspeakcmd(String getStr) {
  String cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ESP8266.println(cmd);
  Serial.println(cmd);

  if(ESP8266.find(">"))
  {
    ESP8266.print(getStr);
    Serial.println(getStr);
    delay(500);
    String messageBody = "";
    while (ESP8266.available()) 
    {
      String line = ESP8266.readStringUntil('\n');
      if (line.length() == 1) 
      { 
        messageBody = ESP8266.readStringUntil('\n');
      }
    }
    Serial.print("MessageBody received: ");
    Serial.println(messageBody);
    return messageBody;
  }
  else
  {
    ESP8266.println("AT+CIPCLOSE");     
    Serial.println("AT+CIPCLOSE"); 
  } 
}
