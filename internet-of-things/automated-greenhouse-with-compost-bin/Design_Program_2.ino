//Libraries
#include "DHT.h" //DHT11
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
#include <SD.h>
#include <SPI.h>

//DHT11 pinouts, and dht type
#define DHT1_PIN 7     
#define DHT2_PIN 6

#define DHTTYPE DHT11

//Initialization of DHT11s
DHT dht1(DHT1_PIN, DHTTYPE);
DHT dht2(DHT2_PIN, DHTTYPE);
  
//Declaration of DS3231 Object
DS3231 rtc(SDA, SCL);

//Declaration of LCD Object, Address, # of columns, and rows
LiquidCrystal_I2C lcd(0x27, 20, 4);

//Declaration of SD Card Data Logging Object and CS Pinout
File myFile;
int pinCS = 10;

//Photoresistor pinout and integer value for the Photoresistor
  int lightSensor = A3;
  int lightVal = 0;

//Integer values for the soil moisture sensor
  int sensorVal_one = 0;
//  int sensorVal_two = 0;
  
//% values for the soil moisture 
  int percentageSoilMoisture_one = 0;
//  int percentageSoilMoisture_two = 0;

//Integer values for DHT11 (temperature and humidity)
  int temperatureOne = 0;
  int temperatureTwo = 0;
  int humidityOne = 0;
  int humidityTwo = 0;
  
//Millis used as an alternative for delays
unsigned long currentMillis = 0;
unsigned long previousMillis=0;

unsigned long currentMillis2 = 0;
unsigned long previousMillis2=0;

unsigned long currentMillis3 = 0;
unsigned long previousMillis3=0;

unsigned long currentMillis4 = 0;
unsigned long previousMillis4=0;

//Status of the Irrigation System and Exhaust System
int fanStatus = 0;
int sprinklerStatus = 0;
int mistStatusOne = 0;
int mistStatusTwo = 0;
int composterStatusOne = 0;
int composterStatusTwo = 0;
int waterPumpStatus = 0;

void setup()  {

//  Serial.begin(9600);

  pinMode(pinCS, OUTPUT);
  
  //DS3231 RTC
  rtc.begin(); // Initialize the rtc object
 
//  The following lines can be uncommented to set the date and time
//  rtc.setDOW(SUNDAY);     // Set Day-of-Week to SUNDAY
//  rtc.setTime(23, 52, 00);     // Set the time to 12:00:00 (24hr format)
//  rtc.setDate(31, 10, 2021);   // Set the date DD/MM/YY

//Initialization of DHT11
  dht1.begin();
  dht2.begin();
  
//Initialization of the LCD, and turning on its backlight
  lcd.begin();
  lcd.backlight();
  
//SD Card Initialization
  if (SD.begin())
  {
//    Serial.println("SD card successfully initialized");
    lcd.clear(); //Clear LCD
    lcd.setCursor(0,0);
    lcd.print("Initialization OK");
  } 
  else {
//    Serial.println("SD card initialization failed");
    lcd.clear(); //Clear LCD
    lcd.setCursor(0,0);
    lcd.print("Initialization Failed");
    return;
  }

}

