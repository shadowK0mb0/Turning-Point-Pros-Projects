#include "main.h"
#include "PID.h"


void catapultMove(int speed) { // use positive number
   motorSet(8, speed);
   motorSet(7, -speed);
}

bool catapultSet(int distance) {
    int error = analogRead(1) - distance;
    double kp = 0.67;

    double proportionalError = 0;
    if (abs(error) > 2) {
        error = analogRead(1)- distance;
        proportionalError = error * kp;
        if (proportionalError < 0) {
            proportionalError = 0;
        }
        catapultMove((int)proportionalError);
        delay(20);
    }
    return error > 2;
}

bool catapultThrow() {
  int potVal = analogRead(1);
  if (analogRead(1) < 2300) {
    catapultMove(120);
  } else {
    catapultMove(0);
  }
  return potVal < 2300;
}
