#include "main.h"
#include "PID.h"


void catapultMove(int speed) { // use positive number
   motorSet(8, speed);
   motorSet(7, -speed);
}

void catapultSet(int distance) {
    int potVal = analogRead(1);
    int error = potVal - distance;

    double kp = 0.1;

    double proportionalError = 0;
    
    while (true) {
        error = distance - analogRead(1);
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
