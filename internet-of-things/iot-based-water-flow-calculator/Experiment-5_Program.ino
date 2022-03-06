//Libraries
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

String myAPIkey= "KN3D2M74HPEB38M7";   // Write API KEY

boolean relay1_st = false; 
boolean relay2_st = false; 
unsigned char check_connection=0;
unsigned char times_check=0;
boolean error;

SoftwareSerial ESP8266(2,3); //rx, tx

//Pinout of the Transistor toggling the Water Pump
  int waterPump = 5; 

//Pinout of the Water Sensor 
  #define waterSensor A0

//Value for storing whether water sensor detects water or not
  int waterStatus = 0;
  
//Declaration of variables
  int volumeStatus = 0;
  double volumeFlowRate = 0; 
  double velocityVariable = 0;
  double crossSectionalArea = 0;
  double diameterVariable = 0;
  double radiusVariable = 0;
  
//Pinout of the Pushbuttons 
  const int PButtonPin1 = 4; //pb go

//Push Button States
int PButtonState1 = 0;       
int lastPButtonState1 = 0;    


//Millis for Timer
  double pastMillis = 0;
  double timerMillis = 0;
  double presentMillis = 0;
  
//Millis used as an alternative for delays
  unsigned long currentMillis = 0;
  unsigned long previousMillis = 0;
  unsigned long currentMillis2 = 0;
  unsigned long previousMillis2 =0;

//Declaration of LCD Object, Address, # of columns, and rows
  LiquidCrystal_I2C lcd(0x27, 16, 2);
  
void setup() {
  
  Serial.begin(9600); 

  pinMode(waterPump, OUTPUT);
  pinMode(PButtonPin1, INPUT_PULLUP); 

  //Initialization of the LCD, and turning on its backlight
  lcd.begin();
  lcd.backlight();

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

//Initially set the water pump to OFF
  digitalWrite(waterPump, LOW);

  volumeStatus = 0;
}

void loop() {
  
  PButtonState1 = digitalRead(PButtonPin1);
  velocityVariable = 1.65 / timerMillis; // velocity = displacement / time (displacement = 165cm or 1.65meters)
  diameterVariable = 0.008; //8mm 
  radiusVariable = diameterVariable / 2; //Radius = Diameter / 2
  crossSectionalArea = 3.1415926535*(sq(radiusVariable)); //Cross Sectional Area of a Cylinder = Pi*Radius^2
  volumeFlowRate = velocityVariable*crossSectionalArea;  // volumetric flow rate (meters/second) = velocity * cross-sectional vector area
  
  Serial.print("Volume Status: ");
  Serial.println(volumeStatus);

  waterStatus = analogRead(waterSensor);
  Serial.print("Water Level: ");
  Serial.println(waterStatus);
 
  if (volumeStatus == 0) {    
    Serial.print("Velocity: ");
    Serial.println(velocityVariable, 8);
    Serial.print("Diameter: ");
    Serial.println(diameterVariable, 8);
    Serial.print("Radius: ");
    Serial.println(radiusVariable, 8);
    Serial.print("Cross Sectional Area: ");
    Serial.println(crossSectionalArea, 8);
    Serial.print("Volume Flow Rate: ");
    Serial.println(volumeFlowRate, 8);
    digitalWrite(waterPump, LOW);
    pastMillis = millis();
    PushButton1();
    delay(100);
  }

  if (volumeStatus == 1) {
    analogWrite(waterPump, 255);
    presentMillis = millis();
    timerMillis = ((presentMillis - pastMillis) / 1000);
    waterSensorFunction();
    Serial.print("Time: ");
    Serial.println(timerMillis);
  }

//LCD Printing
  unsigned long currentMillis2 = millis();
  
  if ((unsigned long)(currentMillis2 - previousMillis2) >= 1000) {
    lcd.clear(); //Clear LCD
    //Line 1
    lcd.setCursor(0,0);
    lcd.print("T: ");
    lcd.setCursor(3,0);
    lcd.print(timerMillis);
    lcd.setCursor(8,0);
    lcd.print("V: ");
    lcd.setCursor(11,0);
    lcd.print(velocityVariable);
    lcd.setCursor(0,1);
    lcd.print("VFR: ");
    lcd.setCursor(5,1);
    lcd.print(volumeFlowRate, 8);
    previousMillis2 = currentMillis2;
  }

//Writing to thingspeak every 20 seconds
  unsigned long currentMillis = millis();
  
  if ((unsigned long)(currentMillis - previousMillis) >= 20000) {
    writeThingSpeak();
    delay(100);
    previousMillis = currentMillis;
  }

} //loop end
  
//Functions ***************************************************************************************************************************************

void PushButton1() { //Checks if Push button 1 is Pressed
  if (PButtonState1 != lastPButtonState1) { 
    if (PButtonState1 == LOW) { 
      Serial.println("Push Button Pressed");
      volumeStatus = 1;
    }
  lastPButtonState1 = PButtonState1;   // Save the current state as the last state, for next time through the loop
  }
}

void waterSensorFunction() {
  if (waterStatus >= 200) {
    Serial.println("Detected Water");
    volumeStatus = 0;
  }
}

void writeThingSpeak(void) {
  startThingSpeakCmd();
  String getStr = "GET /update?api_key=";
  getStr += myAPIkey;
  getStr +="&field1=";
  getStr += String(timerMillis, 8);
  getStr +="&field2=";
  getStr += String(volumeFlowRate, 8);
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
