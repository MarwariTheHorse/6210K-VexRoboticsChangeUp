#include "vex.h"
#include "miscmethods.h"
#include "auton-externs.h"

// THIS METHOD SHOULD BE AN ADD-ON TO THREE-GOAL

void threeGoalCenter(int dir, bool colorBool){
  threeGoal(dir, colorBool);
  // More code below
  if(dir==LEFT){
    centerLeft(colorBool);
  } else{
    centerRight(colorBool);
  }
}