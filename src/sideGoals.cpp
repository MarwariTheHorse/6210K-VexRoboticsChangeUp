#include "vex.h"
#include "auton-externs.h"
#include "miscmethods.h"
// Descores 1 ball from the side goals
void sideGoal(int dir){
  sInertial.setRotation(0, deg);
  driveViaDistanceGyro(-2000, 0);
  if(dir==LEFT){
    strafeViaDistanceGyro(9000, 0);
  }
  if(dir==RIGHT){
    strafeViaDistanceGyro(5000, 0);
  }
  intakeOpen();
  driveViaDistanceGyro(2000, 0);
  wait(100, msec);
  driveViaDistanceGyro(-2000, 0);
  turnTo(0, 2);
  intakeOff();
}
