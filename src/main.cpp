// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// sVisionUpper         vision        17              
// sVisionLower         vision        20              
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// sVision              vision        21
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// sVision              vision        21
// ---- END VEXCODE CONFIGURED DEVICES ----
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// Caleb Baker
// Caleb Buening
// Joey Curnutt

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// sVision              vision        21
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

#include "miscmethods.h"

using namespace vex;

// A global instance of competition
competition Competition;

// Other global variables
char mode = 'N';
bool disableIntakes = false;
// these are global variables
// float ForwardDistance;
// float TurnDistance;
// float StrafeDistance;
float ForwardVelocity;
float TurnVelocity;
float StrafeVelocity;

void pre_auton(void) {
  vexcodeInit();

  // Print options
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1, 1);
  Controller1.Screen.print("V-auton B-none");
  Controller1.Screen.setCursor(2, 1);
  Controller1.Screen.print("<-L1 ^-L2 >-L3");
  Controller1.Screen.setCursor(3, 1);
  Controller1.Screen.print("Y-R1 X-R2 A-R3");

  disableIntakes = true;

  // Get input
  while (mode == 'N') {
    if (Controller1.ButtonDown.pressing())
      mode = 'V';
    if (Controller1.ButtonB.pressing())
      break;

    if (Controller1.ButtonY.pressing())
      mode = 'Y';
    if (Controller1.ButtonA.pressing())
      mode = 'A';
    if (Controller1.ButtonX.pressing())
      mode = 'X';

    if (Controller1.ButtonLeft.pressing())
      mode = '<';
    if (Controller1.ButtonRight.pressing())
      mode = '>';
    if (Controller1.ButtonUp.pressing())
      mode = '^';
    if (Controller1.ButtonR1.pressing()) {
      mode = 'S';
    }
  }
  Controller1.Screen.clearScreen();

  // Calibrate
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1, 1);
  Controller1.Screen.print("Calibrate?");
  Controller1.Screen.setCursor(2, 1);
  Controller1.Screen.print("Press A");
  waitUntil(Controller1.ButtonA.pressing());

  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1, 1);
  Controller1.Screen.print("Calibrating...");
  sInertial.calibrate();
  waitUntil(!sInertial.isCalibrating());

  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1, 1);
  Controller1.rumble("..");
  Controller1.Screen.print("DONE");
  wait(500, msec);
  Controller1.Screen.clearScreen();
  disableIntakes = false;

  // Activiate the opticals
  sOpticalFront.setLight(ledState::on);
  sOpticalBack.setLight(ledState::on);
  sOpticalFront.setLightPower(100, pct);
  sOpticalBack.setLightPower(100, pct);
}

// Experimental reusable auton methods

void scoreFirstCornerGoal(int dir) {
  int leftVelocity;
  int rightVelocity;
  if (dir == LEFT) {
    leftVelocity = 0;
    rightVelocity = -40;
  } else {
    leftVelocity = 40;
    rightVelocity = 0;
  }

  // Get to the goal
  mWheelFrontLeft.setVelocity(leftVelocity, pct);
  mWheelFrontRight.setVelocity(rightVelocity, pct);
  mWheelBackLeft.setVelocity(leftVelocity, pct);
  mWheelBackRight.setVelocity(rightVelocity, pct);
  vexDelay(300);
  mWheelFrontLeft.setVelocity(0, pct);
  mWheelFrontRight.setVelocity(0, pct);
  mWheelBackLeft.setVelocity(0, pct);
  mWheelBackRight.setVelocity(0, pct);

  // Ensure we are in the goal
  driveForward(100, 1000);

  // Outtake the preload
  output(100, 400);

  // Make sure we are out of the goal
  driveForward(-20, 500);
}

void turnTo(double angle){
  while(fabs(fabs(angle) - fabs(sInertial.rotation(deg))) > 2 || fabs(TurnVelocity) > 10) // uses global variable TurnVelocity
  {
    // Calculate error
    double error = angle - sInertial.rotation(deg);
    if(error > 90) error = 90;   // cap positive motor power at +90
    if(error < -90) error = -90; // cap negative motor power at -90
    mWheelFrontLeft.setVelocity(error, pct);
    mWheelFrontRight.setVelocity(error, pct);
    mWheelBackLeft.setVelocity(error, pct);
    mWheelBackRight.setVelocity(error, pct);
    wait(5, msec);
  }
  driveForward(0, 0);
}

void driveViaDistanceGyro(double dist, double a){
  // reset all motor encoders to zero
  // 10000 units is equal to 56" of travel
  mWheelFrontLeft.resetRotation();
  mWheelBackLeft.resetRotation();
  mWheelFrontRight.resetRotation();
  mWheelBackRight.resetRotation();
  int d = 0;

  d = mWheelFrontLeft.rotation(rotationUnits::raw) - mWheelFrontRight.rotation(rotationUnits::raw) + mWheelBackLeft.rotation(rotationUnits::raw) - mWheelBackRight.rotation(rotationUnits::raw);

  if(d < dist){
    while (d < dist){
      int leftY = 80;
      int rightX = (a - sInertial.rotation(deg)) * 3;
      int leftX = 0;
      mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
      mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
      mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
      mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);
      wait(5, msec);
      d = mWheelFrontLeft.rotation(rotationUnits::raw) - mWheelFrontRight.rotation(rotationUnits::raw) + mWheelBackLeft.rotation(rotationUnits::raw) - mWheelBackRight.rotation(rotationUnits::raw);
    }
  }else{
    while (d > dist){
      int leftY = -80;
      int rightX = (a - sInertial.rotation(deg)) * 3;
      int leftX = 0;
      mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
      mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
      mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
      mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);
      wait(5, msec);
      d = mWheelFrontLeft.rotation(rotationUnits::raw) - mWheelFrontRight.rotation(rotationUnits::raw) + mWheelBackLeft.rotation(rotationUnits::raw) - mWheelBackRight.rotation(rotationUnits::raw);
    }
  }
  driveForward(0, 0);
}

