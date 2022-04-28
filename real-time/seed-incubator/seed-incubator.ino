#include <dht.h>
#include <TinyStepper.h> //library for stepper motor
#include <Servo.h> //Library for the Servo motor
#include <LiquidCrystal_I2C.h> //Library for the LCD

dht DHT; // Declaration of DHT11 variable. Required for the library
#define DHT11_PIN 2 //DHT11 digital pin connected to digital pin 7

#define sensorPin A3

#define StepPin1 13 //Stepper Pin 1 will be connected to Analog Pin 2 
#define StepPin2 12 //Stepper Pin 2 will be connected to Analog Pin 3
#define StepPin3 11 //Stepper Pin 3 will be connected to Analog Pin 4
#define StepPin4 10 //Stepper Pin 4 will be connected to Analog Pin 5
#define HALFSTEPS 4096  // This is the numbers of step for a full 360 degree rotation
TinyStepper stepper(HALFSTEPS, StepPin1, StepPin2, StepPin3, StepPin4); //Code required for the Stepper Motor library

Servo servoMotor; //Declaration of servo motor variable. Needed for the library 

LiquidCrystal_I2C lcd(0x27, 16, 2); //Declaration of the address, number of columns and rows of letters (16x2)

int RelayPin = 7; //Relay Control's signal pin will be connected to Digital Pin 9
const int IRPin1 = 4; //IR Sensor for Exit connected to Digital Pin 4
const int IRPin2 = 3; //IR Sensor for Entrance connected to Digital Pin 5
const int SecondButtonPin = 5; //The push button for the Second will be connected to Analog Pin 5
const int MinuteButtonPin = 6; //The push button for the Minute will be connected to Analog Pin 4

int WaterSensor_Pin = A2; //Signal Pin of the Water Season is connected to the Analog Pin 0 of the Arduino Uno

const int buzzerPin = 8; //Piezo buzzer's signal pin will be connected to Digital Pin 4

int Soil_threshold = 0;
int Water_threshold = 0;

int SecondbuttonState = 0;         // current state of the button
int lastSecondbuttonState = 0;     // previous state of the button
int MinutebuttonState = 0;         // current state of the button
int lastMinutebuttonState = 0;     // previous state of the button

int counter = 0; //Declaration of counter variable 
int temporaryVal = 0; //Declaration of temporaryVal

int pos = 0;    // variable to store the servo position

const int a = 440; //Variable for note a (sound produced by the buzzer)

void setup() {
  
  Serial.begin(9600);
  pinMode(sensorPin, OUTPUT);
  
  pinMode(RelayPin,OUTPUT); //Relay set as output device
  
  pinMode(IRPin1, INPUT); //IR for Exit set as Input Source
  pinMode(IRPin2, INPUT); //IR for Entrance set as Input Source

  pinMode(SecondButtonPin, INPUT_PULLUP); //Push button for second set as Input Source
  pinMode(MinuteButtonPin, INPUT_PULLUP); //Push button for minute set as Input Source

  pinMode(StepPin1, OUTPUT); //Stepper motor's Pin 1 set as Output source
  pinMode(StepPin2, OUTPUT); //Stepper motor's Pin 2 set as Output source
  pinMode(StepPin3, OUTPUT); //Stepper motor's Pin 3 set as Output source
  pinMode(StepPin4, OUTPUT); //Stepper motor's Pin 4 set as Output source

  lcd.begin(); //Initialize LCD
  lcd.backlight(); //Turn on Backlight of LCD

  servoMotor.attach(9); //Servo Motor connected to Digital Pin 2
  servoMotor.write(0); //Set servo motor to 0 degrees

  stepper.Enable(); //Enable Stepper Motor. Needed for the Stepper motor library

  
  
}

void loop() {
  
  Soil_threshold = analogRead(sensorPin);  // Read the analog value form sensor
  delay(10);              // Allow power to settle

  Soil_Detect();


  Water_threshold = analogRead(WaterSensor_Pin); //Threshold = Data read from analog pin 0
  delay(10);

  Water_Detect();

 
  SecondbuttonState = digitalRead(SecondButtonPin); //Set SecondButtonState to the digital value of Analog Pin 5
  MinutebuttonState = digitalRead(MinuteButtonPin); //Set MinuteButtonState to the digital value of Analog Pin 4

  SecondIncrementSevenSeg();
  MinuteIncrementSevenSeg();

  IR_Open();
  IR_Close();
  DHT11_Detect();
  LCD_Func();

  temporaryVal = counter % 2; //Temporary = the remainder of counter/2 (modulo)

  if( SecondbuttonState == HIGH && MinutebuttonState == HIGH) { // If the two push buttons were pushed at the same time
    counter++; //Increment counter
    delay(100); //Delay 1000ms 
  }

  SecondIncrementSevenSeg();
  MinuteIncrementSevenSeg();
//  if (temporaryVal == 0){ // If temporaryVal is 
//    
//    delay(10); //Delay 100ms
//  }
//
//  else if (temporaryVal == 1) { // If temporaryVal is 1
//    Serial.println("Buttons Pressed at the Same Time!");
//    Sounds(); //Call Sounds function
//    delay(1000); //Delay 100ms
//    counter++; //Increment counter
//  }
}


void IR_Open() { //Decrement Function for Seven Segment Display
  if(digitalRead (IRPin1) == LOW){ //If IR2 detects an object then add 1 to the value displayed on Seven Segment Display
  Serial.println("Open Seed Germinator");
  Sounds(); //Call Sounds function
  Stepper_Open();
  delay (500); //Delay
  }
}  

