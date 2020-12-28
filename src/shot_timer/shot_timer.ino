#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <elapsedMillis.h>

LiquidCrystal_I2C lcd(0x27,20,4);
// config
int pin = 12; // pin for switch
int targetTimeUpPin = 11;
int targetTimeDownPin = 10;
int beanSelectionPin = 8;

// variables
bool isCounting = false;
int lastShots[4];
int targetShotTime = 30;
int targetTimeUpLastState = 2;
int targetTimeDownLastState = 2;
int beanSelectionLastState = 2;
int currentBeanSelection = 1;
int beanSelectionLimit = 2; //number of bean types
elapsedMillis timer;
elapsedMillis flakyTimer;

void setup()
{
  pinMode(pin, INPUT_PULLUP);
  pinMode(targetTimeUpPin, INPUT_PULLUP);
  pinMode(targetTimeDownPin, INPUT_PULLUP);
  pinMode(beanSelectionPin, INPUT_PULLUP);
  
  lcd.init();
  lcd.backlight();
  printBeanSelection();
}

void updateBeanSelection() {
  int beanSelectionButton = digitalRead(beanSelectionPin);
  if (beanSelectionButton == LOW && beanSelectionLastState != beanSelectionButton){    
    if(currentBeanSelection + 1 <= beanSelectionLimit) {
      currentBeanSelection++;
    } else {
      currentBeanSelection = 1;
    }
    
    clearShotHistory();
    printBeanSelection(); 
  }
  beanSelectionLastState = beanSelectionButton;
}

void printBeanSelection() {
  lcd.setCursor(1,0);
  lcd.print("Bean : " + String(currentBeanSelection) + "        ");
}

void updateTargetTimer() {
  // Pressing up button
  int upButton = digitalRead(targetTimeUpPin);
  if (upButton == LOW && targetTimeUpLastState != upButton){
    targetShotTime++;
  }
  targetTimeUpLastState = upButton;

  // Pressing down button
  int downButton = digitalRead(targetTimeDownPin);
  if (downButton == LOW && targetTimeDownLastState != downButton){
    targetShotTime--;
  }
  targetTimeDownLastState = downButton;

  // print
  lcd.setCursor(1,1);
  lcd.print("Target : " + String(targetShotTime) + "        ");
}

void startTimer() {
  timer = 0;
  isCounting = true;
}

void stopTimer(){
  isCounting = false;
  int currentShotTime = (timer - flakyTimer) /1000;
  if (currentShotTime < 3) {
    // not counting this, probably a flush
    return;
  }
  
  updateShotHistory(currentShotTime);
}

void clearShotHistory() {
  memset(lastShots, 0, sizeof(lastShots));
  lcd.setCursor(1,3);
  lcd.print("History:          "); // whipe previous history
}

void printShothistory() {
  lcd.setCursor(1,3);
  lcd.print("History:          "); // whipe previous history
  lcd.setCursor(10,3); // move to position after colon

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

  if( lastShots[1] != 0) {
    printShothistory();
  }
}

void printActualTimer() {
  if (isCounting) {
    lcd.setCursor(1,2);
    String actual = String(timer/1000);
    lcd.print("Actual : " + actual + "    ");
  }
}

void updateActualTimer() {
  int buttonValue = digitalRead(pin);
  if (buttonValue == LOW){
    flakyTimer = 0;
    if (!isCounting) {
      startTimer();
    }
  } else {
    // sensor is bit flaky when pump runs, needs damping
    if (isCounting && flakyTimer > 1000) {
      stopTimer();
    }
  }
  printActualTimer();
}



void loop()
{ 
  updateBeanSelection();
  updateTargetTimer();
  updateActualTimer();
}