void loop () {
 
  unsigned long currentMillis4 = millis();
  
  if ((unsigned long)(currentMillis4 - previousMillis4) >= 1000) {
    //DHT11
    DHT_sensorOne();
    DHT_sensorTwo();
    
    //Soil Moisture 
    int sensorVal_one = analogRead(A0);
    percentageSoilMoisture_one = map(sensorVal_one, 520 , 980, 100, 0); //wet, dry = 0-100% (int to %)
    delay(100);

    //Photoresistor *********************************************************************
    lightVal = analogRead(lightSensor);
    lightVal = map(lightVal, 14, 1023, 0, 100);
    delay(100);

    previousMillis4 = currentMillis4;
  }

  unsigned long currentMillis3 = millis();

  if ((unsigned long)(currentMillis3 - previousMillis3) >= 60000) {

    //Water Pump Trigger OFF *******************************************************
    if (sprinklerStatus == 0 && mistStatusOne == 0 && mistStatusTwo == 0 && composterStatusOne == 0 && composterStatusTwo == 0) {
      waterPumpStatus = 0;
    }

    //Sprinkler Irrigation Trigger OFF *********************************************
    if (percentageSoilMoisture_one >= 70 && sprinklerStatus == 1 && percentageSoilMoisture_one != 0) {
      sprinklerStatus = 0;
    }

    //Sprinkler Irrigation Trigger ON *********************************************
    if (percentageSoilMoisture_one <= 30 && sprinklerStatus == 0 && percentageSoilMoisture_one != 0) {
      waterPumpStatus = 1;
      sprinklerStatus = 1;
    }

    //Mist Irrigation Trigger OFF for temperature*********************************************
    if (temperatureOne <= 42 && mistStatusOne == 1 && temperatureOne != 0) {
      mistStatusOne = 0;
    }

    //Mist Irrigation Trigger ON for temperature*********************************************
    if (temperatureOne >= 44 && mistStatusOne == 0 && temperatureOne != 0) {
      waterPumpStatus = 1;
      mistStatusOne = 1;
    }

   //Mist Irrigation Trigger OFF for humidity*********************************************
    if (humidityOne >= 23 && mistStatusTwo == 1 && humidityOne != 0) {
      mistStatusTwo = 0;
    }
    
  //Mist Irrigation Trigger ON for humidity*********************************************
    if (humidityOne <= 20 && mistStatusTwo == 0 && humidityOne != 0) {
      waterPumpStatus = 1;
      mistStatusTwo = 1;
    }
  
  //Compost Irrigation Trigger OFF for temperature*********************************************
    if (temperatureTwo <= 40 && composterStatusOne == 1 && temperatureTwo != 0) {
      composterStatusOne = 0;
    }

    //Compost Irrigation Trigger ON for temperature *********************************************
    if (temperatureTwo >= 42 && composterStatusOne == 0 && temperatureTwo != 0) {
      waterPumpStatus = 1;
      composterStatusOne = 1;
    }

    //Compost Irrigation Trigger OFF for humidity*********************************************
    if (humidityTwo >= 23 && composterStatusTwo == 1 && humidityTwo != 0) {
      composterStatusTwo = 0;
    }

    //Compost Irrigation Trigger ON for humidity*********************************************
    if (humidityTwo <= 20 && composterStatusTwo == 0 && humidityTwo != 0) {
      waterPumpStatus = 1;
      composterStatusTwo = 1;
    }

    //Exhaust System Trigger OFF *********************************************
    if (humidityOne <= 75 && humidityOne != 0) {
      fanStatus = 0;
    }

    //Exhaust System Trigger ON *********************************************
    if (humidityOne >= 80 && humidityOne != 0) {
      fanStatus = 1;
    }
    previousMillis3 = currentMillis3;
  }

  
////LCD *******************************************************************************
  unsigned long currentMillis = millis();
  
  if ((unsigned long)(currentMillis - previousMillis) >= 5000) {
    lcd.clear(); //Clear LCD
    //Line 1
    lcd.setCursor(0,0);
    lcd.print("SM1: ");
    lcd.setCursor(5,0);
    lcd.print(percentageSoilMoisture_one);
    lcd.setCursor(8,0);
    lcd.print("%");
    lcd.setCursor(10,0);
//    lcd.print("SM2: ");
//    lcd.setCursor(15,0);
//    lcd.print(percentageSoilMoisture_two);
//    lcd.setCursor(18,0);
//    lcd.print("%");
    //Line 2
    lcd.setCursor(0,1);
    lcd.print("T1: ");
    lcd.setCursor(5,1);
    lcd.print(temperatureOne);
    lcd.setCursor(10,1);
    lcd.print("H1: ");
    lcd.setCursor(15,1);
    lcd.print(humidityOne);
    lcd.setCursor(18,1);
    lcd.print("%");
    //Line 3
    lcd.setCursor(0,2);
    lcd.print("T2: ");
    lcd.setCursor(5,2);
    lcd.print(temperatureTwo);
    lcd.setCursor(10,2);
    lcd.print("H2: ");
    lcd.setCursor(15,2);
    lcd.print(humidityTwo);
    lcd.setCursor(18,2);
    lcd.print("%");
    //Line 4
    lcd.setCursor(0,3);
    lcd.print("L: ");
    lcd.setCursor(5,3);
    lcd.print(lightVal);
    lcd.setCursor(8,3);
    lcd.print("%");    
    previousMillis = currentMillis;
  }
  
  DataLogging(); //SD Card Data Logging

}

//Functions ***************************************************************************************************************************************

void DataLogging() { //SD Card Data Logging
  unsigned long currentMillis2 = millis();
  
  if ((unsigned long)(currentMillis2 - previousMillis2) >= 30000 && temperatureOne != 0 && humidityOne != 0 && temperatureTwo != 0 && humidityTwo != 0) {
 
    myFile = SD.open("ftest2.txt", FILE_WRITE);
    if (myFile) {    
      myFile.print(rtc.getDateStr());
      myFile.print(",");    
      myFile.print(rtc.getTimeStr());
      myFile.print(",");     
      myFile.print(percentageSoilMoisture_one);
      myFile.print(",");    
//      myFile.print(percentageSoilMoisture_two);
//      myFile.print(",");    
      myFile.print(temperatureOne);
      myFile.print(",");    
      myFile.print(humidityOne);
      myFile.print(",");    
      myFile.print(temperatureTwo);
      myFile.print(",");    
      myFile.print(humidityTwo);
      myFile.print(",");
      myFile.print(lightVal);
      myFile.print(",");    
      myFile.print(waterPumpStatus);
      myFile.print(",");    
      myFile.print(sprinklerStatus);
      myFile.print(",");    
      myFile.print(mistStatusOne);
      myFile.print(",");    
      myFile.print(mistStatusTwo);
      myFile.print(",");
      myFile.print(composterStatusOne);
      myFile.print(",");
      myFile.print(composterStatusTwo);
      myFile.print(",");
      myFile.println(fanStatus);
      myFile.close(); // close the file    
//      Serial.println("Successful data logging");
      lcd.clear(); //Clear LCD
      lcd.setCursor(0,0);
      lcd.print("Data Logging: OK ");
  }
  // if the file didn't open, print an error:
    else {
//      Serial.println("error opening test.txt");
      lcd.clear(); //Clear LCD
      lcd.setCursor(0,0);
      lcd.print("Data Logging: ERROR ");

    }
    previousMillis2 = currentMillis2;
  }
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
//      Serial.println("Failed to read from DHT sensor!");
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
//      Serial.println("Failed to read from DHT sensor!");
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
