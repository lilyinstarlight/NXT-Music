#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     IR,             sensorAnalog)
#pragma config(Motor,  mtr_S1_C1_1,     RightForward,  tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     BackSideways,  tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     FrontSideways, tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     LeftForward,   tmotorTetrix, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"

//Initialize our globals
ubyte joystickFactor = 100; //Used to scale down robot movements
bool forward = true; //Used for direction locking
bool sideways = true;

task joystickControl() { //Asynchronous task for joystick control
	while(true) {
		if(forward) { //Part of direction locking mechanism
			//Big fancy statement to set the left forward motors equal to the converted joystick's left y axis unless it is in the natural variant range.
			motor[LeftForward] = joystick.joy1_y1 > 10 || joystick.joy1_y1 < -10 ? joystick.joy1_y1 / 128.0 * joystickFactor : 0;
			//Same for right and for sideways (but use the second y axis for right side and use the x axes for sideways motors)
			motor[RightForward] = joystick.joy1_y2 > 10 || joystick.joy1_y2 < -10 ? joystick.joy1_y2 / 128.0 * joystickFactor : 0;
		}
  	if(sideways) {
			motor[BackSideways] = joystick.joy1_x1 > 10 || joystick.joy1_x1 < -10 ? joystick.joy1_x1 / 128.0 * joystickFactor : 0;
			motor[FrontSideways] = joystick.joy1_x2 > 10 || joystick.joy1_x2 < -10 ? joystick.joy1_x2 / 128.0 * joystickFactor : 0;
		}
	}
}

task main() {
	//Initialize
	motor[LeftForward] = motor[RightForward] = motor[BackSideways] = motor[FrontSideways] = 0; //Turn off the motors
	nMotorEncoder[LeftForward] = nMotorEncoder[RightForward] = nMotorEncoder[BackSideways] = nMotorEncoder[FrontSideways] = 0; //Might as well reset the encoders too
	waitForStart();
	StartTask(joystickControl); //Go ahead and start joysticks in their own task
	while(true) {
		if(joy1Btn(6)) //If the driver is pressing button 6, scale down robot movements
			joystickFactor = 30;
		else
			joystickFactor = 100;
		if(joy1Btn(5)) //If the driver is pressing button 5, lock to y-axis movement
			sideways = false;
		else
			sideways = true;
		if(joy1Btn(7)) //If the driver is pressing button 7, lock to x-axis movement
			forward = false;
		else
			forward = true;
	}
}
