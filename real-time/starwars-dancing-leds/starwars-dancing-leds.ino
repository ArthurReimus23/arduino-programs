// The frequencies of the tones 

const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;

// The Pin configuration of the Buzzer and the LEDs
const int buzzerPin = 2;
const int ledPin1 = 8;
const int ledPin2 = 1;
const int ledPin3 = 13;
const int ledPin4 = 12;
const int ledPin5 = 11;
const int ledPin6 = 10;
const int ledPin7 = 9;
const int ledPin8 = 7;
const int ledPin9 = 6;
const int ledPin10 = 5;
const int ledPin11 = 4;
const int ledPin12 = 3;
 
int counter = 0; // Counter
 
void setup()
{
  
// Setup the Pin Modes whether it is input or output

  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(ledPin5, OUTPUT);
  pinMode(ledPin6, OUTPUT);
  pinMode(ledPin7, OUTPUT);
  pinMode(ledPin8, OUTPUT);
  pinMode(ledPin9, OUTPUT);
  pinMode(ledPin10, OUTPUT);
  pinMode(ledPin11, OUTPUT);
  pinMode(ledPin12, OUTPUT);

}
 
void loop()
{
 // Play music sections (1st-4th section of the Starwars Theme)
 
  firstSection();
  secondSection();
  thirdSection();
  secondSection();
  fourthSection();

}
 
void beep(int note, int duration)
{

  tone(buzzerPin, note, duration);   //Play tone on Buzzer

  if(counter % 2 == 0)   //Turn on/off LED 1 depending on value of 'counter'
  {
    digitalWrite(ledPin1, HIGH);
    delay(duration);
    digitalWrite(ledPin1, LOW);
  }
  else //Turn on/off LED 2 depending on value of 'counter'
  {
    digitalWrite(ledPin2, HIGH);
    delay(duration);
    digitalWrite(ledPin2, LOW);
  }
 

  noTone(buzzerPin);   //Stop playing tone on buzzerPin
 
  delay(50);
 
   counter++;   //Increment the counter
}
 
void firstSection() // The tones and the corresponding LED controls (ON/OFF)
{
  beep(a, 500);   digitalWrite(ledPin3, HIGH); digitalWrite(ledPin7, HIGH); 
  beep(a, 500);   digitalWrite(ledPin3, LOW);  digitalWrite(ledPin7, LOW);
  beep(a, 500);   digitalWrite(ledPin4, HIGH); digitalWrite(ledPin6, HIGH); 
  beep(f, 350);   digitalWrite(ledPin4, LOW);  digitalWrite(ledPin6, LOW);
  beep(cH, 150);  digitalWrite(ledPin5, HIGH); 
  beep(a, 500);   digitalWrite(ledPin5, LOW);
  beep(f, 350);   digitalWrite(ledPin4, HIGH); digitalWrite(ledPin6, HIGH);  
  beep(cH, 150);  digitalWrite(ledPin4, LOW);  digitalWrite(ledPin6, LOW);
  beep(a, 650);   digitalWrite(ledPin3, HIGH); digitalWrite(ledPin7, HIGH); 
 
  delay(500); // delay
 
  beep(eH, 500);  digitalWrite(ledPin3, LOW);  digitalWrite(ledPin7, LOW); digitalWrite(ledPin8, HIGH); digitalWrite(ledPin12, HIGH); 
  beep(eH, 500);  digitalWrite(ledPin8, LOW); digitalWrite(ledPin12, LOW);
  beep(eH, 500);  digitalWrite(ledPin9, HIGH); digitalWrite(ledPin11, HIGH);
  beep(fH, 350);  digitalWrite(ledPin9, LOW); digitalWrite(ledPin11, LOW);
  beep(cH, 150);  digitalWrite(ledPin10, HIGH);
  beep(gS, 500);  digitalWrite(ledPin10, LOW); 
  beep(f, 350);   digitalWrite(ledPin9, HIGH); digitalWrite(ledPin11, HIGH);
  beep(cH, 150);  digitalWrite(ledPin9, LOW); digitalWrite(ledPin11, LOW); 
  beep(a, 650);   digitalWrite(ledPin8, HIGH); digitalWrite(ledPin12, HIGH);

  delay(500);  // delay
}
 
