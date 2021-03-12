#include "vex.h"
#include "miscmethods.h"
#include "auton-externs.h"

void oneGoal(int dir){
  // Set Gyro
  sInertial.setRotation(-135 * dir, deg); // Everything is going to be relative to our starting position for the sake of my sanity

  // Deploy
  mOutputUpper.setVelocity(100, pct);
  wait(300, msec);
  mOutputUpper.setVelocity(0, pct);

  intakeIn();

  // Wait to fully deplay
  wait(750, msec);

  // Get into the goal
  mWheelFrontLeft.setVelocity(dir==LEFT ? 20 : -40, pct);
  mWheelFrontRight.setVelocity(dir==LEFT ? -40 : 20, pct);
  mWheelBackLeft.setVelocity(dir==LEFT ? 20 : -40, pct);
  mWheelBackRight.setVelocity(dir==LEFT ? -40 : 20, pct);
  vexDelay(500);
  mWheelFrontLeft.setVelocity(0, pct);
  mWheelFrontRight.setVelocity(0, pct);
  mWheelBackLeft.setVelocity(0, pct);
  mWheelBackRight.setVelocity(0, pct);

  // Score preload, score the red ball off the ground, and descore the blues
  output(100, 2000);

  // Fully deploy
  driveForward(-75, 500);

  intakeIn();
  wait(500, msec);

  driveForward(100, 1000);

  output(100, 2000);
  intakeOff();
  intakeOpen();
  wait(100, msec);
  intakeIn();
  prepOutput(100, 400);
  driveForward(-100, 500);
  intakeOff();
}