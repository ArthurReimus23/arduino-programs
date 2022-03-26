#include <Wire.h> //Library for communicating with I2C
#include <Keypad.h> //Library for Keypad
#include <TinyStepper.h> //Library for stepper motor
#include <LiquidCrystal_I2C.h> //Library for the LCD

#define StepPin1 A0 //Stepper Pin 1 will be connected to Analog Pin 0
#define StepPin2 A1 //Stepper Pin 2 will be connected to Analog Pin 1
#define StepPin3 A2 //Stepper Pin 3 will be connected to Analog Pin 2
#define StepPin4 A3 //Stepper Pin 4 will be connected to Analog Pin 3
#define HALFSTEPS 4096  // This is the numbers of step for a full 360 degree rotation
TinyStepper stepper(HALFSTEPS, StepPin1, StepPin2, StepPin3, StepPin4); //Code required for the Stepper Motor library

const int buzzerPin = 13; //Piezo buzzer's signal pin will be connected to Digital Pin 13

#define Password_Digits 7
char Data[Password_Digits];
char Master[Password_Digits] = "118208";

byte character_count = 0;

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

LiquidCrystal_I2C lcd(0x27, 20, 4);

const int a = 1500; //Variable for note a (sound produced by the buzzer)
const int b = 1550; //Variable for note b (sound produced by the buzzer)
const int c = 523;  //Variable for note c (sound produced by the buzzer)
const int d = 330;  //Variable for note d (sound produced by the buzzer)

int i = 10;
int interval=1000;

unsigned long currentMillis = 0;
unsigned long previousMillis=0;

void setup() {
  
  Serial.begin(9600); //initialise the serial communication at 9600 bps
  
  pinMode(buzzerPin,OUTPUT); 

  pinMode(StepPin1, OUTPUT); //Stepper motor's Pin 1 set as Output source
  pinMode(StepPin2, OUTPUT); //Stepper motor's Pin 2 set as Output source
  pinMode(StepPin3, OUTPUT); //Stepper motor's Pin 3 set as Output source
  pinMode(StepPin4, OUTPUT); //Stepper motor's Pin 4 set as Output source

  lcd.begin(); //Initialize LCD
  lcd.backlight(); //Turn on Backlight of LCD
  
  stepper.Enable(); //Enable Stepper Motor. Needed for the Stepper motor library
}

void loop() {
  
  char key = keypad.getKey();
  unsigned long currentMillis = millis();
  int i = 10;
  
  
  lcd.setCursor(0,0);  
  lcd.print("Password:");
  lcd.setCursor(0,1);  
  lcd.print("Enter 6 Digits");
  lcd.setCursor(0,2); 
  lcd.print("A-Enter B-Reset");
 
    if (key){
    Data[character_count] = key;
    Sounds_One();
    Serial.println(key);
    lcd.setCursor(character_count + 10, 0);
    lcd.print("*");
    character_count++;
    delay(100);
    }
    
    if (key == 'B') {
      Serial.println("B Pressed");
      lcd.clear();
      clearData();
      delay(50);
    }
        

    if (character_count == Password_Digits - 1) {
      char key = keypad.getKey();
      
      if (key == 'A') { 
       Serial.println("A Pressed");

       if (!strcmp(Data, Master)) {
        lcd.setCursor(0,3);  //Go to line 1 of LCD
        Serial.println("Password entered is Correct");
        lcd.print("Password Correct");
        Sounds_Two();
        delay(100);
        clearData();
        stepper.Move(-100);
        Password_Correct();
    }
    else {
      Serial.println("Password entered is Incorrect");
      lcd.setCursor(0,3);  
      lcd.print("Password Incorrect");
      Sounds_Three();
      delay(100);
      lcd.clear();
      clearData();
    }
    }
    if (key == 'B') {
      Serial.println("B Pressed");
      lcd.clear();
      clearData();
      delay(50);
    }
    }
    
    if (key == 'B') {
      Serial.println("B Pressed");
      lcd.clear();
      clearData();
      delay(50);
    }
    
    
  }

void clearData() {
  // Go through array and clear data
  while (character_count != 0) {
    Data[character_count--] = 0;
  }
  return;
}

void Password_Correct() { 
  
  while (i > 0) {

    char key = keypad.getKey();
    unsigned long currentMillis = millis();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Time Remaining: ");
    lcd.print(i); 
    
    
    if ((unsigned long)(currentMillis - previousMillis) >= interval) {
      i--;
      previousMillis = currentMillis;
    }
  
    if (key == '*') {
      Serial.println("* Pressed");
      lcd.setCursor(0,1); 
      lcd.print("Time Extended by 5s");
      Sounds_One();
      i = i + 5;
      delay(10);
    }
    
    if (key == '#') {
      Serial.println("# Pressed");
      i = 0;
      delay(10);
    }

    if (i == 0) {
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print("Time Remaining: ");
      lcd.print(i);
      lcd.setCursor(0,1);
      lcd.print("Vault Locked");
      Sounds_Four();
      stepper.Move(100);
      delay(1000);
      lcd.clear();
    }
    delay(100);
  }
  i = 10;
}

void beep(int note, int duration){
  tone(buzzerPin, note, duration);  
  delay(50);

}

void Sounds_One() {
    beep(a, 200); 
    delay(200); 
}

void Sounds_Two() {
    beep(b, 500);
    delay(500); 
    beep(b, 500); 
    delay(500); 
    beep(b, 500);
    delay(100);  
}

void Sounds_Three() {
    beep(c, 1500);
    delay(1500); 
}

void Sounds_Four() {
    beep(d, 1500);
    delay(1500); 
}
