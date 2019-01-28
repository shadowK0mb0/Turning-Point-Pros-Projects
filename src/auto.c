/** @file auto.c
 * @brief File for autonomous code
 *
 * This file should contain the user autonomous() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
#include "chassis.h"
#include "PID.h"
#include "catapult.h"

/*
 * Runs the user autonomous code. This function will be started in its own task with the default
 * priority and stack size whenever the robot is enabled via the Field Management System or the
 * VEX Competition Switch in the autonomous mode. If the robot is disabled or communications is
 * lost, the autonomous task will be stopped by the kernel. Re-enabling the robot will restart
 * the task, not re-start it from where it left off.
 *
 * Code running in the autonomous task cannot access information from the VEX Joystick. However,
 * the autonomous function can be invoked from another task if a VEX Competition Switch is not
 * available, and it can access joystick information if called in this way.
 *
 * The autonomous task may exit, unlike operatorControl() which should never exit. If it does
 * so, the robot will await a switch to another mode or disable/enable cycle.
 */

  /*void driveDistance(double distance) {
     // initialize static variables
     double PI = 3.14159265358979323846;
     int rotations = (int)(distance*360/(4*PI));
     getTo(rotations, rotations, 0.121,0.000,0.00);
  }*/

  // positive degrees is right, negative is left
  void turn(double degrees) {
    getTo(-degrees/2,degrees/2, 0.121, 0.005, 0.071);
  }

void autonomous() {
    /*driveDistance(24.6);
    chassisSet(-40,40);
    delay(510);
    chassisSet(30,30);
    delay(400);
    chassisSet(110,110);
    delay(3050);
    chassisSet(-50,-50);
    delay(30);
    chassisSet(0,0);*/

    /*delay(2000);
    motorSet(10, -120);
    driveDistance(-40);
    delay(300);
    turnLeftDistance(-3);
    driveDistance(45);
    motorSet(10, 0);
    turnLeftDistance(-6.5);
    chassisSet(0,0);
    delay(200);
    catapultThrow();
    motorSet(10,120);
    turnLeftDistance(9);
    driveDistance(-20);*/
    /*delay(2000);
    motorSet(10, -120);
    driveDistance(-40);
    delay(300);
    turnLeftDistance(-3);
    driveDistance(45);
    motorSet(10, 0);
    turnLeftDistance(-4.9);
    chassisSet(0,0);
    delay(500);
    //catapultThrow();
    driveDistance(23);
    delay(200);
    driveDistance(-72);
    turnLeftDistance(-9);
    chassisSet(80,80);
    delay(500);
    chassisSet(0,0);
    chassisSet(120,120);
    delay(2000);
    chassisSet(0,0);*/
    delay(2000);
    motorSet(10, -120);
    driveDistance(-41);
    delay(300);
    driveDistance(11);
    turnLeftDistance(9);
    chassisSet(120,120);
    delay(900);
    chassisSet(0,0);

}
