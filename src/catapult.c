#include "main.h"
#include "PID.h"


void catapultMove(int speed) { // use positive number
  motorSet(8, speed);
   motorSet(7, -speed);
}

void catapultSet() {
  catapult(2570);
}
void catapultThrow() {
  while (analogRead(1) > 1600) {
    catapultMove(120);
  }
  catapultMove(0);
}
