#include <TinyStepper.h> //library for stepper motor
#include <Servo.h> //Library for the Servo motor

#define StepPin1 A2 //Stepper Pin 1 will be connected to Analog Pin 2 
#define StepPin2 A3 //Stepper Pin 2 will be connected to Analog Pin 3
#define StepPin3 A4 //Stepper Pin 3 will be connected to Analog Pin 4
#define StepPin4 A5 //Stepper Pin 4 will be connected to Analog Pin 5
#define HALFSTEPS 4096  // This is the numbers of step for a full 360 degree rotation
TinyStepper stepper(HALFSTEPS, StepPin1, StepPin2, StepPin3, StepPin4); //Code required for the Stepper Motor library

Servo servoMotor; //Declaration of servo motor variable. Needed for the library 

  int LEDPin1=12; //Positive side of LED 1 connected to digital pin 12
  int LEDPin2=11; //Positive side of LED 2 connected to digital pin 11
  int LEDPin3=8;  //Positive side of LED 3 connected to digital pin 8
  
  const int BiodegradableButtonPin = 5; //The push button for the Biodegradable Trash bin will be connected to Digital Pin 5
  const int NonBiodegradableButtonPin = 6; //The push button for the Non-Biodegradable Trash bin will be connected to Digital Pin 6

  int RelayPin = 9; //Relay Control's signal pin will be connected to Digital Pin 9
    
int counter = 0; //Declaration of counter variable 
int temporaryVal = 0; //Declaration of temporaryVal

int pos = 0;    // variable to store the servo position
 
int BiodegradablebuttonState = 0;         // current state of the Biodegradablebutton
int lastBiodegradablebuttonState = 0;     // previous state of the Biodegradablebutton
int NonBiodegradablebuttonState = 0;         // current state of the Non-Biodegradable button
int lastNonBiodegradablebuttonState = 0;     // previous state of the Non-Biodegradable button

const int buzzerPin = 4; //Piezo buzzer's signal pin will be connected to Digital Pin 4

const int a = 440; //Variable for note a (sound produced by the buzzer)

void setup() {

  
  pinMode(StepPin1, OUTPUT); //Stepper motor's Pin 1 set as Output source
  pinMode(StepPin2, OUTPUT); //Stepper motor's Pin 2 set as Output source
  pinMode(StepPin3, OUTPUT); //Stepper motor's Pin 3 set as Output source
  pinMode(StepPin4, OUTPUT); //Stepper motor's Pin 4 set as Output source
  
  pinMode(LEDPin1,OUTPUT); //LED 1 set as output device
  pinMode(LEDPin2,OUTPUT); //LED 2 set as output device
  pinMode(LEDPin3,OUTPUT); //LED 3 set as output device

  pinMode(RelayPin,OUTPUT); //Relay set as output device

  pinMode(buzzerPin, OUTPUT); //Buzzer set as output device

  pinMode(BiodegradableButtonPin, INPUT_PULLUP); //Push button for Biodegrable Trash bin set as Input Source
  pinMode(NonBiodegradableButtonPin, INPUT_PULLUP); //Push button for Non-Biodegrable Trash bin set as Input Source

  servoMotor.attach(2); //Servo Motor connected to Digital Pin 2
  servoMotor.write(0); //Set servo motor to 0 degrees
  
  stepper.Enable(); //Enable Stepper Motor. Needed for the Stepper motor library

}

void loop() {
  
   BiodegradablebuttonState = digitalRead(BiodegradableButtonPin); //Set BiodegrableButtonState to the digital value of Digital Pin 5
   NonBiodegradablebuttonState = digitalRead(NonBiodegradableButtonPin); //Set NonBiodegrableButtonState to the digital value of Digital Pin 6
   temporaryVal = counter % 2; //Temporary = the remainder of counter/2 (modulo)


  if( BiodegradablebuttonState == HIGH && NonBiodegradablebuttonState == HIGH) { // If the two push buttons were pushed at the same time
    counter++; //Increment counter
    delay(100); //Delay 1000ms 
  }

   
  if (temporaryVal == 0){ // If temporaryVal is 0
      digitalWrite(12, LOW); //Turn off LED 
      digitalWrite(11, LOW); //Turn off LED 
      digitalWrite(8, LOW);  //Turn off LED 
      digitalWrite(9, HIGH);  //Turn on Relay Control 
      BiodegradableIncrementSevenSeg(); //Call SecondIncrementSevenSeg Function
      NonBiodegradableIncrementSevenSeg(); //Call MInuteIncrementSevenSeg Function 
      delay(100); //Delay 100ms
  }

  else if (temporaryVal == 1) { // If temporaryVal is 1
    digitalWrite(12, HIGH); //Turn on LED 
    digitalWrite(11, HIGH); //Turn on LED 
    digitalWrite(8, HIGH);  //Turn on LED 
    digitalWrite(9, LOW);  //Turn off relay control
    Sounds(); //Call Sounds function
    delay(1000); //Delay 100ms
    counter++; //Increment counter
  }

}

