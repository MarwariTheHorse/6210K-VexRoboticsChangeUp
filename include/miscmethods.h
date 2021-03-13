#include "vex.h"

using namespace vex;
using namespace std;

void driveForward(int, int);
int sgn(double);
void intake(int, int);
void intakeIn();
void intakeOff();
void intakeOpen();
void intakeOpenAuton();
void intakeOpenWall();
void output(int, int);
void outputIn();
void outputOff();
void strafeRight(int, int);
void strafeLeft(int, int);
void halt();
void prepOutput(int, int);
double getForwardVelocity();
double getStrafeVelocity();
double getTurnVelocity();
void sdLog(string, string);
void turnFast(double);
void turnTo(double, int);
void driveViaDistanceGyro(double, double);
void driveViaDistanceGyroCamera(double, double);
void strafeViaDistanceGyro(double, double);
void driveViaTimeGyroCamera(double, double, signature);
void driveViaTimeGyro(double, double);
void driveBackwardsViaTimeGyro(double, double);
void alignToGoal(double);
void strafeUntilGreen(int, double);
void strafeUntilRed(int, double);
void ejectBalls();
void waitForColor(bool);
void waitForNoGreen();