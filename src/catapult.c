#include "main.h"

void catapultMove(int speed) { // use positive number
  motorSet(8, speed);
   motorSet(7, -speed);
}

void catapultSet() {
  while(analogRead(1) < 2600) {
    catapultMove(120);
  }
  catapultMove(0);
}
void catapultThrow() {
  while (analogRead(1) > 1600) {
    catapultMove(120);
  }
  catapultMove(0);
}
