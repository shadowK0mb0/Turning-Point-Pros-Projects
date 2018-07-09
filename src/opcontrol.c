/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
#include "chassis.h"
#include "turn.h"
#include "PID.h"

/*
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */

void aauto() {
  delay(1000);
  // initialize static variables
  double PI = 3.14159265358979323846;
  int rotations = (int)(50*360/(4*PI));
  getTo(rotations, rotations);
}
void operatorControl() {
	//aauto();
	//delay(10000000);
	int power; // forward backward speed
 	int turn; // turn power

    int encoderPosL = 0; // left encoder position for PID maintain
    int encoderPosR = 0; // right encoder position for PID maintain

	encoderReset(encoderL); // clear encoders
	encoderReset(encoderR);
    encoderReset(encoderF);
    // whether power and turn values are positive or 0
    bool powerPositive = false;
    bool turnPositive = false;
    // current encoder values, we save them as ints to minimize function
    // calls and initializations, thus speed up program
    int encoderLDegrees = 0;
    int encoderRDegrees = 0;

    int encoderFCurr = 0;
    int encoderFPrev = 0;
    int prevTime = 0;
    int currTime = 0;
    int rotationsPerSec = 0;
	while (1) {
		power = joystickGetAnalog(1, 3); // vertical axis on left joystick
        turn  = joystickGetAnalog(1, 1); // horizontal axis on right joystick

        encoderLDegrees = encoderGet(encoderL); // save encoder values as ints
        encoderRDegrees = encoderGet(encoderR); //so you don't have to call
        encoderFCurr = encoderGet(encoderF);    // function every time
        currTime = millis();
        /* check if either:
              power has shifted from positive to 0, and
              the turn value is either within deadzone or 0
              or
              turn has shifted from positive to 0, and
              the power value is either within deadzone or 0
            this means that the joystick has just stopped giving input to motors
            thus we want to remain at this exact position, thus capture the
            current encoder values to set as the goal we want to get to
        */
        if (currTime - prevTime != 0) {
            rotationsPerSec = (int)(encoderFCurr - encoderFPrev)*1000/((currTime - prevTime)*360);
        } else {
            rotationsPerSec = 0;
        }
        if (
              (power < 20 && power > -20 && powerPositive &&
              ((turn < 20 && turn > -20) || !turnPositive))
              ||
              (turn < 20 && turn > - 20 && turnPositive &&
              ((power < 20 && power > -20) || !powerPositive))
           ) {
               // all joystick values have just gone to 0, thus maintain robot
               // at this position
               encoderPosL = encoderLDegrees;
               encoderPosR = encoderRDegrees;
        }
        // deadzone code, if joystick value is smaller than certain amount
        // running the motors at that power will accomplish nothing, so
        // just set them to 0
    	if (abs(power) < 20) {
    		power = 0;
            powerPositive = false;
    	} else {
            powerPositive = true;
        }
        // deadzone code for turn, however boost turn if it's not 0 to make it
        // more sensitive for driver to have easier time turning
    	if (abs(turn) < 20) {
    		turn = 0;
            turnPositive = false;
    	} else if (turn >= 20){
            turn += 10;
            turnPositive = true;
        } else {
            turn -= 10;
            turnPositive = true;
        }
        printf("%d\n",rotationsPerSec);
        // set chassis speed (left, right) based on power and turn values
        chassisSet(power+turn, power-turn); // accessed from chassis.c
        // i
        if (!powerPositive && !turnPositive) {
            getTo(encoderPosL, encoderPosR);
        }
        encoderFPrev = encoderFCurr;
        prevTime = currTime;
        delay(20);
    }
}
