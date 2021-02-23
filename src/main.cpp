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
  while(mode == 'N'){
    if(Controller1.ButtonDown.pressing())
      mode = 'V';
    if(Controller1.ButtonB.pressing())
      break;
    
    if(Controller1.ButtonY.pressing())
      mode = 'Y';
    if(Controller1.ButtonA.pressing())
      mode = 'A';
    if(Controller1.ButtonX.pressing())
      mode = 'X';
    
    if(Controller1.ButtonLeft.pressing())
      mode = '<';
    if(Controller1.ButtonRight.pressing())
      mode = '>';
    if(Controller1.ButtonUp.pressing())
      mode = '^';
    if(Controller1.ButtonR1.pressing()){
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

void scoreFirstCornerGoal(int dir){
  int leftVelocity;
  int rightVelocity;
  if(dir == LEFT){
    leftVelocity = 0;
    rightVelocity = -40;
  }else{
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

void autonomous(void) {
  // output(100, 5000); // Spit the ball into the goal
  // intake(-75, 1000); // Flick the intakes outward
  // driveForward(-50, 1000); // Back out and away from the goal

  // Auton Methods
  // * Gyro
  // * Drive
  //   * Distance
  //   * Camera
  //   * Gyro

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

  int leftX;
  int leftY;
  int rightX;


  // // Gyro Turn
  // while(sInertial.rotation() < 90){ // the number and sign will change along with all the other numbers 
  //   mWheelFrontLeft.setVelocity((90 - sInertial.rotation()) * 1.5, pct);
  //   mWheelFrontRight.setVelocity((90 - sInertial.rotation()) * 1.5, pct);
  //   mWheelBackLeft.setVelocity((90 - sInertial.rotation()) * 1.5, pct);
  //   mWheelBackRight.setVelocity((90 - sInertial.rotation()) * 1.5, pct);
  //   vexDelay(5);
  // }

  // // Drive at the goal using the camera with time limit
  // double startTime = Brain.timer(msec);
  // while(Brain.timer(msec) - startTime < 1500){
  //   int ySpeed;
  //   int xSpeed;
  //   ySpeed = 100;

  //   sVision.takeSnapshot(sVision__SIG_BLUE);

  //   Controller1.Screen.clearScreen();
  //   Controller1.Screen.setCursor(1, 1);
  //   Controller1.Screen.print(sVision.objectCount);

  //   if(sVision.objectCount > 0){
  //     xSpeed = (sVision.largestObject.centerX - 158) * 1;
  //   }else{
  //     xSpeed = 0;
  //   }

  //   mWheelFrontLeft.setVelocity(ySpeed + xSpeed, pct);
  //   mWheelFrontRight.setVelocity(-ySpeed + xSpeed, pct);
  //   mWheelBackLeft.setVelocity(ySpeed - xSpeed, pct);
  //   mWheelBackRight.setVelocity(-ySpeed - xSpeed, pct);
  //   vexDelay(5);
  // }
  // mWheelFrontLeft.setVelocity(0, pct);
  // mWheelFrontRight.setVelocity(0, pct);
  // mWheelBackLeft.setVelocity(0, pct);
  // mWheelBackRight.setVelocity(0, pct);

  // Autonomous
  /*if(mode == 'V'){

    // Open top flap and toss ball into the goal
    output(100, 200);
    // Calibrate Gyro
    sInertial.setRotation(33, deg);
    // Run output
    outputIn();
    // Drive forward and grab the first ball.
    double startTime = Brain.timer(msec);
    mWheelFrontLeft.setVelocity(100, pct);
    mWheelFrontRight.setVelocity(-100, pct);
    mWheelBackLeft.setVelocity(100, pct);
    mWheelBackRight.setVelocity(-100, pct);
    while(Brain.timer(msec) - startTime < 800){
      vexDelay(5);
    }
    outputOff();
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    // Deploy the intakes
    intakeIn();
    vexDelay(1000);
    intakeOff();

    // Drive forward and grab the first ball.
    startTime = Brain.timer(msec);
    mWheelFrontLeft.setVelocity(100, pct);
    mWheelFrontRight.setVelocity(-100, pct);
    mWheelBackLeft.setVelocity(100, pct);
    mWheelBackRight.setVelocity(-100, pct);

    // Drive for 800 ms at 0 degrees
    while(Brain.timer(msec) - startTime < 800){
      mWheelFrontLeft.setVelocity(100 - (sInertial.rotation() * 3), pct);
      mWheelFrontRight.setVelocity(-100 - (sInertial.rotation() * 3), pct);
      mWheelBackLeft.setVelocity(100 - (sInertial.rotation() * 3), pct);
      mWheelBackRight.setVelocity(-100 - (sInertial.rotation() * 3), pct);
      vexDelay(5);
    }
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    // Rotate towards the middle goal
    while(sInertial.rotation() < 89){
      mWheelFrontLeft.setVelocity((90 - sInertial.rotation()) * 1.5, pct);
      mWheelFrontRight.setVelocity((90 - sInertial.rotation()) * 1.5, pct);
      mWheelBackLeft.setVelocity((90 - sInertial.rotation()) * 1.5, pct);
      mWheelBackRight.setVelocity((90 - sInertial.rotation()) * 1.5, pct);
    }
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    // Open the intakes so that they go around the goal
    intakeOpenAuton();

    // Drive at the goal using the camera to stay aligned
    startTime = Brain.timer(msec);
    while(Brain.timer(msec) - startTime < 1500){
      int ySpeed;
      int xSpeed;
      ySpeed = 100;

      sVision.takeSnapshot(sVision__SIG_BLUE);

      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1, 1);
      Controller1.Screen.print(sVision.objectCount);

      if(sVision.objectCount > 0){
        xSpeed = (sVision.largestObject.centerX - 158) * 1;
      }else{
        xSpeed = 0;
      }

      mWheelFrontLeft.setVelocity(ySpeed + xSpeed, pct);
      mWheelFrontRight.setVelocity(-ySpeed + xSpeed, pct);
      mWheelBackLeft.setVelocity(ySpeed - xSpeed, pct);
      mWheelBackRight.setVelocity(-ySpeed - xSpeed, pct);
      vexDelay(5);
    }
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    // Descore the bottom blue ball
    intakeIn();
    vexDelay(1000);
    intakeOff();

    // Score the red ball we picked up earlier
    output(100, 1000);


    // Back our of the goal at 100 pct for 2 seconds
    driveForward(-100, 2000);
    

    // Back our of the goal at 100 pct
    driveForward(-100, 500);

    // Turn around to face the goal behind

    // Drive via camera and gyro into the goal

    // Score the other red we grabbed

  }*/

  if(mode == 'V'){
    sInertial.setRotation(57, deg);

    // Step 1 - Deploy Camera and Hood and flcik ball into goal
    mOutputUpper.setVelocity(100, pct);
    wait(300, msec);
    mOutputUpper.setVelocity(0, pct);

    outputIn();

    // Step 2 - Get ball 1 (Drive forward and try to alight with the next goal, picking up both balls along the way
    driveForward(100, 500); // STARTHERE
    outputOff();

    // Step 3 - Drive towards ball 2 using Gyro
    double startTime = Brain.timer(msec);
    while(Brain.timer(msec) - startTime < 1000);
    {
      if(Brain.timer(msec) - startTime > 500)
        outputIn();
      leftX = 0;
      leftY = 100;
      rightX = -(50 - sInertial.rotation(deg)) * 5;
      mWheelFrontLeft.setVelocity(rightX + leftY + leftX, pct);
      mWheelFrontRight.setVelocity(rightX - leftY + leftX, pct);
      mWheelBackLeft.setVelocity(rightX + leftY - leftX, pct);
      mWheelBackRight.setVelocity(rightX - leftY - leftX, pct);
      wait(5, msec);
    }
    driveForward(0, 0);
    outputOff();


    // Step 4 - Gyro turn to face center goal
    while(sInertial.rotation(deg) < 0){
      mWheelBackLeft.setVelocity(40, pct);
      mWheelFrontLeft.setVelocity(40, pct);
      mWheelBackRight.setVelocity(0, pct);
      mWheelFrontRight.setVelocity(0, pct);
    }
    mWheelBackLeft.setVelocity(0, pct);
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);

    // Turn towards the goal



    // Get into the goal using camera

    // Score






    /* Joey's new auton
    mOutputUpper.setVelocity(100, pct);

    intakeIn();

    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(-40, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(-40, pct);
    vexDelay(300);
    mOutputUpper.setVelocity(0, pct);
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    // Outtake the preload
    output(100, 400); //500 > 300 timems

    // Drive in reverse
    driveForward(-20, 200);
    output(-100, 300);

    // Turn 45 deg ccw
    sInertial.resetRotation();
    while(sInertial.rotation(deg) > -40){
      mWheelBackLeft.setVelocity(30, pct);
      mWheelFrontLeft.setVelocity(30, pct);
      mWheelBackRight.setVelocity(50, pct);
      mWheelFrontRight.setVelocity(50, pct);
    }
    mWheelBackLeft.setVelocity(0, pct);
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    // Get a ball
    driveForward(-30, 1000);
    strafeLeft(20, 300);
    intakeOpenAuton();
    driveForward(100, 500);
    intakeIn();
    prepOutput(100, 500);
    driveForward(-30, 200);
    strafeLeft(20, 2000);
    intakeOpenAuton();
  
    // Thrust into the goal
    driveForward(100, 1000);
    vexDelay(100);
    
    // Spit out the ball we have
    output(100, 400); //500 > 300 timems

    // Descore Blue
    intakeIn();
    vexDelay(400);

  
    // Back up and spit out blue
    output(100, 200);
    mWheelFrontLeft.setVelocity(-50, pct);
    mWheelFrontRight.setVelocity(50, pct);
    mWheelBackLeft.setVelocity(-50, pct);
    mWheelBackRight.setVelocity(50, pct);
    intakeOff();
    intakeOpenAuton();
    mOutputUpper.setVelocity(-100, pct);
    mOutputLower.setVelocity(-100, pct);

    vexDelay(1000);

    halt();
    outputOff();

    // Get the ball
    driveForward(-30, 500);
    strafeLeft(20, 1000);
    intakeOpenAuton();
    driveForward(100, 500);
    prepOutput(100, 500);
    driveForward(-30, 400);
    strafeLeft(70, 2000);

    // Gun it into the goal
    driveForward(100, 1000);
    intakeOpenAuton();

    // Spit the ball out
    output(100, 600);

    // Descore Blues
    intakeIn();
    prepOutput(100, 500);
    intakeOff();
    intakeOpenAuton();
    vexDelay(100);
    intakeIn();
    prepOutput(100, 200);

    // Drive in reverse and yeet them blues outta here
    driveForward(-20, 200);
    output(-100, 600);

    // Turn 45 deg ccw
    sInertial.resetRotation();
    while(sInertial.rotation(deg) > -40){
      mWheelBackLeft.setVelocity(-40, pct);
      mWheelFrontLeft.setVelocity(-40, pct);
    }
    mWheelBackLeft.setVelocity(0, pct);
    mWheelFrontLeft.setVelocity(0, pct);

    // Open the intakes
    intakeOpenAuton();
    
    // Strafe to ball
    strafeLeft(30, 600);

    //Get ball
    driveForward(100, 500);
    intakeIn();
    prepOutput(100, 500);
    intakeOff();
    driveForward(-30, 500);

    //Strafe to Goal
    strafeLeft(30, 2000);
    // Thrust into the goal
    intakeOpenAuton();
    driveForward(100, 500);
    vexDelay(200);
    // Spit out the ball we have
    output(100, 600); //500 > 300 timems
    // Descore Blue
    intakeIn();
    
    // Back up
    mWheelFrontLeft.setVelocity(-50, pct);
    mWheelFrontRight.setVelocity(50, pct);
    mWheelBackLeft.setVelocity(-50, pct);
    mWheelBackRight.setVelocity(50, pct);

    vexDelay(500);
    prepOutput(100, 200);
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    // Strafe to ball
    intakeOpenAuton();
    output(-100, 300);
    vexDelay(200);
    strafeLeft(20, 2000);

    // Get Ball
    driveForward(100, 500);
    intakeIn();
    prepOutput(100, 500);
    intakeOff();
    driveForward(-30, 400);

    // Gun it to the goal
    mWheelFrontLeft.setVelocity(-70, pct);
    mWheelFrontRight.setVelocity(70, pct);
    mWheelBackLeft.setVelocity(70, pct);
    mWheelBackRight.setVelocity(-70, pct);

    waitUntil(sDistanceLeft.objectDistance(distanceUnits::in) < 4);

    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    //Score
    driveForward(100, 1000);
    vexDelay(100);
    output(100, 600);
    
    // Descore Blues
    intakeIn();
    prepOutput(100, 500);
    intakeOff();
    intakeOpenAuton();
    vexDelay(100);
    intakeIn();
    prepOutput(100, 200);

    // Drive in reverse and yeet them blues outta here
    driveForward(-20, 200);
    intakeOpenAuton();
    output(-100, 600);
    // Get away from the goal
    driveForward(-30, 300);

    // Relax the intakes
    intakeOff();

    sInertial.resetRotation();
    while(sInertial.rotation(deg) < 132){
      mWheelBackLeft.setVelocity(40, pct);
      mWheelFrontLeft.setVelocity(40, pct);
    }
    mWheelBackLeft.setVelocity(0, pct);
    mWheelFrontLeft.setVelocity(0, pct);

    // Line up with center goal
    strafeLeft(40, 2000);

    int xSpeed;
    int ySpeed;

    // Drive at the goal using the camera to stay aligned
    double startTime = Brain.timer(msec);
    while(Brain.timer(msec) - startTime < 1500){
      ySpeed = 100;
      // Score Center
      sVision.takeSnapshot(sVision__SIG_BLUE);

      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1, 1);
      Controller1.Screen.print(sVision.objectCount);

      if(sVision.objectCount > 0){
        xSpeed = (sVision.largestObject.centerX - 158) * 1;
      }
      else{
        xSpeed = 0;
      }

      mWheelFrontLeft.setVelocity(ySpeed + xSpeed, pct);
      mWheelFrontRight.setVelocity(-ySpeed + xSpeed, pct);
      mWheelBackLeft.setVelocity(ySpeed - xSpeed, pct);
      mWheelBackRight.setVelocity(-ySpeed - xSpeed, pct);
      vexDelay(5);
    }
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    // Descore the bottom blue ball
    intakeIn();
    vexDelay(1000);
    intakeOff();

    // Score the red ball we picked up earlier
    output(100, 1000);

    //Descore the other two
    intakeOpenAuton();
    intakeIn();
    prepOutput(100, 500);
    intakeOff();
    intakeOpenAuton();
    intakeIn();
    prepOutput(100, 200);
    intakeOff();
    intakeOpenAuton();

    // Back out of the goal and yeet blues
    driveForward(-100, 1000);
    output(-100, 1000);
    */
  }

  // Right 1
  if(mode == 'Y'){
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
    output(100, 400); //500 > 300 timems

    // Drive in reverse to make sure we aren't touching anything in the goal
    driveForward(-20, 500);
  }

  // Left 1
  if(mode == '<'){
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
  if(mode == 'X'){
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
    output(100, 400); //500 > 300 timems
    // Drive in reverse
    driveForward(-20, 200);
    // Turn 45 deg ccw
    sInertial.resetRotation();
    while(sInertial.rotation(deg) < 40){
      mWheelBackRight.setVelocity(40, pct);
      mWheelFrontRight.setVelocity(40, pct);
    }
    mWheelBackLeft.setVelocity(0, pct);
    mWheelFrontLeft.setVelocity(0, pct);

    intakeIn();
    
    // Line track until we reach the perpendicular line
    while(sOpticalFront.getRgb().clear < OPTICAL_THRESHOLD){
      // Calculate new moving average for the front optical
      // Assign movement variables
      leftX = 25;
      leftY;
      rightX = 0;
      if(sOpticalBack.getRgb().clear > OPTICAL_THRESHOLD)
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
    vexDelay(300); //500
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
    output(100, 600); //500 > 300 timems
    
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
  if(mode == '^'){
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
    output(100, 400); //500 > 300 timems
    // Drive in reverse
    driveForward(-20, 200);
    // Turn 45 deg ccw
    sInertial.resetRotation();
    while(sInertial.rotation(deg) > -40){
      mWheelBackLeft.setVelocity(-40, pct);
      mWheelFrontLeft.setVelocity(-40, pct);
    }
    mWheelBackLeft.setVelocity(0, pct);
    mWheelFrontLeft.setVelocity(0, pct);

    intakeIn();
    
    // Line track until we reach the perpendicular line
    while(sOpticalFront.getRgb().clear < OPTICAL_THRESHOLD){
      // Calculate new moving average for the front optical
      // Assign movement variables
      leftX = -25;
      leftY;
      rightX = 0;
      if(sOpticalBack.getRgb().clear > OPTICAL_THRESHOLD)
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
    vexDelay(300); //500
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
    output(100, 600); //500 > 300 timems
    
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
  if(mode == 'A'){
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
    output(100, 400); //500 > 300 timems
    // Drive in reverse
    driveForward(-20, 200);
    // Turn 45 deg ccw
    sInertial.resetRotation();
    while(sInertial.rotation(deg) < 40){
      mWheelBackRight.setVelocity(40, pct);
      mWheelFrontRight.setVelocity(40, pct);
    }
    mWheelBackLeft.setVelocity(0, pct);
    mWheelFrontLeft.setVelocity(0, pct);

    intakeIn();
    
    // Line track until we reach the perpendicular line
    while(sOpticalFront.getRgb().clear < OPTICAL_THRESHOLD){
      // Calculate new moving average for the front optical
      // Assign movement variables
      leftX = 25;
      leftY;
      rightX = 0;
      if(sOpticalBack.getRgb().clear > OPTICAL_THRESHOLD)
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
    vexDelay(300); //500
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    // Thrust into the goal
    driveForward(100, 1000);

    // Turn intake off

    // Spit out the ball we have
    output(100, 600); //500 > 300 timems
    
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
  if(mode == '>'){
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
    output(100, 400); //500 > 300 timems

    // Drive in reverse
    driveForward(-20, 200);

    // Turn 45 deg ccw
    sInertial.resetRotation();
    while(sInertial.rotation(deg) > -40){
      mWheelBackLeft.setVelocity(-40, pct);
      mWheelFrontLeft.setVelocity(-40, pct);
    }
    mWheelBackLeft.setVelocity(0, pct);
    mWheelFrontLeft.setVelocity(0, pct);

    // Deploy the intakes
    intakeIn();
    
    // Line track until we reach the perpendicular line
    while(sOpticalFront.getRgb().clear < OPTICAL_THRESHOLD){
      // Calculate new moving average for the front optical
      // Assign movement variables
      leftX = -25;
      leftY;
      rightX = 0;
      if(sOpticalBack.getRgb().clear > OPTICAL_THRESHOLD)
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
    vexDelay(300); //500
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    // Thrust into the goal
    driveForward(100, 1000);

    // Spit out the ball we have
    output(100, 600); //500 > 300 timems
    
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
  if(mode == 'S'){
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
    output(100, 500); //500 > 300 timems
    // Drive in reverse
    driveForward(-20, 200);
    // Turn 45 deg ccw
    while(sInertial.rotation(deg) > -175){
      mWheelFrontLeft.setVelocity(-20, pct);
      mWheelBackLeft.setVelocity(-20, pct);
    }
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    intakeIn();
    
    // Line track until we reach the perpendicular line
    while(sOpticalFront.getRgb().clear < OPTICAL_THRESHOLD){
      // Calculate new moving average for the front optical
      // Assign movement variables
      leftX = -25;
      leftY;
      rightX = 0;
      if(sOpticalBack.getRgb().clear > OPTICAL_THRESHOLD)
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
    vexDelay(200); //500 > 300
    mWheelFrontLeft.setVelocity(0, pct);
    mWheelFrontRight.setVelocity(0, pct);
    mWheelBackLeft.setVelocity(0, pct);
    mWheelBackRight.setVelocity(0, pct);

    // Thrust into the goal
    driveForward(100, 1000);

    // Spit out the ball we have
    output(100, 600); //500 > 300 timems

    // Reverse into the center goal

    double startTime = Brain.timer(msec);
    while(Brain.timer(msec) - startTime < 2000){
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

void buttonDown(){
  autonomous();
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

  while(true){

    // Drivetrain /////////////////////////////////////////////////////////////////////////
    
    // Controller joystick values
    leftX = Controller1.Axis4.position();
    leftY = Controller1.Axis3.position();
    rightX = Controller1.Axis1.position();

    // Zero the values
    if(leftX < 20 && leftX > -20)
      leftX = 0;
    if(leftY < 20 && leftY > -20)
      leftY = 0;
    if(rightX < 20 && rightX > -20)
      rightX = 0;

    // Assign wheel speeds
    mWheelFrontLeft.setVelocity((rightX * 1.5) + leftY + leftX, pct);
    mWheelFrontRight.setVelocity((rightX * 1.5) - leftY + leftX, pct);
    mWheelBackLeft.setVelocity((rightX * 1.5) + leftY - leftX, pct);
    mWheelBackRight.setVelocity((rightX * 1.5) - leftY - leftX, pct);

    // Intake ////////////////////////////////////////////////////////////////////////////////////////////////////

    // Spin out untill it hits
    // Spin out at low torque

    if(!disableIntakes){
      // If intake out pressed, begin the intake stages process
      if(Controller1.ButtonR2.pressing() && intakePhase == 0){
        intakePhase = 1;
      }
      // if(intakePhase == 2 && mIntakeLeft.position(deg) > INTAKE_OPEN_TARGET && mIntakeRight.position(deg) > INTAKE_OPEN_TARGET){
      //   intakePhase = 3;
      // }

      // If both arms have hit, move on to stage 2
      if(mIntakeLeft.torque(Nm) > TORQUE_THRESHOLD && mIntakeRight.torque() > TORQUE_THRESHOLD && intakePhase == 1){
        intakePhase = 2; //2
      }

      // Set each intake velocity to 100 pct
      if(intakePhase == 1){
        mIntakeLeft.setVelocity(100, pct);
        mIntakeRight.setVelocity(100, pct);
      }

      // Prepare to move the arms inward to -1 deg
      if(intakePhase == 2){
        mIntakeLeft.setVelocity(100, pct);
        mIntakeRight.setVelocity(100, pct);
        mIntakeLeft.setMaxTorque(40, pct);
        mIntakeRight.setMaxTorque(40, pct);
        // intakePhase = 3;
      }

      // Move the arms inward -1 deg
      if(intakePhase == 3){
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
      if(intakePhase == 4){
        
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
      //     mIntakeRight.setVelocity((INTAKE_OPEN_TARGET - mIntakeLeft.position(deg)) * 4, pct);
      //   else
      //     mIntakeRight.setVelocity(0, pct);
      //   if(mIntakeLeft.position(deg) < INTAKE_OPEN_TARGET)
      //     mIntakeLeft.setVelocity((INTAKE_OPEN_TARGET - mIntakeLeft.position(deg)) * 4, pct);
      //   else
      //     mIntakeLeft.setVelocity(0, pct);
      // }
      // // The stay put phase
      // if(intakePhase == 3){
      //   mIntakeRight.setVelocity(0, pct);
      //   mIntakeLeft.setVelocity(0, pct);
      // }


      // If we are pressing the inward button, set the intakes to coast, reset the intakePhase, and spin inwards
      if(Controller1.ButtonR1.pressing()){
        intakePhase = 0;
        mIntakeLeft.setVelocity(-100, pct);
        mIntakeRight.setVelocity(-100, pct);
        mIntakeLeft.setMaxTorque(100, pct);
        mIntakeRight.setMaxTorque(100, pct);
        mIntakeLeft.setStopping(coast);
        mIntakeRight.setStopping(coast);
      }

      // If absolutly nothing is happening, stop
      if(!Controller1.ButtonR1.pressing() && !Controller1.ButtonR2.pressing() && intakePhase == 0){
        mIntakeLeft.setVelocity(0, pct);
        mIntakeRight.setVelocity(0, pct);
      }

      // Hold at -1 deg
      if(intakePhase == 4){
        
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
      //     mIntakeRight.setVelocity((INTAKE_OPEN_TARGET - mIntakeLeft.position(deg)) * 4, pct);
      //   else
      //     mIntakeRight.setVelocity(0, pct);
      //   if(mIntakeLeft.position(deg) < INTAKE_OPEN_TARGET)
      //     mIntakeLeft.setVelocity((INTAKE_OPEN_TARGET - mIntakeLeft.position(deg)) * 4, pct);
      //   else
      //     mIntakeLeft.setVelocity(0, pct);
      // }
      // // The stay put phase
      // if(intakePhase == 3){
      //   mIntakeRight.setVelocity(0, pct);
      //   mIntakeLeft.setVelocity(0, pct);
      // }


      // If we are pressing the inward button, set the intakes to coast, reset the intakePhase, and spin inwards
      if(Controller1.ButtonR1.pressing()){
        intakePhase = 0;
        mIntakeLeft.setVelocity(-100, pct);
        mIntakeRight.setVelocity(-100, pct);
        mIntakeLeft.setStopping(coast);
        mIntakeRight.setStopping(coast);
      }

      // If absolutly nothing is happening, stop
      if(!Controller1.ButtonR1.pressing() && !Controller1.ButtonR2.pressing() && intakePhase == 0){
        mIntakeLeft.setVelocity(0, pct);
        mIntakeRight.setVelocity(0, pct);
      }
    }
    // Output ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Spin in
    if(Controller1.ButtonL1.pressing()){
      mOutputLower.setVelocity(100, pct);
      mOutputUpper.setVelocity(100, pct);
    // Spin out
    }else if(Controller1.ButtonL2.pressing()){
      mOutputLower.setVelocity(-100, pct);
      mOutputUpper.setVelocity(-100, pct);
    // Stop
    }else{
      mOutputLower.setVelocity(0, pct);
      mOutputUpper.setVelocity(0, pct);
    }

    vexDelay(5);
  }
}

int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  Competition.test_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}


