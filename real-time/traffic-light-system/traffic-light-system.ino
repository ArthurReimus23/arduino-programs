#include <SevSeg.h> //library for seven segment display

SevSeg SevenSegDisplay;

  const int digit1 = 5; //Digit 1 of the Seven Segment Display is to connected to Digital Pin 3 of the Arduino
  const int digit2 = 6; //Digit 2 of the Seven Segment Display is to connected to Digital Pin 4 of the Arduino
  const int digit3 = 2; //Digit 3 of the Seven Segment Display is to connected to Digital Pin 2 of the Arduino. 
  const int digit4 = 2; //Digit 4 of the Seven Segment Display is to connected to Digital Pin 2 of the Arduino. 
  const int segmentA = 7; //Segment A of the Seven Segment Display is to connected to Digital Pin 7 of the Arduino
  const int segmentB = 8; //Segment B of the Seven Segment Display is to connected to Digital Pin 8 of the Arduino
  const int segmentC = 9; //Segment C of the Seven Segment Display is to connected to Digital Pin 9 of the Arduino
  const int segmentD = 10; //Segment D of the Seven Segment Display is to connected to Digital Pin 10 of the Arduino
  const int segmentE = 11; //Segment E of the Seven Segment Display is to connected to Digital Pin 11 of the Arduino
  const int segmentF = 12; //Segment F of the Seven Segment Display is to connected to Digital Pin 12 of the Arduino
  const int segmentG = 13; //Segment G of the Seven Segment Display is to connected to Digital Pin 13 of the Arduino
  const int segmentDP= 2; //Segment Decimal Point of the Seven Segment Display is to connected to Digital Pin 2 of the Arduino  

  const int IRPin1 = A0; //IR Sensor for Opening Lid connected to Analog Pin 0
  const int IRPin2 = A1; //IR Sensor for Closing Lid connected to Analog Pin 1
  
  const int PButtonPin1 = 4; //The push button 1 is connected to Digital Pin 4
  const int PButtonPin2 = 3; //The push button 2 is connected to Digital Pin 3

  const int LEDPin1=A2; //Anode side of LED connected to Analog pin 2
  const int LEDPin2=A3; //Anode side of LED connected to Analog pin 3
  const int LEDPin3=A4; //Anode side of LED connected to Analog pin 4

  const int buzzerPin = A5; //Piezo buzzer's signal pin will be connected to Analog Pin 5
  
  const int a = 440; //Variable for note a (sound produced by the buzzer)
  const int f = 349; //Variable for note f (sound produced by the buzzer)
  const int d = 294; //Variable for note d (sound produced by the buzzer)

int PButtonState1 = 0;         // Declaration of the variable for the current state of the Push Button
int lastPButtonState1 = 0;     // Declaration of the variable for the previous state of the Push Button
int PButtonState2 = 0;         // Declaration of the variable for the current state of the Push Button
int lastPButtonState2 = 0;     // Declaration of the variable for the previous state of the Push Button

int counterOne= 0; //Declaration of counter variable 
int temporaryValOne = 0; //Declaration of temporaryValOne
int counterTwo = 0; //Declaration of counter variable 
int temporaryValTwo = 0; //Declaration of temporaryValTwo
int counterThree= 0; //Declaration of counter variable 
int temporaryValThree = 0; //Declaration of temporaryValThree

unsigned long timer; // variable for counting 
int deciSecond = 0; // Variable of the value to be displayed in the Seven Segment Display


