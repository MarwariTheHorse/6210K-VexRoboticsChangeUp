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
void intake(int speed, int timems){
  mIntakeLeft.setVelocity(speed, pct);
  mIntakeRight.setVelocity(speed, pct);
  vexDelay(timems);
  mIntakeLeft.setVelocity(0, pct);
  mIntakeRight.setVelocity(0, pct);
}
void intakeIn(){
  mIntakeLeft.setVelocity(-100, pct);
  mIntakeRight.setVelocity(-100, pct);
  mIntakeLeft.setStopping(coast);
  mIntakeRight.setStopping(coast);
}
void intakeOff(){
  mIntakeLeft.setVelocity(0, pct);
  mIntakeRight.setVelocity(0, pct);
}
void intakeOpen(){
  mIntakeLeft.setVelocity(100, pct);
  mIntakeRight.setVelocity(100, pct);

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
void output(int speed, int timems){
  mOutputLower.setVelocity(speed, pct);
  mOutputUpper.setVelocity(speed, pct);
  vexDelay(timems);
  mOutputUpper.setVelocity(0, pct);
  mOutputLower.setVelocity(0, pct);
}
void outputIn(){
  mOutputLower.setVelocity(100, pct);
  mOutputUpper.setVelocity(100, pct);
}
void outputOff(){
  mOutputLower.setVelocity(0, pct);
  mOutputUpper.setVelocity(0, pct);
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