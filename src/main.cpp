/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Practically all the members of 6210K                      */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include "miscmethods.h"
#include "auton-externs.h"

using namespace vex;

// A global instance of competition
competition Competition;

// Other global variables
char mode = 'N';
bool disableIntakes = false;
// these are global variables
float ForwardDistance;
float TurnDistance;
float StrafeDistance;
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

// Used for turning when we don't need accuracy
void turnFast(double angle){
  double error = angle - sInertial.rotation(deg);
  int leftY = 0;
  int rightX = 0;
  int leftX = 0;
  while(fabs(error) > 10) // keeps turning until within 10 degrees of objective
  {
    if (fabs(error) < 40){
      // if within 40 degrees of objective, the motors start slowing
      // and the speed never drops below 20
      rightX = (2 * error);
    } else {
      // otherwise maintain fast turning speed of 90
      rightX = 90 * sgn(error);
    }
    mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
    mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
    mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
    mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);
    wait(5, msec);
    error = angle - sInertial.rotation(deg);
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
}

// Used for turning with accuracy
void turnTo(double angle, int accuracy){
  double error = angle - sInertial.rotation(deg);
  int leftY = 0;
  int rightX = 0;
  int leftX = 0;
  error = (angle - sInertial.rotation(deg));
  // keep turning until within 10 degrees of objective
  // keep adjusting until the robot's velocity slows
  while(fabs(error) > accuracy || fabs(TurnVelocity) > 80) 
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
    error = angle - sInertial.rotation(deg);
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
}

// Self explanatory
void driveViaDistanceGyro(double dist, double a){
  int leftY = 0;
  int rightX = 0;
  int leftX = 0;
  // reset all motor encoders to zero
  // 10000 units is equal to 56" of travel
  mWheelFrontLeft.resetRotation();
  mWheelBackLeft.resetRotation();
  mWheelFrontRight.resetRotation();
  mWheelBackRight.resetRotation();
  int d = 0;
  if(d < dist){
    while (d < dist){
      leftY = 80;
      rightX = (a - sInertial.rotation(deg)) * 3;
      leftX = 0;
      mWheelFrontLeft.spin(fwd, rightX + leftY + leftX, pct);
      mWheelFrontRight.spin(fwd, rightX - leftY + leftX, pct);
      mWheelBackLeft.spin(fwd, rightX + leftY - leftX, pct);
      mWheelBackRight.spin(fwd, rightX - leftY - leftX, pct);
      wait(5, msec);
      d = mWheelFrontLeft.rotation(rotationUnits::raw) - mWheelFrontRight.rotation(rotationUnits::raw) + mWheelBackLeft.rotation(rotationUnits::raw) - mWheelBackRight.rotation(rotationUnits::raw);
    }
  }else{
    while (d > dist){
      leftY = -80;
      rightX = (a - sInertial.rotation(deg)) * 3;
      leftX = 0;
      mWheelFrontLeft.spin(fwd, rightX + leftY + leftX, pct);
      mWheelFrontRight.spin(fwd, rightX - leftY + leftX, pct);
      mWheelBackLeft.spin(fwd, rightX + leftY - leftX, pct);
      mWheelBackRight.spin(fwd, rightX - leftY - leftX, pct);
      wait(5, msec);
      d = mWheelFrontLeft.rotation(rotationUnits::raw) - mWheelFrontRight.rotation(rotationUnits::raw) + mWheelBackLeft.rotation(rotationUnits::raw) - mWheelBackRight.rotation(rotationUnits::raw);
    }
  }
  driveForward(0, 0);
}

