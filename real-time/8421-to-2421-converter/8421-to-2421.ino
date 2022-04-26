//Declaration of the Arduin Pin where the buttons and LEDs are to be connected on
const int ButtonPin1 = 7; // Button 1 to Digital Pin 7
const int ButtonPin2 = 6; // Button 2 to Digital Pin 6
const int ButtonPin3 = 5; // Button 3 to Digital Pin 5
const int ButtonPin4 = 4; // Button 4 to Digital Pin 4
const int LedPin1 = 11;  // LED 1 to Digital Pin 11
const int LedPin2 = 10;  // LED 1 to Digital Pin 11
const int LedPin3 = 9;   // LED 1 to Digital Pin 11
const int LedPin4 = 8;   // LED 1 to Digital Pin 11

int ButtonBit1 = 0; //Declaration of variable ButtonBit1 which will be used for calculation later
int ButtonBit2 = 0; //Declaration of variable ButtonBit2 which will be used for calculation later
int ButtonBit3 = 0; //Declaration of variable ButtonBit3 which will be used for calculation later
int ButtonBit4 = 0; //Declaration of variable ButtonBit4 which will be used for calculation later

void setup() {


  pinMode(ButtonPin1, INPUT); //Declaration of the button 1 as the source of input
  pinMode(ButtonPin2, INPUT); //Declaration of the button 2 as the source of input
  pinMode(ButtonPin3, INPUT); //Declaration of the button 3 as the source of input
  pinMode(ButtonPin4, INPUT); //Declaration of the button 4 as the source of input
  

  pinMode(LedPin1, OUTPUT); //Declaration of the LED1 as the output medium
  pinMode(LedPin2, OUTPUT); //Declaration of the LED2 as the output medium
  pinMode(LedPin3, OUTPUT); //Declaration of the LED3 as the output medium
  pinMode(LedPin4, OUTPUT); //Declaration of the LED4 as the output medium


}

void loop() { //LOOP


  ButtonBit1 = digitalRead(ButtonPin1);  //Check Button 1 for input
  ButtonBit2 = digitalRead(ButtonPin2);  //Check Button 2 for input
  ButtonBit3 = digitalRead(ButtonPin3);  //Check Button 3 for input
  ButtonBit4 = digitalRead(ButtonPin4);  //Check Button 4 for input

  int ButtonBit1Cal = ButtonBit1;   //Declaration of variable for calculation
  int ButtonBit2Cal = ButtonBit2;   //Declaration of variable for calculation
  int ButtonBit3Cal = ButtonBit3;   //Declaration of variable for calculation
  int ButtonBit4Cal = ButtonBit4;   //Declaration of variable for calculation

  int BinaryTotal;    //Declaration of variable for calculation
  BinaryTotal=(ButtonBit4*8)+(ButtonBit3*4)+(ButtonBit2*2)+(ButtonBit1*1); // Binary to Decimal conversion
  Serial.println(BinaryTotal); //Print BinaryTotal 
  
    digitalWrite(LedPin1, LOW); //Reset LED1 to LOW
    digitalWrite(LedPin2, LOW); //Reset LED2 to LOW
    digitalWrite(LedPin3, LOW); //Reset LED3 to LOW
    digitalWrite(LedPin4, LOW); //Reset LED4 to LOW

    if(BinaryTotal == 9) // if Binary Total = 9; Convert Decimal 9 to 2421 wherein the output is 1111 
  {
    digitalWrite(LedPin1, HIGH); digitalWrite(LedPin2, HIGH); digitalWrite(LedPin3, HIGH); digitalWrite(LedPin4, HIGH); //Turn LEDs ON
    
  }
  
      if(BinaryTotal == 8) // if Binary Total = 9; Convert Decimal 8 to 2421 wherein the output is 1110
  {
    digitalWrite(LedPin2, HIGH); digitalWrite(LedPin3, HIGH); digitalWrite(LedPin4, HIGH); //Turn LEDs ON
  }
  
      if(BinaryTotal == 7) // if Binary Total = 9; Convert Decimal 7 to 2421 wherein the output is 1101
  {
    digitalWrite(LedPin1, HIGH); digitalWrite(LedPin3, HIGH); digitalWrite(LedPin4, HIGH); //Turn LEDs ON
  }
  
      if(BinaryTotal == 6) // if Binary Total = 9; Convert Decimal 6 to 2421 wherein the output is 1100
  {
    digitalWrite(LedPin3, HIGH); digitalWrite(LedPin4, HIGH); //Turn LEDs ON
  }

      if(BinaryTotal == 5) // if Binary Total = 9; Convert Decimal 5 to 2421 wherein the output is 1011
  {
    digitalWrite(LedPin1, HIGH); digitalWrite(LedPin2, HIGH); digitalWrite(LedPin4, HIGH); //Turn LEDs ON
  }

     if(BinaryTotal == 4) // if Binary Total = 9; Convert Decimal 4 to 2421 wherein the output is 0100
  {
    digitalWrite(LedPin3, HIGH); //Turn LEDs ON
  }

    if(BinaryTotal == 3) // if Binary Total = 9; Convert Decimal 3 to 2421 wherein the output is 0011 
  {
    digitalWrite(LedPin1, HIGH); digitalWrite(LedPin2, HIGH); //Turn LEDs ON
  }
  
    if(BinaryTotal == 2) // if Binary Total = 9; Convert Decimal 2 to 2421 wherein the output is 0010
  {
    digitalWrite(LedPin2, HIGH); //Turn LEDs ON
  }
  
  if(BinaryTotal == 1) // if Binary Total = 9; Convert Decimal 1 to 2421 wherein the output is 0001
  {
    digitalWrite(LedPin1, HIGH); //Turn LEDs ON
  }

  if(BinaryTotal == 0) // if Binary Total = 9; Convert Decimal 0 to 2421 wherein the output is 0000
  {
    digitalWrite(LedPin1, LOW); digitalWrite(LedPin2, LOW); digitalWrite(LedPin3, LOW); digitalWrite(LedPin4, LOW); //Turn LEDs OFF
  }
  
} //end of program
