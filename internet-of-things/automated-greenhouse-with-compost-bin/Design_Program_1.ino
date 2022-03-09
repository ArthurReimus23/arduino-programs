//Libraries
#include "DHT.h" //DHT11
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

String myAPIkey= "25K5DSA5LGJRAU4R";   // Write API KEY

boolean relay1_st = false; 
boolean relay2_st = false; 
unsigned char check_connection=0;
unsigned char times_check=0;
boolean error;

SoftwareSerial ESP8266(12,13); //rx, tx

//DHT11 pinouts, and dht type
  #define DHT1_PIN 1     
  #define DHT2_PIN 0

  #define DHTTYPE DHT11

//Initialization of DHT11s
  DHT dht1(DHT1_PIN, DHTTYPE);
  DHT dht2(DHT2_PIN, DHTTYPE);
  
//Push Button Pinouts
  const int PButtonPin1 = 5; //pb for irrigation 1 (sprinkler)
  const int PButtonPin2 = 6; //pb for irrigation 2 (misting nozzle)
  const int PButtonPin3 = 7; //pb for irrigation 3 (composter)
  const int PButtonPin4 = 8; //pb for fans

//Push Button States
int PButtonState1 = 0;       
int lastPButtonState1 = 0;    
int PButtonState2 = 0;     
int lastPButtonState2 = 0;    
int PButtonState3 = 0;    
int lastPButtonState3 = 0;  
int PButtonState4 = 0;       
int lastPButtonState4 = 0;    

//Pinouts of the Input for the Logic Gates
int irrigation_one = 2; //sprinkler
int irrigation_two = 3; //misting
int irrigation_three = 4; //normal

//Pinout of the Water Pump
  const int waterPump = 10;

//Pinout of the DC Motor Fans
  const int dcFans = 11;

//Pinouts of the potentiometers
  const int potentiometer_water = A3;
  const int potentiometer_fans = A2;

//Speed of the water pump and fans based on the potentiometer position
  int speedVal_water; 
  int speedVal_fans;

//Integer values for the soil moisture sensor
  int sensorVal_one = 0;
  int sensorVal_two = 0;

//% values for the soil moisture 
  int percentageSoilMoisture_one = 0;

//Integer values for DHT11 (temperature and humidity)
  int temperatureOne = 0;
  int temperatureTwo = 0;
  int humidityOne = 0;
  int humidityTwo = 0;
  

//Millis used as an alternative for delays
  unsigned long currentMillis = 0;
  unsigned long previousMillis = 0;

  unsigned long currentMillis2 = 0;
  unsigned long previousMillis2 = 0;

  unsigned long currentMillis3 = 0;
  unsigned long previousMillis3 = 0;

  unsigned long currentMillis4 = 0;
  unsigned long previousMillis4 = 0;

  unsigned long currentMillis5 = 0;
  unsigned long previousMillis5 = 0;

  unsigned long currentMillis6 = 0;
  unsigned long previousMillis6 = 0;

  unsigned long currentMillis7 = 0;
  unsigned long previousMillis7 = 0;

  unsigned long currentMillis8 = 0;
  unsigned long previousMillis8 = 0;

  unsigned long currentMillis9 = 0;
  unsigned long previousMillis9 = 0;

//Declaration of LCD Object, Address, # of columns, and rows
  LiquidCrystal_I2C lcd(0x27, 16, 2);

//Status of the Misting System and Compost Irrigation System (0 = false, 1 = true);
int sprinklerStatus = 0;
int mistStatusOne = 0;
int mistStatusTwo = 0;
int composterStatusOne = 0;
int composterStatusTwo = 0;

