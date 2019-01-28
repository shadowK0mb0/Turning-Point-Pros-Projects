#ifndef _CATAPULT_H_
#define _CATAPULT_H_

// Sets the speeds of the left and right wheels of the chassis
void catapultMove(int speed);

bool catapultSet(int distance);

bool catapultThrow();

#endif // _CHASSIS_H_
