#include <IRremote.h>
#include <Keypad.h> //include library code
#include <TinyStepper.h> //library for stepper motor
#include <Servo.h> //Library for the Servo motor

#define StepPin1 A0 //Stepper Pin 1 will be connected to Digital Pin 13
#define StepPin2 A1 //Stepper Pin 2 will be connected to Digital Pin 12
#define StepPin3 A2 //Stepper Pin 3 will be connected to Digital Pin 11
#define StepPin4 A3 //Stepper Pin 4 will be connected to Digital Pin 10
#define HALFSTEPS 4096  // This is the numbers of step for a full 360 degree rotation
TinyStepper stepper(HALFSTEPS, StepPin1, StepPin2, StepPin3, StepPin4); //Code required for the Stepper Motor library

Servo servoMotor; //Declaration of servo motor variable. Needed for the library 

#define Button_1 0xFF30CF
#define Button_2 0xFF18E7 
#define Button_3 0xFF7A85
#define Button_4 0xFF10EF
#define Button_5 0xFF38C7
#define Button_6 0xFF5AA5

IRsend irsend;

int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
decode_results results;

const int buzzerPin = 13; //Piezo buzzer's signal pin will be connected to Digital Pin 8

const byte rows = 4;      //number of keypad rows
const byte cols = 4;      //number of keypad columns
char keys[rows][cols] = { //enter the setup of your keypad
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};

byte rowPins[rows] = {11, 10, 9, 8}; //ROW1, ROW2, ROW3 and ROW4 pins declared here. Change them to yours 
byte colPins[cols] = {7, 6, 5, 4}; //COL1, COL2, COL3 and COL4 pins declared here. Change them to yours
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

int counterOne= 0; //Declaration of counter variable 
int temporaryValOne = 0; //Declaration of temporaryValOne
int counterTwo = 0; //Declaration of counter variable 
int temporaryValTwo = 0; //Declaration of temporaryValTwo
int counterThree= 0; //Declaration of counter variable 
int temporaryValThree = 0; //Declaration of temporaryValThree

int timer;

