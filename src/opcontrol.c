/** @file opcontrol.c*
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
#include <stdlib.h>
#include "catapult.h"

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

 void intakeSet (int speed) {
     motorSet(10,speed);
 }
 void grabSet(int speed) {
   motorSet(9, speed);
 }

 void pid() {
   double PI = 3.14159265358979323846;
   int rotations = (int)(23*360/(4*PI));
   getTo(+2*rotations,-2*rotations,1,0,1);
   intakeSet(-120);
   delay(500);
   getTo(rotations, rotations, 1,0,1);
   intakeSet(0);
   getTo(-90/2,90/2, 1, 0.0, 1);
   getTo(rotations * 3/2, rotations * 3/2, 1,0,1);
 }

 void pid2() {
   double PI = 3.14159265358979323846;
   int rotations = (int)(23*360/(4*PI));
   getTo(-2*rotations,-2*rotations,1,0,1);
   intakeSet(-120);
   delay(500);
   getTo(2*rotations, 2*rotations, 1,0,1);
   getTo(-90/2,90/2,1,0,1);
   catapultThrow();
   getTo(3*rotations, 3*rotations,1,0,1);
   getTo(-1.5*rotations, -1.5*rotations,1,0,1);
   getTo(90/2,-90/2,1,0,1);
   getTo(-rotations, -rotations, 1, 0, 1);
   // flip the cap somehow

 }

void tempauton() {

  driveDistance(24.6);
  chassisSet(-40,40);
  delay(510);
  chassisSet(30,30);
  delay(400);
  chassisSet(110,110);
  delay(3050);
  chassisSet(-50,-50);
  delay(30);
  chassisSet(0,0);
}

void auton() {
  //catapultSet(1000);
  //grabSet(-120);
  //delay(50);
  //grabSet(0);
  //delay(200);

  chassisSet(-50,-50);
  delay(800);
  intakeSet(-120);
  delay(700);
  chassisSet(0,0);
  delay(1250);
  chassisSet(30,30);
  delay(1000);
  chassisSet(-30,-30);
  delay(100);
  chassisSet(40,-40);
  delay(580);
  chassisSet(0,0);
  catapultThrow();
}


void operatorControl() {
	int power = 0; // forward backward speed
 	int turn = 0; // turn power

    int encoderPosL = 0; // left encoder position for PID maintain
    int encoderPosR = 0; // right encoder position for PID maintain
    //int encoderPosF = 0;
    int leftSide = 0;
    int rightSide = 0;
    encoderReset(encoderL); // clear encoders
    encoderReset(encoderR);
    encoderReset(encoderF);

    //stuff for flywheel :|

    //double previousVelocity = 0;

    // whether power and turn values are positive or 0
    bool powerPositive = false;
    bool turnPositive = false;
    // current encoder values, we save them as ints to minimize function
    // calls and initializ`ations, thus speed up program
    int encoderLDegrees = 0;
    int encoderRDegrees = 0;
    //int encoderFDegrees = 0;
	while (1) {

        power = joystickGetAnalog(1, 3); // vertical axis on left joystick
        turn  = joystickGetAnalog(1, 1); // horizontal axis on right joystick

        encoderLDegrees = encoderGet(encoderL); // save encoder values as ints
        encoderRDegrees = encoderGet(encoderR); //so you don't have to call
                                                // function every time
        /*  check if either:
                power has shifted from positive to 0, and
                the turn value is either within deadzone or 0
            or
                turn has shifted from positive to 0, and
                the power value is either within deadzone or 0
                this means that the joystick has just stopped giving input to motors
                thus we want to remain at this exact position, thus capture the
                current encoder values to set as the goal we want to get to
        */
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
            //turnPositive = false;
        } else if (turn >= 20){
            turn += 10;
            //turnPositive = true;
        } else {
            turn -= 10;
            //turnPositive = true;
        }

        leftSide = power+turn;
        rightSide = power-turn;
        // set chassis speed (left, right) based on power and turn values
        chassisSet(leftSide,rightSide); // accessed from chassis.c

        if (joystickGetDigital(1, 8, JOY_UP)) {
          //tempauton();
         // encoderReset(encoderL);
          //encoderReset(encoderR);
      }

        if (joystickGetDigital(1,6, JOY_DOWN)) {
          intakeSet(120);
        }
        else if (joystickGetDigital(1,6, JOY_UP)) {
          intakeSet(-120);
        }
        else {
          intakeSet(0);
        }

        if (joystickGetDigital(1,8, JOY_RIGHT)) {
          catapultMove(120);
        }
        else {
          catapultMove(0);
        }

        if (joystickGetDigital(1,8, JOY_LEFT)) {
          catapultSet(1000);
        }
        if (joystickGetDigital(1,8, JOY_DOWN)){
          catapultThrow();
        }

        if (joystickGetDigital(1,5, JOY_UP)) {
          grabSet(120);
        }
        else if (joystickGetDigital(1,5, JOY_DOWN)) {
          grabSet(-120);
        }
        else {
          grabSet(0);
        }


        /*if (!powerPositive && !turnPositive) {
            getTo(encoderPosL, encoderPosR);
        }*/
        delay(20);
    }
}
