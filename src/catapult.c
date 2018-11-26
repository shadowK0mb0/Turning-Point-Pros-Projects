#include "main.h"
#include "PID.h"


void catapultMove(int speed) { // use positive number
  motorSet(8, speed);
   motorSet(7, -speed);
}

void catapultSet(int distance) {
  int error = distance - analogRead(1);

  double kp = 0.1;

  double proportionalError;


  while (true) {
    error = distance - analogRead(1);
    if (error < 0.1) {
      return;
    }
    proportionalError = error * kp;
    catapultMove((int)proportionalError);
    delay(20);
  }
}

void catapultThrow() {
  while (analogRead(1) > 1600) {
    catapultMove(120);
  }
  catapultMove(0);
}
