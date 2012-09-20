#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     IR,             sensorAnalog)
#pragma config(Motor,  mtr_S1_C1_1,     RightForward,  tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     BackSideways,  tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     FrontSideways, tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     LeftForward,   tmotorTetrix, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"

task main() {
	//Initialize
	motor[LeftForward] = motor[RightForward] = motor[BackSideways] = motor[FrontSideways] = 0; //Turn off the motors
	nMotorEncoder[LeftForward] = nMotorEncoder[RightForward] = nMotorEncoder[BackSideways] = nMotorEncoder[FrontSideways] = 0; //Might as well reset the encoders too
	waitForStart();
	//Autonomous
	/* Here is the autonomous idea:
	*			Read the IR sensor to get a general area of the Beacon.  Use that to figure out the proper column (shouldn't be too hard)
	*			Then decide which autonomous set of commands to run.  Proper a bunch of if...else if... else if... else statements.  And
	*			If we can't find the IR Beacon (which we should make code for that just in case) then we will just put the ring on the
	*			center peg (or some other predefined spot if necessary).  The movements will be hardcoded so we can tune to the best accuracy.
	*/
}