void strafeViaDistanceGyro(double dist, double a){
  // reset all motor encoders to zero
  // 10000 units is equal to 56" of travel
  mWheelFrontLeft.resetRotation();
  mWheelBackLeft.resetRotation();
  mWheelFrontRight.resetRotation();
  mWheelBackRight.resetRotation();
  int d = 0;

  d = mWheelFrontLeft.rotation(rotationUnits::raw) + mWheelFrontRight.rotation(rotationUnits::raw) - mWheelBackLeft.rotation(rotationUnits::raw) - mWheelBackRight.rotation(rotationUnits::raw);

  if(d < dist){
    while (d < dist){
      int leftY = 0;
      int rightX = (a - sInertial.rotation(deg)) * 3;
      int leftX = 50;
      mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
      mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
      mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
      mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);
      wait(5, msec);
      d = mWheelFrontLeft.rotation(rotationUnits::raw) + mWheelFrontRight.rotation(rotationUnits::raw) - mWheelBackLeft.rotation(rotationUnits::raw) - mWheelBackRight.rotation(rotationUnits::raw);
    }
  }else{
    while (d > dist){
      int leftY = -80;
      int rightX = (a - sInertial.rotation(deg)) * 3;
      int leftX = 0;
      mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
      mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
      mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
      mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);
      wait(5, msec);
      d = mWheelFrontLeft.rotation(rotationUnits::raw) - mWheelFrontRight.rotation(rotationUnits::raw) + mWheelBackLeft.rotation(rotationUnits::raw) - mWheelBackRight.rotation(rotationUnits::raw);
    }
  }
  driveForward(0, 0);
}