// Using lower camera on the red sig
void driveViaDistanceGyroCamera(double dist, double a){
  // reset all motor encoders to zero
  // 10000 units is equal to 56" of travel
  mWheelFrontLeft.resetRotation();
  mWheelBackLeft.resetRotation();
  mWheelFrontRight.resetRotation();
  mWheelBackRight.resetRotation();
  int d = 0;
  double leftY;
  double leftX;
  double rightX;

    leftY = 50;
    while (d < dist){
      rightX = (a - sInertial.rotation(deg)) * 2;
      leftX = 0;
      sVisionLower.takeSnapshot(sigRed);
      // Width > 40
      if (sVisionLower.objectCount > 0 && sVisionLower.largestObject.width > 40) {
        leftX = (sVisionLower.largestObject.centerX - 180) * .6;
      }
      mWheelFrontLeft.spin(fwd, rightX + leftY + leftX, pct);
      mWheelFrontRight.spin(fwd, rightX - leftY + leftX, pct);
      mWheelBackLeft.spin(fwd, rightX + leftY - leftX, pct);
      mWheelBackRight.spin(fwd, rightX - leftY - leftX, pct);
      wait(10, msec);
      d = mWheelFrontLeft.rotation(rotationUnits::raw) - mWheelFrontRight.rotation(rotationUnits::raw) + mWheelBackLeft.rotation(rotationUnits::raw) - mWheelBackRight.rotation(rotationUnits::raw);
    }
  driveForward(0, 0);
}

// Self explanatory
void strafeViaDistanceGyro(double dist, double a){
  int leftY = 0;
  int rightX = 0;
  int leftX = 0;
  int d = 0;
  // reset all motor encoders to zero
  // 10000 units is equal to 56" of travel
  mWheelFrontLeft.resetRotation();
  mWheelBackLeft.resetRotation();
  mWheelFrontRight.resetRotation();
  mWheelBackRight.resetRotation();
  
  d = mWheelFrontLeft.rotation(rotationUnits::raw) + mWheelFrontRight.rotation(rotationUnits::raw) - mWheelBackLeft.rotation(rotationUnits::raw) - mWheelBackRight.rotation(rotationUnits::raw);
  if(d < dist){
    while (d < dist){
      leftY = 0;
      rightX = (a - sInertial.rotation(deg)) * 3;
      leftX = 50;
      mWheelFrontLeft.spin(fwd, rightX + leftY + leftX, pct);
      mWheelFrontRight.spin(fwd, rightX - leftY + leftX, pct);
      mWheelBackLeft.spin(fwd, rightX + leftY - leftX, pct);
      mWheelBackRight.spin(fwd, rightX - leftY - leftX, pct);
      wait(5, msec);
      d = mWheelFrontLeft.rotation(rotationUnits::raw) + mWheelFrontRight.rotation(rotationUnits::raw) - mWheelBackLeft.rotation(rotationUnits::raw) - mWheelBackRight.rotation(rotationUnits::raw);
    }
  }else{
    while (d > dist){
      leftY = -50;
      rightX = (a - sInertial.rotation(deg)) * 3;
      leftX = 0;
      mWheelFrontLeft.spin(fwd, rightX + leftY + leftX, pct);
      mWheelFrontRight.spin(fwd, rightX - leftY + leftX, pct);
      mWheelBackLeft.spin(fwd, rightX + leftY - leftX, pct);
      mWheelBackRight.spin(fwd, rightX - leftY - leftX, pct);
      wait(5, msec);
      d = mWheelFrontLeft.rotation(rotationUnits::raw) - mWheelFrontRight.rotation(rotationUnits::raw) + mWheelBackLeft.rotation(rotationUnits::raw) - mWheelBackRight.rotation(rotationUnits::raw);
    }
  }
  driveForward(0, 0);
}

