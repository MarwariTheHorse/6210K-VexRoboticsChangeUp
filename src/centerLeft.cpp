#include "vex.h"
#include "auton-externs.h"
#include "miscmethods.h"

void centerLeft(bool colorBool){
  sInertial.setRotation(135, deg);
  turnTo(0, 2);
  driveViaDistanceGyro(1000, 0);
  strafeViaDistanceGyro(1000, 0);
}