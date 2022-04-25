#include <TinyStepper.h>
#include <SevSeg.h>

SevSeg myDisplay;

#define StepPin1 A1 //
#define StepPin2 A2
#define StepPin3 A3
#define StepPin4 A4
#define HALFSTEPS 4096  // This is the numbers of step for a full 360 degree rotation
TinyStepper stepper(HALFSTEPS, StepPin1, StepPin2, StepPin3, StepPin4); //Code required for the TinyStepper library

  const int digit1 = 2; //Digit 1 of the Seven Segment Display is to connected to Digital Pin 2 of the Arduino
  const int digit2 = 3; //Digit 2 of the Seven Segment Display is to connected to Digital Pin 3 of the Arduino
  const int digit3 = 0; //Digit 3 of the Seven Segment Display is to connected to Digital Pin 0 of the Arduino. PIN NOT USED SINCE WE ONLY DISPLAYED FROM 0-99
  const int digit4 = 0; //Digit 4 of the Seven Segment Display is to connected to Digital Pin 0 of the Arduino. PIN NOT USED SINCE WE ONLY DISPLAYED FROM 0-99
  const int segmentA = 6; //Segment A of the Seven Segment Display is to connected to Digital Pin 6 of the Arduino
  const int segmentB = 7; //Segment B of the Seven Segment Display is to connected to Digital Pin 7 of the Arduino
  const int segmentC = 8; //Segment C of the Seven Segment Display is to connected to Digital Pin 8 of the Arduino
  const int segmentD = 9; //Segment D of the Seven Segment Display is to connected to Digital Pin 9 of the Arduino
  const int segmentE = 10; //Segment E of the Seven Segment Display is to connected to Digital Pin 10 of the Arduino
  const int segmentF = 11; //Segment F of the Seven Segment Display is to connected to Digital Pin 11 of the Arduino
  const int segmentG = 12; //Segment G of the Seven Segment Display is to connected to Digital Pin 12 of the Arduino
  const int segmentDP= 13; //Segment Decimal Point of the Seven Segment Display is to connected to Digital Pin 13 of the Arduino

  const int IRPin1 = 4; //IR Sensor for Exit connected to Digital Pin 4
  const int IRPin2 = 5; //IR Sensor for Entrance connected to Digital Pin 5

int temporaryVal = 0; //Temporary value used for the locking and unlock of the Parking Lot through the Stepper Motor
unsigned long timer; // variable for counting 
int deciSecond = 0; // the value to be displayed in the Seven Segment Display


void setup() {

  pinMode(segmentA, OUTPUT);  //A in the 4-Digit Seven Segment Display set as Output source
  pinMode(segmentB, OUTPUT);  //B in the 4-Digit Seven Segment Display set as Output source
  pinMode(segmentC, OUTPUT);  //C in the 4-Digit Seven Segment Display set as Output source
  pinMode(segmentD, OUTPUT);  //D in the 4-Digit Seven Segment Display set as Output source
  pinMode(segmentE, OUTPUT);  //E in the 4-Digit Seven Segment Display set as Output source
  pinMode(segmentF, OUTPUT);  //F in the 4-Digit Seven Segment Display set as Output source
  pinMode(segmentG, OUTPUT);  //G in the 4-Digit Seven Segment Display set as Output source
  pinMode(segmentDP, OUTPUT); // Decimal Point in the 4-Digit Seven Segment Display set as Output source

  pinMode(StepPin1, OUTPUT); //Stepper motor's Pin 1 set as Output source
  pinMode(StepPin2, OUTPUT); //Stepper motor's Pin 2 set as Output source
  pinMode(StepPin3, OUTPUT); //Stepper motor's Pin 3 set as Output source
  pinMode(StepPin4, OUTPUT); //Stepper motor's Pin 4 set as Output source

  pinMode(IRPin1, INPUT); //IR for Exit set as Input Source
  pinMode(IRPin2, INPUT); //IR for Entrance set as Input Source
  
  int displayType = COMMON_CATHODE; // The type of display used
  int numberOfDigits = 2;  // The number of digits to be displayed in the Seven Segment Display
  
  myDisplay.Begin(displayType, numberOfDigits, digit1, digit2, digit3, digit4, segmentA, segmentB, segmentC, segmentD, segmentE, segmentF, segmentG, segmentDP); //Setup of Seven Segment Display
  myDisplay.SetBrightness(100); //Set the brightness of the Seven Segment Display

  timer = millis(); //Set timer = millis 
  stepper.Enable(); //Enable Stepper Motor
  
  Serial.begin(9600);
}

void loop() {
  
   char tempString[10]; //Used for sprintf (Used for displaying in Seven Segment Display )
   sprintf(tempString, "%2d", deciSecond); //Convert deciSecond into a string that is right adjusted (Used for displaying in Seven Segment Display)
   myDisplay.DisplayString(tempString, 0); // Used for displaying in Seven Segment Display 
   
  if (millis() - timer >= 100) // if value displayed on seven segment displayed is less than 100 then call increment and decrement function
   incrementSevenSeg();
   decrementSevenSeg();
 

  if(temporaryVal == 1) { // if temporary value = 1. Then call the motorlock function. This fuction is used to lock the Parking lot / move the stepper motor 90 degrees to the left
    motorlock();
  }

  if( deciSecond == 98 && temporaryVal == 2) { // if digit displayed on seven seg disp = 98, and temporary value = 2, then unlock the parking lot / move the stepper motor 90 degrees to the right
    motorunlock();
  }

  if( deciSecond > 99){ // if value to be displayed is greater than 99, then display 99 instead. 
    deciSecond = 99;
  }
}

void incrementSevenSeg(){ //Decrement Function for Seven Segment Display
  if(digitalRead (IRPin2) == LOW){ //If IR2 detects an object then add 1 to the value displayed on Seven Segment Display
  deciSecond = deciSecond +1; // Increment value displayed
  delay (500); //Delay
  
      if(deciSecond == 99) { //if value displayed = 99, then lock the parking lot
      temporaryVal = 1; //set temporary value = 1
      Serial.println("on");
    }
  }
}  


void decrementSevenSeg(){ //Increment Function for Seven Segment Display
  if(digitalRead (IRPin1) == LOW){ //If IR1 detects an object then subtract 1 to the value displayed on Seven Segment Display
  deciSecond = deciSecond -1; // Decrement Value displayed
  delay (500); //Delay
      if( deciSecond < 0) { // if value displayed it to be less than 0, then display 0 instead.
      deciSecond =0 ;
      }
  }
  Serial.println("on");
}


void motorlock(){ //function for locking the parking lot
   stepper.Move(100); //Move stepper motor 90 degrees to the left
   delay(300); //Delay
   temporaryVal++; //Increment temporary value
  }

void motorunlock(){  //function for unlocking the parking lot
   stepper.Move(-100); //Move stepper motor 100 degrees to the right
   delay(300); //Delay
   temporaryVal = 0; //Temporary value = 0
} // end of program 