void driveViaTimeGyroCamera(double timeInMS, double a, signature sig){
  // This method drives according to time and corrects with camera (strafe) and gyro (angle)
  // The loop breaks if the robot runs into an obstacle
  double startTime = Brain.timer(msec);
  int leftX;
  int leftY;
  int rightX;
  while (Brain.timer(msec) - startTime < timeInMS){
    sVisionUpper.takeSnapshot(sig);
    if (sVisionUpper.objectCount > 0) {
      leftX = (sVisionUpper.largestObject.centerX - 180) * .8;
    } else {
      leftX = 0;
    }
    leftY = 50;
    rightX = (a - sInertial.rotation(deg)) * 3;
    mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
    mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
    mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
    mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);
    if (Brain.timer(msec) - startTime > 500) {
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
}

void alignToGoal(double a){
  while(fabs(fabs(a) - fabs(sInertial.rotation(deg))) > 2 || fabs(TurnVelocity) > 10) // uses global variable TurnVelocity
  {
    // Calculate error
    double error = a - sInertial.rotation(deg);
    if(error > 90) error = 90;   // cap positive motor power at +90
    if(error < -90) error = -90; // cap negative motor power at -90
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(error, pct);
    mWheelBackRight.setVelocity(error, pct);
    wait(5, msec);
  }
  driveForward(0, 0);
}

void strafeUntilGreen(int speed){
  mWheelBackLeft.setVelocity(-speed, pct);
  mWheelFrontLeft.setVelocity(speed, pct);
  mWheelBackRight.setVelocity(-speed, pct);
  mWheelFrontRight.setVelocity(speed, pct);
  sVisionUpper.takeSnapshot(sigGreen);
  // looks 80 pixels in advance to accomidate for overshoot
  while(sVisionUpper.objectCount == 0 || fabs(sVisionUpper.largestObject.centerX - 180) > 80){
    wait(10, msec);
    sVisionUpper.takeSnapshot(sigGreen);
  }
  mWheelBackLeft.setVelocity(0, pct);
  mWheelFrontLeft.setVelocity(0, pct);
  mWheelBackRight.setVelocity(0, pct);
  mWheelFrontRight.setVelocity(0, pct);
  //setStopping(brake);
}

void autonomous(void) {
  // output(100, 5000); // Spit the ball into the goal
  // intake(-75, 1000); // Flick the intakes outward
  // driveForward(-50, 1000); // Back out and away from the goal

  mWheelFrontLeft.spin(fwd);
  mWheelFrontRight.spin(fwd);
  mWheelBackLeft.spin(fwd);
  mWheelBackRight.spin(fwd);
  mIntakeLeft.spin(fwd);
  mIntakeRight.spin(fwd);
  mOutputLower.spin(fwd);
  mOutputUpper.spin(fwd);

  mWheelFrontLeft.setVelocity(0, pct);
  mWheelFrontRight.setVelocity(0, pct);
  mWheelBackLeft.setVelocity(0, pct);
  mWheelBackRight.setVelocity(0, pct);
  mIntakeLeft.setVelocity(0, pct);
  mIntakeRight.setVelocity(0, pct);
  mOutputLower.setVelocity(0, pct);
  mOutputUpper.setVelocity(0, pct);
  mOutputLower.setBrake(hold);
  mOutputUpper.setBrake(hold);

  int leftX;
  int leftY;
  int rightX;

  // STATE AUTONOMOUS
  if (mode == 'V') {
  
    sInertial.setRotation(-57, deg); // BACK TO -57

    // Step 1 - Deploy Camera and Hood and flick ball into goal
    mOutputUpper.setVelocity(100, pct);
    wait(300, msec);
    mOutputUpper.setVelocity(0, pct);

    // Step 2 - Get ball 1,
    mOutputLower.setVelocity(100, pct);
    driveViaDistanceGyro(6000, -57);

    // Step 2A - Deploy Arms
    intakeIn();
    wait(1000, msec);
    intakeOff();

    // Prepare for strafe
    turnTo(-180);
    intakeOff();
    intakeIn();

    // Strafe until we see the goal
    strafeUntilGreen(50);

    // open arms
    intakeOpenAuton();
    wait(100, msec);

    // drive into goal
    driveViaTimeGyroCamera(1000, -180, sigGreen);
    alignToGoal(-180);
    intakeOff();

    //Score and descore
    output(100, 600);
    mOutputLower.setVelocity(100, pct);
    intakeIn();
    wait(700, msec);
    // Back up and eject blue

    driveViaDistanceGyro(-1700, -180);

    intakeOff();
    intakeOpenAuton();
    turnTo(-270);
    output(-100,500);
    //mOutputLower.startSpinFor(-5, rotationUnits::rev, 100, velocityUnits::pct);
    //mOutputUpper.spinFor(-5, rotationUnits::rev, 100, velocityUnits::pct);
    strafeViaDistanceGyro(200,-270);
    turnTo(-360);

    // pick up red and score center

    mOutputLower.setVelocity(80, pct);
    driveViaTimeGyroCamera(2000, -360, sigBlue);
    alignToGoal(-360);
    intakeIn();
    mOutputUpper.setVelocity(100, pct);
    sVisionUpper.takeSnapshot(sigRed);
    double startTime = Brain.timer(msec);
    while(sVisionUpper.largestObject.width < 100 && Brain.timer(msec) - startTime < 1000){
      wait(10, msec);
      sVisionUpper.takeSnapshot(sigRed);
    }
    mOutputLower.setVelocity(-100, pct);
    mOutputUpper.setVelocity(-100, pct);
    intakeOpenAuton();
    driveViaDistanceGyro(-1000, -360);
    turnTo(-135);

    // // Drive at -90 to align with goal
    // driveViaDistanceGyro(3500, -90);

    // // Turn to face the center goal
    // turnTo(0);
    // intakeOpenAuton();

    // // Drive into the goal
    // driveViaTimeGyroCamera(3000, 0, sigBlue);

    // // Make sure we are perpendicular to goal
    // alignToGoal(0);

    // // Score our red Ball
    // intakeIn();
    // wait(750, msec);
    // mOutputUpper.setVelocity(100, pct);
    // wait(250, msec);
    // mOutputUpper.setVelocity(0, pct);
    // intakeOff();
    

    // // Step 5 - Drive into center goal using camera and gyro
    // intakeOpenAuton();
    // startTime = Brain.timer(msec);
    // while ((Brain.timer(msec) - startTime) < 750) {
    //   sVisionUpper.takeSnapshot(sigBlue);
    //   if (sVisionUpper.objectCount > 0) {
    //     leftX = (sVisionUpper.largestObject.centerX - 158) * 1;
    //   } else {
    //     leftX = 0;
    //   }
    //   leftY = 100;
    //   rightX = (0 - sInertial.rotation(deg)) * 5;
    //   mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
    //   mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
    //   mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
    //   mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);
    //   wait(5, msec);
    // }
    // driveForward(0, 0);

    // // Score
    // intakeIn();
    // output(100, 500);
    // wait(100, msec);
    // mOutputUpper.setVelocity(-100, pct);
    // intakeOpenAuton();
    // wait(300, msec);

    // // Drive backwards and make 180 degree turn towards middle home row goal
    // driveForward(-100, 700);
    // while (sInertial.rotation(deg) > -178) {
    //   mWheelBackRight.setVelocity(40, pct);
    //   mWheelFrontRight.setVelocity(40, pct);
    //   mWheelBackLeft.setVelocity(0, pct);
    //   mWheelFrontLeft.setVelocity(0, pct);
    // }
    // // Use camera and gyro to align
    // // TODO: TEST THIS CODE ON THE BOT
    // intakeOpenAuton();
    // while ((Brain.timer(msec) - startTime) < 750) {
    //   sVisionUpper.takeSnapshot(sVisionUpper__SIG_GREEN);
    //   if (sVisionUpper.objectCount > 0) {
    //     leftX = (sVisionUpper.largestObject.centerX - 158) * 1;
    //   } else {
    //     leftX = 0;
    //   }
    //   leftY = 100;
    //   rightX = (0 - sInertial.rotation(deg)) * 5;
    //   mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
    //   mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
    //   mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
    //   mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);
    //   wait(5, msec);
    // }
    // driveForward(0, 0);
    // // Score
    // output(100, 500);

    // // Back up, turn to eject blue, and turn towards red ball
    // driveForward(-100, 380);
    // wait(100, msec);
    // while (sInertial.rotation(deg) > -200) {
    //   mWheelBackRight.setVelocity(40, pct);
    //   mWheelFrontRight.setVelocity(40, pct);
    //   mWheelBackLeft.setVelocity(0, pct);
    //   mWheelFrontLeft.setVelocity(0, pct);
    // }
    // output(100, 400);

    // while (sInertial.rotation(deg) < -88) {
    //   mWheelBackRight.setVelocity(40, pct);
    //   mWheelFrontRight.setVelocity(40, pct);
    //   mWheelBackLeft.setVelocity(0, pct);
    //   mWheelFrontLeft.setVelocity(0, pct);
    // }
    // // Get ball using gyro and bottom camera
    // while ((Brain.timer(msec) - startTime) < 500) {
    //   sVisionLower.takeSnapshot(
    //       sVisionUpper__SIG_RED); // This needs to be edited so it uses the
    //                               // bottom camera
    //   if (sVisionLower.objectCount > 0) {
    //     leftX = (sVisionLower.largestObject.centerX - 158) * 1;
    //   } else {
    //     leftX = 0;
    //   }
    //   leftY = 100;
    //   rightX = (-88 - sInertial.rotation(deg)) * 5; // Look  into this code
    //   mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
    //   mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
    //   mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
    //   mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);
    //   wait(5, msec);
    // }
    // intakeIn();
    // driveForward(0, 0);
    // mOutputLower.setVelocity(100, pct);
    // wait(300, msec);
    // mOutputLower.setVelocity(0, pct);

    // // Line up with goal
    // while (sInertial.rotation(deg) < -145) {
    //   mWheelBackRight.setVelocity(40, pct);
    //   mWheelFrontRight.setVelocity(40, pct);
    //   mWheelBackLeft.setVelocity(0, pct);
    //   mWheelFrontLeft.setVelocity(0, pct);
    // }
    // // Go to goal using camera and gyro
    // while ((Brain.timer(msec) - startTime) < 700) {
    //   sVisionUpper.takeSnapshot(sVisionUpper__SIG_GREEN);
    //   if (sVisionUpper.objectCount > 0) {
    //     leftX = (sVisionUpper.largestObject.centerX - 158) * 1;
    //   } else {
    //     leftX = 0;
    //   }
    //   leftY = 100;
    //   rightX = (-144 - sInertial.rotation(deg)) * 5; // Look  into this code
    //   mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
    //   mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
    //   mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
    //   mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);
    //   wait(5, msec);
    // }
    // intakeOff();
    // // Score

    // output(100, 400);
    // intakeOpenAuton();
    // driveForward(-100, 400);
    // // TODO: Keep adding goals!

    // */
  }

  // Right 1
  if (mode == 'Y') {
    // Get to the goal
    mWheelFrontLeft.setVelocity(40, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(40, pct);
    mWheelBackRight.setVelocity(0, pct);
    vexDelay(300);
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    // Ensure we are at the goal
    driveForward(100, 1000);

    // Outtake the preload
    output(100, 400); // 500 > 300 timems

    // Drive in reverse to make sure we aren't touching anything in the goal
    driveForward(-20, 500);
  }

  // Left 1
  if (mode == '<') {
    // Get to the goal
    // mWheelFrontLeft.setVelocity(0, pct);
    // mWheelFrontRight.setVelocity(-40, pct);
    // mWheelBackLeft.setVelocity(0, pct);
    // mWheelBackRight.setVelocity(-40, pct);
    // vexDelay(300);
    // mWheelFrontLeft.setVelocity(0, pct);
    // mWheelFrontRight.setVelocity(0, pct);
    // mWheelBackLeft.setVelocity(0, pct);
    // mWheelBackRight.setVelocity(0, pct);

    // // Ensure we are in the goal
    // driveForward(100, 1000);

    // // Outtake the preload
    // output(100, 400); //500 > 300 > 400timems

    // // Drive in reverse
    // driveForward(-20, 500);

    // Deploy
    mOutputUpper.setVelocity(100, pct);
    wait(300, msec);
    mOutputUpper.setVelocity(0, pct);

    intakeIn();

    // Wait to fully deplay
    wait(750, msec);

    // Get into the goal
    mWheelFrontLeft.setVelocity(20, pct);
    mWheelFrontRight.setVelocity(-40, pct);
    mWheelBackLeft.setVelocity(20, pct);
    mWheelBackRight.setVelocity(-40, pct);
    vexDelay(500);
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    // Score preload, score the red ball off the ground, and descore the blues
    output(100, 2000);

    // Fully deploy
    driveForward(-75, 500);

    intakeIn();
    wait(500, msec);

    driveForward(100, 1000);

    output(100, 2000);
    intakeOff();
    intakeOpen();
    wait(100, msec);
    intakeIn();
    prepOutput(100, 400);
    driveForward(-100, 500);
    intakeOff();
  }

  // Right 2
  if (mode == 'X') {
    // Get to the goal
    mWheelFrontLeft.setVelocity(40, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(40, pct);
    mWheelBackRight.setVelocity(0, pct);
    vexDelay(300);
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);
    // Outtake the preload
    output(100, 400); // 500 > 300 timems
    // Drive in reverse
    driveForward(-20, 200);
    // Turn 45 deg ccw
    sInertial.resetRotation();
    while (sInertial.rotation(deg) < 40) {
      mWheelBackRight.setVelocity(40, pct);
      mWheelFrontRight.setVelocity(40, pct);
    }
    mWheelBackLeft.setVelocity(0, pct);
    mWheelFrontLeft.setVelocity(0, pct);

    intakeIn();

    // Line track until we reach the perpendicular line
    while (sOpticalFront.getRgb().clear < OPTICAL_THRESHOLD) {
      // Calculate new moving average for the front optical
      // Assign movement variables
      leftX = 25;
      leftY;
      rightX = 0;
      if (sOpticalBack.getRgb().clear > OPTICAL_THRESHOLD)
        leftY = 30;
      else
        leftY = -10;
      // Assign wheel speeds
      mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
      mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
      mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
      mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);
    }
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    // Drive to the left a bit
    leftX = -25;
    leftY = 0;
    rightX = 0;
    mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
    mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
    mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
    mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);
    vexDelay(300); // 500
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    intakeOpen();
    vexDelay(500);

    // Thrust into the goal
    driveForward(100, 1000);

    // Turn intake off

    // Spit out the ball we have
    output(100, 600); // 500 > 300 timems

    mWheelFrontLeft.setVelocity(-50, pct);
    mWheelFrontRight.setVelocity(50, pct);
    mWheelBackLeft.setVelocity(-50, pct);
    mWheelBackRight.setVelocity(50, pct);

    vexDelay(1000);

    intakeOff();

    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);
  }

  // Left 2
  if (mode == '^') {
    // Get to the goal
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(-40, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(-40, pct);
    vexDelay(300);
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);
    // Outtake the preload
    output(100, 400); // 500 > 300 timems
    // Drive in reverse
    driveForward(-20, 200);
    // Turn 45 deg ccw
    sInertial.resetRotation();
    while (sInertial.rotation(deg) > -40) {
      mWheelBackLeft.setVelocity(-40, pct);
      mWheelFrontLeft.setVelocity(-40, pct);
    }
    mWheelBackLeft.setVelocity(0, pct);
    mWheelFrontLeft.setVelocity(0, pct);

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
      mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
      mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
      mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
      mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);
    }
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    // Drive to the left a bit
    leftX = -25;
    leftY = 0;
    rightX = 0;
    mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
    mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
    mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
    mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);
    vexDelay(300); // 500
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    intakeOpen();
    vexDelay(500);

    // Thrust into the goal
    driveForward(100, 1000);

    // Turn intake off

    // Spit out the ball we have
    output(100, 600); // 500 > 300 timems

    mWheelFrontLeft.setVelocity(-50, pct);
    mWheelFrontRight.setVelocity(50, pct);
    mWheelBackLeft.setVelocity(-50, pct);
    mWheelBackRight.setVelocity(50, pct);

    vexDelay(1000);

    intakeOff();

    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);
  }

  // Right 3
  if (mode == 'A') {
    // Get to the goal
    mWheelFrontLeft.setVelocity(40, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(40, pct);
    mWheelBackRight.setVelocity(0, pct);
    vexDelay(300);
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);
    // Outtake the preload
    output(100, 400); // 500 > 300 timems
    // Drive in reverse
    driveForward(-20, 200);
    // Turn 45 deg ccw
    sInertial.resetRotation();
    while (sInertial.rotation(deg) < 40) {
      mWheelBackRight.setVelocity(40, pct);
      mWheelFrontRight.setVelocity(40, pct);
    }
    mWheelBackLeft.setVelocity(0, pct);
    mWheelFrontLeft.setVelocity(0, pct);

    intakeIn();

    // Line track until we reach the perpendicular line
    while (sOpticalFront.getRgb().clear < OPTICAL_THRESHOLD) {
      // Calculate new moving average for the front optical
      // Assign movement variables
      leftX = 25;
      leftY;
      rightX = 0;
      if (sOpticalBack.getRgb().clear > OPTICAL_THRESHOLD)
        leftY = 30;
      else
        leftY = -10;
      // Assign wheel speeds
      mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
      mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
      mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
      mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);
    }
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    // Drive to the left a bit
    leftX = -25;
    leftY = 0;
    rightX = 0;
    mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
    mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
    mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
    mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);
    vexDelay(300); // 500
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    // Thrust into the goal
    driveForward(100, 1000);

    // Turn intake off

    // Spit out the ball we have
    output(100, 600); // 500 > 300 timems

    mWheelFrontLeft.setVelocity(-50, pct);
    mWheelFrontRight.setVelocity(50, pct);
    mWheelBackLeft.setVelocity(-50, pct);
    mWheelBackRight.setVelocity(50, pct);

    vexDelay(1000);

    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    // Line track until we reach the perpendicular line
    leftX = 100;
    leftY = 0;
    rightX = 0;
    mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
    mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
    mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
    mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);

    waitUntil(sDistanceRight.objectDistance(distanceUnits::in) < 4);

    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    driveForward(100, 1000);

    output(100, 1000);

    intakeOpen();

    driveForward(-100, 500);

    intakeOff();
  }

  // Left 3
  if (mode == '>') {
    // Get to the goal
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(-40, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(-40, pct);
    vexDelay(300);
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    // Outtake the preload
    output(100, 400); // 500 > 300 timems

    // Drive in reverse
    driveForward(-20, 200);

    // Turn 45 deg ccw
    sInertial.resetRotation();
    while (sInertial.rotation(deg) > -40) {
      mWheelBackLeft.setVelocity(-40, pct);
      mWheelFrontLeft.setVelocity(-40, pct);
    }
    mWheelBackLeft.setVelocity(0, pct);
    mWheelFrontLeft.setVelocity(0, pct);

    // Deploy the intakes
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
      mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
      mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
      mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
      mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);
    }
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    // Open the intakes
    intakeOpen();

    // Drive to the left a bit to align with the goal
    leftX = -25;
    leftY = 0;
    rightX = 0;
    mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
    mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
    mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
    mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);
    vexDelay(300); // 500
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    // Thrust into the goal
    driveForward(100, 1000);

    // Spit out the ball we have
    output(100, 600); // 500 > 300 timems

    // Back up
    mWheelFrontLeft.setVelocity(-50, pct);
    mWheelFrontRight.setVelocity(50, pct);
    mWheelBackLeft.setVelocity(-50, pct);
    mWheelBackRight.setVelocity(50, pct);

    vexDelay(1000);

    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    // Line track until we reach the wall
    leftX = -100;
    leftY = 0;
    rightX = 0;
    mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
    mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
    mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
    mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);

    waitUntil(sDistanceLeft.objectDistance(distanceUnits::in) < 4);

    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    // Open the intakes
    intakeOpen();

    // Gun it into the goal
    driveForward(100, 1000);

    // Spit the ball out
    output(100, 1000);

    // Get away from the goal
    driveForward(-100, 500);

    // Relax the intakes
    intakeOff();
  }

  // Online tournament 15 seconds
  if (mode == 'S') {
    // Get to the goal
    sInertial.setRotation(-107, deg);
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(-40, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(-40, pct);
    vexDelay(500);
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);
    // Score the preload
    output(100, 500); // 500 > 300 timems
    // Drive in reverse
    driveForward(-20, 200);
    // Turn 45 deg ccw
    while (sInertial.rotation(deg) > -175) {
      mWheelFrontLeft.setVelocity(-20, pct);
      mWheelBackLeft.setVelocity(-20, pct);
    }
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

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
      mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
      mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
      mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
      mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);
    }
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    intakeOpen();

    // Drive to the left a bit
    leftX = -25;
    leftY = 0;
    rightX = 0;
    mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
    mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
    mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
    mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);
    vexDelay(200); // 500 > 300
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

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
      mWheelFrontLeft.setVelocity(rotSpeed + ySpeed, pct);
      mWheelFrontRight.setVelocity(rotSpeed - ySpeed, pct);
      mWheelBackLeft.setVelocity(rotSpeed + ySpeed, pct);
      mWheelBackRight.setVelocity(rotSpeed - ySpeed, pct);
    }
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    intakeOff();

    // Turn towards
    mWheelFrontRight.setVelocity(-50, pct);
    mWheelBackRight.setVelocity(-50, pct);
    waitUntil(sInertial.rotation() > -225);

    // Gun it towards the wall
    mWheelFrontLeft.setVelocity(-100, pct);
    mWheelFrontRight.setVelocity(-100, pct);
    mWheelBackLeft.setVelocity(100, pct);
    mWheelBackRight.setVelocity(100, pct);

    intakeOpen();

    vexDelay(2000);

    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    output(100, 1000);

    intakeOpen();

    driveForward(-100, 500);

    intakeOff();
  }
}

