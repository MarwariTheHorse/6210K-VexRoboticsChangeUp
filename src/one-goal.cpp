#include "vex.h"
#include "miscmethods.h"
#include "auton-externs.h"
void oneGoal(int dir, bool colorBool){
  // Set Gyro
  sInertial.setRotation(-90 * dir, deg);

  // Deploy
  mOutputUpper.setVelocity(100, pct);
  wait(200, msec);
  mOutputUpper.setVelocity(0, pct);

  // Wait to fully deplay
  wait(750, msec);

  // If strafing doesn't work we can try this code below
  mWheelFrontLeft.setVelocity(-40*(dir==RIGHT), pct);
  mWheelFrontRight.setVelocity(40*(dir==LEFT), pct);
  mWheelBackLeft.setVelocity(-40*(dir==LEFT), pct);
  mWheelBackRight.setVelocity(40*(dir==RIGHT), pct);
  vexDelay(1000);
  mWheelFrontLeft.setVelocity(0, pct);
  mWheelFrontRight.setVelocity(0, pct);
  mWheelBackLeft.setVelocity(0, pct);
  mWheelBackRight.setVelocity(0, pct);

  // Make sure we are facing the right way
  turnTo(-135*dir, 2);

  // Score preload
  mOutputUpper.spin(fwd, 100, pct);
  mOutputLower.spin(fwd, 60, pct);
  wait(400, msec);
  mOutputLower.spin(fwd, 100, pct);
  wait(900, msec);

  // Wait until the final ball is scored
  waitForColor(colorBool);

  // Descore lower ball
  intakeOpen();
  wait(400, msec);
  output(100, 700);
  intakeIn();

  // Back out of the goal
  driveViaDistanceGyro(-4000, -135*dir);

  
  // Try to deploy the intakes
  intakeIn();
}