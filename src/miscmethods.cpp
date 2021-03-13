#include "vex.h"
#include "miscmethods.h"

#include <cstdio>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>

using namespace vex;
using namespace std;

ofstream ofs;
stringstream ss;

// Functions
/** Drives fwd a certain speed and then stops
 *  @speed The Speed of the robot
 *  @time How long to perform the movement
 */
void driveForward(int speed, int timems){
  mWheelFrontLeft.setVelocity(speed, pct);
  mWheelFrontRight.setVelocity(-speed, pct);
  mWheelBackLeft.setVelocity(speed, pct);
  mWheelBackRight.setVelocity(-speed, pct);
  vexDelay(timems);
  mWheelFrontLeft.setVelocity(0, pct);
  mWheelFrontRight.setVelocity(0, pct);
  mWheelBackLeft.setVelocity(0, pct);
  mWheelBackRight.setVelocity(0, pct);
}
int sgn(double d){
  return ((d > 0) - (d < 0));
}
void intake(int speed, int timems){
  mIntakeLeft.setMaxTorque(100, pct);
  mIntakeRight.setMaxTorque(100, pct);

  mIntakeLeft.setVelocity(speed, pct);
  mIntakeRight.setVelocity(speed, pct);
  vexDelay(timems);
  mIntakeLeft.setVelocity(0, pct);
  mIntakeRight.setVelocity(0, pct);
}
void intakeIn(){
  mIntakeLeft.setMaxTorque(100, pct);
  mIntakeRight.setMaxTorque(100, pct);
  mIntakeLeft.spin(fwd);
  mIntakeRight.spin(fwd);
  mIntakeLeft.setStopping(coast);
  mIntakeRight.setStopping(coast);
  mIntakeLeft.setVelocity(-100, pct);
  mIntakeRight.setVelocity(-100, pct);
}
void intakeOff(){
  mIntakeLeft.setMaxTorque(100, pct);
  mIntakeRight.setMaxTorque(100, pct);
  mIntakeLeft.setVelocity(0, pct);
  mIntakeRight.setVelocity(0, pct);
}
void intakeOpen(){
  mIntakeLeft.setMaxTorque(100, pct);
  mIntakeRight.setMaxTorque(100, pct);

  mIntakeLeft.spin(fwd, 100, pct);
  mIntakeRight.spin(fwd, 100, pct);

  waitUntil(mIntakeLeft.torque(Nm) > TORQUE_THRESHOLD && mIntakeRight.torque() > TORQUE_THRESHOLD);

  mIntakeLeft.spin(fwd, 100, pct);
  mIntakeRight.spin(fwd, 100, pct);
  mIntakeLeft.setMaxTorque(40, pct);
  mIntakeRight.setMaxTorque(40, pct);
}
void intakeOpenAuton(){
  mIntakeLeft.setMaxTorque(100, pct);
  mIntakeRight.setMaxTorque(100, pct);
  mIntakeLeft.setStopping(hold);
  mIntakeRight.setStopping(hold);
  mIntakeLeft.spin(fwd);
  mIntakeRight.spin(fwd);
  wait(5, msec);
  mIntakeLeft.setPosition(0, deg);
  mIntakeRight.setPosition(0, deg);
  wait(5, msec);
  mIntakeLeft.startRotateTo(80, rotationUnits::deg, 50, velocityUnits::pct);
  mIntakeRight.startRotateTo(80, rotationUnits::deg, 50, velocityUnits::pct);
}
void intakeOpenWall(){
  mIntakeLeft.setMaxTorque(100, pct);
  mIntakeRight.setMaxTorque(100, pct);
  mIntakeLeft.setStopping(hold);
  mIntakeRight.setStopping(hold);
  mIntakeLeft.spin(fwd);
  mIntakeRight.spin(fwd);
  wait(5, msec);
  mIntakeLeft.setPosition(0, deg);
  mIntakeRight.setPosition(0, deg);
  wait(5, msec);
  mIntakeLeft.startRotateTo(140, rotationUnits::deg, 50, velocityUnits::pct);
  mIntakeRight.startRotateTo(140, rotationUnits::deg, 50, velocityUnits::pct);
}
void output(int speed, int timems){
  mOutputLower.spin(fwd);
  mOutputUpper.spin(fwd);
  mOutputLower.setVelocity(speed, pct);
  mOutputUpper.setVelocity(speed, pct);
  vexDelay(timems);
  mOutputUpper.setVelocity(0, pct);
  mOutputLower.setVelocity(0, pct);
}
void outputIn(){
  mOutputLower.spin(fwd);
  mOutputUpper.spin(fwd);
  mOutputLower.setVelocity(100, pct);
  mOutputUpper.setVelocity(100, pct);
}
void outputOff(){
  mOutputLower.spin(fwd);
  mOutputUpper.spin(fwd);
  mOutputLower.setVelocity(0, pct);
  mOutputUpper.setVelocity(0, pct);
}
void strafeRight(int speed, int timems){
  mWheelFrontRight.setVelocity(-speed, pct);
  mWheelFrontLeft.setVelocity(speed, pct);
  mWheelBackRight.setVelocity(speed, pct);
  mWheelBackLeft.setVelocity(-speed, pct);
  vexDelay(timems);
  mWheelFrontRight.setVelocity(0, pct);
  mWheelFrontLeft.setVelocity(0, pct);
  mWheelBackRight.setVelocity(0, pct);
  mWheelBackLeft.setVelocity(0, pct);
}
void strafeLeft(int speed, int timems){
  mWheelFrontRight.setVelocity(speed, pct);
  mWheelFrontLeft.setVelocity(-speed, pct);
  mWheelBackRight.setVelocity(-speed, pct);
  mWheelBackLeft.setVelocity(speed, pct);
  vexDelay(timems);
  mWheelFrontRight.setVelocity(0, pct);
  mWheelFrontLeft.setVelocity(0, pct);
  mWheelBackRight.setVelocity(0, pct);
  mWheelBackLeft.setVelocity(0, pct);
}
void halt(){
  mWheelFrontLeft.setVelocity(0, pct);
  mWheelFrontRight.setVelocity(0, pct);
  mWheelBackLeft.setVelocity(0, pct);
  mWheelBackRight.setVelocity(0, pct);
}
void prepOutput(int speed, int timems){
  mOutputLower.setVelocity(speed, pct);
  vexDelay(timems);
  mOutputLower.setVelocity(speed, pct);
}

