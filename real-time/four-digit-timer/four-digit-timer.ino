#include <SevSeg.h> //library for seven segment display

SevSeg SevenSegDisplay;

  const int digit1 = 2; //Digit 1 of the Seven Segment Display is to connected to Digital Pin 2 of the Arduino
  const int digit2 = 3; //Digit 2 of the Seven Segment Display is to connected to Digital Pin 3 of the Arduino
  const int digit3 = 4; //Digit 3 of the Seven Segment Display is to connected to Digital Pin 0 of the Arduino. PIN NOT USED SINCE WE ONLY DISPLAYED FROM 0-99
  const int digit4 = 5; //Digit 4 of the Seven Segment Display is to connected to Digital Pin 0 of the Arduino. PIN NOT USED SINCE WE ONLY DISPLAYED FROM 0-99
  const int segmentA = 6; //Segment A of the Seven Segment Display is to connected to Digital Pin 6 of the Arduino
  const int segmentB = 7; //Segment B of the Seven Segment Display is to connected to Digital Pin 7 of the Arduino
  const int segmentC = 8; //Segment C of the Seven Segment Display is to connected to Digital Pin 8 of the Arduino
  const int segmentD = 9; //Segment D of the Seven Segment Display is to connected to Digital Pin 9 of the Arduino
  const int segmentE = 10; //Segment E of the Seven Segment Display is to connected to Digital Pin 10 of the Arduino
  const int segmentF = 11; //Segment F of the Seven Segment Display is to connected to Digital Pin 11 of the Arduino
  const int segmentG = 12; //Segment G of the Seven Segment Display is to connected to Digital Pin 12 of the Arduino
  const int segmentDP= 13; //Segment Decimal Point of the Seven Segment Display is to connected to Digital Pin 13 of the Arduino

  int LEDPin=13; //Positive side of LED connected to digital pin 13

  const int SecondButtonPin = A5; //The push button for the Second will be connected to Analog Pin 5
  const int MinuteButtonPin = A4; //The push button for the Minute will be connected to Analog Pin 4
    
int counter = 0; //Declaration of counter variable 
int temporaryVal = 0; //Declaration of temporaryVal
unsigned long timer; // variable for counting 
int deciSecond = 0; // Variable of the value to be displayed in the Seven Segment Display


int SecondbuttonState = 0;         // current state of the button
int lastSecondbuttonState = 0;     // previous state of the button
int MinutebuttonState = 0;         // current state of the button
int lastMinutebuttonState = 0;     // previous state of the button

void setup() {

  Serial.begin(9600); 
  
  pinMode(segmentA, OUTPUT);  //A in the 4-Digit Seven Segment Display set as Output source
  pinMode(segmentB, OUTPUT);  //B in the 4-Digit Seven Segment Display set as Output source
  pinMode(segmentC, OUTPUT);  //C in the 4-Digit Seven Segment Display set as Output source
  pinMode(segmentD, OUTPUT);  //D in the 4-Digit Seven Segment Display set as Output source
  pinMode(segmentE, OUTPUT);  //E in the 4-Digit Seven Segment Display set as Output source
  pinMode(segmentF, OUTPUT);  //F in the 4-Digit Seven Segment Display set as Output source
  pinMode(segmentG, OUTPUT);  //G in the 4-Digit Seven Segment Display set as Output source
  pinMode(segmentDP, OUTPUT); // Decimal Point in the 4-Digit Seven Segment Display set as Output source
  
  pinMode(LEDPin,OUTPUT); //LED set as output device

  pinMode(SecondButtonPin, INPUT_PULLUP); //Push button for second set as Input Source
  pinMode(MinuteButtonPin, INPUT_PULLUP); //Push button for minute set as Input Source
  
  int displayType = COMMON_CATHODE; // The type of display used
  int numberOfDigits = 4;  // The number of digits to be displayed in the Seven Segment Display
  
  SevenSegDisplay.Begin(displayType, numberOfDigits, digit1, digit2, digit3, digit4, segmentA, segmentB, segmentC, segmentD, segmentE, segmentF, segmentG, segmentDP); //Setup of Seven Segment Display
  SevenSegDisplay.SetBrightness(100); //Set the brightness of the Seven Segment Display


}

