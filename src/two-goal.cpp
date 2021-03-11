#include "vex.h"
#include "miscmethods.h"
#include "auton-externs.h"

// THIS CODE IS AN ADD-ON TO ONE-GOAL

void twoGoal(int dir){
  float TurnVelocity;
  float ForwardVelocity;
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1, 1);
  Controller1.Screen.print("<-Blue Y-Red");
  if(Controller1.ButtonLeft.pressing()){
    if(dir == LEFT){
      oneGoal(dir);
      double error = 180 - sInertial.rotation(deg);
      int leftY = 0;
      int rightX = 0;
      int leftX = 0;
      error = (180 - sInertial.rotation(deg));
      // keep turning until within 10 degrees of objective
      // keep adjusting until the robot's velocity slows
      while(fabs(error) > 2 || fabs(TurnVelocity) > 80) 
      {
        if(fabs(error) < 40){
         // if within 40 degrees of objective, the motors start slowing
          rightX = (2 * error);
        }else{
          rightX = 90 * sgn(error);
          // otherwise maintain fast turning speed of 90
          // in the proper direction
        }
        mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
        mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
        mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
        mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);
        wait(5, msec);
        error = 180 - sInertial.rotation(deg);
      }
      // these next lines attempt to slow down the robot's rotational momentum
      // might be better just to put the motors into braking mode
      rightX = -5 * sgn(error);
      mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
      mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
      mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
      mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);
      wait(50, msec);
      driveForward(0, 0);
      // strafe
      leftX = 65;
      leftY = 0;
      sVisionUpper.takeSnapshot(sigGreen);
      // looks 80 pixels in advance to accomidate for overshoot
      while(sVisionUpper.objectCount == 0 || fabs(sVisionUpper.largestObject.centerX - 180) > 60 || sVisionUpper.largestObject.width < 40){
        wait(10, msec);
        sVisionUpper.takeSnapshot(sigGreen);
        rightX = (180 - sInertial.rotation(deg)) * 2;
        mWheelFrontLeft.spin(fwd, rightX + leftY + leftX, pct);
        mWheelFrontRight.spin(fwd, rightX - leftY + leftX, pct);
        mWheelBackLeft.spin(fwd, rightX + leftY - leftX, pct);
        mWheelBackRight.spin(fwd, rightX - leftY - leftX, pct);

      }
      mWheelBackLeft.spin(fwd, 0, pct);
      mWheelFrontLeft.spin(fwd, 0, pct);
      mWheelBackRight.spin(fwd, 0, pct);
      mWheelFrontRight.spin(fwd, 0, pct);
      // move into goal
      intakeOpen();
      double startTime = Brain.timer(msec);
      while (Brain.timer(msec) - startTime < 8000){
        sVisionUpper.takeSnapshot(sigGreen);
        if (sVisionUpper.objectCount > 0 && sVisionUpper.largestObject.width > 40) {
          leftX = (sVisionUpper.largestObject.centerX - 180) * .6; // used to be .8
        } else {
          leftX = 0;
        }
        leftY = 50;
        rightX = (135 - sInertial.rotation(deg)) * 2; // used to be 3
        mWheelFrontLeft.spin(fwd, rightX + leftY + leftX, pct);
        mWheelFrontRight.spin(fwd, rightX - leftY + leftX, pct);
        mWheelBackLeft.spin(fwd, rightX + leftY - leftX, pct);
        mWheelBackRight.spin(fwd, rightX - leftY - leftX, pct);
        if (Brain.timer(msec) - startTime > 400) {
          if (ForwardVelocity < 20) {
          break;
          }
        }
        // this line gives the robot 200ms to speed up (assuming it started at rest)
        // ForwardVelocity is the sum of the 4 wheels, so leftY would make ForwardVelocity = 320
        // ForwardVelocity will go towards zero when the robot runs into something and stops
        wait(5, msec);
      }
      driveForward(0, 0);
      // Score
      intakeIn();
      mOutputLower.spin(fwd, 100, pct);
      mOutputUpper.spin(fwd, 100, pct);
      sVisionUpper.takeSnapshot(sigBlue);
      while(sVisionUpper.largestObject.width < 40 && Brain.timer(msec) - startTime < 1000){
        wait(10, msec);
        sVisionUpper.takeSnapshot(sigBlue);
      }
      mOutputLower.spin(fwd, 0, pct);
      mOutputUpper.spin(fwd, 0, pct);
      intakeOpen();
      wait(400, msec);
      intakeIn();
      while (0 > -4500){
        int d = 0;
        mWheelFrontLeft.resetRotation();
        mWheelBackLeft.resetRotation();
        mWheelFrontRight.resetRotation();
        mWheelBackRight.resetRotation();
        leftY = -80;
        rightX = (135 - sInertial.rotation(deg)) * 3;
        leftX = 0;
        mWheelFrontLeft.spin(fwd, rightX + leftY + leftX, pct);
        mWheelFrontRight.spin(fwd, rightX - leftY + leftX, pct);
        mWheelBackLeft.spin(fwd, rightX + leftY - leftX, pct);
        mWheelBackRight.spin(fwd, rightX - leftY - leftX, pct);
        wait(5, msec);
        d = mWheelFrontLeft.rotation(rotationUnits::raw) - mWheelFrontRight.rotation(rotationUnits::raw) + mWheelBackLeft.rotation(rotationUnits::raw) - mWheelBackRight.rotation(rotationUnits::raw);
      }
      driveForward(0, 0);
    }
  }
}