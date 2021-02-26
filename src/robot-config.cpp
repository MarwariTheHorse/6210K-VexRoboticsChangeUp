#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
/*vex-vision-config:begin*/
signature sVisionUpper__SIG_BLUE = signature (1, -3091, -275, -1683, 1245, 14043, 7644, 0.8, 0);
signature sVisionUpper__SIG_RED = signature (2, 7493, 9919, 8706, -145, 353, 104, 3, 0);
signature sVisionUpper__SIG_GREEN = signature (3, -4477, -3933, -4205, -4861, -4157, -4509, 4, 0);
signature sVisionUpper__SIG_4 = signature (4, 0, 0, 0, 0, 0, 0, 3, 0);
signature sVisionUpper__SIG_5 = signature (5, 0, 0, 0, 0, 0, 0, 3, 0);
signature sVisionUpper__SIG_6 = signature (6, 0, 0, 0, 0, 0, 0, 3, 0);
signature sVisionUpper__SIG_7 = signature (7, 0, 0, 0, 0, 0, 0, 3, 0);
vision sVisionUpper = vision (PORT17, 50, sVisionUpper__SIG_BLUE, sVisionUpper__SIG_RED, sVisionUpper__SIG_GREEN, sVisionUpper__SIG_4, sVisionUpper__SIG_5, sVisionUpper__SIG_6, sVisionUpper__SIG_7);
/*vex-vision-config:end*/
/*vex-vision-config:begin*/
vision sVisionLower = vision (PORT20, 50);
/*vex-vision-config:end*/

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}