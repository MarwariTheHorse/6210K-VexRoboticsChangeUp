#include "vex.h"
#include "miscmethods.h"
#include "auton-externs.h"


void fullAuton(){
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
  mWheelFrontLeft.setStopping(brake);
  mWheelFrontRight.setStopping(brake);
  mWheelBackLeft.setStopping(brake);
  mWheelBackRight.setStopping(brake);

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