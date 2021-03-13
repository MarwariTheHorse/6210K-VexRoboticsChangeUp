#include "vex.h"
#include "miscmethods.h"
#include "auton-externs.h"

// THIS CODE IS AN ADD-ON TO ONE-GOAL

void twoGoal(int dir, bool colorBool){
  oneGoal(dir, colorBool);

  // Turn to wall
  turnTo(180 * dir, 2);

  // strafe
  strafeUntilGreen(65, 180 * dir);

  // open arms
  intakeOpen();

  // drive into goal
  driveViaTimeGyroCamera(8000, 180 * dir, sigGreen);
  intakeOff();

  //Score and descore
  intakeIn();
  mOutputUpper.spin(fwd, 100, pct);
  mOutputLower.spin(fwd, 100, pct);
  waitForColor(colorBool);
  mOutputUpper.spin(fwd, 0, pct);
  
  intakeOpen();
  wait(400, msec);
  intakeIn();
  wait(500, msec);
  intakeOpen();
  driveViaDistanceGyro(-4500, 180 * dir);
}