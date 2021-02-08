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