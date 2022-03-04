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

//Pinout of the Flame Sensor
const int flameSensor = A0;

//Declaration of variable for the value detected by the flame sensor and the flame status
int flameVal = 0; 
int flameStatus = 0;

//Declaration of variable for the floor level
int floorVal = 0; 

//Pinout of the Pushbuttons 
  const int PButtonPin1 = 5; 

//Push Button States
int PButtonState1 = 0;       
int lastPButtonState1 = 0;    

//Pinout of the Piezo Buzzer
const int buzzerPin = 4;

//Millis used as an alternative for delays
  unsigned long currentMillis = 0;
  unsigned long previousMillis = 0;
  unsigned long currentMillis2 = 0;
  unsigned long previousMillis2 =0;
  unsigned long currentMillis3 = 0;
  unsigned long previousMillis3 =0;

//Declaration of LCD Object, Address, # of columns, and rows
  LiquidCrystal_I2C lcd(0x27, 16, 2);
  
void setup() {
  
  Serial.begin(9600);
  
  pinMode(flameSensor, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(PButtonPin1, INPUT_PULLUP); 

  ESP8266.begin(9600);  
  ESP8266.println("AT+RST");
  delay(2000);
  Serial.println("Connecting to Wifi");
   while(check_connection==0)
  {
    Serial.print(".");
    ESP8266.print("AT+CWJAP=\"HUAWEI-2.4G-U2Yg\",\"3kgbKX79\"\r\n");
    ESP8266.setTimeout(5000);
    if(ESP8266.find("WIFI CONNECTED\r\n")==1) {
      Serial.println("WIFI CONNECTED");
      break;
    }
    times_check++;
    if(times_check>3) {
      times_check=0;
      Serial.println("Trying to Reconnect..");
    }
  }

//Initialization of the LCD, and turning on its backlight
  lcd.begin();
  lcd.backlight();

  flameStatus=0;
  floorVal=1;
}

void loop() {

  PButtonState1 = digitalRead(PButtonPin1);

//Detection of flame using flame sensor   
  int flameVal=analogRead(flameSensor);
  Serial.print("Flame Status: ");
  Serial.println(flameStatus);

  Serial.print("Floor: ");
  Serial.println(floorVal);


//Check whether threshold was reached once every 3 seconds
  unsigned long currentMillis = millis();
  if ((unsigned long)(currentMillis - previousMillis) >= 3000) {
    if (flameVal < 300) { //If flame was detected, turn on buzzer. Else do nothing. 
      digitalWrite(buzzerPin, HIGH);
      Serial.println("Flame detected!");
      flameStatus=1;
    } else {
      digitalWrite(buzzerPin, LOW);
      Serial.println("No flame detected");
      flameStatus=0;
    }
    previousMillis = currentMillis;
  }

//Check pushbutton for input
  PushButton1();

//LCD Printing
  unsigned long currentMillis2 = millis();
  
  if ((unsigned long)(currentMillis2 - previousMillis2) >= 1000) {
    lcd.clear(); //Clear LCD
    //Line 1
    lcd.setCursor(0,0);
    lcd.print("FLR: ");
    lcd.setCursor(5,0);
    lcd.print(floorVal);
    //Line 2
    lcd.setCursor(0,1);
    lcd.print("FLAME: ");
    lcd.setCursor(8,1);
    lcd.print(flameStatus);
    previousMillis2 = currentMillis2;
  }

//Writing to thingspeak every 20 seconds
  unsigned long currentMillis3 = millis();
  
  if ((unsigned long)(currentMillis3 - previousMillis3) >= 20000) {
    lcd.clear(); //Clear LCD
    lcd.setCursor(0,0);
    lcd.print("Uploading..");
    writeThingSpeak();
    delay(100);
    previousMillis3 = currentMillis3;
    
  }

} //loop end

//Functions ***************************************************************************************************************************************

void PushButton1() { //Checks if Push button 1 is Pressed
  if (PButtonState1 != lastPButtonState1) { 
    if (PButtonState1 == LOW) { 
      Serial.println("Push Button Pressed");
      floorFunction();
      Serial.print("Floor: ");
      Serial.println(floorVal);
    }
  lastPButtonState1 = PButtonState1;   // Save the current state as the last state, for next time through the loop
  }
}

void floorFunction() {
  if (floorVal == 4) {
    floorVal = 1;
  } else if (floorVal < 4) {
    floorVal++;
  }
}

void writeThingSpeak(void) {
  startThingSpeakCmd();
  String getStr = "GET /update?api_key=";
  getStr += myAPIkey;
  getStr +="&field1=";
  getStr += String(floorVal);
  getStr +="&field2=";
  getStr += String(flameStatus);
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