void setup() {
  Serial.begin(9600);                   //initialise the serial communication at 9600 bps
  irrecv.enableIRIn(); // Start the receiver
  pinMode(buzzerPin,OUTPUT);

  pinMode(StepPin1, OUTPUT); //Stepper motor's Pin 1 set as Output source
  pinMode(StepPin2, OUTPUT); //Stepper motor's Pin 2 set as Output source
  pinMode(StepPin3, OUTPUT); //Stepper motor's Pin 3 set as Output source
  pinMode(StepPin4, OUTPUT); //Stepper motor's Pin 4 set as Output source

  servoMotor.attach(12); //Servo Motor connected to Digital Pin 12
  servoMotor.write(90); //Set servo motor to 90 degrees

  stepper.Enable(); //Enable Stepper Motor. Needed for the Stepper motor library

}
void loop() { //checks for a key pressed
  
  
  char key = keypad.getKey();
  
  temporaryValOne = counterOne % 2; //TemporaryVal = the remainder of counter/2 (modulo)
  temporaryValTwo = counterTwo % 2; //TemporaryVal = the remainder of counter/2 (modulo)
  temporaryValThree = counterThree % 2; //TemporaryVal = the remainder of counter/2 (modulo)

    if (key){
    Serial.println(key);
    }

    
    if (key == '1') {
      Serial.println("1 Pressed");
      timer = 0;
      counterOne = 1;
      delay(50);
      irsend.sendNEC(0xF720DF, 32);     //Enter Remote Hex Value
    }

    if (key == '2') {
      Serial.println("2 Pressed");
      timer = 0;
      counterOne = 0;
      delay(50);
      irsend.sendNEC(0xF7A05F, 32);     //Enter Remote Hex Value
    }

    if (key == '3') {
      Serial.println("3 Pressed");
      timer = 0;
      counterTwo = 1;
      delay(50);
      irsend.sendNEC(0xF7609F, 32);     //Enter Remote Hex Value
    }

    if (key == '4') {
      Serial.println("4 Pressed");
      timer = 0;
      counterTwo = 0;
      delay(50);
      irsend.sendNEC(0xF748B7, 32);     //Enter Remote Hex Value
    }

    if (key == '5') {
      Serial.println("5 Pressed");
      timer = 0;
      counterThree = 1;
      delay(50);
      irsend.sendNEC(0xF7708F, 32);     //Enter Remote Hex Value
    }
    
    if (key == '6') {
      Serial.println("6 Pressed");
      timer = 0;
      counterThree = 0;
      delay(50);
      irsend.sendNEC(0xF728D7, 32);     //Enter Remote Hex Value
    }
    
    if (key == '7') {
      Serial.println("7 Pressed");
      timer = 0;
      delay(50);
      irsend.sendNEC(0xF7F00F, 32);     //Enter Remote Hex Value
    }
    
    if (key == '8') {
      Serial.println("8 Pressed");
      timer = 0;
      delay(50);
      irsend.sendNEC(0xF7E817, 32);     //Enter Remote Hex Value
    }
    
    if (key == '9') {
      Serial.println("9 Pressed");
      timer = 0;
      delay(50);
      irsend.sendNEC(0xF7D02F, 32);     //Enter Remote Hex Value
    }

    if (irrecv.decode(&results)) {
      Serial.println(results.value, DEC);
      dump(&results);
      switch(results.value) {
           case Button_1 :  counterOne = 1; break; 
           case Button_2 :  counterOne = 0; break;
           case Button_3 :  counterTwo = 1; break; 
           case Button_4 :  counterTwo = 0; break;
           case Button_5 :  counterThree = 1; break; 
           case Button_6 :  counterThree = 0; break;     
      } 
      irrecv.resume(); // Receive the next value
  }

  if (temporaryValOne == 0){
    digitalWrite(buzzerPin,LOW);
    delay(100);
    
  }

  if (temporaryValOne == 1){
    digitalWrite(buzzerPin,HIGH);
    delay(100);
  }

  if (temporaryValTwo == 0){
    servoMotor.write(90); //Set servo motor to 0 degrees
    delay(100); 
  }

  if (temporaryValTwo == 1){
    servoMotor.write(180); //Set servo motor to 0 degrees
    delay(100);
    servoMotor.write(90); //Set servo motor to 0 degrees
    delay(100);
  }

  if (temporaryValThree == 0){
    stepper.Move(0); //Move stepper motor 90 degrees to the left 
    delay(100);
  }


  if (temporaryValThree == 1){
    stepper.Move(360); //Move stepper motor 90 degrees to the left 
    delay(500);
    stepper.Move(0); //Move stepper motor 90 degrees to the left 
    delay(500);
  }
  
  
}

void dump(decode_results *results) {
  // Dumps out the decode_results structure.
  // Call this after IRrecv::decode()
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN) {
    Serial.print("Unknown encoding: ");
  }
  else if (results->decode_type == NEC) {
    Serial.print("Decoded NEC: ");
  }
  else if (results->decode_type == SONY) {
    Serial.print("Decoded SONY: ");
  }
  else if (results->decode_type == RC5) {
    Serial.print("Decoded RC5: ");
  }
  else if (results->decode_type == RC6) {
    Serial.print("Decoded RC6: ");
  }
  else if (results->decode_type == PANASONIC) {
    Serial.print("Decoded PANASONIC - Address: ");
    Serial.print(results->address, HEX);
    Serial.print(" Value: ");
  }
  else if (results->decode_type == LG) {
    Serial.print("Decoded LG: ");
  }
  else if (results->decode_type == JVC) {
    Serial.print("Decoded JVC: ");
  }
  else if (results->decode_type == AIWA_RC_T501) {
    Serial.print("Decoded AIWA RC T501: ");
  }
  else if (results->decode_type == WHYNTER) {
    Serial.print("Decoded Whynter: ");
  }
  Serial.print(results->value, HEX);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): ");
  
  for (int i = 1; i < count; i++) {
    if (i & 1) {
      Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
    }
    else {
      Serial.write('-');
      Serial.print((unsigned long) results->rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.print(" ");
  }
  Serial.println();
}

void CheckTempVal() {
  Serial.print("Temporary Value One: "); 
  Serial.println(temporaryValOne); //Print temporaryValOne
  Serial.print("Temporary Value Two: ");
  Serial.println(temporaryValTwo); //Print temporaryValTwo
  Serial.print("Temporary Value Three: ");
  Serial.println(temporaryValThree); //Print temporaryValThree
}