void setup() {

  Serial.begin(9600); //Serial Monitor
  
  pinMode(segmentA, OUTPUT);  //A in the 4-Digit Seven Segment Display set as Output source
  pinMode(segmentB, OUTPUT);  //B in the 4-Digit Seven Segment Display set as Output source
  pinMode(segmentC, OUTPUT);  //C in the 4-Digit Seven Segment Display set as Output source
  pinMode(segmentD, OUTPUT);  //D in the 4-Digit Seven Segment Display set as Output source
  pinMode(segmentE, OUTPUT);  //E in the 4-Digit Seven Segment Display set as Output source
  pinMode(segmentF, OUTPUT);  //F in the 4-Digit Seven Segment Display set as Output source
  pinMode(segmentG, OUTPUT);  //G in the 4-Digit Seven Segment Display set as Output source
  pinMode(segmentDP, OUTPUT); // Decimal Point in the 4-Digit Seven Segment Display set as Output source
  
  pinMode(LEDPin1, OUTPUT); //LED 1 set as output source
  pinMode(LEDPin2, OUTPUT); //LED 2 set as output source
  pinMode(LEDPin3, OUTPUT); //LED 3 set as output source

  pinMode(buzzerPin, OUTPUT); //Buzzer set as output source

  pinMode(PButtonPin1, INPUT_PULLUP); //Push button set as Input Source
  pinMode(PButtonPin2, INPUT_PULLUP); //Push button set as Input Source

  pinMode(IRPin1, INPUT); //IR 1 set as Input Source
  pinMode(IRPin2, INPUT); //IR 2 set as Input Source

  int displayType = COMMON_CATHODE; // The type of display used
  int numberOfDigits = 2;  // The number of digits to be displayed in the Seven Segment Display
  
  SevenSegDisplay.Begin(displayType, numberOfDigits, digit1, digit2, digit3, digit4, segmentA, segmentB, segmentC, segmentD, segmentE, segmentF, segmentG, segmentDP); //Setup of Seven Segment Display
  SevenSegDisplay.SetBrightness(100); //Set the brightness of the Seven Segment Display

  timer = millis(); //Returns the number of milliseconds passed since the Arduino board began running the current program

  deciSecond == 0;
}

void loop() {

   char tempString[4]; //Number of Characters to be Displayed
   sprintf(tempString, "%d", deciSecond); //Print n characters of deciSecond (n=tempString)
   SevenSegDisplay.DisplayString(tempString, 0); // Display deciSecond in Seven Segment Display 
  
   PButtonState1 = digitalRead(PButtonPin1); //POButtonState = Digital Data read from Push Button 1
   PButtonState2 = digitalRead(PButtonPin2); //POButtonState = Digital Data read from Push Button 2

   
   if (deciSecond <= 0) {
    deciSecond = 0; //Set deciSecond = 0
    soundStop(); //Go to SoundStop
    digitalWrite(LEDPin1, HIGH); // Turn on Red LED
    digitalWrite(LEDPin2, LOW); // Turn off Yellow LED
    counterOne++; //Increment Counter
    delay(100); //Delay 100ms
   }

   if (deciSecond >= 60) {
    deciSecond = 60; //Set deciSecond = 60
    soundGo(); //Go to SoundGo
    digitalWrite(LEDPin3, HIGH); // Turn on Green LED
    digitalWrite(LEDPin2, LOW); // Turn on Yellow LED
    counterOne++; //Increment Counter
    delay(100); //Delay 100ms
   }

   temporaryValOne = counterOne % 2; //TemporaryVal = the remainder of counter/2 (modulo)
   temporaryValTwo = counterTwo % 2; //TemporaryVal = the remainder of counter/2 (modulo)
   temporaryValThree = counterThree % 2; //TemporaryVal = the remainder of counter/2 (modulo)

   if (temporaryValOne == 0) {
    Serial.print("Status: ");
    Serial.println("Green Light"); //Print "Status: Green Light" in Serial Monitor 
    digitalWrite(LEDPin1, LOW); // Turn off Red LED
    deciSecond--; //Decrement value displayed on SSD 
    Serial.print("Temporary Value One: "); 
    Serial.println(temporaryValOne); //Print temporaryValOne
    Serial.print("Temporary Value Two: ");
    Serial.println(temporaryValTwo); //Print temporaryValTwo
    Serial.print("Temporary Value Three: ");
    Serial.println(temporaryValThree); //Print temporaryValThree
    delay(1000); //Delay 1000ms
    
    if (deciSecond <= 30){
      digitalWrite(LEDPin3, LOW); // Turn off Green LED
      digitalWrite(LEDPin2, HIGH); // Turn on Yellow LED
    }

    if (temporaryValTwo == 0){
      PushButton1(); //Go to PushButton1
    }

    if (temporaryValThree == 0){
      PushButton2(); //Go to PushButton2
    }
   }

   if (temporaryValOne == 1) {
    Serial.print("Status: ");
    Serial.println("Red Light"); //Print "Status: Red Light" on Serial Monitor
    digitalWrite(LEDPin3, LOW); // Turn off Green LED
    deciSecond++; //Increment value displayed on SSD 
    counterTwo = 0; //Set counter = 0
    counterThree = 0; //Set counter = 0
    IR_West(); //Go to IR_West
    IR_East(); //Go to IR_East
    Serial.print("Temporary Value One: ");
    Serial.println(temporaryValOne); //Print temporaryValOne
    Serial.print("Temporary Value Two: ");
    Serial.println(temporaryValTwo); //Print temporaryValTwo
    Serial.print("Temporary Value Three: ");
    Serial.println(temporaryValThree); //Print temporaryValThree
    delay(1000); //Delay 1000ms
    if (deciSecond >= 30){
      digitalWrite(LEDPin1, LOW); // Turn off Red LED
      digitalWrite(LEDPin2, HIGH); // Turn on Yellow LED
    }
   }
   
}