void beep(int note, int duration){
  tone(buzzerPin, note, duration);   //Play tone on Buzzer
  delay(50); //Delay 50ms

}

void Sounds(){
    beep(a, 1500); //Product note a on Piezo Buzzer for 1500ms
    delay(1500);  // Delay 1500ms
}

void BiodegradableIncrementSevenSeg(){ //The function used to open the lid for the biodegradable trash bin
  if (BiodegradablebuttonState != lastBiodegradablebuttonState) { //If Current State of the Push button is not equal to Previous State

    if (BiodegradablebuttonState == LOW) { //If Push button was pressed
      delay(5000); //Delay 5000ms
      digitalWrite(9, LOW);  //Turn off relay control
      digitalWrite(8, LOW);  //Turn off LED 
      for (pos = 0; pos <= 180; pos += 1) { // Servo goes from 0 degrees to 180 degrees
      servoMotor.write(pos);              // Servo to go to position in variable 'pos'
      delay(15);                       // Delay 15ms 
  }
      delay(5000);
      digitalWrite(9, HIGH);  //Turn on relay control
      delay(300); //Delay 100ms
      digitalWrite(9, LOW);  //Turn off relay control
      delay(300); //Delay 100ms
      digitalWrite(9, HIGH);  //Turn on relay control
      delay(300); //Delay 100ms
      digitalWrite(9, LOW);  //Turn off relay control
      delay(300); //Delay 100ms
      digitalWrite(9, HIGH);  //Turn on relay control
      delay(300); //Delay 100ms
      digitalWrite(9, LOW);  //Turn off relay control
      delay(300); //Delay 100ms
      digitalWrite(9, HIGH);  //Turn on relay control
      digitalWrite(8, HIGH);  //Turn off LED 
      for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      servoMotor.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // Delay 15ms
  }
    }
  }
  lastBiodegradablebuttonState = BiodegradablebuttonState;   // Save the current state as the last state, for next time through the loop
}

void NonBiodegradableIncrementSevenSeg(){ //The function used to open the lid for the non-biodegradable trash bin
  if (NonBiodegradablebuttonState != lastNonBiodegradablebuttonState) { //If Current State of the Push button is not equal to Previous State

    if (NonBiodegradablebuttonState == LOW) { //If Push button was pressed
      digitalWrite(12, HIGH); //Turn on LED 
      stepper.Move(180); //Move stepper motor 90 degrees to the left 
      delay(5000); //Delay 3000ms 
      digitalWrite(12, LOW); //Turn off LED 
      delay(300); //Delay 100ms 
      digitalWrite(12, HIGH); //Turn on LED 
      delay(300); //Delay 100ms 
      digitalWrite(12, LOW); //Turn off LED 
      delay(300); //Delay 100ms 
      digitalWrite(12, HIGH); //Turn on LED 
      delay(300); //Delay 100ms 
      digitalWrite(12, LOW); //Turn off LED 
      delay(300); //Delay 100ms 
      digitalWrite(12, HIGH); //Turn on LED 
      delay(300); //Delay 100ms 
      digitalWrite(12, LOW); //Turn off LED 
      delay(300); //Delay 100ms 
      digitalWrite(11, HIGH); //Turn on LED 
      stepper.Move(-180); //Move stepper motor 90 degrees to the left
      delay(300); //Delay 300ms 
      digitalWrite(11, LOW); //Turn off LED 
    } 
    delay(500); //Delay 500ms
   }
  lastNonBiodegradablebuttonState = NonBiodegradablebuttonState;   // Save the current state as the last state, for next time through the loop
}
