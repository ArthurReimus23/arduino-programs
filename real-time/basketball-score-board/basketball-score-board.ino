#include <SevSeg.h> //library for seven segment display

SevSeg SevenSegDisplay;

  const int digit1 = 2; //Digit 1 of the Seven Segment Display is to connected to Digital Pin 2 of the Arduino
  const int digit2 = 3; //Digit 2 of the Seven Segment Display is to connected to Digital Pin 3 of the Arduino
  const int digit3 = 4; //Digit 3 of the Seven Segment Display is to connected to Digital Pin 4 of the Arduino. 
  const int digit4 = 5; //Digit 4 of the Seven Segment Display is to connected to Digital Pin 5 of the Arduino. 
  const int segmentA = 6; //Segment A of the Seven Segment Display is to connected to Digital Pin 6 of the Arduino
  const int segmentB = 7; //Segment B of the Seven Segment Display is to connected to Digital Pin 7 of the Arduino
  const int segmentC = 8; //Segment C of the Seven Segment Display is to connected to Digital Pin 8 of the Arduino
  const int segmentD = 9; //Segment D of the Seven Segment Display is to connected to Digital Pin 9 of the Arduino
  const int segmentE = 10; //Segment E of the Seven Segment Display is to connected to Digital Pin 10 of the Arduino
  const int segmentF = 11; //Segment F of the Seven Segment Display is to connected to Digital Pin 11 of the Arduino
  const int segmentG = 12; //Segment G of the Seven Segment Display is to connected to Digital Pin 12 of the Arduino
  const int segmentDP= 1; //Segment Decimal Point of the Seven Segment Display is to connected to Digital Pin 1 of the Arduino

  const int IRPin1 = A0; //IR Sensor for Opening Lid connected to Analog Pin 0
  const int IRPin2 = A1; //IR Sensor for Closing Lid connected to Analog Pin 1
  
  const int PButtonPin = A2; //The push button for Opening Lid will be connected to Analog Pin 2

  const int buzzerPin = A3; //Piezo buzzer's signal pin will be connected to Analog Pin 3

  int LEDPin1=A5; //Positive side of LED connected to Analog pin 5
  int LEDPin2=A4; //Positive side of LED connected to Analog pin 4

  const int a = 440; //Variable for note a (sound produced by the buzzer)
  const int f = 349; //Variable for note f (sound produced by the buzzer)

int PButtonState = 0;         // Declaration of the variable for the current state of the Push Button
int lastPButtonState = 0;     // Declaration of the variable for the previous state of the Push Button

int counter = 0; //Declaration of counter variable 
int temporaryVal = 0; //Declaration of temporaryVal
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
  
  pinMode(IRPin1, INPUT); //IR 1 set as Input Source
  pinMode(IRPin2, INPUT); //IR 2 set as Input Source


  pinMode(PButtonPin, INPUT_PULLUP); //Push button set as Input Source

  pinMode(buzzerPin, OUTPUT); //Buzzer set as output source

  pinMode(LEDPin1, OUTPUT); //LED 1 set as output source
  pinMode(LEDPin2, OUTPUT); //LED 2 set as output source
  
  int displayType = COMMON_CATHODE; // The type of display used
  int numberOfDigits = 4;  // The number of digits to be displayed in the Seven Segment Display
  
  SevenSegDisplay.Begin(displayType, numberOfDigits, digit1, digit2, digit3, digit4, segmentA, segmentB, segmentC, segmentD, segmentE, segmentF, segmentG, segmentDP); //Setup of Seven Segment Display
  SevenSegDisplay.SetBrightness(100); //Set the brightness of the Seven Segment Display

  timer = millis(); //Set timer = millis 

}

