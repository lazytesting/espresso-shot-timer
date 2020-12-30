#include <Wire.h> 
#include "src/LiquidCrystal_I2C/LiquidCrystal_I2C.h"
#include "src/elapsedMillis/elapsedMillis.h"

LiquidCrystal_I2C lcd(0x27,20,4);
// config
const int pumpSwitchPin = 12; // pin for switch
const int targetTimeUpPin = 11;
const int targetTimeDownPin = 10;
const int beanSelectionPin = 8;
const int beanSelectionLimit = 2; //number of bean types

// variables
bool isCounting = false;
int shotHistory[beanSelectionLimit][4];
int targetShotTime = 30;
int targetTimeUpLastState = 2;
int targetTimeDownLastState = 2;
int beanSelectionLastState = 2;
int currentBeanSelection = 0;
elapsedMillis timer;
elapsedMillis flakyTimer;

void setup()
{
  pinMode(pumpSwitchPin, INPUT_PULLUP);
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
    if(currentBeanSelection + 1 < beanSelectionLimit) {
      currentBeanSelection++;
    } else {
      currentBeanSelection = 0;
    }
    printBeanSelection();
    printShothistory();
  }
  beanSelectionLastState = beanSelectionButton;
}

void printBeanSelection() {
  lcd.setCursor(1,0);
  lcd.print("Bean : " + String(currentBeanSelection + 1) + "        ");
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
  if (currentShotTime < 1o) {
    // not counting this, probably a flush
    return;
  }
  
  updateShotHistory(currentShotTime);
}

void printShothistory() {
  lcd.setCursor(1,3);
  lcd.print("History:          "); // whipe previous history

  if (shotHistory[currentBeanSelection][0] == 0) {
    //nothing to display
    return;
  }
  
  lcd.setCursor(10,3); // move to position after colon

  // print history
  for (int i=0; i<=4; i++) {   
    if(shotHistory[currentBeanSelection][i] > 0) {
      if(i>0) {
        lcd.print("-");
      }
      lcd.print(shotHistory[currentBeanSelection][i]);
    }
  }
}

void updateShotHistory(int lastShotTime) {
  int tempLastShots[3];
  // copy into temp array
  tempLastShots[0] = shotHistory[currentBeanSelection][0];
  tempLastShots[1] = shotHistory[currentBeanSelection][1];
  tempLastShots[2] = shotHistory[currentBeanSelection][2];

  // add new time and shift everything one position
  shotHistory[currentBeanSelection][0] = lastShotTime;
  shotHistory[currentBeanSelection][1] = tempLastShots[0];
  shotHistory[currentBeanSelection][2] = tempLastShots[1];
  shotHistory[currentBeanSelection][3] = tempLastShots[2];
  
  printShothistory();
}

void printActualTimer() {
  if (isCounting) {
    lcd.setCursor(1,2);
    String actual = String(timer/1000);
    lcd.print("Actual : " + actual + "    ");
  }
}

void updateActualTimer() {
  int buttonValue = digitalRead(pumpSwitchPin);
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