void loop() {
  
   char tempString[1]; //Used for sprintf (Used for displaying in Seven Segment Display )
   sprintf(tempString, "%4d", deciSecond); //Convert deciSecond into a string that is right adjusted (Used for displaying in Seven Segment Display)
   SevenSegDisplay.DisplayString(tempString, 0); // Used for displaying in Seven Segment Display 

   SecondbuttonState = digitalRead(SecondButtonPin); //Set SecondButtonState to the digital value of Analog Pin 5
   MinutebuttonState = digitalRead(MinuteButtonPin); //Set MinuteButtonState to the digital value of Analog Pin 4
   temporaryVal = counter % 2; //Temporary = the remainder of counter/2 (modulo)

  if( SecondbuttonState == HIGH && MinutebuttonState == HIGH) { // If the two push buttons were pushed at the same time
    counter++; //Increment counter
    delay(1500); //Delay 1500ms 
  }

   
  if (temporaryVal == 0){ // If temporaryVal is 0
     deciSecond++; //Increment digit displayed on Seven Segment Display
     digitalWrite(13, LOW); //Turn off LED 
     delay(1000); //Delay 200ms
  }

  else if (temporaryVal == 1) { // If temporaryVal is 1
    SecondIncrementSevenSeg(); //Call SecondIncrementSevenSeg Function
    MinuteIncrementSevenSeg(); //Call MInuteIncrementSevenSeg Function
    digitalWrite(13, HIGH); //Turn on LED
    delay(100); //Delay 100ms
  }

//The following if statements were used to properly display the minutes and seconds in the Seven Segment Display

  if( deciSecond == 60){ 
    deciSecond = 100; 
  }

  if( deciSecond == 160){ 
    deciSecond = 200; 
  }

  if( deciSecond == 260){ 
    deciSecond = 300; 
  }

  if( deciSecond == 360){ 
    deciSecond = 400; 
  }

  if( deciSecond == 460){  
    deciSecond = 500; 
  }

  if( deciSecond == 560){ 
    deciSecond = 600; 
  }

  if( deciSecond == 660){ 
    deciSecond = 700; 
  }

  if( deciSecond == 760){ 
    deciSecond = 800; 
  }

  if( deciSecond == 860){ 
    deciSecond = 900; 
  }

  if( deciSecond == 960){ 
    deciSecond = 1000; 
  }

  if( deciSecond == 1060){ 
    deciSecond = 1100; 
  }

  if( deciSecond == 1160){  
    deciSecond = 1200; 
  }

  if( deciSecond == 1260){  
    deciSecond = 1300; 
  }
  
  if( deciSecond == 1360){  
    deciSecond = 1400; 
  }

  if( deciSecond == 1460){  
    deciSecond = 1500; 
  }

  if( deciSecond == 1560){  
    deciSecond = 1600; 
  }

  if( deciSecond == 1660){  
    deciSecond = 1700; 
  }

  if( deciSecond == 1760){  
    deciSecond = 1800; 
  }

  if( deciSecond == 1860){  
    deciSecond = 1900; 
  }

  if( deciSecond == 1960){  
    deciSecond = 2000; 
  }

  if( deciSecond == 2060){  
    deciSecond = 2100; 
  }

  if( deciSecond == 2160){  
    deciSecond = 2200; 
  }

  if( deciSecond == 2260){  
    deciSecond = 2300; 
  }

  if( deciSecond == 2360){  
    deciSecond = 2400; 
  }

  if( deciSecond == 2460){  
    deciSecond = 2500; 
  }

  if( deciSecond == 2560){  
    deciSecond = 2600; 
  }
  if( deciSecond == 2660){  
    deciSecond = 2700; 
  }
  if( deciSecond == 2760){  
    deciSecond = 2800; 
  }
  if( deciSecond == 2860){  
    deciSecond = 2900; 
  }

  if( deciSecond == 2960){  
    deciSecond = 3000; 
  }

  if( deciSecond == 3060){  
    deciSecond = 3100; 
  }

  if( deciSecond == 3160){  
    deciSecond = 3200; 
  }

  if( deciSecond == 3260){  
    deciSecond = 3300; 
  }

  if( deciSecond == 3360){  
    deciSecond = 3400; 
  }

  if( deciSecond == 3460){  
    deciSecond = 3500; 
  }

  if( deciSecond == 3560){  
    deciSecond = 3600; 
  }
  if( deciSecond == 3660){  
    deciSecond = 3700; 
  }
  if( deciSecond == 3760){  
    deciSecond = 3800; 
  }
  if( deciSecond == 3860){  
    deciSecond = 3900; 
  }

  if( deciSecond == 3960){  
    deciSecond = 4000; 
  }

  if( deciSecond == 4060){  
    deciSecond = 4100; 
  }

  if( deciSecond == 4160){  
    deciSecond = 4200; 
  }

  if( deciSecond == 4260){  
    deciSecond = 4300; 
  }

  if( deciSecond == 4360){  
    deciSecond = 4400; 
  }

  if( deciSecond == 4460){  
    deciSecond = 4500; 
  }

  if( deciSecond == 4560){  
    deciSecond = 4600; 
  }
  if( deciSecond == 4660){  
    deciSecond = 4700; 
  }
  if( deciSecond == 4760){  
    deciSecond = 4800; 
  }
  if( deciSecond == 4860){  
    deciSecond = 4900; 
  }

  if( deciSecond == 4960){  
    deciSecond = 5000; 
  }

  if( deciSecond == 5060){  
    deciSecond = 5100; 
  }

  if( deciSecond == 5160){  
    deciSecond = 5200; 
  }

  if( deciSecond == 5260){  
    deciSecond = 5300; 
  }

  if( deciSecond == 5360){  
    deciSecond = 5400; 
  }

  if( deciSecond == 5460){  
    deciSecond = 5500; 
  }

  if( deciSecond == 5560){  
    deciSecond = 5600; 
  }
  if( deciSecond == 5660){  
    deciSecond = 5700; 
  }
  if( deciSecond == 5760){  
    deciSecond = 5800; 
  }
  if( deciSecond == 5860){  
    deciSecond = 5900; 
  }

  if( deciSecond >= 5960){  
    deciSecond = 6000; 
  }

  if( deciSecond >= 6001){  
    deciSecond = 0; 
  }

}


void SecondIncrementSevenSeg(){ //The function used to add 1 second when push button is pressed
  if (SecondbuttonState != lastSecondbuttonState) { //If Current State of the Push button is not equal to Previous State

    if (SecondbuttonState == LOW) { //If Push button is LOW
      deciSecond++; //Increment by 1 the digit displayed on Seven Segment Display
    }
    delay(500); //Delay 500ms
  }
  lastSecondbuttonState = SecondbuttonState;   // Save the current state as the last state, for next time through the loop
}

void MinuteIncrementSevenSeg(){ //The function used to add 1 minute when push button is pressed
  if (MinutebuttonState != lastMinutebuttonState) { //If Current State of the Push button is not equal to Previous State

    if (MinutebuttonState == LOW) { //If Push button is LOW
      deciSecond = deciSecond + 100;  //Increment by 100 the digit displayed on the Seven Segment Display
    } 
    delay(500); //Delay 500ms
   }
  lastMinutebuttonState = MinutebuttonState;   // Save the current state as the last state, for next time through the loop
}