// Using the signature passed into the method and upper camera
void driveViaTimeGyroCamera(double timeInMS, double a, signature sig){
  // This method drives according to time and corrects with camera (strafe) and gyro (angle)
  // The loop breaks if the robot runs into an obstacle
  double startTime = Brain.timer(msec);
  int leftX;
  int leftY;
  int rightX;
  while (Brain.timer(msec) - startTime < timeInMS){
    sVisionUpper.takeSnapshot(sig);
    if (sVisionUpper.objectCount > 0 && sVisionUpper.largestObject.width > 40) {
      leftX = (sVisionUpper.largestObject.centerX - 180) * .6; // used to be .8
    } else {
      leftX = 0;
    }
    leftY = 50;
    rightX = (a - sInertial.rotation(deg)) * 2; // used to be 3
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
}

// Self explanatory
void driveViaTimeGyro(double timeInMS, double a){
  // This method drives according to time and corrects with camera (strafe) and gyro (angle)
  // The loop breaks if the robot runs into an obstacle
  double startTime = Brain.timer(msec);
  int leftX;
  int leftY;
  int rightX;
  while (Brain.timer(msec) - startTime < timeInMS){
    leftX = 0;
    leftY = 50;
    rightX = (a - sInertial.rotation(deg)) * 2; // used to be 3
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
}

void driveBackwardsViaTimeGyro(double timeInMS, double a){
  // This method drives according to time and corrects with camera (strafe) and gyro (angle)
  // The loop breaks if the robot runs into an obstacle
  double startTime = Brain.timer(msec);
  int leftX;
  int leftY;
  int rightX;
  while (Brain.timer(msec) - startTime < timeInMS){
    leftX = 0;
    leftY = -100;
    rightX = (a - sInertial.rotation(deg)) * 2; // used to be 3
    mWheelFrontLeft.spin(fwd, rightX + leftY + leftX, pct);
    mWheelFrontRight.spin(fwd, rightX - leftY + leftX, pct);
    mWheelBackLeft.spin(fwd, rightX + leftY - leftX, pct);
    mWheelBackRight.spin(fwd, rightX - leftY - leftX, pct);
    if (Brain.timer(msec) - startTime > 750) {
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
  double startTime = Brain.timer(msec);
  double error = 0;
  // Needs tuning:  the accuracy (2 degrees) the velocity indicator (40 units) and gyro multiplier (x 5)
  while(fabs(fabs(a) - fabs(sInertial.rotation(deg))) > 2 || fabs(TurnVelocity) > 40) // uses global variable TurnVelocity
  {
    // Calculate error
    error = (a - sInertial.rotation(deg)) * 3;
    if(error > 90) error = 90;   // cap positive motor power at +90
    if(error < -90) error = -90; // cap negative motor power at -90
    mWheelFrontLeft.spin(fwd, 0, pct);
    mWheelFrontRight.spin(fwd, 0, pct);
    mWheelBackLeft.spin(fwd, error, pct);  // when left and right are same polarity
    mWheelBackRight.spin(fwd, error, pct); // the robot rotates (turns)
    wait(5, msec);
    if(Brain.timer(msec) - startTime > 500) break;
  }
  driveForward(0, 0);
}

void strafeUntilGreen(int speed, double a){
  int leftX;
  int leftY;
  int rightX;
  leftX = speed;
  leftY = 0;
  sVisionUpper.takeSnapshot(sigGreen);
  // looks 80 pixels in advance to accomidate for overshoot
  while(sVisionUpper.objectCount == 0 || fabs(sVisionUpper.largestObject.centerX - 180) > 60 || sVisionUpper.largestObject.width < 40){
    wait(10, msec);
    sVisionUpper.takeSnapshot(sigGreen);
    rightX = (a - sInertial.rotation(deg)) * 2;
    mWheelFrontLeft.spin(fwd, rightX + leftY + leftX, pct);
    mWheelFrontRight.spin(fwd, rightX - leftY + leftX, pct);
    mWheelBackLeft.spin(fwd, rightX + leftY - leftX, pct);
    mWheelBackRight.spin(fwd, rightX - leftY - leftX, pct);

  }
  mWheelBackLeft.spin(fwd, 0, pct);
  mWheelFrontLeft.spin(fwd, 0, pct);
  mWheelBackRight.spin(fwd, 0, pct);
  mWheelFrontRight.spin(fwd, 0, pct);
  //setStopping(brake);
}

void strafeUntilRed(int speed, double a){
  int leftX;
  int leftY;
  int rightX;
  leftX = speed;
  leftY = 0;
  sVisionLower.takeSnapshot(sigRed);
  // looks for red ball within +/-60 pixels of centerline
  // red ball must be 40 pixels in width
  while(sVisionLower.objectCount == 0 || fabs(sVisionLower.largestObject.centerX - 180) > 60 || sVisionLower.largestObject.width < 40){
    wait(10, msec);
    sVisionLower.takeSnapshot(sigRed);
    rightX = (a - sInertial.rotation(deg)) * 2;
    mWheelFrontLeft.spin(fwd, rightX + leftY + leftX, pct);
    mWheelFrontRight.spin(fwd, rightX - leftY + leftX, pct);
    mWheelBackLeft.spin(fwd, rightX + leftY - leftX, pct);
    mWheelBackRight.spin(fwd, rightX - leftY - leftX, pct);

  }
  mWheelBackLeft.spin(fwd, 0, pct);
  mWheelFrontLeft.spin(fwd, 0, pct);
  mWheelBackRight.spin(fwd, 0, pct);
  mWheelFrontRight.spin(fwd, 0, pct);
  //setStopping(brake);
}

void ejectBalls(){
  mOutputLower.startSpinFor(-8, rotationUnits::rev, 100, velocityUnits::pct);
  mOutputUpper.spinFor(-8, rotationUnits::rev, 100, velocityUnits::pct);
}

void waitForRed(){
  double startTime = Brain.timer(msec);
  while(sVisionUpper.largestObject.width < 100 && Brain.timer(msec) - startTime < 1000){
    wait(10, msec);
    sVisionUpper.takeSnapshot(sigRed);
  }
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

  mWheelFrontLeft.spin(fwd, 0, pct);
  mWheelFrontRight.spin(fwd, 0, pct);
  mWheelBackLeft.spin(fwd, 0, pct);
  mWheelBackRight.spin(fwd, 0, pct);
  mIntakeLeft.spin(fwd, 0, pct);
  mIntakeRight.spin(fwd, 0, pct);
  mOutputLower.spin(fwd, 0, pct);
  mOutputUpper.spin(fwd, 0, pct);
  mOutputLower.setBrake(hold);
  mOutputUpper.setBrake(hold);

  int leftX;
  int leftY;
  int rightX;
  double startTime;
  double error;

  //         XXXXXXXXXXXXXX
  //      XXXXXXXXXXXXXXXXXXXX
  //    XXXXXXXXXXXXXXXXXXXXXXXX
  //    XXXXXXX          XXXXXXX
  //    XXXXXXX          XXXXXXX
  //    XXXXXXX          XXXXXXX
  //    XXXXXXXXXXXXXXXXXXXXXXXX
  //    XXXXXXXXXXXXXXXXXXXXXXXX
  //    XXXXXXXXXXXXXXXXXXXXXXXX
  //    XXXXXXX          XXXXXXX
  //    XXXXXXX          XXXXXXX    
  //    XXXXXXX          XXXXXXX
  //    XXXXXXX          XXXXXXX
  //    XXXXXXX          XXXXXXX

  // STATE AUTONOMOUS
  if (mode == 'V') {
    // -----------------------------------------------------------------TEST CODE
    // sInertial.setRotation(0, deg);
    // wait(1000,msec);
    // turnFast(90);       // turn clockwise
    // wait(5000,msec);    // look at controller display and see what actual angle is recorded
    // turnTo(180, 5);     // turn clockwise to 180
    // wait(5000,msec);    // look at controller display and see what actual angle is recorded
    // turnFast(90);       // turn counterclockwise
    // wait(5000,msec);    // look at controller display and see what actual angle is recorded
    // turnTo(180, 5);     // turn counterclockwise to 0
    // wait(5000,msec);    // look at controller display and see what actual angle is recorded
    // -----------------------------------------------------------------TEST CODE


     //-----------------------------------------------------------------CODE COMMENTED OUT BELOW
    sInertial.setRotation(-57, deg);

    // PART 1 - Deploy Camera and Hood and flick ball into goal
    mOutputUpper.spin(fwd, 100, pct);
    wait(300, msec);
    mOutputUpper.spin(fwd, 0, pct);


  ///////////////////////////////////////////////////////


    // PART 2 - Get ball 1,
    mOutputLower.spin(fwd, 100, pct);
    driveViaDistanceGyro(6000, -57);

    // Step 2 - Deploy Arms
    intakeIn();
    wait(1000, msec);
    intakeOff();

    // Prepare for strafe
    turnTo(-180, 2);
    intakeIn();

    // Strafe until we see the goal
    strafeUntilGreen(65, -180);

    // open arms
    intakeOpenAuton();

    // drive into goal
    driveViaTimeGyroCamera(8000, -180, sigGreen);
    //alignToGoal(-180);
    intakeOff();

    //Score and descore
    intakeIn();
    mOutputUpper.spin(fwd, 100, pct);
    mOutputLower.spin(fwd, 100, pct);
    waitForRed();
    mOutputUpper.spin(fwd, 0, pct);


  ///////////////////////////////////////////////////////////
    // PART 3 - Back up and eject blue
    driveViaDistanceGyro(-2000, -180);

    
    turnTo(-225, 2); // turn quickly to eject blue

    intakeOpenAuton();
    ejectBalls();
    intakeIn();

    turnTo(-270, 2);
    strafeViaDistanceGyro(400,-270);
    turnTo(-360, 2); // face center goal
    //strafeViaDistanceGyro(700, -360);

    // Drive towards center goal and scoop red ball along the way
    intakeOpenAuton();
    mOutputLower.spin(fwd,100, pct);
    driveViaDistanceGyroCamera(5000, -360);
    driveViaTimeGyroCamera(8000, -360, sigBlue);
    // Strike center goal and align
    alignToGoal(-360);
    // Descore first blue from center goal
    intakeIn();
    // Eject red and score into center goal
    mOutputUpper.spin(fwd,100, pct);
    mOutputLower.spin(fwd,70, pct);
    // Watch for red ball going into goal
    waitForRed();
    // Stop upper output scoring wheel
    mOutputUpper.spin(fwd,0, pct);
    wait(400, msec);
    intakeOpenAuton();
    wait(100, msec);
    // Intake second blue
    intakeIn();
    mOutputLower.spin(fwd,80, pct);
    wait(600, msec);
    
    intakeOpenAuton();
    
    // Back away from center goal
    driveViaDistanceGyro(-1700, -360);
    // Turn towards goal 1 to eject balls
    turnTo(-205, 2);
    ejectBalls();
    // Close intake arms
    mIntakeLeft.startSpinFor(-3, rotationUnits::rev, 90, velocityUnits::pct);
    mIntakeRight.startSpinFor(-3, rotationUnits::rev, 90, velocityUnits::pct);

    /////////////////////////////////////////////////////////////////////////
    // PART 4 - goal 4
    // Get red ball
    turnTo(-135, 2); // did trig to find this
    intakeOpenAuton();
    mOutputLower.spin(fwd,80, pct);
    driveViaDistanceGyroCamera(7500, -135);
    intakeIn();

    // Go to goal
    turnTo(-135, 2); //did trig to find this
    strafeViaDistanceGyro(1500, -135);
    driveViaTimeGyroCamera(8000, -135, sigGreen);
    //alignToGoal(-135);

    // Goal 4 score red and descore blue
    mOutputUpper.spin(fwd,100, pct);
    mOutputLower.spin(fwd,80, pct);
    wait(500, msec);
    waitForRed();
    mOutputUpper.spin(fwd,0, pct);
    


  ///////////////////////////////////////////////////////////////////////////////////////////////////////


    // Part 5 - Back out of the goal

    intakeIn();
    driveViaDistanceGyro(-2500, -135);
    intakeOpenAuton();
    driveViaDistanceGyro(-2300, -135);

    // Turn to face the wall
    turnTo(-225, 2);
    // Eject Blue 
    ejectBalls();
    // Turn to red
    wait(300, msec);
    turnTo(-90, 2);
    
    // Get the red ball
    intakeOpenWall();
    mOutputLower.spin(fwd,80, pct);
    driveViaTimeGyro(3000, -90);
    intakeIn();
    wait(500, msec); // Added the in to give the intakes time to back out and almost grab the ball

    // Scoot back
    driveViaDistanceGyro(-3000, -90);
    

    // Drive to goal
    turnTo(0, 2);
    mOutputLower.spin(fwd, 0, pct);
    // open arms
    intakeOpenAuton();
    wait(50, msec);

    // drive to goal and grab second red along the way
    driveViaDistanceGyroCamera(6700, 0);
    intakeIn();
    turnTo(-90, 2);
    intakeOpenAuton();
    driveViaTimeGyroCamera(8000, -90, sigGreen);
    //alignToGoal(-90);

    //Score goal 5
    mOutputUpper.spin(fwd, 100, pct);
    mOutputLower.spin(fwd, 100, pct);
    waitForRed();
    mOutputUpper.spin(fwd, -100, pct);
    wait(200, msec);
    mOutputUpper.spin(fwd, 0, pct);
  

  ////////////////////////////////////////////////////////////////////////


    // PART 6 - Back up and eject blue
    driveViaDistanceGyro(-1700, -90);

    intakeIn();

    // Strafe to the next red ball
    strafeViaDistanceGyro(7000, -90);
    strafeUntilRed(50, -90);
    
    // Get the red ball
    intakeOpenWall();
    mOutputLower.spin(fwd,80, pct);
    driveViaTimeGyro(3000, -90);
    intakeIn();
    wait(500, msec); // Added the in to give the intakes time to back out and almost grab the ball

    // Scoot back
    driveViaDistanceGyro(-2000, -90);

    // Rotate toward goal 6
    turnTo(-45, 2);

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  // Part 7 - Goals 6 and 7
    // Score goal 6
    intakeIn();
    strafeViaDistanceGyro(1250, -45);
    driveViaTimeGyroCamera(8000, -45, sigGreen);
    //alignToGoal(-45);
    mOutputUpper.spin(fwd,100, pct);
    mOutputLower.spin(fwd,100, pct);
    waitForRed();
    mOutputUpper.spin(fwd,-100, pct);
    wait(200, msec);
    mOutputUpper.spin(fwd, 0, pct);
    intakeOff();
    driveViaDistanceGyro(-3000, -45);
    turnTo(0, 2);
    
    // Get Red
    strafeUntilGreen(65, 0);
    intakeOpenAuton();
    driveViaTimeGyroCamera(8000, 0, sigGreen);
    intakeIn();
    mOutputLower.spin(fwd, 100, pct);
    mOutputUpper.spin(fwd, 100, pct);
    waitForRed();
    mOutputUpper.spin(fwd, 0, pct);
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Goal 8 score corner

    driveViaDistanceGyro(-3500, 0);

    // Needs tuning:  the accuracy (2 degrees) the velocity indicator (40 units) and gyro multiplier (x 5)
    turnTo(-90, 2);

    //driveViaDistanceGyro(1000, -360);
    intakeOpenAuton();
    wait(200, msec);
    ejectBalls();
    turnTo(45, 2);
    
    // Get red ball

    mOutputLower.spin(fwd,80, pct);
    strafeViaDistanceGyro(1000, 45);
    driveViaDistanceGyroCamera(6500, 45);
    intakeIn();

    // Go to goal
    turnTo(45, 2); //did trig to find this
    strafeViaDistanceGyro(750, 45);
    driveViaTimeGyroCamera(8000, 45, sigGreen);
    //alignToGoal(-315);

    // Score goal 8
    mOutputUpper.spin(fwd,100, pct);
    mOutputLower.spin(fwd,100, pct);
    waitForRed();
    mOutputUpper.spin(fwd,0, pct);
    mOutputLower.spin(fwd,0, pct);
    intakeOff();
    // Back out
    driveViaDistanceGyro(-6000, 45);
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // PART 8 - Goal nine followed by CELEBRATION!
    turnTo(0, 2);
    intakeOpenAuton();
    ejectBalls();
    turnTo(90, 2);
    // Get the red ball
    intakeOpenWall();
    mOutputLower.spin(fwd,90, pct);
    driveViaTimeGyro(3000, 90);
    intakeIn();
    wait(500, msec); // Added the in to give the intakes time to back out and almost grab the ball

    // Scoot back
    driveViaDistanceGyro(-3000, 90);
    

    // Drive to goal
    turnTo(180, 2);
    mOutputLower.spin(fwd, 0, pct);
    // open arms
    intakeOpenAuton();
    wait(50, msec);

    // drive to goal and grab second red along the way
    driveViaDistanceGyroCamera(6500, 180);
    intakeIn();
    turnTo(90, 2);
    intakeOpenAuton();
    driveViaTimeGyroCamera(8000, 90, sigGreen);
    //alignToGoal(-90);

    //Score goal 9
    intakeIn();
    mOutputUpper.spin(fwd, 100, pct);
    mOutputLower.spin(fwd, 100, pct);
    wait(800, msec);
    waitForRed();
    mOutputUpper.spin(fwd, 0, pct);
    
    // Gun backwards
    driveBackwardsViaTimeGyro(3000, 90);

    while(1){
      // forever loop so the controller continues to display data
      wait(100,msec);
    }
  }

  //    XXXXXXXXXXXXXXXXXXXXX
  //    XXXXXXXXXXXXXXXXXXXXX
  //    XXXXXXXXXXXXXXXXXXXXX
  //           XXXXXXX
  //           XXXXXXX
  //           XXXXXXX
  //           XXXXXXX
  //           XXXXXXX
  //           XXXXXXX
  //           XXXXXXX
  //           XXXXXXX  
  //           XXXXXXX
  //           XXXXXXX
  //           XXXXXXX

  // TOURNAMENT AUTONOMOUS
  // Right 1
  if (mode == 'Y') oneGoal(RIGHT);
  // Left 1
  if (mode == '<') oneGoal(LEFT);
  // Right 2
  if (mode == 'X') twoGoal(RIGHT);
  // Left 2
  if (mode == '^') twoGoal(LEFT);
  // Right 3
  if (mode == 'A') threeGoal(RIGHT);
  // Left 3
  if (mode == '>') threeGoal(LEFT);
  // Online tournament 15 seconds
  if (mode == 'S') specialAuton();
}

//    XXXXXXXXXXXXXXXXXXX
//    XXXXXXXXXXXXXXXXXXXX
//    XXXXXXX        XXXXXXX
//    XXXXXXX         XXXXXXX
//    XXXXXXX          XXXXXXX
//    XXXXXXX          XXXXXXX
//    XXXXXXX          XXXXXXX
//    XXXXXXX          XXXXXXX
//    XXXXXXX          XXXXXXX
//    XXXXXXX          XXXXXXX
//    XXXXXXX         XXXXXXX    
//    XXXXXXX        XXXXXXX
//    XXXXXXXXXXXXXXXXXXXX
//    XXXXXXXXXXXXXXXXXXX

// DRIVER MODE
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

    // Zero the values (Likely not needed if we use the cubing function)
    if (leftX < 20 && leftX > -20)
      leftX = 0;
    if (leftY < 20 && leftY > -20)
      leftY = 0;
    if (rightX < 20 && rightX > -20)
      rightX = 0;

    // Assign wheel speeds
    mWheelFrontLeft.spin(fwd, (rightX * 1) + leftY + leftX, pct);
    mWheelFrontRight.spin(fwd, (rightX * 1) - leftY + leftX, pct);
    mWheelBackLeft.spin(fwd, (rightX * 1) + leftY - leftX, pct);
    mWheelBackRight.spin(fwd, (rightX * 1) - leftY - leftX, pct);

    // Intake
    // ////////////////////////////////////////////////////////////////////////////////////////////////////

    if (!disableIntakes) {

      if(Controller1.ButtonDown.pressing()){
      autonomous();
    }

      // ButtonR2 > Begin opening the intakes
      if (Controller1.ButtonR2.pressing() && intakePhase == 0) {
        intakePhase = 1;
      }

      // ButtonR1 > Spin intakes inward
      if (Controller1.ButtonR1.pressing()) {
        intakePhase = 0;
        mIntakeLeft.spin(fwd, -100, pct);
        mIntakeRight.spin(fwd, -100, pct);
        mIntakeLeft.setMaxTorque(100, pct);
        mIntakeRight.setMaxTorque(100, pct);
        mIntakeLeft.setStopping(coast);
        mIntakeRight.setStopping(coast);
      }

      // If absolutly nothing is happening, stop
      if (!Controller1.ButtonR1.pressing() &&
          !Controller1.ButtonR2.pressing() && intakePhase == 0) {
        mIntakeLeft.spin(fwd, 0, pct);
        mIntakeRight.spin(fwd, 0, pct);
      }

      // Phase 1: Spin to the hardstops
      if (intakePhase == 1) {
        mIntakeLeft.spin(fwd, 100, pct);
        mIntakeRight.spin(fwd, 100, pct);
      }

      // Both arms hit > Phase 2
      if (mIntakeLeft.torque(Nm) > TORQUE_THRESHOLD &&
          mIntakeRight.torque() > TORQUE_THRESHOLD && intakePhase == 1) {
        intakePhase = 2;
      }

      // Phase 2: Continue to press against the hard stops, but relax the torque a bit
      if (intakePhase == 2) {
        mIntakeLeft.spin(fwd, 100, pct);
        mIntakeRight.spin(fwd, 100, pct);
        mIntakeLeft.setMaxTorque(40, pct);
        mIntakeRight.setMaxTorque(40, pct);
      }
    }

    // Output
    // ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Spin in
    if (Controller1.ButtonL1.pressing()) {
      mOutputLower.spin(fwd, 100, pct);
      mOutputUpper.spin(fwd, 100, pct);
      // Spin out
    } else if (Controller1.ButtonL2.pressing()) {
      mOutputLower.spin(fwd, -100, pct);
      mOutputUpper.spin(fwd, -100, pct);
      // Stop
    } else {
      mOutputLower.spin(fwd, 0, pct);
      mOutputUpper.spin(fwd, 0, pct);
    }


    vexDelay(5);
  }
}

//    XXXXXXXXXXX          XXXXXXXXXXX
//    XXXXXXXXXXXX        XXXXXXXXXXXX
//    XXXXXXXXXXXXX      XXXXXXXXXXXXX
//    XXXXXXXXXXXXXX    XXXXXXXXXXXXXX
//    XXXXXXX   XXXXX  XXXXX   XXXXXXX
//    XXXXXXX   XXXXXXXXXXXX   XXXXXXX
//    XXXXXXX    XXXXXXXXXX    XXXXXXX
//    XXXXXXX     XXXXXXXX     XXXXXXX
//    XXXXXXX      XXXXXX      XXXXXXX
//    XXXXXXX                  XXXXXXX
//    XXXXXXX                  XXXXXXX
//    XXXXXXX                  XXXXXXX
//    XXXXXXX                  XXXXXXX

// MAIN program and MULTI-TASK METHODS
int computeGlobals() {
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
while (1 > 0) {
    // Compute distance each wheel has traveled
    // May not need this code in the multitask (instead put the code in the
    // method that uses it)
    float FrontLeftDistance = mWheelFrontLeft.rotation(rotationUnits::raw);
    float FrontRightDistance = mWheelFrontRight.rotation(rotationUnits::raw);
    float BackLeftDistance = mWheelBackLeft.rotation(rotationUnits::raw);
    float BackRightDistance = mWheelBackRight.rotation(rotationUnits::raw);
    ForwardDistance = FrontLeftDistance - FrontRightDistance + BackLeftDistance - BackRightDistance;
    TurnDistance =    FrontLeftDistance + FrontRightDistance + BackLeftDistance + BackRightDistance;
    StrafeDistance =  FrontLeftDistance - FrontRightDistance - BackLeftDistance - BackRightDistance;

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

    wait(5, msec);
  }
} // end of computerMotorParameters

int printInfo() {
  wait(100, msec);
  while (1 > 0) {
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

    Controller1.Screen.setCursor(3, 1);
    Controller1.Screen.print("VUX "); // Vision Upper X-Axis
    Controller1.Screen.setCursor(3, 5);
    Controller1.Screen.print(sVisionUpper.largestObject.centerX - 180);

    Controller1.Screen.setCursor(3, 11);
    Controller1.Screen.print("VLX"); // Vision Lower X-Axis
    Controller1.Screen.setCursor(3, 15);
    Controller1.Screen.print(sVisionLower.largestObject.centerX - 180);

    wait(250, msec);
  }
} // end of printCameraObjects

int main() {
  // Mark the start of the program
  //sdLog("[PROGRAM START](" +  + ")");

  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  task taskPrintCameraObjects(printInfo);
  task taskComputeMotorParameters(computeGlobals);

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
