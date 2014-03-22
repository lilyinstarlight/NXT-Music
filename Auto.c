#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     IR,             sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  motorA,          RightHand,     tmotorNXT, PIDControl, reversed, encoder)
#pragma config(Motor,  motorB,          LeftHand,      tmotorNXT, PIDControl, reversed, encoder)
#pragma config(Motor,  motorC,          Flag,          tmotorNXT, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     FrontLeft,     tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     FrontRight,    tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     BackLeft,      tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     BackRight,     tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S2_C1_1,     LeftArmShoulder, tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S2_C1_2,     LeftArmElbow,  tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S2_C2_1,     RightArmShoulder, tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S2_C2_2,     RightArmElbow, tmotorTetrix, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"

//Constants
#include "constants.h"

//Useful functions
#include "common.h"

//Music player
#include "Player.h"

string programs[] = { "Left IR", "Left Non-IR", "Right IR", "Right Non-IR" };

task main() {
	bDisplayDiagnostics = false; //Disable screen diagnostics until Autonomous is set up

	//Initialize
	initialize();

	int prog = 0; //Autonomous program

	//Prompt for autonomous program
	nxtDisplayCenteredTextLine(3, "Select program:");
	nxtDisplayCenteredTextLine(5, "%s", programs[prog]);
	while(nNxtButtonPressed != 3) { //Wait for press
		wait10Msec(20);

		if(nNxtButtonPressed == 2 && prog > 0) { //Decrement the program
			prog--;
			nxtDisplayCenteredTextLine(5, "%s", programs[prog]); //Display its name
		}
		else if(nNxtButtonPressed == 1 && prog < sizeof(programs)/sizeof(programs[0]) - 1) { //Increment the program
			prog++;
			nxtDisplayCenteredTextLine(5, "%s", programs[prog]);
		}
	}
	while(nNxtButtonPressed == 3); //Wait for unpress

	//Add a delay
	int delay = 0;
	nxtDisplayCenteredTextLine(3, "Delay:");
	nxtDisplayCenteredTextLine(5, "%d", delay);
	while(nNxtButtonPressed != 3) { //Wait for press
		wait10Msec(20);

		if(nNxtButtonPressed == 2 && delay > 0) { //Decrement the delay
			delay -= 2;
			nxtDisplayCenteredTextLine(5, "%d", delay);
		}
		else if(nNxtButtonPressed == 1 && delay < 30) { //Increment the delay
			delay += 2;
			nxtDisplayCenteredTextLine(5, "%d", delay);
		}
	}
	while(nNxtButtonPressed == 3); //Wait for unpress

	bDisplayDiagnostics = true; //Enable screen diagnostics for match

	//Go time!
	waitForStart();

	//Play the Imperial March
	setSong("ImperialMarch.nms");
	play();

	wait10Msec(delay * 100);

	if(prog == 0 || prog == 2) {
		//Go forward until the IR beacon is found
		nMotorEncoder[FrontLeft] = nMotorEncoder[FrontRight] = nMotorEncoder[BackLeft] = nMotorEncoder[BackRight] = 0;
		if(prog == 0) {
			motor[FrontLeft] = -DRIVE_HIGH;
			motor[BackRight] = DRIVE_HIGH;
		}
		else {
			motor[FrontLeft] = DRIVE_HIGH;
			motor[BackRight] = -DRIVE_HIGH;
		}
		while(SensorValue[IR] != 6 && (abs(nMotorEncoder[FrontLeft]) < AUTO_DETECT || abs(nMotorEncoder[BackRight]) < AUTO_DETECT)); //Wait until we either hit the IR or the last basket
		motor[FrontLeft] = motor[BackRight] = 0;
		//Drop a block and move back
		int offset = nMotorEncoder[FrontLeft]; //Save current distance traveled
		wait();
		//Correct for IR detection
		move(0, AUTO_IR_CORRECT);
		wait();
		turn(AUTO_IR_TURN);
		wait();
		//Drop
		moveRightArm(ARM_SHOULDER_BASKET, 0); //Move shoulder first to prevent problems
		moveRightArm(ARM_SHOULDER_BASKET, ARM_ELBOW_BASKET);
		wait();
		openRightHand();
		wait();
		closeRightHand();
		wait();
		moveRightArm(0, 0);
		wait();
		//Undo actions to continue on
		turn(-AUTO_IR_TURN);
		wait();
		offset -= AUTO_IR_CORRECT; //Correct offset for amount moved for IR correction
		//Continue to end of line
		nMotorEncoder[FrontLeft] = nMotorEncoder[FrontRight] = nMotorEncoder[BackLeft] = nMotorEncoder[BackRight] = 0;
		if(prog == 0) {
			motor[FrontLeft] = -DRIVE_HIGH;
			motor[BackRight] = DRIVE_HIGH;
		}
		else {
			motor[FrontLeft] = DRIVE_HIGH;
			motor[BackRight] = -DRIVE_HIGH;
		}
		while(abs(nMotorEncoder[FrontLeft]) + abs(offset) < AUTO_DETECT || abs(nMotorEncoder[BackRight]) + abs(offset) < AUTO_DETECT); //Continue but adjust for offset
		motor[FrontLeft] = motor[BackRight] = 0;
		wait();

		//Go to ramp
		if(prog == 0) {
			move(0, AUTO_RAMP_1); //Go a little bit further
		}
		else {
			move(0, -AUTO_RAMP_1);
		}
		wait();
		move(AUTO_RAMP_2, 0); //Move in front of the ramp
		wait();
		turn(AUTO_RAMP_TURN); //Turn so that two wheels hit the front of the ramp
		wait();
		if(prog == 0) {
			move(AUTO_RAMP_UP, -AUTO_RAMP_UP); //Go up the ramp
		}
		else {
			move(-AUTO_RAMP_UP, AUTO_RAMP_UP);
		}
		wait();
	}
	else if(prog == 1 || prog == 3) {
		//Place a block in first basket
		if(prog == 1) {
			move(AUTO_FIRST_BASKET, -AUTO_FIRST_BASKET); //Move up to first basket
		}
		else {
			move(-AUTO_FIRST_BASKET, AUTO_FIRST_BASKET);
		}
		wait();
		//Drop
		moveRightArm(ARM_SHOULDER_BASKET, 0); //Move shoulder first to prevent problems
		moveRightArm(ARM_SHOULDER_BASKET, ARM_ELBOW_BASKET);
		wait();
		openRightHand();
		wait();
		closeRightHand();
		wait();
		moveRightArm(0, 0);
		wait();

		//Go to ramp and drive through to the opposite side
		if(prog == 1) {
			move(AUTO_RAMP, -AUTO_RAMP); //Move in front of ramp
		}
		else {
			move(-AUTO_RAMP, AUTO_RAMP);
		}
		wait();
		move(AUTO_RAMP_OVER, AUTO_RAMP_OVER); //Go onto and to the other side of the ramp
		wait();
	}
}
