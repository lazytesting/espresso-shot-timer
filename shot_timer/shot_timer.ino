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
const int valuePosition = 8;
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
  printLabelsWithDefaults();
}

void printLabelsWithDefaults() {
  lcd.setCursor(0,0);
  lcd.print("Bean:   1");
  lcd.setCursor(0,1);
  lcd.print("Target: 30");
  lcd.setCursor(0,2);
  lcd.print("Actual: 0");
  lcd.setCursor(0,3);
  lcd.print("Past:");
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
  lcd.setCursor(valuePosition, 0);
  lcd.print(String(currentBeanSelection + 1));
}

void updateTargetTimer() {
  // Pressing up button
  int upButton = digitalRead(targetTimeUpPin);
  if (upButton == LOW && targetTimeUpLastState != upButton){
    if (targetShotTime < 99) {
      targetShotTime++;
    }
  }
  targetTimeUpLastState = upButton;

  // Pressing down button
  int downButton = digitalRead(targetTimeDownPin);
  if (downButton == LOW && targetTimeDownLastState != downButton){
    if (targetShotTime > 0) {
      targetShotTime--;
    } 
  }
  targetTimeDownLastState = downButton;

  // print
  lcd.setCursor(valuePosition, 1);
  lcd.print(String(targetShotTime) + "        ");
}

void startTimer() {
  timer = 0;
  isCounting = true;
}

void stopTimer(){
  isCounting = false;
  int currentShotTime = (timer - flakyTimer) /1000;
  if (currentShotTime < 10) {
    // not counting this, probably a flush
    return;
  }
  
  updateShotHistory(currentShotTime);
}

void printShothistory() {
  lcd.setCursor(valuePosition, 3);
  lcd.print("            "); // whipe previous history

  if (shotHistory[currentBeanSelection][0] == 0) {
    //nothing to display
    return;
  }
  
  lcd.setCursor(valuePosition,3); // move to position after colon

  // print history
  for (int i=0; i<4; i++) {   
    if(shotHistory[currentBeanSelection][i] > 0) {
      if(i>0) {
        lcd.print("-");
      }
      lcd.print(shotHistory[currentBeanSelection][i]);
    }
  }
}

void updateShotHistory(int lastShotTime) {
  // limit to 2 characters
  if (lastShotTime > 99) {
    lastShotTime = 99;
  }

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
    lcd.setCursor(valuePosition, 2);
    String actual = String(timer/1000);
    lcd.print(actual + "    ");
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
