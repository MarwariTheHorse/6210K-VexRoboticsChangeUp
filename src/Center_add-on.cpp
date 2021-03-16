#include "vex.h"
#include "auton-externs.h"
#include "miscmethods.h"

void center(int dir, bool colorBool){
  sInertial.setRotation(-135*dir, deg);
  turnTo(0, 2);
  strafeViaDistanceGyro(7000*dir, 0);
  driveViaDistanceGyro(1000, 0);
  strafeViaDistanceGyro(1000*dir, 0);
}