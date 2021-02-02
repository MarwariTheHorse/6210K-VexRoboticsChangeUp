#include "vex.h"

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