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
int lastShots[4];
String shotHistory[beanSelectionLimit];
int targetShotTime = 30;
int targetTimeUpLastState = 2;
int targetTimeDownLastState = 2;
int beanSelectionLastState = 2;
int currentBeanSelection = 1; //TODO bean selection should start on 0
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
  //memset(lastShots, 0, sizeof(lastShots));
  shotHistory[currentBeanSelection] = "";
  lcd.setCursor(1,3);
  lcd.print("History:          "); // whipe previous history
}

void printShothistory() {
  lcd.setCursor(1,3);
  lcd.print("History:          "); // whipe previous history
  lcd.setCursor(10,3); // move to position after colon

  // print all shots except the last one
//  for (int i=1; i<4; i++) {   
//    if(lastShots[i] > 0) {
//      if(i>1) {
//        lcd.print("-");
//      }
//      lcd.print(lastShots[i]);
//    }
//  }

  lcd.print(shotHistory[currentBeanSelection - 1]);
}

void updateShotHistory(int lastShotTime) {
  const int maxLength = 11;
  String shotHistory = shotHistory[currentBeanSelection - 1] + "-" + lastShotTime;

  // get lenght
  shotHistory.length();

  // while lenght > max
   // get position of first -
   // remove everything before/on that position
  
  
//  int tempLastShots[3];
//  tempLastShots[0] = lastShots[0];
//  tempLastShots[1] = lastShots[1];
//  tempLastShots[2] = lastShots[2];
//
//  lastShots[0] = lastShotTime;
//  lastShots[1] = tempLastShots[0];
//  lastShots[2] = tempLastShots[1];
//  lastShots[3] = tempLastShots[2];
//
//  if( lastShots[1] != 0) {
//    printShothistory();
//  }
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
