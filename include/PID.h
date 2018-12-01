#ifndef _PID_H_
#define _PID_H_

// Sets the speeds of the left and right wheels of the chassis
void getTo(int rotationsL, int rotationsR, double kp, double ki, double kd);

// Sets the speeds of the flywheel
int flywheel(double error, int prevError);

void flywheelSet(int current);

void driveDistance(int distance);

#endif // _CHASSIS_H_
