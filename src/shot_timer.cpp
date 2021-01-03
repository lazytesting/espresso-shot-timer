#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <elapsedMillis.h>
#include <pushButton.h>

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
int currentBeanSelection = 0;
elapsedMillis timer;
elapsedMillis flakyTimer;
PushButton targetTimeUpButton(LOW);
PushButton targetTimeDownButton(LOW);
PushButton selectBeanButton(LOW);

void printBeanSelection();
void updateBeanSelection();
void updateTargetTimer();
void startTimer();
void stopTimer();
void printShothistory();
void updateShotHistory(int);
void updateActualTimer();
void updateButtonStates();
void printLabelsWithDefaults();

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
  if (selectBeanButton.isPushed()){    
    if(currentBeanSelection + 1 < beanSelectionLimit) {
      currentBeanSelection++;
    } else {
      currentBeanSelection = 0;
    }
    printBeanSelection();
    printShothistory();
  }
}

void printBeanSelection() {
  lcd.setCursor(valuePosition, 0);
  lcd.print(String(currentBeanSelection + 1));
}

void updateTargetTimer() {
  if (targetTimeUpButton.isPushed() && targetShotTime < 99){
    targetShotTime++;
  }
  if (targetTimeDownButton.isPushed() && targetShotTime > 0){
    targetShotTime--;
  }

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


void updateButtonStates() {
  targetTimeUpButton.updateState(digitalRead(targetTimeUpPin));
  targetTimeDownButton.updateState(digitalRead(targetTimeDownPin));
  selectBeanButton.updateState(digitalRead(beanSelectionPin));
}

void loop()
{ 
  updateButtonStates();

  updateBeanSelection();
  updateTargetTimer();
  updateActualTimer();
}