void PushButton1() { 
  if (PButtonState1 != lastPButtonState1) { //If Current State of the Push button is not equal to Previous State
    if (PButtonState1 == HIGH) { //If Push button is Pressed
      Serial.println("Button 1 Pressed"); //Print "Button 1 Pressed" on Serial Monitor
      deciSecond = deciSecond - 15; //Subtract 15 from value displayed on SSD 
      counterTwo++; //Increment Counter
      delay(500); //Delay 100ms
    }
  }
  lastPButtonState1 = PButtonState1;   // Save the current state as the last state, for next time through the loop
}

void PushButton2() { 
  if (PButtonState2 != lastPButtonState2) { //If Current State of the Push button is not equal to Previous State
    if (PButtonState2 == HIGH) { //If Push button is Pressed
      Serial.println("Button 2 Pressed"); //Print "Button 2 Pressed" on Serial Monitor
      deciSecond = deciSecond - 30;  //Subtract 30 from value displayed on SSD 
      counterThree++; //Increment Counter
      delay(500); //Delay 500ms
    }
  }
  lastPButtonState2 = PButtonState2;   // Save the current state as the last state, for next time through the loop
}


void IR_West() { 
  if(digitalRead (IRPin1) == LOW){ //If IR Sensor 1 detects an object
    Serial.println("Someone from the West Beat the Red Light!"); //Print "Someone from the West Beat the Red Light!" in Serial Monitor
    soundAlarm(); //Go to SoundAlarm
    delay (300); //Delay 300ms
  }
}  

void IR_East() { 
  if(digitalRead (IRPin2) == LOW){ //If IR Sensor 2 detects an object
  Serial.println("Someone from the East Beat the Red Light!"); //Print "Someone from the East Beat the Red Light!" in Serial Monitor
  soundAlarm(); //Go to SoundAlarm
  delay (300); //Delay 300ms
  }
}

void beep(int note, int duration){ //Function for setting note and duration of the Beep
  tone(buzzerPin, note, duration);   //Play tone on Buzzer
  delay(50); //Delay 50ms
}

void soundAlarm() {//Function for producing sounds on the Piezo Buzzer
    Serial.println("Buzz Alarm");
    beep(a, 1500); //Produces note a on Piezo Buzzer for 1500ms
    delay(1500);  // Delay 1500ms
}

void soundGo() {//Function for producing sounds on the Piezo Buzzer
    Serial.println("Buzz Go");
    beep(f, 1500); //Produces note a on Piezo Buzzer for 1500ms
    delay(1500);  // Delay 1500ms
}

void soundStop() {//Function for producing sounds on the Piezo Buzzer
    Serial.println("Buzz Stop");
    beep(d, 1500); //Produces note a on Piezo Buzzer for 1500ms
    delay(1500);  // Delay 1500ms
}
