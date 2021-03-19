#include "vex.h"
#include "miscmethods.h"
#include "auton-externs.h"

// THIS SHOULD BE AN ADD-ON TO TWO-GOAL

void threeGoal(int dir, bool colorBool){
  twoGoal(dir, colorBool);
  // more code here
  sInertial.setRotation(0, deg);
  turnTo(-45*dir, 2);
  strafeViaDistanceGyro(-1000*dir, -45*dir);
  intakeOff();
  driveViaTimeGyroCamera(8000, -45*dir, sigGreen);
  intakeIn();
  wait(200, msec);
  intakeOpen();
  mOutputLower.spin(fwd, 100, pct);
  mOutputUpper.spin(fwd, 100, pct);
  waitForColor(colorBool);
  driveViaDistanceGyro(-4000, -45*dir);
}