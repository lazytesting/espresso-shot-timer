#include <pushButton.h>

bool hasBeenActivated = false;
const int pushState = 0;

void PushButton::updateState(int state)
{
  if (state == pushState) {
    hasBeenActivated = true;
  }
}

bool PushButton::isPushed()
{
  if (hasBeenActivated) {
    // reset
    hasBeenActivated = false;
    return true;
  }
  return false;
}