double getForwardVelocity(){
  return mWheelBackLeft.rotation(rotationUnits::raw) - mWheelBackRight.rotation(rotationUnits::raw);
}

double getTurnVelocity(){
  return mWheelFrontLeft.rotation(rotationUnits::raw) + mWheelFrontRight.rotation(rotationUnits::raw) + mWheelBackLeft.rotation(rotationUnits::raw) + mWheelBackRight.rotation(rotationUnits::raw);
}

double getStrafeVelocity(){
  return mWheelFrontLeft.rotation(rotationUnits::raw) + mWheelFrontRight.rotation(rotationUnits::raw) - mWheelBackLeft.rotation(rotationUnits::raw) - mWheelBackRight.rotation(rotationUnits::raw);
}

void sdLog(string label, string message){
  if(Brain.SDcard.isInserted() ) {
    // Variables to store our date info
    string y;
    string m;
    string d;

    // Put int year into the converter
    ss<<Date.da_year;
    // Get the string year out of the converter
    ss>>y;

    ss<<Date.da_mon;
    ss>>m;
    ss<<Date.da_day;
    ss>>d;

    // create a file with long filename
    // Types of labels: 
    // [PROGRAM-START] [15-SEC-AUTON-START] [AUTON-START] [DRIVER-START] //TODO: change these to START-_____
    // [INFO] [WARNING] [ERROR] [FATAL]
    // [OVERHEAT] 
    ofs.open("LOG:"+y+"."+m+"."+d, std::ofstream::out);
    ofs << "[" << label << "](" << Time.ti_hour << ":" << Time.ti_min << ":" << Time.ti_sec << "." << Time.ti_hund << ") " << message << "\r\n"; // ie [WARNING](11:25:34.45) Motor ___ has reached overheat temperature
    ofs.close();
  }
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
  while(fabs(error) > accuracy || fabs(getTurnVelocity()) > 80) 
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
      if (getForwardVelocity() < 20) {
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
      if (getForwardVelocity() < 20) {
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
      if (getForwardVelocity() < 20) {
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
  while(fabs(fabs(a) - fabs(sInertial.rotation(deg))) > 2 || fabs(getTurnVelocity()) > 40) // uses global variable TurnVelocity
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

void waitForColor(bool blue){
  sVisionUpper.takeSnapshot(blue ? sigBlue : sigRed);
  double startTime = Brain.timer(msec);
  while(sVisionUpper.largestObject.width < 100 && Brain.timer(msec) - startTime < 1000){
    wait(10, msec);
    sVisionUpper.takeSnapshot(blue ? sigBlue : sigRed);
  }
}

void waitForNoGreen(){
  double startTime = Brain.timer(msec);
  while(sVisionUpper.largestObject.width > 10 && Brain.timer(msec) - startTime < 1000){
    wait(10, msec);
    sVisionUpper.takeSnapshot(sigGreen);
  }
}