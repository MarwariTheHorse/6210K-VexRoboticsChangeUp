#include "vex.h"
#include "auton-externs.h"
#include "miscmethods.h"

void centerRight(bool colorBool){
  sInertial.setRotation(-135, deg);
  turnTo(0, 2);
  strafeViaDistanceGyro(7000, 0);
  driveViaDistanceGyro(1000, 0);
  strafeViaDistanceGyro(1000, 0);
  wait(100, msec);
  driveViaDistanceGyro(-2000, 0);
}