void setup() {
  
//  Serial.begin(9600); 
  
  pinMode(PButtonPin1, INPUT_PULLUP); 
  pinMode(PButtonPin2, INPUT_PULLUP); 
  pinMode(PButtonPin3, INPUT_PULLUP); 
  pinMode(PButtonPin4, INPUT_PULLUP); 

  pinMode(irrigation_one, OUTPUT);
  pinMode(irrigation_two, OUTPUT);
  pinMode(irrigation_three, OUTPUT);

  pinMode(waterPump, OUTPUT); 
  pinMode(dcFans, OUTPUT); 

//Initialization of DHT11
  dht1.begin();
  dht2.begin();

//Initially turn on irrigation and exhaust systems
  digitalWrite(irrigation_one, HIGH);
  digitalWrite(irrigation_two, HIGH);
  digitalWrite(irrigation_three, HIGH);
  analogWrite(dcFans, 0);
  analogWrite(waterPump, 0);

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
//  ESP8266.print("AT+CWJAP=\"Amy\",\"ubrv9407\"\r\n");
//  ESP8266.print("AT+CWJAP=\"AndroidAP3CB2\",\"zavv6510\"\r\n");
  ESP8266.setTimeout(5000);
 if(ESP8266.find("WIFI CONNECTED\r\n")==1)
 {
 Serial.println("WIFI CONNECTED");
 lcd.clear(); //Clear LCD
 lcd.setCursor(0,0);
 lcd.print("Wi-Fi: OK"); 
 digitalWrite(irrigation_one, LOW);
 digitalWrite(irrigation_two, LOW);
 digitalWrite(irrigation_three, LOW);
 break;
 }
 times_check++;
 if(times_check>3) 
 {
  times_check=0;
   Serial.println("Trying to Reconnect..");
   lcd.clear(); //Clear LCD
   lcd.setCursor(0,0);
   lcd.print("Trying.."); 
  }
  }

}

