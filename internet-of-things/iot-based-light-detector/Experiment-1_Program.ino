//Libraries
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

String myAPIkey= "VVC61VNXYW4XXJMP";   // Write API KEY

boolean relay1_st = false; 
boolean relay2_st = false; 
unsigned char check_connection=0;
unsigned char times_check=0;
boolean error;

SoftwareSerial ESP8266(2,3); //rx, tx

//Pinouts of the LEDs
  const int ledPinout=4; //sprinkler will be changed to pin 2

//Photoresistor pinout and integer value for the Photoresistor
  int lightSensor = A0;
  int lightVal = 0;

//Led Status
int ledStatus = 0; 

//Millis used as an alternative for delays
  unsigned long currentMillis = 0;
  unsigned long previousMillis =0;

//Millis used as an alternative for delays
  unsigned long currentMillis2 = 0;
  unsigned long previousMillis2 =0;
  
//Declaration of LCD Object, Address, # of columns, and rows
  LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  
  Serial.begin(9600); 

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

  //Initialization of the LCD, and turning on its backlight
  lcd.begin();
  lcd.backlight();

}

void loop() {


//Photoresistor *********************************************************************
  lightVal = analogRead(lightSensor);
  Serial.println(lightVal);
  delay(100);
  
//if light detected by photoresistor is less than , turn on LEDs
  if (lightVal < 500) {
    digitalWrite(ledPinout, HIGH); 
    Serial.println("Turn on LEDs");
    delay(100);
    ledStatus = 1;
  }
  else {
    digitalWrite(ledPinout, LOW); 
    Serial.println("Turn off LEDs");
    delay(100);
    ledStatus = 0;
  }

//else, turn off LEDs

  
//LCD Printing
  unsigned long currentMillis = millis();
  
  if ((unsigned long)(currentMillis - previousMillis) >= 1000) {
    lcd.clear(); //Clear LCD
    //Line 1
    lcd.setCursor(0,0);
    lcd.print("LIGHT: ");
    lcd.setCursor(7,0);
    lcd.print(lightVal);
    lcd.setCursor(0,1);
    lcd.print("LED Status: ");
    lcd.setCursor(12,1);
    lcd.print(ledStatus);
  }

//Writing to thingspeak
  unsigned long currentMillis2 = millis();
  
  if ((unsigned long)(currentMillis - previousMillis) >= 20000) {
    writeThingSpeak();
    delay(100);
    previousMillis2 = currentMillis2;
  }

}
  
//Functions ***************************************************************************************************************************************
void writeThingSpeak(void) {
  startThingSpeakCmd();
  String getStr = "GET /update?api_key=";
  getStr += myAPIkey;
  getStr +="&field2=";
  getStr += String(lightVal);
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
