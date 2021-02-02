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

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );