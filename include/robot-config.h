using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern signature sVisionUpper__SIG_BLUE;
extern signature sVisionUpper__SIG_RED;
extern signature sVisionUpper__SIG_GREEN;
extern signature sVisionUpper__SIG_4;
extern signature sVisionUpper__SIG_5;
extern signature sVisionUpper__SIG_6;
extern signature sVisionUpper__SIG_7;
extern vision sVisionUpper;
extern signature sVisionLower__SIG_1;
extern signature sVisionLower__SIG_2;
extern signature sVisionLower__SIG_3;
extern signature sVisionLower__SIG_4;
extern signature sVisionLower__SIG_5;
extern signature sVisionLower__SIG_6;
extern signature sVisionLower__SIG_7;
extern vision sVisionLower;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );