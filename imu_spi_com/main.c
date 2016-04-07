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

// Forbidden stuff!
#include <stdio.h>
//#include <util/delay.h>

#define BAUD 9600

//USART function for outputting floats to stdout
int usart_putchar_printf(char var, FILE *stream){
	if(var == 'n'){USART_Transmit('\r');}
	USART_Transmit(var);
	return 0;
}

// OSTREAM to use for floats
static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE);


int main(){
	stdout = &mystdout;
	asm("sei");
	USART_init(BAUD);
	IMU_init();
	motorInit(right,3);	// Pin 5
	motorInit(left,4);	// Pin 6

	int acc_data[3];
	int angularRate[3];
	float acc_res = 4.0/65536.0;
	float gyro_res = 245.0/32768.0;

	float dt = 0.045;	// Time between reads, as measured on a scope


	float angularPos = 0.0;
	float setReference = 0.0;
	float error;
	char Kp = 10;
	char Ki = 0.005;
	char Kd = 0.001;
	float derivativeError = 0;
	float integralError = 0;
	float previousError = 0;
	float robotHeight = 0.144;
	float output;
	unsigned int currentTime = 0;
	unsigned int previousTime = 0;
	unsigned char begin = 0;

	// Debouncing the angular Rate sensor
	unsigned char angRatePositive = 0;
	unsigned char angRateNegative = 0;
	int angularThreshold = 20;

	// Variables to allow accelerometer to check if it's time to reset the angularPos variable
	unsigned char lowAccCounter = 0;


	DDRC |= ( 1 << PC1 );
	while(1){
			if(calibrationFlag){
				calibrateIMU();
				angularPos = 0.0;
				begin = 1;
			}
			if(begin){
			readAcc(acc_data,20);
			readGyro(angularRate,20);

			PORTC ^= ( 1 << PC1 );

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

				if(angularPos > 4.0){
					if ((acc_data[2] < 1638) & (acc_data[2] > -1638)){	// Z acceleration < +-0.1g
						lowAccCounter += 1;
					}
					if (lowAccCounter > 100){
						angularPos = 0.0;
					}
				}
				else{
					lowAccCounter = 0;
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

				setMotorSpeed((unsigned char)(output),right);
				setMotorSpeed((unsigned char)(output),left);

				USART_Transmit_dec((int)output);
				USART_Transmit(0x0A);

			}
			}
	}
	return 0;
}
