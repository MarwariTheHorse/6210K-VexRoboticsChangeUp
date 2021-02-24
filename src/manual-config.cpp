#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

motor mWheelFrontLeft = motor(PORT1, ratio18_1, false);
motor mWheelFrontRight = motor(PORT2, ratio18_1, false);
motor mWheelBackLeft = motor(PORT3, ratio18_1, false);
motor mWheelBackRight = motor(PORT4, ratio18_1, false);
motor mIntakeLeft = motor(PORT5, ratio36_1, true);
motor mIntakeRight = motor(PORT6, ratio36_1, false);
motor mOutputLower = motor(PORT7, ratio18_1, false);
controller Controller1 = controller(primary);
motor mOutputUpper = motor(PORT13, ratio6_1, false);
optical sOpticalFront = optical(PORT18);
optical sOpticalBack = optical(PORT20);
inertial sInertial = inertial(PORT12);
distance sDistanceLeft = distance(PORT19);
distance sDistanceRight = distance(PORT14);

signature sVisionUpper__SIG_BLUE = signature (1, -3091, -275, -1683, 1245, 14043, 7644, 0.8, 0);
signature sVisionUpper__SIG_RED = signature (2, 7493, 9919, 8706, -145, 353, 104, 3, 0);
signature sVisionUpper__SIG_GREEN = signature (3, -4477, -3933, -4205, -4861, -4157, -4509, 4, 0);
signature sVisionUpper__SIG_4 = signature (4, 0, 0, 0, 0, 0, 0, 3, 0);
signature sVisionUpper__SIG_5 = signature (5, 0, 0, 0, 0, 0, 0, 3, 0);
signature sVisionUpper__SIG_6 = signature (6, 0, 0, 0, 0, 0, 0, 3, 0);
signature sVisionUpper__SIG_7 = signature (7, 0, 0, 0, 0, 0, 0, 3, 0);
vision sVisionUpper = vision (PORT21, 50, sVisionUpper__SIG_BLUE, sVisionUpper__SIG_RED, sVisionUpper__SIG_GREEN, sVisionUpper__SIG_4, sVisionUpper__SIG_5, sVisionUpper__SIG_6, sVisionUpper__SIG_7);

signature sVisionLower__SIG_BLUE = signature (1, -3091, -275, -1683, 1245, 14043, 7644, 0.8, 0);
signature sVisionLower__SIG_RED = signature (2, 7493, 9919, 8706, -145, 353, 104, 3, 0);
signature sVisionLower__SIG_GREEN = signature (3, -4477, -3933, -4205, -4861, -4157, -4509, 4, 0);
signature sVisionLower__SIG_4 = signature (4, 0, 0, 0, 0, 0, 0, 3, 0);
signature sVisionLower__SIG_5 = signature (5, 0, 0, 0, 0, 0, 0, 3, 0);
signature sVisionLower__SIG_6 = signature (6, 0, 0, 0, 0, 0, 0, 3, 0);
signature sVisionLower__SIG_7 = signature (7, 0, 0, 0, 0, 0, 0, 3, 0);
vision sVisionLower = vision (PORT20, 50, sVisionUpper__SIG_BLUE, sVisionUpper__SIG_RED, sVisionUpper__SIG_GREEN, sVisionUpper__SIG_4, sVisionUpper__SIG_5, sVisionUpper__SIG_6, sVisionUpper__SIG_7);