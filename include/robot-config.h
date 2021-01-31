using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern signature sVision__SIG_BLUE;
extern signature sVision__SIG_RED;
extern signature sVision__SIG_GREEN;
extern signature sVision__SIG_4;
extern signature sVision__SIG_5;
extern signature sVision__SIG_6;
extern signature sVision__SIG_7;
extern vision sVision;
extern motor mWheelFrontLeft;
extern motor mWheelFrontRight;
extern motor mWheelBackLeft;
extern motor mWheelBackRight;
extern motor mIntakeLeft;
extern motor mIntakeRight;
extern motor mOutputLower;
extern controller Controller1;
extern motor mOutputUpper;
extern optical sOpticalFront;
extern optical sOpticalBack;
extern inertial sInertial;
extern distance sDistanceLeft;
extern distance sDistanceRight;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );