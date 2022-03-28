#include <TimeLib.h>
#include "SevSeg.h"

#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

SevSeg SSD;

unsigned long timer;
int deciSecond = 0;

const int latchPin = A2;
const int clockPin = A1;
const int dataPin = A0;

int SSDArray[24] = {B10011010, B01100000, B11011100, B11110100, B01100110, B10110110, B10111110, B11100000, B11111110, B11110110, B11101110, B00111110, B10011010, B01100000, B11011100, B11110100, B01100110, B10110110, B10111110, B11100000, B11111110, B11110110, B11101110, B00111110};

void setup()  {
  Serial.begin(9600);

  byte numDigits = 4;
  byte digitPins[] = {6, 5, 4, 3};
  byte segmentPins[] = {13, 12, 11, 10, 9, 8, 7};
  bool resistorsOnSegments = true; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = true; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = true; // Use 'true' if your decimal point doesn't exist or isn't connected

  SSD.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
  SSD.setBrightness(150);

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  while (!Serial) ; // Needed for Leonardo only
  setSyncProvider( requestSync);  //set function to call when sync required
  Serial.println("Waiting for sync message");
}

void loop(){    
  static unsigned long timer = millis();
  static int deciSeconds = 0;
  
  if (Serial.available()) {
    processSyncMessage();
  }
  if (timeStatus()!= timeNotSet) {
    digitalClockDisplay();  
  }

  delay(200);

  THours();

  deciSeconds = (100 * minute()) + second();

  SSD.setNumber(deciSeconds, 4);
  delay(100);
  SSD.refreshDisplay(); // Must run repeatedly
  
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
     }
  }
}

time_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}

void THours(){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, SSDArray[hour()]);
  digitalWrite(latchPin, HIGH);
  delay(100);
}