void loop() {
  
   char tempString[1]; //Used for sprintf (Used for displaying in Seven Segment Display )
   sprintf(tempString, "%4d", deciSecond); //Convert deciSecond into a string that is right adjusted (Used for displaying in Seven Segment Display)
   SevenSegDisplay.DisplayString(tempString, 0); // Used for displaying in Seven Segment Display 

   PButtonState = digitalRead(PButtonPin); //POButtonState = Digital Data read from Push Button 14
   
   temporaryVal = counter % 2; //Temporary = the remainder of counter/2 (modulo)
   
  if (temporaryVal == 0){ // If temporaryVal is 0
     PushButton(); //Check Pushbutton for Input
     delay(1000); //Delay 1000ms
  }

  if (temporaryVal == 1) { // If temporaryVal is 1
    
    if (deciSecond == 98 || deciSecond == 198 || deciSecond == 298 || deciSecond == 398 || deciSecond == 498 || deciSecond == 598 || deciSecond == 698 || deciSecond == 798 ||  deciSecond == 898 || deciSecond == 998 ||
     deciSecond == 1098 || deciSecond == 1198 || deciSecond == 1298 || deciSecond == 1398 || deciSecond == 1498 || deciSecond == 1598 || deciSecond == 1698 || deciSecond == 1798 || deciSecond == 1898 || deciSecond == 1998 ||
     deciSecond == 2098 || deciSecond == 2198 || deciSecond == 2298 || deciSecond == 2398 || deciSecond == 2498 || deciSecond == 2598 || deciSecond == 2698 || deciSecond == 2798 || deciSecond == 2898 || deciSecond == 2998 ||
     deciSecond == 3098 || deciSecond == 3198 || deciSecond == 3298 || deciSecond == 3398 || deciSecond == 3498 || deciSecond == 3598 || deciSecond == 3698 || deciSecond == 3798 || deciSecond == 3898 || deciSecond == 3998 ||
     deciSecond == 4098 || deciSecond == 4198 || deciSecond == 4298 || deciSecond == 4398 || deciSecond == 4498 || deciSecond == 4598 || deciSecond == 4698 || deciSecond == 4798 || deciSecond == 4898 || deciSecond == 4998 ||
     deciSecond == 5098 || deciSecond == 5198 || deciSecond == 5298 || deciSecond == 5398 || deciSecond == 5498 || deciSecond == 5598 || deciSecond == 5698 || deciSecond == 5798 || deciSecond == 5898 || deciSecond == 5998 ||
     deciSecond == 6098 || deciSecond == 6198 || deciSecond == 6298 || deciSecond == 6398 || deciSecond == 6498 || deciSecond == 6598 || deciSecond == 6698 || deciSecond == 6798 || deciSecond == 6898 || deciSecond == 6998 ||
     deciSecond == 7098 || deciSecond == 7198 || deciSecond == 7298 || deciSecond == 7398 || deciSecond == 7498 || deciSecond == 7598 || deciSecond == 7698 || deciSecond == 7798 || deciSecond == 7898 || deciSecond == 7998 ||
     deciSecond == 8098 || deciSecond == 8198 || deciSecond == 8298 || deciSecond == 8398 || deciSecond == 8498 || deciSecond == 8598 || deciSecond == 8698 || deciSecond == 8798 || deciSecond == 8898 || deciSecond == 8998 ||
     deciSecond == 9098 || deciSecond == 9198 || deciSecond == 9298 || deciSecond == 9398 || deciSecond == 9498 || deciSecond == 9598 || deciSecond == 9698 || deciSecond == 9798 || deciSecond == 9898) {
      IR_BasketOnePlusOne();
     }
  
    IR_BasketOnePlusTwo(); //Detect IR Sensors 
    IR_BasketTwo(); //Detect IR Sensors 
    delay(100); //Delay 100ms
    
    if (deciSecond == 99 || deciSecond == 199 || deciSecond == 299 || deciSecond == 399 || deciSecond == 499 || deciSecond == 599 || deciSecond == 699 || deciSecond == 799 ||  deciSecond == 899 || deciSecond == 999 ||
     deciSecond == 1099 || deciSecond == 1199 || deciSecond == 1299 || deciSecond == 1399 || deciSecond == 1499 || deciSecond == 1599 || deciSecond == 1699 || deciSecond == 1799 || deciSecond == 1899 || deciSecond == 1999 ||
     deciSecond == 2099 || deciSecond == 2199 || deciSecond == 2299 || deciSecond == 2399 || deciSecond == 2499 || deciSecond == 2599 || deciSecond == 2699 || deciSecond == 2799 || deciSecond == 2899 || deciSecond == 2999 ||
     deciSecond == 3099 || deciSecond == 3199 || deciSecond == 3299 || deciSecond == 3399 || deciSecond == 3499 || deciSecond == 3599 || deciSecond == 3699 || deciSecond == 3799 || deciSecond == 3899 || deciSecond == 3999 ||
     deciSecond == 4099 || deciSecond == 4199 || deciSecond == 4299 || deciSecond == 4399 || deciSecond == 4499 || deciSecond == 4599 || deciSecond == 4699 || deciSecond == 4799 || deciSecond == 4899 || deciSecond == 4999 ||
     deciSecond == 5099 || deciSecond == 5199 || deciSecond == 5299 || deciSecond == 5399 || deciSecond == 5499 || deciSecond == 5599 || deciSecond == 5699 || deciSecond == 5799 || deciSecond == 5899 || deciSecond == 5999 ||
     deciSecond == 6099 || deciSecond == 6199 || deciSecond == 6299 || deciSecond == 6399 || deciSecond == 6499 || deciSecond == 6599 || deciSecond == 6699 || deciSecond == 6799 || deciSecond == 6899 || deciSecond == 6999 ||
     deciSecond == 7099 || deciSecond == 7199 || deciSecond == 7299 || deciSecond == 7399 || deciSecond == 7499 || deciSecond == 7599 || deciSecond == 7699 || deciSecond == 7799 || deciSecond == 7899 || deciSecond == 7999 ||
     deciSecond == 8099 || deciSecond == 8199 || deciSecond == 8299 || deciSecond == 8399 || deciSecond == 8499 || deciSecond == 8599 || deciSecond == 8699 || deciSecond == 8799 || deciSecond == 8899 || deciSecond == 8999 ||
     deciSecond == 9099 || deciSecond == 9199 || deciSecond == 9299 || deciSecond == 9399 || deciSecond == 9499 || deciSecond == 9599 || deciSecond == 9699 || deciSecond == 9799 || deciSecond == 9899 || deciSecond >= 9900) {  
      counter++; //Increment Counter
      SoundsEnd(); // Play tone on Buzzer
      delay (100); //Delay 100ms
    }
  }
}