void secondSection() // The tones and the corresponding LED controls (ON/OFF)
{
  beep(aH, 500);  digitalWrite(ledPin8, LOW); digitalWrite(ledPin12, LOW);
  beep(a, 300); digitalWrite(ledPin3, HIGH); digitalWrite(ledPin7, HIGH); digitalWrite(ledPin8, HIGH); digitalWrite(ledPin12, HIGH);  
  beep(a, 150); digitalWrite(ledPin4, HIGH); digitalWrite(ledPin6, HIGH); digitalWrite(ledPin9, HIGH); digitalWrite(ledPin11, HIGH);
  beep(aH, 500); digitalWrite(ledPin5, HIGH); digitalWrite(ledPin10, HIGH); 
  beep(gSH, 325);
  beep(gH, 175); digitalWrite(ledPin3, LOW); digitalWrite(ledPin4, LOW); digitalWrite(ledPin5, LOW); digitalWrite(ledPin6, LOW);  digitalWrite(ledPin7, LOW); digitalWrite(ledPin8, LOW); digitalWrite(ledPin9, LOW); digitalWrite(ledPin10, LOW); digitalWrite(ledPin11, LOW);  digitalWrite(ledPin12, LOW);
  beep(fSH, 125); digitalWrite(ledPin3, HIGH); digitalWrite(ledPin4, HIGH); digitalWrite(ledPin5, HIGH); digitalWrite(ledPin6, HIGH);  digitalWrite(ledPin7, HIGH); digitalWrite(ledPin8, HIGH); digitalWrite(ledPin9, HIGH); digitalWrite(ledPin10, HIGH); digitalWrite(ledPin11, HIGH);  digitalWrite(ledPin12, HIGH);
  beep(fH, 125); digitalWrite(ledPin3, LOW); digitalWrite(ledPin4, LOW); digitalWrite(ledPin5, LOW); digitalWrite(ledPin6, LOW);  digitalWrite(ledPin7, LOW); digitalWrite(ledPin8, LOW); digitalWrite(ledPin9, LOW); digitalWrite(ledPin10, LOW); digitalWrite(ledPin11, LOW);  digitalWrite(ledPin12, LOW);    
  beep(fSH, 250);
 
  delay(325);  // delay
 
  beep(aS, 250);  digitalWrite(ledPin3, HIGH); digitalWrite(ledPin4, HIGH); digitalWrite(ledPin5, HIGH); digitalWrite(ledPin6, HIGH);  digitalWrite(ledPin7, HIGH);
  beep(dSH, 500); digitalWrite(ledPin3, LOW); digitalWrite(ledPin4, LOW); digitalWrite(ledPin5, LOW); digitalWrite(ledPin6, LOW);  digitalWrite(ledPin7, LOW);
  beep(dH, 325);  digitalWrite(ledPin8, HIGH); digitalWrite(ledPin9, HIGH); digitalWrite(ledPin10, HIGH); digitalWrite(ledPin11, HIGH);  digitalWrite(ledPin12, HIGH);
  beep(cSH, 175); digitalWrite(ledPin8, LOW); digitalWrite(ledPin9, LOW); digitalWrite(ledPin10, LOW); digitalWrite(ledPin11, LOW);  digitalWrite(ledPin12, LOW);
  beep(cH, 125);  digitalWrite(ledPin3, HIGH); digitalWrite(ledPin4, HIGH); digitalWrite(ledPin5, HIGH); digitalWrite(ledPin6, HIGH);  digitalWrite(ledPin7, HIGH); digitalWrite(ledPin8, HIGH); digitalWrite(ledPin9, HIGH); digitalWrite(ledPin10, HIGH); digitalWrite(ledPin11, HIGH);  digitalWrite(ledPin12, HIGH);
  beep(b, 125);   digitalWrite(ledPin3, LOW); digitalWrite(ledPin4, LOW); digitalWrite(ledPin5, LOW); digitalWrite(ledPin6, LOW);  digitalWrite(ledPin7, LOW); digitalWrite(ledPin8, LOW); digitalWrite(ledPin9, LOW); digitalWrite(ledPin10, LOW); digitalWrite(ledPin11, LOW);  digitalWrite(ledPin12, LOW);
  beep(cH, 250);  
 
  delay(350);  // delay
}

void thirdSection() // The tones and the corresponding LED controls (ON/OFF)
{
  beep(f, 250);  
  beep(gS, 500);  
  beep(f, 350);  
  beep(a, 125);
  beep(cH, 500);
  beep(a, 375);  
  beep(cH, 125); digitalWrite(ledPin3, HIGH); digitalWrite(ledPin4, HIGH); digitalWrite(ledPin5, HIGH); digitalWrite(ledPin6, HIGH);  digitalWrite(ledPin7, HIGH); digitalWrite(ledPin8, HIGH); digitalWrite(ledPin9, HIGH); digitalWrite(ledPin10, HIGH); digitalWrite(ledPin11, HIGH);  digitalWrite(ledPin12, HIGH); 
  beep(eH, 650); digitalWrite(ledPin3, LOW); digitalWrite(ledPin4, LOW); digitalWrite(ledPin5, LOW); digitalWrite(ledPin6, LOW);  digitalWrite(ledPin7, LOW); digitalWrite(ledPin8, LOW); digitalWrite(ledPin9, LOW); digitalWrite(ledPin10, LOW); digitalWrite(ledPin11, LOW);  digitalWrite(ledPin12, LOW);
  
 
  delay(500);  // delay
}

void fourthSection() // The tones and the corresponding LED controls (ON/OFF)
{
  beep(f, 250); digitalWrite(ledPin3, HIGH); digitalWrite(ledPin4, HIGH); digitalWrite(ledPin5, HIGH); digitalWrite(ledPin6, HIGH);  digitalWrite(ledPin7, HIGH); digitalWrite(ledPin8, HIGH); digitalWrite(ledPin9, HIGH); digitalWrite(ledPin10, HIGH); digitalWrite(ledPin11, HIGH);  digitalWrite(ledPin12, HIGH); 
  beep(gS, 500);  
  beep(f, 375);  
  beep(cH, 125);
  beep(a, 500);  
  beep(f, 375);  
  beep(cH, 125); 
  beep(a, 650);  digitalWrite(ledPin3, LOW); digitalWrite(ledPin4, LOW); digitalWrite(ledPin5, LOW); digitalWrite(ledPin6, LOW);  digitalWrite(ledPin7, LOW); digitalWrite(ledPin8, LOW); digitalWrite(ledPin9, LOW); digitalWrite(ledPin10, LOW); digitalWrite(ledPin11, LOW);  digitalWrite(ledPin12, LOW);
 
  delay(650); // The tones and the corresponding LED controls (ON/OFF)
}
