#include "vex.h"
#include "miscmethods.h"

/*
  Currently we have no use for this method, but if the need arises, we should rewrite this code
  with the new fancy methods we have developed over time
*/

void specialAuton(){
  double leftX;
  double leftY;
  double rightX;

  // Get to the goal
  sInertial.setRotation(-107, deg);
  mWheelFrontLeft.spin(fwd, 0, pct);
  mWheelFrontRight.spin(fwd, -40, pct);
  mWheelBackLeft.spin(fwd, 0, pct);
  mWheelBackRight.spin(fwd, -40, pct);
  vexDelay(500);
  mWheelFrontLeft.spin(fwd, 0, pct);
  mWheelFrontRight.spin(fwd, 0, pct);
  mWheelBackLeft.spin(fwd, 0, pct);
  mWheelBackRight.spin(fwd, 0, pct);
  // Score the preload
  output(100, 500); // 500 > 300 timems
  // Drive in reverse
  driveForward(-20, 200);
  // Turn 45 deg ccw
  while (sInertial.rotation(deg) > -175) {
    mWheelFrontLeft.spin(fwd, -20, pct);
    mWheelBackLeft.spin(fwd, -20, pct);
  }
  mWheelFrontLeft.spin(fwd, 0, pct);
  mWheelFrontRight.spin(fwd, 0, pct);
  mWheelBackLeft.spin(fwd, 0, pct);
  mWheelBackRight.spin(fwd, 0, pct);

  intakeIn();

  // Line track until we reach the perpendicular line
  while (sOpticalFront.getRgb().clear < OPTICAL_THRESHOLD) {
    // Calculate new moving average for the front optical
    // Assign movement variables
    leftX = -25;
    leftY;
    rightX = 0;
    if (sOpticalBack.getRgb().clear > OPTICAL_THRESHOLD)
      leftY = 30;
    else
      leftY = -10;
    // Assign wheel speeds
    mWheelFrontLeft.spin(fwd, rightX + leftY + leftX, pct);
    mWheelFrontRight.spin(fwd, rightX - leftY + leftX, pct);
    mWheelBackLeft.spin(fwd, rightX + leftY - leftX, pct);
    mWheelBackRight.spin(fwd, rightX - leftY - leftX, pct);
  }
  mWheelFrontLeft.spin(fwd, 0, pct);
  mWheelFrontRight.spin(fwd, 0, pct);
  mWheelBackLeft.spin(fwd, 0, pct);
  mWheelBackRight.spin(fwd, 0, pct);

  intakeOpen();

  // Drive to the left a bit
  leftX = -25;
  leftY = 0;
  rightX = 0;
  mWheelFrontLeft.spin(fwd, rightX + leftY + leftX, pct);
  mWheelFrontRight.spin(fwd, rightX - leftY + leftX, pct);
  mWheelBackLeft.spin(fwd, rightX + leftY - leftX, pct);
  mWheelBackRight.spin(fwd, rightX - leftY - leftX, pct);
  vexDelay(200); // 500 > 300
  mWheelFrontLeft.spin(fwd, 0, pct);
  mWheelFrontRight.spin(fwd, 0, pct);
  mWheelBackLeft.spin(fwd, 0, pct);
  mWheelBackRight.spin(fwd, 0, pct);

  // Thrust into the goal
  driveForward(100, 1000);

  // Spit out the ball we have
  output(100, 600); // 500 > 300 timems

  // Reverse into the center goal

  double startTime = Brain.timer(msec);
  while (Brain.timer(msec) - startTime < 2000) {
    int ySpeed;
    int rotSpeed;
    ySpeed = -75;
    rotSpeed = -(sInertial.rotation() + 180) * 2;
    mWheelFrontLeft.spin(fwd, rotSpeed + ySpeed, pct);
    mWheelFrontRight.spin(fwd, rotSpeed - ySpeed, pct);
    mWheelBackLeft.spin(fwd, rotSpeed + ySpeed, pct);
    mWheelBackRight.spin(fwd, rotSpeed - ySpeed, pct);
  }
  mWheelFrontLeft.spin(fwd, 0, pct);
  mWheelFrontRight.spin(fwd, 0, pct);
  mWheelBackLeft.spin(fwd, 0, pct);
  mWheelBackRight.spin(fwd, 0, pct);

  intakeOff();

  // Turn towards
  mWheelFrontRight.spin(fwd, -50, pct);
  mWheelBackRight.spin(fwd, -50, pct);
  waitUntil(sInertial.rotation() > -225);

  // Gun it towards the wall
  mWheelFrontLeft.spin(fwd, -100, pct);
  mWheelFrontRight.spin(fwd, -100, pct);
  mWheelBackLeft.spin(fwd, 100, pct);
  mWheelBackRight.spin(fwd, 100, pct);

  intakeOpen();

  vexDelay(2000);

  mWheelFrontLeft.spin(fwd, 0, pct);
  mWheelFrontRight.spin(fwd, 0, pct);
  mWheelBackLeft.spin(fwd, 0, pct);
  mWheelBackRight.spin(fwd, 0, pct);

  output(100, 1000);

  intakeOpen();

  driveForward(-100, 500);

  intakeOff();
}