void PushButton() { //PushButton_Open function - Function for opening lid through push button
  if (PButtonState != lastPButtonState) { //If Current State of the Push button is not equal to Previous State
    if (PButtonState == HIGH) { //If Push button is Pressed
      Serial.println("Button Pressed"); //Print "Button Pressed" on Serial Monitor
      delay(100); //Delay 100ms
      deciSecond = 0; //Set deciSecond = 0
      SoundsStart(); //Go to SoundStart Function
      counter++; //Increment counter
    }
  }
  lastPButtonState = PButtonState;   // Save the current state as the last state, for next time through the loop
}

void IR_BasketOnePlusOne() { //IR_Open Function  - Function for detecting object on IR Sensor
  if(digitalRead (IRPin1) == LOW){ //If IR Sensor 1 detects an object
    Serial.println("Player One Scored 1!"); //Print "Player One Scored 1!" in Serial Monitor
    digitalWrite(A5, HIGH); //Turn on LED
    deciSecond++; //Increment digit displayed on Seven Segment Display
    delay (300); //Delay 100ms
    digitalWrite(A5, LOW); //Turn off LED
  }
}  

void IR_BasketOnePlusTwo() { //IR_Open Function  - Function for detecting object on IR Sensor
  if(digitalRead (IRPin1) == LOW){ //If IR Sensor 1 detects an object
    Serial.println("Player One Scored 2!"); //Print "Player One Scored 2!" in Serial Monitor
    digitalWrite(A5, HIGH); //Turn on LED
    deciSecond = deciSecond + 2; //Increment by 2, the digit displayed on Seven Segment Display
    delay (300); //Delay 100ms
    digitalWrite(A5, LOW); //Turn off LED
  }
}  

void IR_BasketTwo() { //IR_Close Function  - Function for detecting object on IR Sensor
  if(digitalRead (IRPin2) == LOW){ //If IR Sensor 2 detects an object
  Serial.println("Player Two Scored 2!"); //Print "Player Two Scored!" in Serial Monitor
  digitalWrite(A4, HIGH); //Turn on LED
  deciSecond = deciSecond + 200;  //Increment by 2, the digit displayed on Seven Segment Display
  delay (300); //Delay 300ms
  digitalWrite(A4, LOW); //Turn off LED
  }
}

void beep(int note, int duration){ //Function for setting note and duration of the Beep
  tone(buzzerPin, note, duration);   //Play tone on Buzzer
  delay(50); //Delay 50ms
}

void SoundsStart() {//Function for producing sounds on the Piezo Buzzer
    beep(a, 1500); //Produces note a on Piezo Buzzer for 1500ms
    delay(1500);  // Delay 1500ms
}

void SoundsEnd() {//Function for producing sounds on the Piezo Buzzer
    beep(f, 1500); //Produces note a on Piezo Buzzer for 1500ms
    delay(1500);  // Delay 1500ms
}
