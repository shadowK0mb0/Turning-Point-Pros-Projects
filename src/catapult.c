#include "main.h"
#include "PID.h"


void catapultMove(int speed) { // use positive number
   motorSet(8, speed);
   motorSet(7, -speed);
}

void catapultSet(int distance) {
    int error = analogRead(1) - distance;

    double kp = 0.61;

    double proportionalError = 0;

    while (true) {
        error = analogRead(1)- distance;
        if (error < 50) {
            return;
        }
        proportionalError = error * kp;
        catapultMove((int)proportionalError);
        delay(20);
    }
}

void catapultThrow() {
  while (analogRead(1) < 2300) {
    catapultMove(120);
  }
  catapultMove(0);
}
