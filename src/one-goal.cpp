#include "vex.h"
#include "miscmethods.h"
#include "auton-externs.h"
void oneGoal(int dir, bool colorBool){
  // Set Gyro
  sInertial.setRotation(-135 * dir, deg);

  // Deploy
  mOutputUpper.setVelocity(100, pct);
  wait(200, msec);
  mOutputUpper.setVelocity(0, pct);

  // Wait to fully deplay
  wait(750, msec);

  // Get into the goal
  intakeIn();
  // mWheelFrontLeft.setVelocity(-40*(dir==RIGHT), pct); // TODO: This code is currently not being mirrored so it will only work on one side.
  // mWheelFrontRight.setVelocity(40*(dir==LEFT), pct);  //       We should probably use a ternary operator to fix this.
  // mWheelBackLeft.setVelocity(-40*(dir==LEFT), pct);
  // mWheelBackRight.setVelocity(40*(dir==RIGHT), pct);
  // vexDelay(1000);
  // mWheelFrontLeft.setVelocity(0, pct);
  // mWheelFrontRight.setVelocity(0, pct);
  // mWheelBackLeft.setVelocity(0, pct);
  // mWheelBackRight.setVelocity(0, pct);
  strafeUntilGreen(-65*dir, -135*dir);
  vexDelay(200);
  turnTo(-135*dir, 2);
  intakeOff();
  driveViaTimeGyroCamera(8000, -135*dir, sigGreen);
  mOutputUpper.spin(fwd, 100, pct);
  mOutputLower.spin(fwd, 60, pct);
  wait(400, msec);
  mOutputLower.spin(fwd, 100, pct);
  wait(900, msec);
  waitForColor(colorBool);
  intakeOpen();
  wait(400, msec);
  output(100, 700);
  intakeIn();
  driveViaDistanceGyro(-4000, -135*dir);
  while(Brain.Timer < 8000){
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
  }
  center(dir, colorBool);
}