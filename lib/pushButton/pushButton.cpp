#include <pushButton.h>

int lastState;
int currentState;
int pushState;

PushButton::PushButton(int state) {
  pushState = state;
}

void PushButton::updateState(int state)
{
  currentState = state;
}

bool PushButton::isPushed()
{
  bool isPushed = false;
  if (currentState == pushState && currentState != lastState) {
    isPushed = true;
  }
  lastState = currentState;

  return isPushed;
}