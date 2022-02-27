//Libraries
#include "DHT.h" 
#include <LiquidCrystal_I2C.h> 
#include <SoftwareSerial.h>

String myAPIkey= "KN3D2M74HPEB38M7";   // Write API KEY

boolean relay1_st = false; 
boolean relay2_st = false; 
unsigned char check_connection=0;
unsigned char times_check=0;
boolean error;

SoftwareSerial ESP8266(2,3); //rx, tx

LiquidCrystal_I2C lcd(0x27, 16, 2); //address, column, rows

//Definition of DHT11 Pinout, Type, and the Declaration of DHT11 object
#define DHT1_PIN 5
#define DHTTYPE DHT11
DHT dht1(DHT1_PIN, DHTTYPE);

//Integer values for DHT11 (temperature and humidity)
  int temperatureOne = 0;
  int humidityOne = 0;

//Millis used as an alternative for delays
  unsigned long currentMillis = 0;
  unsigned long previousMillis =0;

  unsigned long currentMillis2 = 0;
  unsigned long previousMillis2 =0;

  unsigned long currentMillis3 = 0;
  unsigned long previousMillis3 =0;
  
void setup() {
  
  Serial.begin(9600); 

  ESP8266.begin(9600);  
  ESP8266.println("AT+RST");
  delay(2000);
  Serial.println("Connecting to Wifi");
  while(check_connection==0) {
    Serial.print(".");
    ESP8266.print("AT+CWJAP=\"HUAWEI-2.4G-U2Yg\",\"3kgbKX79\"\r\n");
    ESP8266.setTimeout(5000);
    
    if(ESP8266.find("WIFI CONNECTED\r\n")==1) {
      Serial.println("WIFI CONNECTED");
      break;
    }
    times_check++;
    if(times_check>3)  {
      times_check=0;
      Serial.println("Trying to Reconnect..");
    }
  }

//Initialization of DHT11
  dht1.begin();

//Initialization of the LCD, and turning on its backlight
  lcd.begin(); 
  lcd.backlight();
}
 
void loop() {
  
//Temperature and Humidity
  unsigned long currentMillis = millis();
  
  if ((unsigned long)(currentMillis - previousMillis) >= 200) {
    dhtSensor();
    Serial.print("Temperature: ");
    Serial.println(temperatureOne);
    Serial.print("Humidity: ");
    Serial.println(humidityOne);
    previousMillis = currentMillis;
  }
  
//Writing to thingspeak
  unsigned long currentMillis2 = millis();
  
  if ((unsigned long)(currentMillis2 - previousMillis2) >= 20000) {
    writeThingSpeak();
    delay(100);
    previousMillis2 = currentMillis2;
  }

//LCD Printing
  unsigned long currentMillis3 = millis();
  
  if ((unsigned long)(currentMillis3 - previousMillis3) >= 1000) {
    lcd.clear(); //Clear LCD
    //Line 1
    lcd.setCursor(0,0);
    lcd.print("TEMP: ");
    lcd.setCursor(5,0);
    lcd.print(temperatureOne);
    lcd.setCursor(0,1);
    lcd.print("HUMIDITY: ");
    lcd.setCursor(9,1);
    lcd.print(humidityOne);
    previousMillis3 = currentMillis3;
  }


} //loop end
//Functions ***************************************************************************************************************************************

void writeThingSpeak(void) {
  startThingSpeakCmd();
  String getStr = "GET /update?api_key=";
  getStr += myAPIkey;
  getStr +="&field1=";
  getStr += String(temperatureOne);
  getStr +="&field2=";
  getStr += String(humidityOne);
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

void dhtSensor() { //Get Temperature and Humidity from DHT11 (1)
  temperatureOne = (getTemp("c", 1));
  humidityOne = (getTemp("h", 1));
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
}//getTemp end
