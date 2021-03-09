#include "vex.h"
#include "miscmethods.h"
#include "auton-externs.h"

void oneGoal(int dir){
  float ForwardVelocity;
  // Code here
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1, 1);
  Controller1.Screen.print("<-Blue Y-Red");
  if(Controller1.ButtonLeft.pressing()){
    if(dir == LEFT){
      // Set Gyro
      sInertial.setRotation(135, deg);
      // Deploy intakes
      intakeIn();
      // Strafe until Blue ball
      int leftX;
      int leftY;
      int rightX;
      leftX = 55;
      leftY = 0;
      sVisionLower.takeSnapshot(sigBlue);
      // looks for red ball within +/-60 pixels of centerline
      // red ball must be 40 pixels in width
      while(sVisionLower.objectCount == 0 || fabs(sVisionLower.largestObject.centerX - 180) > 60 || sVisionLower.largestObject.width < 40){
        wait(10, msec);
        sVisionLower.takeSnapshot(sigBlue);
        rightX = (135 - sInertial.rotation(deg)) * 2;
        mWheelFrontLeft.spin(fwd, rightX + leftY + leftX, pct);
        mWheelFrontRight.spin(fwd, rightX - leftY + leftX, pct);
        mWheelBackLeft.spin(fwd, rightX + leftY - leftX, pct);
        mWheelBackRight.spin(fwd, rightX - leftY - leftX, pct);
      }
      mWheelBackLeft.spin(fwd, 0, pct);
      mWheelFrontLeft.spin(fwd, 0, pct);
      mWheelBackRight.spin(fwd, 0, pct);
      mWheelFrontRight.spin(fwd, 0, pct);
      // Go to blue ball
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
      // Grab ball
      intakeIn();
      // Ensure we are in the goal
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
        wait(5, msec);
      }
      driveForward(0, 0);
      // Score
      mOutputLower.spin(fwd, 100, pct);
      mOutputUpper.spin(fwd, 100, pct);
      wait(800, msec);
      sVisionUpper.takeSnapshot(sigBlue);
      while(sVisionUpper.largestObject.width < 100 && Brain.timer(msec) - startTime < 2500){
        wait(10, msec);
        sVisionUpper.takeSnapshot(sigBlue);
      }
      mOutputLower.spin(fwd, 0, pct);
      // Make sure we score the last ball
      mOutputUpper.spin(fwd, -100, pct);
      wait(200, msec);
      intakeOpen();
      wait(400, msec);
      mOutputLower.spin(fwd, 100, pct);
      mOutputUpper.spin(fwd, 100, pct);
      // Make sure we don't score red
      sVisionUpper.takeSnapshot(sigBlue);
      while(sVisionUpper.largestObject.width < 100 && Brain.timer(msec) - startTime < 2500){
        wait(10, msec);
        sVisionUpper.takeSnapshot(sigBlue);
      }
      // Drive backwards
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
    if(dir == RIGHT){
      // Set Gyro
      sInertial.setRotation(-135, deg);
      // Deploy intakes
      intakeIn();
      // Strafe until Blue ball
      int leftX;
      int leftY;
      int rightX;
      leftX = 55;
      leftY = 0;
      sVisionUpper.takeSnapshot(sigGreen);
      // looks for red ball within +/-60 pixels of centerline
      // red ball must be 40 pixels in width
      while(sVisionLower.objectCount == 0 || fabs(sVisionLower.largestObject.centerX - 180) > 60 || sVisionLower.largestObject.width < 40){
        wait(10, msec);
        sVisionLower.takeSnapshot(sigGreen);
        rightX = (-135 - sInertial.rotation(deg)) * 2;
        mWheelFrontLeft.spin(fwd, rightX + leftY + leftX, pct);
        mWheelFrontRight.spin(fwd, rightX - leftY + leftX, pct);
        mWheelBackLeft.spin(fwd, rightX + leftY - leftX, pct);
        mWheelBackRight.spin(fwd, rightX - leftY - leftX, pct);
      }
      mWheelBackLeft.spin(fwd, 0, pct);
      mWheelFrontLeft.spin(fwd, 0, pct);
      mWheelBackRight.spin(fwd, 0, pct);
      mWheelFrontRight.spin(fwd, 0, pct);
      // Go to blue ball
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
        rightX = (-135 - sInertial.rotation(deg)) * 2; // used to be 3
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
      // Grab ball
      intakeIn();
      // Ensure we are in the goal
      while (Brain.timer(msec) - startTime < 8000){
        sVisionUpper.takeSnapshot(sigGreen);
        if (sVisionUpper.objectCount > 0 && sVisionUpper.largestObject.width > 40) {
          leftX = (sVisionUpper.largestObject.centerX - 180) * .6; // used to be .8
        } else {
          leftX = 0;
        }
        leftY = 50;
        rightX = (-135 - sInertial.rotation(deg)) * 2; // used to be 3
        mWheelFrontLeft.spin(fwd, rightX + leftY + leftX, pct);
        mWheelFrontRight.spin(fwd, rightX - leftY + leftX, pct);
        mWheelBackLeft.spin(fwd, rightX + leftY - leftX, pct);
        mWheelBackRight.spin(fwd, rightX - leftY - leftX, pct);
        if (Brain.timer(msec) - startTime > 400) {
          if (ForwardVelocity < 20) {
          break;
          }
        }
        wait(5, msec);
      }
      driveForward(0, 0);
      // Score
      mOutputLower.spin(fwd, 100, pct);
      mOutputUpper.spin(fwd, 100, pct);
      wait(800, msec);
      sVisionUpper.takeSnapshot(sigBlue);
      while(sVisionUpper.largestObject.width < 100 && Brain.timer(msec) - startTime < 2500){
        wait(10, msec);
        sVisionUpper.takeSnapshot(sigBlue);
      }
      mOutputLower.spin(fwd, 0, pct);
      // Make sure we score the last ball
      mOutputUpper.spin(fwd, -100, pct);
      wait(200, msec);
      intakeOpen();
      wait(400, msec);
      mOutputLower.spin(fwd, 100, pct);
      mOutputUpper.spin(fwd, 100, pct);
      // Make sure we don't score red
      sVisionUpper.takeSnapshot(sigBlue);
      while(sVisionUpper.largestObject.width < 100 && Brain.timer(msec) - startTime < 2500){
        wait(10, msec);
        sVisionUpper.takeSnapshot(sigBlue);
      }
      // Drive backwards
      while (0 > -4500){
        int d = 0;
        mWheelFrontLeft.resetRotation();
        mWheelBackLeft.resetRotation();
        mWheelFrontRight.resetRotation();
        mWheelBackRight.resetRotation();
        leftY = -80;
        rightX = (-135 - sInertial.rotation(deg)) * 3;
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
  if(Controller1.ButtonY.pressing()){
    if(dir == LEFT){
      // Set Gyro
      sInertial.setRotation(135, deg);
      // Deploy intakes
      intakeIn();
      // Strafe until Blue ball
      int leftX;
      int leftY;
      int rightX;
      leftX = 55;
      leftY = 0;
      sVisionLower.takeSnapshot(sigRed);
      // looks for red ball within +/-60 pixels of centerline
      // red ball must be 40 pixels in width
      while(sVisionLower.objectCount == 0 || fabs(sVisionLower.largestObject.centerX - 180) > 60 || sVisionLower.largestObject.width < 40){
        wait(10, msec);
        sVisionLower.takeSnapshot(sigRed);
        rightX = (135 - sInertial.rotation(deg)) * 2;
        mWheelFrontLeft.spin(fwd, rightX + leftY + leftX, pct);
        mWheelFrontRight.spin(fwd, rightX - leftY + leftX, pct);
        mWheelBackLeft.spin(fwd, rightX + leftY - leftX, pct);
        mWheelBackRight.spin(fwd, rightX - leftY - leftX, pct);
      }
      mWheelBackLeft.spin(fwd, 0, pct);
      mWheelFrontLeft.spin(fwd, 0, pct);
      mWheelBackRight.spin(fwd, 0, pct);
      mWheelFrontRight.spin(fwd, 0, pct);
      // Go to blue ball
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
      // Grab ball
      intakeIn();
      // Ensure we are in the goal
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
        wait(5, msec);
      }
      driveForward(0, 0);
      // Score
      mOutputLower.spin(fwd, 100, pct);
      mOutputUpper.spin(fwd, 100, pct);
      wait(800, msec);
      sVisionUpper.takeSnapshot(sigRed);
      while(sVisionUpper.largestObject.width < 100 && Brain.timer(msec) - startTime < 2500){
        wait(10, msec);
        sVisionUpper.takeSnapshot(sigRed);
      }
      mOutputLower.spin(fwd, 0, pct);
      // Make sure we score the last ball
      mOutputUpper.spin(fwd, -100, pct);
      wait(200, msec);
      intakeOpen();
      wait(400, msec);
      mOutputLower.spin(fwd, 100, pct);
      mOutputUpper.spin(fwd, 100, pct);
      // Make sure we don't score red
      sVisionUpper.takeSnapshot(sigRed);
      while(sVisionUpper.largestObject.width < 100 && Brain.timer(msec) - startTime < 2500){
        wait(10, msec);
        sVisionUpper.takeSnapshot(sigRed);
      }
      // Drive backwards
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
    if(dir == RIGHT){
      // Set Gyro
      sInertial.setRotation(-135, deg);
      // Deploy intakes
      intakeIn();
      // Strafe until Blue ball
      int leftX;
      int leftY;
      int rightX;
      leftX = 55;
      leftY = 0;
      sVisionUpper.takeSnapshot(sigGreen);
      // looks for red ball within +/-60 pixels of centerline
      // red ball must be 40 pixels in width
      while(sVisionLower.objectCount == 0 || fabs(sVisionLower.largestObject.centerX - 180) > 60 || sVisionLower.largestObject.width < 40){
        wait(10, msec);
        sVisionLower.takeSnapshot(sigGreen);
        rightX = (-135 - sInertial.rotation(deg)) * 2;
        mWheelFrontLeft.spin(fwd, rightX + leftY + leftX, pct);
        mWheelFrontRight.spin(fwd, rightX - leftY + leftX, pct);
        mWheelBackLeft.spin(fwd, rightX + leftY - leftX, pct);
        mWheelBackRight.spin(fwd, rightX - leftY - leftX, pct);
      }
      mWheelBackLeft.spin(fwd, 0, pct);
      mWheelFrontLeft.spin(fwd, 0, pct);
      mWheelBackRight.spin(fwd, 0, pct);
      mWheelFrontRight.spin(fwd, 0, pct);
      // Go to blue ball
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
        rightX = (-135 - sInertial.rotation(deg)) * 2; // used to be 3
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
      // Grab ball
      intakeIn();
      // Ensure we are in the goal
      while (Brain.timer(msec) - startTime < 8000){
        sVisionUpper.takeSnapshot(sigGreen);
        if (sVisionUpper.objectCount > 0 && sVisionUpper.largestObject.width > 40) {
          leftX = (sVisionUpper.largestObject.centerX - 180) * .6; // used to be .8
        } else {
          leftX = 0;
        }
        leftY = 50;
        rightX = (-135 - sInertial.rotation(deg)) * 2; // used to be 3
        mWheelFrontLeft.spin(fwd, rightX + leftY + leftX, pct);
        mWheelFrontRight.spin(fwd, rightX - leftY + leftX, pct);
        mWheelBackLeft.spin(fwd, rightX + leftY - leftX, pct);
        mWheelBackRight.spin(fwd, rightX - leftY - leftX, pct);
        if (Brain.timer(msec) - startTime > 400) {
          if (ForwardVelocity < 20) {
          break;
          }
        }
        wait(5, msec);
      }
      driveForward(0, 0);
      // Score
      mOutputLower.spin(fwd, 100, pct);
      mOutputUpper.spin(fwd, 100, pct);
      wait(800, msec);
      sVisionUpper.takeSnapshot(sigRed);
      while(sVisionUpper.largestObject.width < 100 && Brain.timer(msec) - startTime < 2500){
        wait(10, msec);
        sVisionUpper.takeSnapshot(sigRed);
      }
      mOutputLower.spin(fwd, 0, pct);
      // Make sure we score the last ball
      mOutputUpper.spin(fwd, -100, pct);
      wait(200, msec);
      intakeOpen();
      wait(400, msec);
      mOutputLower.spin(fwd, 100, pct);
      mOutputUpper.spin(fwd, 100, pct);
      // Make sure we don't score red
      sVisionUpper.takeSnapshot(sigRed);
      while(sVisionUpper.largestObject.width < 100 && Brain.timer(msec) - startTime < 2500){
        wait(10, msec);
        sVisionUpper.takeSnapshot(sigRed);
      }
      // Drive backwards
      while (0 > -4500){
        int d = 0;
        mWheelFrontLeft.resetRotation();
        mWheelBackLeft.resetRotation();
        mWheelFrontRight.resetRotation();
        mWheelBackRight.resetRotation();
        leftY = -80;
        rightX = (-135 - sInertial.rotation(deg)) * 3;
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