void loop() {

  //Check for input from push buttons 
  PButtonState1 = digitalRead(PButtonPin1);
  PButtonState2 = digitalRead(PButtonPin2); 
  PButtonState3 = digitalRead(PButtonPin3); 
  PButtonState4 = digitalRead(PButtonPin4); 

  unsigned long currentMillis4 = millis();
  
  if ((unsigned long)(currentMillis4 - previousMillis4) >= 5000) {
    //Get DHT11 readings
    DHT_sensorOne();
    DHT_sensorTwo();
    
    //Get Soil Moisture readings
    int sensorVal_one = analogRead(A0);
    percentageSoilMoisture_one = map(sensorVal_one, 520, 980, 100, 0); //wet, dry = 0-100% (int to %)
    delay(100);
    
    //Get Potentiometer readings
    speedVal_water = analogRead(potentiometer_water);
    speedVal_water = map(speedVal_water, 0, 1023, 0, 255); //right
    speedVal_fans = analogRead(potentiometer_fans);
    speedVal_fans = map(speedVal_fans, 0, 1023, 0, 255); //left
    previousMillis4 = currentMillis4;
  }
  
  //Push Button Function Calls 
  PushButton1();
  PushButton2();
  PushButton3();
  PushButton4();

  unsigned long currentMillis5 = millis();
  
  if ((unsigned long)(currentMillis5 - previousMillis5) >= 5000) {
     
    if (sprinklerStatus == 0 && mistStatusOne == 0 && mistStatusTwo == 0 && composterStatusOne == 0 && composterStatusTwo == 0) { //Water Pump Trigger OFF
      analogWrite(waterPump, 0);
    }
    
    if (percentageSoilMoisture_one >= 70 && sprinklerStatus == 1 && percentageSoilMoisture_one != 0) { //Sprinkler Irrigation Trigger OFF 
      digitalWrite(irrigation_one, LOW);
      sprinklerStatus = 0;
    } 
    else if (percentageSoilMoisture_one <= 30 && sprinklerStatus == 0 && percentageSoilMoisture_one != 0)  { //Sprinkler Irrigation Trigger ON 
      analogWrite(waterPump, speedVal_water);
      digitalWrite(irrigation_one, HIGH);
      digitalWrite(irrigation_two, LOW);
      digitalWrite(irrigation_three, LOW);
      mistStatusOne= 0;
      mistStatusTwo = 0;
      composterStatusOne= 0;
      composterStatusTwo = 0;
      sprinklerStatus = 1;
    }

    if (humidityOne <= 75) { //Exhaust System Trigger OFF 
      analogWrite(dcFans, 0);
    }
    else if (humidityOne >= 80) { //Exhaust System Trigger ON
      analogWrite(dcFans, speedVal_fans);
    }
    previousMillis5 = currentMillis5;
  }

  unsigned long currentMillis6 = millis();
  
  if ((unsigned long)(currentMillis6 - previousMillis6) >= 6000) {

    if (sprinklerStatus == 0 && mistStatusOne == 0 && mistStatusTwo == 0 && composterStatusOne == 0 && composterStatusTwo == 0) { //Water Pump Trigger OFF 
      analogWrite(waterPump, 0);
    }

    
    if (temperatureOne <= 42 && mistStatusOne == 1 && temperatureOne != 0) { //Mist Irrigation Trigger OFF for temperature
      digitalWrite(irrigation_two, LOW);
      mistStatusOne = 0;
    }
    else if (temperatureOne >= 44 && mistStatusOne == 0 && temperatureOne != 0 && sprinklerStatus == 0) { //Mist Irrigation Trigger ON for temperature
      analogWrite(waterPump, speedVal_water);
      digitalWrite(irrigation_two, HIGH);   
      digitalWrite(irrigation_three, LOW);   
      composterStatusOne= 0;
      composterStatusTwo = 0;
      mistStatusOne = 1;
    }
    
    if (humidityOne >= 23 && mistStatusTwo == 1 && humidityOne != 0) { //Mist Irrigation Trigger OFF for humidity
      digitalWrite(irrigation_two, LOW);
      mistStatusTwo = 0;
    }
    else if (humidityOne <= 20 && mistStatusTwo == 0 && humidityOne != 0 && sprinklerStatus == 0) { //Mist Irrigation Trigger ON for humidity
      analogWrite(waterPump, speedVal_water);
      digitalWrite(irrigation_two, HIGH);
      digitalWrite(irrigation_three, LOW);
      composterStatusOne= 0;
      composterStatusTwo = 0;
      mistStatusTwo = 1;
    }
    previousMillis6 = currentMillis6;
  }

  unsigned long currentMillis7 = millis();
  
  if ((unsigned long)(currentMillis7 - previousMillis7) >= 7000) {

    if (sprinklerStatus == 0 && mistStatusOne == 0 && mistStatusTwo == 0 && composterStatusOne == 0 && composterStatusTwo == 0) { //Water Pump Trigger OFF 
      analogWrite(waterPump, 0);
    }
    
    if (temperatureTwo <= 40 && composterStatusOne == 1 && temperatureTwo != 0) { //Compost Irrigation Trigger OFF for temperature
      digitalWrite(irrigation_three, LOW);
      composterStatusOne = 0;
    }
    else if (temperatureTwo >= 42 && composterStatusOne == 0 && temperatureTwo != 0 && sprinklerStatus == 0 && mistStatusOne == 0 && mistStatusTwo == 0) { //Compost Irrigation Trigger ON for temperature 
      analogWrite(waterPump, speedVal_water);
      digitalWrite(irrigation_three, HIGH);
      composterStatusOne = 1;
    }

    if (humidityTwo >= 23 && composterStatusTwo == 1 && humidityTwo != 0) { //Compost Irrigation Trigger OFF for humidity
      digitalWrite(irrigation_three, LOW);
      composterStatusTwo = 0;
    }
    else if (humidityTwo <= 20 && composterStatusTwo == 0 && humidityTwo != 0 && sprinklerStatus == 0 && mistStatusOne == 0 && mistStatusTwo == 0) { //Compost Irrigation Trigger ON for humidity
      analogWrite(waterPump, speedVal_water);
      digitalWrite(irrigation_three, HIGH);
      composterStatusTwo = 1;
    }
    previousMillis7 = currentMillis7;
  }

  unsigned long currentMillis8 = millis();
  
  if ((unsigned long)(currentMillis8 - previousMillis8) >= 1000) {
    if (sprinklerStatus == 1 || mistStatusOne == 1 || mistStatusTwo == 1 || composterStatusOne == 1 || composterStatusTwo == 1) { //Water Pump Trigger ON
      analogWrite(waterPump, speedVal_water);
    }
    previousMillis8 = currentMillis8;
  }

//
//  //Printing on the LCD (for debugging only)
//  unsigned long currentMillis = millis();  
//  if ((unsigned long)(currentMillis - previousMillis) >= 5000) {
//    lcd.clear(); //Clear LCD
//    //Line 1
//    lcd.setCursor(0,0);
//    lcd.print(percentageSoilMoisture_one);
//    lcd.setCursor(8,0);
//    lcd.print(temperatureOne);
//    lcd.setCursor(11,0);
//    lcd.print(humidityOne);
//    //Line 2
//    lcd.setCursor(0,1);
//    lcd.print(speedVal_fans); 
//    lcd.setCursor(4,1);  
//    lcd.print(speedVal_water); 
//    lcd.setCursor(8,1);
//    lcd.print(temperatureTwo);
//    lcd.setCursor(11,1);
//    lcd.print(humidityTwo);
//    previousMillis = currentMillis;
//  }

  //Writing to thingspeak
  unsigned long currentMillis2 = millis();
  
  if ((unsigned long)(currentMillis2 - previousMillis2) >= 180000) {
    if (temperatureOne != 0 && humidityOne != 0 && temperatureTwo != 0 && humidityTwo != 0) {
      writeThingSpeak();
      lcd.clear(); //Clear LCD
      lcd.setCursor(0,0);
      lcd.print("IoT: OK"); 
      previousMillis2 = currentMillis2;
    }
  }

} //loop end

