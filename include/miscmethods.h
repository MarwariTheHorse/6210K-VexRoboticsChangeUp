#include "vex.h"

using namespace vex;
using namespace std;

void driveForward(int speed, int timems);
int sgn(double d);
void intake(int speed, int timems);
void intakeIn();
void intakeOff();
void intakeOpen();
void intakeOpenAuton();
void intakeOpenWall();
void output(int speed, int timems);
void outputIn();
void outputOff();
void strafeRight(int speed, int timems);
void strafeLeft(int speed, int timems);
void halt();
void prepOutput(int speed, int timems);
void sdLog(string label, string message);