void usercontrol(void) {

  // Set everything into motion
  mWheelFrontLeft.spin(fwd);
  mWheelFrontRight.spin(fwd);
  mWheelBackLeft.spin(fwd);
  mWheelBackRight.spin(fwd);
  mIntakeLeft.spin(fwd);
  mIntakeRight.spin(fwd);
  mOutputLower.spin(fwd);
  mOutputUpper.spin(fwd);

  mIntakeLeft.setStopping(hold);
  mIntakeRight.setStopping(hold);

  // Variables local to usercontrol
  int intakePhase = 0;
  int leftX;
  int leftY;
  int rightX;

  while (true) {

    // Drivetrain
    // /////////////////////////////////////////////////////////////////////////

    // Controller joystick values
    leftX = Controller1.Axis4.position();
    leftY = Controller1.Axis3.position();
    rightX = Controller1.Axis1.position();

    // Zero the values
    if (leftX < 20 && leftX > -20)
      leftX = 0;
    if (leftY < 20 && leftY > -20)
      leftY = 0;
    if (rightX < 20 && rightX > -20)
      rightX = 0;

    // Assign wheel speeds
    mWheelFrontLeft.setVelocity((rightX * 1.5) + leftY + leftX, pct);
    mWheelFrontRight.setVelocity((rightX * 1.5) - leftY + leftX, pct);
    mWheelBackLeft.setVelocity((rightX * 1.5) + leftY - leftX, pct);
    mWheelBackRight.setVelocity((rightX * 1.5) - leftY - leftX, pct);

    // Intake
    // ////////////////////////////////////////////////////////////////////////////////////////////////////

    // Spin out untill it hits
    // Spin out at low torque

    if (!disableIntakes) {
      // If intake out pressed, begin the intake stages process
      if (Controller1.ButtonR2.pressing() && intakePhase == 0) {
        intakePhase = 1;
      }
      // if(intakePhase == 2 && mIntakeLeft.position(deg) > INTAKE_OPEN_TARGET
      // && mIntakeRight.position(deg) > INTAKE_OPEN_TARGET){
      //   intakePhase = 3;
      // }

      // If both arms have hit, move on to stage 2
      if (mIntakeLeft.torque(Nm) > TORQUE_THRESHOLD &&
          mIntakeRight.torque() > TORQUE_THRESHOLD && intakePhase == 1) {
        intakePhase = 2; // 2
      }

      // Set each intake velocity to 100 pct
      if (intakePhase == 1) {
        mIntakeLeft.setVelocity(100, pct);
        mIntakeRight.setVelocity(100, pct);
      }

      // Prepare to move the arms inward to -1 deg
      if (intakePhase == 2) {
        mIntakeLeft.setVelocity(100, pct);
        mIntakeRight.setVelocity(100, pct);
        mIntakeLeft.setMaxTorque(40, pct);
        mIntakeRight.setMaxTorque(40, pct);
        // intakePhase = 3;
      }

      // Move the arms inward -1 deg
      if (intakePhase == 3) {
        // mIntakeLeft.setVelocity(-10, pct);
        // mIntakeRight.setVelocity(-10, pct);
        // if(mIntakeLeft.position(deg) < -1){
        mIntakeLeft.setVelocity(0, pct);
        mIntakeRight.setVelocity(0, pct);
        mIntakeLeft.setStopping(hold);
        mIntakeRight.setStopping(hold);
        intakePhase = 4;
        // }
      }

      // Hold at -1 deg
      if (intakePhase == 4) {
      }

      // // Move the arms inward -1 deg
      // if(intakePhase == 3){
      //     mIntakeLeft.setVelocity(-10, pct);
      //     mIntakeRight.setVelocity(-10, pct);
      //     if(mIntakeLeft.position(deg) < -1){
      //     mIntakeLeft.setVelocity(0, pct);
      //     mIntakeRight.setVelocity(0, pct);
      //     mIntakeLeft.setStopping(hold);
      //     mIntakeRight.setStopping(hold);
      //     intakePhase = 4;
      //   }
      // }
      // Reset motor encoders and set holding to on
      // if(intakePhase == 1){
      //   mIntakeLeft.setPosition(0, deg);
      //   mIntakeRight.setPosition(0, deg);
      //   mIntakeLeft.setStopping(hold);
      //   mIntakeRight.setStopping(hold);
      //   intakePhase = 2;
      // }
      // // Spin to 60 deg and stop
      // if(intakePhase == 2){
      //   if(mIntakeRight.position(deg) < INTAKE_OPEN_TARGET)
      //     mIntakeRight.setVelocity((INTAKE_OPEN_TARGET -
      //     mIntakeLeft.position(deg)) * 4, pct);
      //   else
      //     mIntakeRight.setVelocity(0, pct);
      //   if(mIntakeLeft.position(deg) < INTAKE_OPEN_TARGET)
      //     mIntakeLeft.setVelocity((INTAKE_OPEN_TARGET -
      //     mIntakeLeft.position(deg)) * 4, pct);
      //   else
      //     mIntakeLeft.setVelocity(0, pct);
      // }
      // // The stay put phase
      // if(intakePhase == 3){
      //   mIntakeRight.setVelocity(0, pct);
      //   mIntakeLeft.setVelocity(0, pct);
      // }

      // If we are pressing the inward button, set the intakes to coast, reset
      // the intakePhase, and spin inwards
      if (Controller1.ButtonR1.pressing()) {
        intakePhase = 0;
        mIntakeLeft.setVelocity(-100, pct);
        mIntakeRight.setVelocity(-100, pct);
        mIntakeLeft.setMaxTorque(100, pct);
        mIntakeRight.setMaxTorque(100, pct);
        mIntakeLeft.setStopping(coast);
        mIntakeRight.setStopping(coast);
      }

      // If absolutly nothing is happening, stop
      if (!Controller1.ButtonR1.pressing() &&
          !Controller1.ButtonR2.pressing() && intakePhase == 0) {
        mIntakeLeft.setVelocity(0, pct);
        mIntakeRight.setVelocity(0, pct);
      }

      // Hold at -1 deg
      if (intakePhase == 4) {
      }

      // Reset motor encoders and set holding to on
      // if(intakePhase == 1){
      //   mIntakeLeft.setPosition(0, deg);
      //   mIntakeRight.setPosition(0, deg);
      //   mIntakeLeft.setStopping(hold);
      //   mIntakeRight.setStopping(hold);
      //   intakePhase = 2;
      // }
      // // Spin to 60 deg and stop
      // if(intakePhase == 2){
      //   if(mIntakeRight.position(deg) < INTAKE_OPEN_TARGET)
      //     mIntakeRight.setVelocity((INTAKE_OPEN_TARGET -
      //     mIntakeLeft.position(deg)) * 4, pct);
      //   else
      //     mIntakeRight.setVelocity(0, pct);
      //   if(mIntakeLeft.position(deg) < INTAKE_OPEN_TARGET)
      //     mIntakeLeft.setVelocity((INTAKE_OPEN_TARGET -
      //     mIntakeLeft.position(deg)) * 4, pct);
      //   else
      //     mIntakeLeft.setVelocity(0, pct);
      // }
      // // The stay put phase
      // if(intakePhase == 3){
      //   mIntakeRight.setVelocity(0, pct);
      //   mIntakeLeft.setVelocity(0, pct);
      // }

      // If we are pressing the inward button, set the intakes to coast, reset
      // the intakePhase, and spin inwards
      if (Controller1.ButtonR1.pressing()) {
        intakePhase = 0;
        mIntakeLeft.setVelocity(-100, pct);
        mIntakeRight.setVelocity(-100, pct);
        mIntakeLeft.setStopping(coast);
        mIntakeRight.setStopping(coast);
      }

      // If absolutly nothing is happening, stop
      if (!Controller1.ButtonR1.pressing() &&
          !Controller1.ButtonR2.pressing() && intakePhase == 0) {
        mIntakeLeft.setVelocity(0, pct);
        mIntakeRight.setVelocity(0, pct);
      }
    }
    // Output
    // ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Spin in
    if (Controller1.ButtonL1.pressing()) {
      mOutputLower.setVelocity(100, pct);
      mOutputUpper.setVelocity(100, pct);
      // Spin out
    } else if (Controller1.ButtonL2.pressing()) {
      mOutputLower.setVelocity(-100, pct);
      mOutputUpper.setVelocity(-100, pct);
      // Stop
    } else {
      mOutputLower.setVelocity(0, pct);
      mOutputUpper.setVelocity(0, pct);
    }

    vexDelay(5);
  }
}