//Functions ***************************************************************************************************************************************

void PushButton1() { //Checks if Push button 1 is Pressed
  if (PButtonState1 != lastPButtonState1) { 
    if (PButtonState1 == LOW) { 
      lcd.clear(); //Clear LCD
      lcd.setCursor(0,0);
      lcd.print("PB1 Pressed"); 
      analogWrite(waterPump, speedVal_water);
      digitalWrite(irrigation_one, HIGH); 
      delay(45000);
      analogWrite(waterPump, 0);
      digitalWrite(irrigation_one, LOW); 
    }
  lastPButtonState1 = PButtonState1;   // Save the current state as the last state, for next time through the loop
  }
}

void PushButton2() { //Checks if Push button 2 is Pressed
  if (PButtonState2 != lastPButtonState2) {
    if (PButtonState2 == LOW) {
      lcd.clear(); //Clear LCD
      lcd.setCursor(0,0);
      lcd.print("PB2 Pressed"); 
      analogWrite(waterPump, speedVal_water);
      digitalWrite(irrigation_two, HIGH); 
      delay(45000);
      analogWrite(waterPump, 0);
      digitalWrite(irrigation_two, LOW); 
      }
  }
  lastPButtonState2 = PButtonState2;   // Save the current state as the last state, for next time through the loop
}

void PushButton3() { //Checks if Push button 3 is Pressed
  
  if (PButtonState3 != lastPButtonState3) { 
    if (PButtonState3 == LOW) {
      lcd.clear(); //Clear LCD
      lcd.setCursor(0,0);
      lcd.print("PB3 Pressed"); 
      analogWrite(waterPump, speedVal_water); 
      digitalWrite(irrigation_three, HIGH); 
      delay(45000);
      analogWrite(waterPump, 0);
      digitalWrite(irrigation_three, LOW); 
      }
  }
  lastPButtonState3 = PButtonState3;   // Save the current state as the last state, for next time through the loop
}

