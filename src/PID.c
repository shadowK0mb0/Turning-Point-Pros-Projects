#include "main.h"
#include "chassis.h"
#include "PID.h"
#include "catapult.h"

void flywheelSet(int current) {
  if (current > 120) {
    current = 120;
  }
  else if (current < -120) {
    current = -120;
  }

  motorSet(7, current);
}



//Velocity is in RPM
int flywheel(double error, int prevError) {
    // initialize sensor error variable

    // initialize pid coefficients
    double kp = 0.121;
    double kd = 0.071;
    // declare PID error values
    double proportionalError;
    double differentialError;
    int current;

    //error = rotations - encoderGet(encoderF);

    proportionalError = error * kp;
    differentialError = (error - prevError) * kd;

    if (error == 0) {
      differentialError = 0;
    }
    current = (int)(proportionalError + differentialError);

    if (current > 0) {
        current += 5;
    } else {
        current -= 5;
    }

    /*if (currentL < 0) {
      currentL = 0;
    }
    if (currentR < 0) {
      currentR = 0;
    }*/

    //flywheelSet(current);
    //flywheelSpeed += current;
    //flywheelSet(flywheelSpeed);

    //printf("--%d %d--\n", current);
    //printf("%d %d\n", rotations - error);
    //printf("%d %d\n", error);
    //printf("%d %d\n", errorTotal);

    //delay(100);

    return error;
}

void driveDistance(int distance) {
  double PI = 3.14159265358979323846;
  int rotations = (int)(distance*360/(2.75*PI));
  getTo(rotations,rotations,0.09,0,0.55);
}

// turning right
// rotation units: degrees
void getTo(int rotationsL, int rotationsR, double kp, double ki, double kd) {
  encoderReset(encoderL);
  encoderReset(encoderR);
  // initialize sensor error variables
  //int errorL = rotationsL - encoderGet(encoderL);
  int errorR = rotationsR - encoderGet(encoderR);
  //int prevErrorL = errorL;
  int prevErrorR = errorR;
  //int errorTotalL;
  int errorTotalR;
  // declare PID error values
  //double proportionalErrorL;
  double proportionalErrorR;
  //double integralErrorL;
  double integralErrorR;
  //double differentialErrorL;
  double differentialErrorR;
  int integralActiveZone = 120; // ticks away from target
  //int currentL;
  int currentR;

  while (true) {
    // update sensor error variables
    //prevErrorL = errorL;
    prevErrorR = errorR;
    //errorL = rotationsL - encoderGet(encoderL);
    errorR = rotationsR - encoderGet(encoderR);
    if (errorR < 50) {
      return;
    }

    // only accumulate integral value when close to target
    /*if (abs(errorL) < integralActiveZone && errorL != 0) {
      errorTotalL += errorL;
    } else {
      errorTotalL = 0;
    }*/
    if (abs(errorR) < integralActiveZone && errorR != 0) {
      errorTotalR += errorR;
    } else {
      errorTotalR = 0;
    }

    //proportionalErrorL = errorL * kp;
    proportionalErrorR = errorR * kp;
    //integralErrorL = errorTotalL * ki;
    integralErrorR = errorTotalR * ki;
    //differentialErrorL = (errorL - prevErrorL) * kd;
    differentialErrorR = (errorR - prevErrorR) * kd;

    if (errorR == 0) {
      differentialErrorR = 0;
    }
    /*if (errorL == 0) {
      differentialErrorL = 0;
    }*/

    //currentL = (int)(proportionalErrorL + integralErrorL + differentialErrorL);
    currentR = (int)(proportionalErrorR + integralErrorR + differentialErrorR);

    /*if (currentL < 0) {
      currentL = 0;
    }
    if (currentR < 0) {
      currentR = 0;
    }*/

    chassisSet(currentR, currentR);
    /*
    printf("--%d %d--\n", currentL, currentR);
    printf("%d %d\n", rotationsL - errorL, rotationsR - errorR);
    printf("%d %d\n", errorL, errorR);
    printf("%d %d\n", errorTotalL, errorTotalR);
    */
    delay(20);
  }


}
