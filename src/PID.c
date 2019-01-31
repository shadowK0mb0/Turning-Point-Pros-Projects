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
  int rotations = (int)(distance*360/(4*PI));
  if (distance < 15) {
      getTo(rotations,rotations,0.12,0.0015,0.03);

  } else if (distance <= 30) {
      getTo(rotations,rotations,0.072,0.002,0.017);

  } else {
      getTo(rotations,rotations,0.077,0.003,0.03);
  }

}
void turnLeftDistance(int distance) {
  double PI = 3.14159265358979323846;
  int rotations = (int)(distance*360/(4*PI));
  if (distance <= 3.1) { // 3 is about 30
      getTo(-1*rotations,rotations,0.5,0.006,0.02);
  } else if (distance <= 5) {// 4.7 is about 45 degrees
      getTo(-1*rotations,rotations,0.4,0.008,0.57);
  } else {
      getTo(-1*rotations,rotations,0.25,0.003,0.00);
  }

}

// turning right
// rotation units: degrees
void getTo(int rotationsL, int rotationsR, double kp, double ki, double kd) {
  encoderReset(encoderL);
  encoderReset(encoderR);
  // initialize sensor error variables
  int errorL = rotationsL - encoderGet(encoderL);
  int errorR = rotationsR - encoderGet(encoderR);
  int prevErrorL = errorL;
  int prevErrorR = errorR;
  int errorTotalL=0;
  int errorTotalR=0;
  // declare PID error values
  double proportionalErrorL;
  double proportionalErrorR;
  double integralErrorL;
  double integralErrorR;
  double differentialErrorL;
  double differentialErrorR;
  int integralActiveZone = 240; // ticks away from target
  int currentL;
  int currentR;

  while (true) {
    // update sensor error variables
    prevErrorL = errorL;
    prevErrorR = errorR;
    errorL = rotationsL - encoderGet(encoderL);
    errorR = rotationsR - encoderGet(encoderR);
    if (abs(errorR) < 10 && abs(errorL) < 10) {
      return;
    }

    // only accumulate integral value when close to target
    if (abs(errorL) < integralActiveZone && errorL != 0) {
      errorTotalL += errorL;
    } else {
      errorTotalL = 0;
    }
    if (abs(errorR) < integralActiveZone && errorR != 0) {
      errorTotalR += errorR;
    } else {
      errorTotalR = 0;
    }

    proportionalErrorL = errorL * kp;
    proportionalErrorR = errorR * kp;
    integralErrorL = errorTotalL * ki;
    integralErrorR = errorTotalR * ki;
    differentialErrorL = (errorL - prevErrorL) * kd;
    differentialErrorR = (errorR - prevErrorR) * kd;

    if (errorR == 0) {
      differentialErrorR = 0;
    }
    if (errorL == 0) {
      differentialErrorL = 0;
    }

    currentL = (int)(proportionalErrorL + integralErrorL + differentialErrorL);
    currentR = (int)(proportionalErrorR + integralErrorR + differentialErrorR);



    chassisSet(currentL, currentR);



    delay(20);
    }
}

  // turning left
  // rotation units: degrees
  void getToTurnLeft(int rotationsL, int rotationsR, double kp, double ki, double kd) {
    encoderReset(encoderL);
    encoderReset(encoderR);
    // initialize sensor error variables
    int errorL = rotationsL - encoderGet(encoderL);
    int errorR = rotationsR - encoderGet(encoderR);
    int prevErrorL = errorL;
    int prevErrorR = errorR;
    int errorTotalL;
    int errorTotalR;
    // declare PID error values
    double proportionalErrorL;
    double proportionalErrorR;
    double integralErrorL;
    double integralErrorR;
    double differentialErrorL;
    double differentialErrorR;
    int integralActiveZone = 120; // ticks away from target
    int currentL;
    int currentR;

    while (true) {
      // update sensor error variables
      prevErrorL = errorL;
      prevErrorR = errorR;
      errorL = rotationsL - encoderGet(encoderL);
      errorR = rotationsR - encoderGet(encoderR);
      if (abs(errorR) < 2 & abs(errorL) < 2) {
        return;
      }

      // only accumulate integral value when close to target
      if (abs(errorL) < integralActiveZone && errorL != 0) {
        errorTotalL += errorL;
      } else {
        errorTotalL = 0;
      }
      if (abs(errorR) < integralActiveZone && errorR != 0) {
        errorTotalR += errorR;
      } else {
        errorTotalR = 0;
      }

      proportionalErrorL = errorL * kp;
      proportionalErrorR = errorR * kp;
      integralErrorL = errorTotalL * ki;
      integralErrorR = errorTotalR * ki;
      differentialErrorL = (errorL - prevErrorL) * kd;
      differentialErrorR = (errorR - prevErrorR) * kd;

      if (errorR == 0) {
        differentialErrorR = 0;
      }
      if (errorL == 0) {
        differentialErrorL = 0;
      }

      currentL = (int)(proportionalErrorL + integralErrorL + differentialErrorL);
      currentR = (int)(proportionalErrorR + integralErrorR + differentialErrorR);

      /*if (currentL < 0) {
        currentL = 0;
      }
      if (currentR < 0) {
        currentR = 0;
      }*/

      chassisSet(currentL, currentR);

      /*printf("--%d %d--\n", currentL, currentR);
      printf("%d %d\n", rotationsL - errorL, rotationsR - errorR);
      printf("%d %d\n", errorL, errorR);
      printf("%d %d\n", errorTotalL, errorTotalR);*/

      printf("--%d--\n", currentR);
      printf("%d\n", rotationsR - errorR);
      printf("%d\n", errorR);
      printf("%d\n", errorTotalR);

      delay(20);
    }

}

const int accel_step = 9;
const int deccel_step = 256; // no decel slew
static int leftSpeed = 0;
static int rightSpeed = 0;

void chassisSlew(int leftTarget, int rightTarget){
  // right slew
  int rightStep;
  if(abs(rightSpeed) < abs(rightTarget)) {
    rightStep = accel_step;
  } else {
    rightStep = deccel_step;
  }

  if(rightTarget > rightSpeed + rightStep) {
    rightSpeed += rightStep;
  } else if(rightTarget < rightSpeed - rightStep) {
    rightSpeed -= rightStep;
  } else {
    rightSpeed = rightTarget;
  }

  int leftStep;

  if(abs(leftSpeed) < abs(leftTarget)) {
    leftStep = accel_step;
  } else {
    leftStep = deccel_step;
  }

  if(leftTarget > leftSpeed + leftStep) {
    leftSpeed += leftStep;
  } else if(leftTarget < leftSpeed - leftStep) {
    leftSpeed -= leftStep;
  } else {
    leftSpeed = leftTarget;
  }

  chassisSet(leftSpeed, rightSpeed);



}