int printCameraObjects() {

  wait(100, msec);
  // Local variables
  // float FrontLeftDistance;
  // float FrontRightDistance;
  // float BackLeftDistance;
  // float BackRightDistance;
  float FrontLeftVelocity;
  float FrontRightVelocity;
  float BackLeftVelocity;
  float BackRightVelocity;
  float gyroangle;
  int loopcount = 0;

  while (1 > 0) {

    // Compute distance each wheel has traveled
    // May not need this code in the multitask (instead put the code in the
    // method that uses it)
    // FrontLeftDistance = mWheelFrontLeft.rotation(rotationUnits::raw);
    // FrontRightDistance = mWheelFrontRight.rotation(rotationUnits::raw);
    // BackLeftDistance = mWheelBackLeft.rotation(rotationUnits::raw);
    // BackRightDistance = mWheelBackRight.rotation(rotationUnits::raw);
    // ForwardDistance = FrontLeftDistance - FrontRightDistance + BackLeftDistance - BackRightDistance;
    // TurnDistance =    FrontLeftDistance + FrontRightDistance + BackLeftDistance + BackRightDistance;
    // StrafeDistance =  FrontLeftDistance - FrontRightDistance - BackLeftDistance - BackRightDistance;

    // Compute velocity of each wheel
    // This code is useful for determining if Robot has hit the goal and stopped
    // moving Also useful to calculate the turn velocity to detect the end of a
    // gyro turn
    FrontLeftVelocity = mWheelFrontLeft.velocity(pct);
    FrontRightVelocity = mWheelFrontRight.velocity(pct);
    BackLeftVelocity = mWheelBackLeft.velocity(pct);
    BackRightVelocity = mWheelBackRight.velocity(pct);
    ForwardVelocity = BackLeftVelocity - BackRightVelocity;
    TurnVelocity = FrontLeftVelocity + FrontRightVelocity + BackLeftVelocity + BackRightVelocity;
    StrafeVelocity = FrontLeftVelocity + FrontRightVelocity - BackLeftVelocity - BackRightVelocity;

    // display every 10 iterations (500ms update rate)
    // display doesn't need to be updated faster
    loopcount = loopcount + 1;
    if (loopcount > 49) {
      gyroangle = sInertial.rotation(deg);
      Controller1.Screen.clearScreen();

      Controller1.Screen.setCursor(1, 1);
      Controller1.Screen.print("GY"); // print Gyro Angle
      Controller1.Screen.setCursor(1, 5);
      Controller1.Screen.print(sInertial.rotation(deg));

      Controller1.Screen.setCursor(1, 11);
      Controller1.Screen.print("FV"); // Print Forward Velocity
      Controller1.Screen.setCursor(1, 15);
      Controller1.Screen.print(ForwardVelocity);

      Controller1.Screen.setCursor(2, 1);
      Controller1.Screen.print(" "); // Empty
      Controller1.Screen.setCursor(2, 5);
      Controller1.Screen.print(" ");

      Controller1.Screen.setCursor(2, 11);
      Controller1.Screen.print("TV"); // Print Turn Velocity
      Controller1.Screen.setCursor(2, 15);
      Controller1.Screen.print(TurnVelocity);

      loopcount = 0; // reset loop counter
    }
    wait(10, msec);
  }
} // end of printCameraObjects

int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  task taskPrintCameraObjects(printCameraObjects);

  Competition.test_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