void IR_Close() { //Increment Function for Seven Segment Display
  if(digitalRead (IRPin2) == LOW){ //If IR1 detects an object then subtract 1 to the value displayed on Seven Segment Display
  Serial.println("Close Seed Germinator");
  Sounds(); //Call Sounds function
  Stepper_Close();
  delay (500); //Delay
  }
}

void DHT11_Detect() { 
  int chk = DHT.read11(DHT11_PIN); //Read data from DHT11
  Serial.println("Temp: "); //Print "Temp on LCD"
  Serial.println(DHT.temperature); //Print temperature detected from DHT11
  Serial.println("Humidity: "); //Print "Humidity"
  Serial.println(DHT.humidity); //Print humidity detected detected from DHT11
  delay(1000); //Delay 
}


void SecondIncrementSevenSeg() { //The function used to add 1 second when push button is pressed
  if (SecondbuttonState != lastSecondbuttonState) { //If Current State of the Push button is not equal to Previous State

    if (SecondbuttonState == HIGH) { //If Push button is LOW
      Serial.println("Button 1 Pressed");
      Servo_Open();
    }
    delay(500); //Delay 500ms
  }
  lastSecondbuttonState = SecondbuttonState;   // Save the current state as the last state, for next time through the loop
}

void MinuteIncrementSevenSeg() { //The function used to add 1 minute when push button is pressed
  if (MinutebuttonState != lastMinutebuttonState) { //If Current State of the Push button is not equal to Previous State

    if (MinutebuttonState == HIGH) { //If Push button is LOW
      Serial.println("Button 2 Pressed");
      Servo_Close();
    } 
    delay(500); //Delay 500ms
   }
  lastMinutebuttonState = MinutebuttonState;   // Save the current state as the last state, for next time through the loop
}

void Stepper_Open() { 
  stepper.Move(180); //Move stepper motor 90 degrees to the left 
  Sounds(); //Call Sounds function
  delay(100); //Delay 
}

void Stepper_Close(){ 
  stepper.Move(-180); //Move stepper motor 90 degrees to the left 
  delay(100); //Delay 
}

void Servo_Open() { 
  for (pos = 0; pos <= 180; pos += 1) { // Servo goes from 0 degrees to 180 degrees
    servoMotor.write(pos);              // Servo to go to position in variable 'pos'
    delay(15);                       // Delay 15ms 
  }
}

void Servo_Close() { 
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      servoMotor.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // Delay 15ms
  }
}



void Soil_Detect() { 
  if(Soil_threshold >= 4) { //if threshold is less than or equal to 300
    Serial.print("Soil Level: ");
    Serial.println(Soil_threshold);
    Serial.println("Soil Moisture Level: High"); //Print "Water Level: Low" in the Serial Monitor
    digitalWrite(7, HIGH);  //Turn off relay control
    delay(500); //Delay 500ms
  } 

  else if(Soil_threshold <= 3) { //if threshold is less than or equal to 300
    Serial.print("Soil Level: ");
    Serial.println(Soil_threshold);
    Serial.println("Soil Moisture Level: Low"); //Print "Water Level: Low" in the Serial Monitor
    digitalWrite(7, LOW);  //Turn off relay control
    delay(500); //Delay 500ms
  } 
}


void Water_Detect() {
  if(Water_threshold <= 250) { //if threshold is less than or equal to 300
    Serial.println("Water Level: Low"); //Print "Water Level: Low" in the Serial Monitor
    Serial.println(Water_threshold);
    Servo_Open();
    delay(500); //Delay 500ms
  }

  else if(Water_threshold >= 251) { //if threshold is less than or equal to 300
    Serial.println("Water Level: High"); //Print "Water Level: Low" in the Serial Monitor
    Serial.println(Water_threshold);
    Servo_Close();
    delay(500); //Delay 500ms
  }
}

void beep(int note, int duration){
  tone(buzzerPin, note, duration);   //Play tone on Buzzer
  delay(50); //Delay 50ms

}

void Sounds() {
    beep(a, 1500); //Product note a on Piezo Buzzer for 1500ms
    delay(1500);  // Delay 1500ms
}

void LCD_Func() {
  lcd.clear(); //Clear LCD
  lcd.setCursor(0,0);  //Go to line 1 of LCD
  lcd.print("Temp: "); //Print "Temp on LCD"
  lcd.print(DHT.temperature); //Print temperature detected from DHT11
  lcd.print((char)223); //Custom character of degree in LCD
  lcd.print("C"); //Print "C"
  lcd.setCursor(0,1); //Go to line 2 of LCD
  lcd.print("Humidity: "); //Print "Humidity"
  lcd.print(DHT.humidity); //Print humidity detected detected from DHT11
  lcd.print("%"); //Print "%"
  delay(2000); //Delay 
  lcd.clear(); //Clear 
  lcd.setCursor(0,0);  //Go to line 1 of LCD
  lcd.print("Soil Moisture: "); //Print "Temp on LCD"
  lcd.print(Soil_threshold); //Print "Temp on LCD"
  lcd.setCursor(0,1); //Go to line 2 of LCD
  lcd.print("Water Level: "); //Print "Temp on LCD"
  lcd.print(Water_threshold); //Print "Temp on LCD"
  delay(2000); //Delay 
}
  
  
  
  
