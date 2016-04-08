/*
 * main.c
 *
 *  Created on: 20. feb. 2016
 *      Author: olafurjonthoroddsen
 */


#include <avr/io.h>

#include "IMU_module.h"
#include "motorControl.h"
#include "myUSART.h"

#define BAUD 9600

int main(){
	asm("sei");			// Set global interrupts flag

	USART_init(BAUD);
	IMU_init();

	motorInit(right,3);	// PWM on pin 5, phase on pin 3
	motorInit(left,4);	// PWM on pin 6, phase on pin 4

	int acc_data[3];	// Buffer for acceleration data
	int angularRate[3];	// Buffer for angular rate data
	//float acc_res = 4.0/65536.0;		// Unused
	float gyro_res = 245.0/32768.0;

	float dt;	// Time between reads, measured each time the while loop runs


	float angularPos = 0.0;
	float setReference = 0.0;
	float error;
	char Kp = 15;
	float Ki = 0.01;
	float Kd = 0.001;
	float derivativeError = 0;
	float integralError = 0;
	float previousError = 0;
	float output;

	// Ensure the motors don't spin on startup
	setMotorSpeed(0,right);
	setMotorSpeed(0,left);

	// Time measurement variables
	unsigned int currentTime = 0;
	unsigned int previousTime = 0;
	unsigned char begin = 0;

	// Debouncing the angular Rate sensor
	unsigned char angRatePositive = 0;
	unsigned char angRateNegative = 0;
	int angularThreshold = 10;

	// Variables to allow accelerometer to check if it's time to reset the angularPos variable
	unsigned char lowAccCounter = 0;

	while(1){
		if(calibrationFlag){
			calibrateIMU();
			angularPos = 0.0;
			begin = 1;
		}
		if(begin){
			readAcc(acc_data,20);
			readGyro(angularRate,20);

			// Calculate the time since last measurement
			currentTime = TCNT1;
				if (currentTime < previousTime){
					dt = (0xFF -  (previousTime - currentTime))*0.000004;
				}
				else{
					dt = (currentTime - previousTime)*(0.000004);
				}
			previousTime = currentTime;

			if(angularRate[1] > 0){
				angRatePositive += 1;
				angRateNegative = 0;
			}
			else{
				angRateNegative += 1;
				angRatePositive = 0;
			}

			if ((
				(angRatePositive > 10) | (angRateNegative > 10)) &
				((angularRate[1] > angularThreshold) |
				(angularRate[1] < -angularThreshold)))
			{
				// Integrate the angular rate to get angular position
				angularPos = angularPos + (float)angularRate[1]*gyro_res*dt	;

				if(angularPos > 2.0){
					if ((acc_data[2] < 2457) & (acc_data[2] > -2457)){	// Z acceleration < +-0.15g
						lowAccCounter += 1;
					}
				}
				else{
					lowAccCounter = 0;
				}
				if(lowAccCounter > 100){
					angularPos = 0.0;
				}
				// PID controller
				error = setReference - angularPos;
				integralError = integralError + error*dt;
				derivativeError = (error - previousError)/dt;
				previousError = error;

				output = Kp*error + Kd*derivativeError + Ki*integralError;

				if(output < 0){
					output = -output;
					setMotorDirection(right,cw);
					setMotorDirection(left,ccw);
				}
				else{
					setMotorDirection(right,ccw);
					setMotorDirection(left,cw);
				}
				if(output > 100){output=100;}

				setMotorSpeed(output,right);
				setMotorSpeed(output,left);

				// Delay for 10000*(1/16e6) ≈ 625us
				for(unsigned int i = 0; i < 10000; i++){
					asm("nop");
				}
			}
		}
	}
	return 0;
}
