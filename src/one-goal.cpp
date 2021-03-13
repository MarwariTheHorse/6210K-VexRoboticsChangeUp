#include "vex.h"
#include "miscmethods.h"
#include "auton-externs.h"

void oneGoal(int dir, bool colorBool){
  // Set Gyro
  sInertial.setRotation(-135 * dir, deg); // Everything is going to be relative to our starting position for the sake of my sanity

  // Deploy
  mOutputUpper.setVelocity(100, pct);
  wait(200, msec);
  mOutputUpper.setVelocity(0, pct);

  // Wait to fully deplay
  wait(750, msec);

  // Get into the goal
  mWheelFrontLeft.setVelocity(-40, pct);
  mWheelFrontRight.setVelocity(0, pct);
  mWheelBackLeft.setVelocity(0, pct);
  mWheelBackRight.setVelocity(40, pct);
  vexDelay(1000);
  mWheelFrontLeft.setVelocity(0, pct);
  mWheelFrontRight.setVelocity(0, pct);
  mWheelBackLeft.setVelocity(0, pct);
  mWheelBackRight.setVelocity(0, pct);
  vexDelay(200);
  intakeIn();
  driveForward(100, 1500);

  // Score preload, score the red ball off the ground, and descore the blues
  output(100, 2000);

  // Fully deploy - back out
  driveForward(-75, 1000);

  // Run intakes
  intakeIn();
  // Wait for the deploy
  wait(500, msec);

  // Gun into the goal
  driveForward(100, 3000);

  output(100, 2000);
  intakeOff();
  intakeOpen();
  wait(100, msec);
  intakeIn();
  prepOutput(100, 400);
  driveForward(-100, 500);
  intakeOff();
}