void PushButton4() { //Checks if Push button 4 is Pressed
  if (PButtonState4 != lastPButtonState4) { 
    if (PButtonState4 == LOW) { 
      lcd.clear(); //Clear LCD
      lcd.setCursor(0,0);
      lcd.print("PB4 Pressed"); 
      analogWrite(dcFans, speedVal_fans);
      delay(45000);
      analogWrite(dcFans, speedVal_fans);
      }
  }
  lastPButtonState4 = PButtonState4;   // Save the current state as the last state, for next time through the loop
}

void DHT_sensorOne() { //Get Temperature and Humidity from DHT11 (1)
  temperatureOne = (getTemp("c", 1));
  humidityOne = (getTemp("h", 1));
}

void DHT_sensorTwo() { //Get Temperature and Humidity from DHT11 (2)  
  temperatureTwo = (getTemp("c", 2));
  humidityTwo = (getTemp("h", 2));
}

float getTemp(String req, int dhtCount) {  //Float function used to convert Temperature to Celsius/Fahrenheit and Calculate Heat Index. (Source code from DHT11 library)
  if(dhtCount ==1){
    
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h1 = dht1.readHumidity();
    // Read temperature as Celsius (the default)
    float t1 = dht1.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f1 = dht1.readTemperature(true);

    // Compute heat index in Fahrenheit (the default)
    float hif1 = dht1.computeHeatIndex(f1, h1);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic1 = dht1.computeHeatIndex(t1, h1, false);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h1) || isnan(t1) || isnan(f1)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  
  // Compute heat index in Kelvin 
    float k1 = t1 + 273.15;
    if(req =="c"){
      return t1;//return Cilsus
    }else if(req =="f"){
     return f1;// return Fahrenheit
    }else if(req =="h"){
     return h1;// return humidity
    }else if(req =="hif"){
     return hif1;// return heat index in Fahrenheit
    }else if(req =="hic"){
     return hic1;// return heat index in Cilsus
    }else if(req =="k"){
     return k1;// return temprature in Kelvin
    }else{
     return 0.000;// if no reqest found, retun 0.000
    }
  }//DHT1 END


  if(dhtCount ==2){
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h2 = dht2.readHumidity();
    // Read temperature as Celsius (the default)
    float t2 = dht2.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f2 = dht2.readTemperature(true);

    // Compute heat index in Fahrenheit (the default)
   float hif2 = dht2.computeHeatIndex(f2, h2);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic2 = dht2.computeHeatIndex(t2, h2, false);

   // Check if any reads failed and exit early (to try again).
    if (isnan(h2) || isnan(t2) || isnan(f2)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  
    // Compute heat index in Kelvin 
    float k2 = t2 + 273.15;
    if(req =="c"){
      return t2;//return Cilsus
    }else if(req =="f"){
      return f2;// return Fahrenheit
    }else if(req =="h"){
      return h2;// return humidity
    }else if(req =="hif"){
      return hif2;// return heat index in Fahrenheit
    }else if(req =="hic"){
     return hic2;// return heat index in Cilsus
    }else if(req =="k"){
     return k2;// return temprature in Kelvin
    }else{
     return 0.000;// if no reqest found, retun 0.000
    }
  }// DHT2 end
}//getTemp end


void writeThingSpeak(void) {
  startThingSpeakCmd();
  String getStr = "GET /update?api_key=";
  getStr += myAPIkey;
  getStr +="&field1=";
  getStr += String(temperatureOne);
  getStr +="&field2=";
  getStr += String(humidityOne);
  getStr +="&field3=";
  getStr += String(temperatureTwo);
  getStr +="&field4=";
  getStr += String(humidityTwo);
  getStr +="&field5=";
  getStr += String(percentageSoilMoisture_one);
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
