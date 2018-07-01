#include "main.h"    // includes API.h and other headers
#include "turn.h" // redundant, but ensures that the corresponding header file (chassis.h) is included

void turnSet(int left, int right) {
  if (left > 120) {
    left = 120;
  } else if (left < -120) {
    left = -120;
  }
  if (right > 120) {
    right = 120;
  } else if (right < -120) {
    right = -120;
  }
  motorSet(1, -right);
  motorSet(2, right);
  motorSet(3, left);
  motorSet(4, left);
  motorSet(5, -left);
  motorSet(6, -right);
}
