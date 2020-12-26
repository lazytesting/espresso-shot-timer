#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <elapsedMillis.h>

elapsedMillis timer;
LiquidCrystal_I2C lcd(0x27,20,4);
int pin = 12; // pin for switch
bool isCounting = false;
int lastShots[4];
int targetShotTime = 30;

void setup()
{
  pinMode(pin, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Target : " + String(targetShotTime));
  lcd.setCursor(1,1);
  lcd.print("Actual : ");
}

void startTimer() {
  timer = 0;
  isCounting = true;
  // clear previous current count
  lcd.setCursor(10,1);
  lcd.print("    ");
}

void stopTimer(){
  isCounting = false;
  int currentShotTime = timer/1000;
  if (currentShotTime < 3) {
    // not counting this, probably a flush
    return;
  }
  
  updateShotHistory(currentShotTime);
  
  if( lastShots[1] != 0) {
    printShothistory();
  }
}

void printShothistory() {
  lcd.setCursor(1,2);
  lcd.print("History:          "); // whipe previous history
  lcd.setCursor(10,2); // move to position after colon

  // print all shots except the last one
  for (int i=1; i<4; i++) {   
    if(lastShots[i] > 0) {
      if(i>1) {
        lcd.print("-");
      }
      lcd.print(lastShots[i]);
    }
  }
}

void updateShotHistory(int lastShotTime) {
  int tempLastShots[3];
  tempLastShots[0] = lastShots[0];
  tempLastShots[1] = lastShots[1];
  tempLastShots[2] = lastShots[2];

  lastShots[0] = lastShotTime;
  lastShots[1] = tempLastShots[0];
  lastShots[2] = tempLastShots[1];
  lastShots[3] = tempLastShots[2];
}

void printCurrentShotTime) {
  lcd.setCursor(10,1);
  lcd.print(timer/1000);
}

void loop()
{
  int buttonValue = digitalRead(12);
  if (buttonValue == LOW){
    if (!isCounting) {
      startTimer();
    }
    printCurrentShotTime();
  } else {
    if (isCounting) {
      stopTimer();
    }
  }
}
