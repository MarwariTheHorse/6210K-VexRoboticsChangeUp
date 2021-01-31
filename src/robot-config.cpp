#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
/*vex-vision-config:begin*/
signature sVision__SIG_BLUE = signature (1, -3091, -275, -1683, 1245, 14043, 7644, 0.8, 0);
signature sVision__SIG_RED = signature (2, 7493, 9919, 8706, -145, 353, 104, 3, 0);
signature sVision__SIG_GREEN = signature (3, -4477, -3933, -4205, -4861, -4157, -4509, 4, 0);
signature sVision__SIG_4 = signature (4, 0, 0, 0, 0, 0, 0, 3, 0);
signature sVision__SIG_5 = signature (5, 0, 0, 0, 0, 0, 0, 3, 0);
signature sVision__SIG_6 = signature (6, 0, 0, 0, 0, 0, 0, 3, 0);
signature sVision__SIG_7 = signature (7, 0, 0, 0, 0, 0, 0, 3, 0);
vision sVision = vision (PORT21, 55, sVision__SIG_BLUE, sVision__SIG_RED, sVision__SIG_GREEN, sVision__SIG_4, sVision__SIG_5, sVision__SIG_6, sVision__SIG_7);
motor mWheelFrontLeft = motor(PORT1, ratio18_1, false);
motor mWheelFrontRight = motor(PORT2, ratio18_1, false);
motor mWheelBackLeft = motor(PORT3, ratio18_1, false);
motor mWheelBackRight = motor(PORT4, ratio18_1, false);
motor mIntakeLeft = motor(PORT5, ratio18_1, true);
motor mIntakeRight = motor(PORT6, ratio18_1, false);
motor mOutputLower = motor(PORT7, ratio18_1, false);
controller Controller1 = controller(primary);
motor mOutputUpper = motor(PORT13, ratio6_1, false);
optical sOpticalFront = optical(PORT18);
optical sOpticalBack = optical(PORT20);
inertial sInertial = inertial(PORT12);
distance sDistanceLeft = distance(PORT19);
distance sDistanceRight = distance(PORT14);
/*vex-vision-config:end*/

bool RemoteControlCodeEnabled = true;


// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}