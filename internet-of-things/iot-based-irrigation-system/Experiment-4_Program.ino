//Libraries
#include <SoftwareSerial.h>

String myAPIkey= "KN3D2M74HPEB38M7";   // Write API KEY

boolean relay1_st = false; 
boolean relay2_st = false; 
unsigned char check_connection=0;
unsigned char times_check=0;
boolean error;

SoftwareSerial ESP8266(2,3); //rx, tx

//Pinout of the Transistor toggling the Water Pump
  int waterPump = 4; 

//Pinout of the Soil Moisture Sensor
  int soilSensor = A0;
  
//Integer value for the Storing the Soil moisture sensor in raw value and percentage
  int soilMoisture = 0;
  int percentageSoilMoisture = 0;

//Millis used as an alternative for delays
  unsigned long currentMillis = 0;
  unsigned long previousMillis =0;
  unsigned long currentMillis2 = 0;
  unsigned long previousMillis2 =0;
  
void setup() {
  
  Serial.begin(9600); 

  pinMode(waterPump, OUTPUT);

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

}

void loop() {

//Detecting the Soil Moisture every 300 milliseconds
  unsigned long currentMillis = millis();
  
  if ((unsigned long)(currentMillis - previousMillis) >= 300) {
    soilMoisture = analogRead(soilSensor);
    Serial.print("Soil Moisture: ");
    Serial.println(soilMoisture);
    percentageSoilMoisture = map(soilMoisture, 655, 1023, 100, 0); //wet, dry = 0-100% (int to %)
    Serial.print("Soil Moisture in %: ");
    Serial.println(percentageSoilMoisture);
    previousMillis = currentMillis;
  }

//Irrigation Trigger OFF
  if (percentageSoilMoisture > 65) {
    digitalWrite(waterPump, LOW);
    Serial.println("Water Pump: OFF");
    delay(200);
  }

//Irrigation Trigger ON
  if (percentageSoilMoisture < 35) {
    digitalWrite(waterPump, HIGH);
    Serial.println("Water Pump: ON");
    delay(200);
  }

//Writing to thingspeak every 20 seconds
  unsigned long currentMillis2 = millis();
  
  if ((unsigned long)(currentMillis2 - previousMillis2) >= 20000) {
    writeThingSpeak();
    delay(100);
    previousMillis2 = currentMillis2;
  }

} //loop end
  
//Functions ***************************************************************************************************************************************
void writeThingSpeak(void) {
  startThingSpeakCmd();
  String getStr = "GET /update?api_key=";
  getStr += myAPIkey;
  getStr +="&field1=";
  getStr += String(soilMoisture);
  getStr +="&field2=";
  getStr += String(percentageSoilMoisture);
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
