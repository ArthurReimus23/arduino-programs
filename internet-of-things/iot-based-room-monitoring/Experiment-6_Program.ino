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

//Pinout of the IR Sensors
  const int irPinEntrance = 4;
  const int irPinExit = 5;

//Pinout of the LEDs
  const int ledPinOne = 6;
  const int ledPinTwo = 7; 
  const int ledPinThree = 8; 
  const int ledPinFour = 9;
  const int ledPinFive = 10; 
  const int ledPinSix = 11; 

//Declaration of variable to hold the amount of LEDs that are on
  int ledIlluminating = 0;

  
//Millis used as an alternative for delays
  unsigned long currentMillis = 0;
  unsigned long previousMillis = 0;
  unsigned long currentMillis2 = 0;
  unsigned long previousMillis2 =0;
  unsigned long currentMillis3 = 0;
  unsigned long previousMillis3 =0;

//Declaration of LCD Object, Address, # of columns, and rows
  LiquidCrystal_I2C lcd(0x27, 16, 2);

//Declaration of variable to hold the number of occupants in the room
  int occupantsPresent = 0;

void setup() {
  
  Serial.begin(9600); 

  //Declaration of the Output / Input Devices
  
  pinMode(ledPinOne, OUTPUT);
  pinMode(ledPinTwo, OUTPUT); 
  pinMode(ledPinThree, OUTPUT);
  pinMode(ledPinFour, OUTPUT);
  pinMode(ledPinFive, OUTPUT); 
  pinMode(ledPinSix, OUTPUT);

  pinMode(irPinEntrance, INPUT); //IR 1 set as Input Source
  pinMode(irPinExit, INPUT); //IR 2 set as Input Source

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

//Initially set the LEDs OFF
  digitalWrite(ledPinOne, LOW);
  digitalWrite(ledPinTwo, LOW);
  digitalWrite(ledPinThree, LOW);
  digitalWrite(ledPinFour, LOW);
  digitalWrite(ledPinFive, LOW);
  digitalWrite(ledPinSix, LOW);

//Initially set occupants equal to 0
  occupantsPresent = 0;

} //setup end

void loop() {

  //Check IR Sensor every 500ms
  unsigned long currentMillis3 = millis();
  if ((unsigned long)(currentMillis3 - previousMillis3) >= 500) {
    if (occupantsPresent != 0) {    
      infraredExit();
    }
    infraredEntrance();
    previousMillis3 = currentMillis3;
  }
    
//LCD Printing
  unsigned long currentMillis2 = millis();
  if ((unsigned long)(currentMillis2 - previousMillis2) >= 1000) {
    lcd.clear();
    //Line 1
    lcd.setCursor(0,0);
    lcd.print("OCCUPANTS: ");
    lcd.setCursor(10,0);
    lcd.print(occupantsPresent);
    lcd.setCursor(0,1);
    lcd.print("LEDs: ");
    lcd.setCursor(6,1);
    lcd.print(ledIlluminating);
    previousMillis2 = currentMillis2;
  }

//Writing to thingspeak every 20 seconds
  unsigned long currentMillis = millis();
  if ((unsigned long)(currentMillis - previousMillis) >= 20000) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Uploading... ");
    writeThingSpeak();
    delay(100);
    previousMillis = currentMillis;
  }

   if (occupantsPresent >= 1) {
    digitalWrite(ledPinOne, HIGH);
    digitalWrite(ledPinTwo, LOW);
    digitalWrite(ledPinThree, LOW);
    digitalWrite(ledPinFour, LOW);
    digitalWrite(ledPinFive, LOW);
    digitalWrite(ledPinSix, LOW);
    ledIlluminating = 1;
   } else {
    digitalWrite(ledPinOne, LOW);
    digitalWrite(ledPinTwo, LOW);
    digitalWrite(ledPinThree, LOW);
    digitalWrite(ledPinFour, LOW);
    digitalWrite(ledPinFive, LOW);
    digitalWrite(ledPinSix, LOW);
    ledIlluminating = 0;
   }

   if (occupantsPresent >= 4) {
    digitalWrite(ledPinOne, HIGH);
    digitalWrite(ledPinTwo, HIGH);
    digitalWrite(ledPinThree, LOW);
    digitalWrite(ledPinFour, LOW);
    digitalWrite(ledPinFive, LOW);
    digitalWrite(ledPinSix, LOW);
    ledIlluminating = 2;
   } else {
    digitalWrite(ledPinTwo, LOW);
    digitalWrite(ledPinThree, LOW);
    digitalWrite(ledPinFour, LOW);
    digitalWrite(ledPinFive, LOW);
    digitalWrite(ledPinSix, LOW);
   }

   if (occupantsPresent >= 7) {
    digitalWrite(ledPinOne, HIGH);
    digitalWrite(ledPinTwo, HIGH);
    digitalWrite(ledPinThree, HIGH);
    digitalWrite(ledPinFour, LOW);
    digitalWrite(ledPinFive, LOW);
    digitalWrite(ledPinSix, LOW);
    ledIlluminating = 3;
   } else {
    digitalWrite(ledPinThree, LOW);
    digitalWrite(ledPinFour, LOW);
    digitalWrite(ledPinFive, LOW);
    digitalWrite(ledPinSix, LOW);
   }

   if (occupantsPresent >= 10) {
    digitalWrite(ledPinOne, HIGH);
    digitalWrite(ledPinTwo, HIGH);
    digitalWrite(ledPinThree, HIGH);
    digitalWrite(ledPinFour, HIGH);
    digitalWrite(ledPinFive, LOW);
    digitalWrite(ledPinSix, LOW);
    ledIlluminating = 4;
   } else {
    digitalWrite(ledPinFour, LOW);
    digitalWrite(ledPinFive, LOW);
    digitalWrite(ledPinSix, LOW);
   }

   if (occupantsPresent >= 13) {
    digitalWrite(ledPinOne, HIGH);
    digitalWrite(ledPinTwo, HIGH);
    digitalWrite(ledPinThree, HIGH);
    digitalWrite(ledPinFour, HIGH);
    digitalWrite(ledPinFive, HIGH);
    digitalWrite(ledPinSix, LOW);
    ledIlluminating = 5;
   } else {
    digitalWrite(ledPinFive, LOW);
    digitalWrite(ledPinSix, LOW);
   }

   if (occupantsPresent >= 16) {
    digitalWrite(ledPinOne, HIGH);
    digitalWrite(ledPinTwo, HIGH);
    digitalWrite(ledPinThree, HIGH);
    digitalWrite(ledPinFour, HIGH);
    digitalWrite(ledPinFive, HIGH);
    digitalWrite(ledPinSix, HIGH);
    ledIlluminating = 6;
   } else {
    digitalWrite(ledPinSix, LOW);
   }

} //loop end
  
//Functions ***************************************************************************************************************************************

void infraredEntrance() { 
  if(digitalRead (irPinEntrance) == LOW){ 
    occupantsPresent++;
    Serial.print("Number of Occupants: ");
    Serial.println(occupantsPresent);
  }
}  

void infraredExit() { 
  if(digitalRead (irPinExit) == LOW){ 
    occupantsPresent--;
    Serial.println("Number of Occupants: ");
    Serial.println(occupantsPresent);
  }
}

void writeThingSpeak(void) {
  startThingSpeakCmd();
  String getStr = "GET /update?api_key=";
  getStr += myAPIkey;
  getStr +="&field1=";
  getStr += String(occupantsPresent);
  getStr +="&field2=";
  getStr += String(ledIlluminating);
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
