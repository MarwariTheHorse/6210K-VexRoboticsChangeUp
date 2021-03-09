#include "vex.h"

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
  mIntakeLeft.setVelocity(speed, pct);
  mIntakeRight.setVelocity(speed, pct);
  vexDelay(timems);
  mIntakeLeft.setVelocity(0, pct);
  mIntakeRight.setVelocity(0, pct);
}
void intakeIn(){
  mIntakeLeft.spin(fwd);
  mIntakeRight.spin(fwd);
  mIntakeLeft.setStopping(coast);
  mIntakeRight.setStopping(coast);
  mIntakeLeft.setVelocity(-100, pct);
  mIntakeRight.setVelocity(-100, pct);
}
void intakeOff(){
  mIntakeLeft.setVelocity(0, pct);
  mIntakeRight.setVelocity(0, pct);
}
void intakeOpen(){
  mIntakeLeft.setVelocity(100, pct);
  mIntakeRight.setVelocity(100, pct);
  mIntakeLeft.spin(fwd);
  mIntakeRight.spin(fwd);

  waitUntil(mIntakeLeft.torque(Nm) > TORQUE_THRESHOLD && mIntakeRight.torque() > TORQUE_THRESHOLD);

  mIntakeLeft.setStopping(hold);
  mIntakeRight.setStopping(hold);
  mIntakeLeft.setPosition(0, deg);
  mIntakeRight.setPosition(0, deg);

  mIntakeLeft.setVelocity(-10, pct);
  mIntakeRight.setVelocity(-10, pct);
  waitUntil(mIntakeLeft.position(deg) < -1);
  mIntakeLeft.setVelocity(0, pct);
  mIntakeRight.setVelocity(0, pct);
  mIntakeLeft.setStopping(hold);
  mIntakeRight.setStopping(hold);
}
void intakeOpenAuton(){
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