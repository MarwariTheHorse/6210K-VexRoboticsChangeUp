/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Practically all the members of 6210K                      */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include <string>
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

  // Battery check
  int batteryCapacity = Brain.Battery.capacity();
  if(batteryCapacity < 75){
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("Battery: %d", batteryCapacity);
    Controller1.Screen.setCursor(2, 1);
    Controller1.Screen.print("Press A to continue");
    waitUntil(Controller1.ButtonA.pressing());
  }

  // Calibration
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1, 1);
  Controller1.Screen.print("Calibrate?");
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

  // Full Auton
  if(mode == 'V') fullAuton();
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
