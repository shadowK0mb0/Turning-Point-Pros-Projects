#include "main.h"
#include "chassis.h"
#include "PID.h"


// PID to get left and right drive encoders to certain amount of rotations
// measured in encoder "ticks" / degrees
void getTo(int rotationsL, int rotationsR) {
    encoderReset(encoderL);
    encoderReset(encoderR);

    // initialize sensor error variables
    int errorL = rotationsL - encoderGet(encoderL);
    int errorR = rotationsR - encoderGet(encoderR);
    int prevErrorL = errorL;
    int prevErrorR = errorR;
    int errorTotalL;
    int errorTotalR;

    // initialize pid coefficients
    double kp = 0.121;
    double ki = 0.005;
    double kd = 0.071;

    // declare PID error values
    double proportionalErrorL;
    double proportionalErrorR;
    double integralErrorL;
    double integralErrorR;
    double differentialErrorL;
    double differentialErrorR;
    int integralActiveZone = 120; // only enable I term at a certain distance from
                // target, otherwise gets out of hand

    // motor values for left and right sides based on PID controller
    int currentL;
    int currentR;

    while (true) {
        // update sensor-error variables
        prevErrorL = errorL;
        prevErrorR = errorR;
        errorL = rotationsL - encoderGet(encoderL);
        errorR = rotationsR - encoderGet(encoderR);

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
        // calculate error for both sides, and multiply by coefficients
        proportionalErrorL = errorL * kp;
        proportionalErrorR = errorR * kp;
        integralErrorL = errorTotalL * ki;
        integralErrorR = errorTotalR * ki;
        differentialErrorL = (errorL - prevErrorL) * kd;
        differentialErrorR = (errorR - prevErrorR) * kd;

        // deal with 0 error for the derivative term
        if (errorR == 0) {
            differentialErrorR = 0;
        }
        if (errorL == 0) {
            differentialErrorL = 0;
        }

        // update motor values using PID controller
        currentL = (int)(proportionalErrorL + integralErrorL + differentialErrorL);
        currentR = (int)(proportionalErrorR + integralErrorR + differentialErrorR);

        // set motor values
        chassisSet(currentL, currentR);
        /*
        debug statements
        printf("--%d %d--\n", currentL, currentR);
        printf("%d %d\n", rotationsL - errorL, rotationsR - errorR);
        printf("%d %d\n", errorL, errorR);
        printf("%d %d\n", errorTotalL, errorTotalR);
        */
        delay(20);
    }
}
