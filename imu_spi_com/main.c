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

	float samplingTime = 0.045;	// Time between reads, as measured on a scope

	//setMotorSpeed(50,right);
	//setMotorSpeed(50,left);

	float angularPos = 0.0;
	float setReference = 0.0;
	float error;
	char Kp = 1;
	char Ki = 0.001;
	char Kd = 0.001;
	float derivativeError = 0;
	float integralError = 0;
	float previousError = 0;
	float output;
	DDRC |= ( 1 << PC1 );
	unsigned int currentTime = 0;
	unsigned int previousTime = 0;
	unsigned char begin = 0;

	while(1){
			if(calibrationFlag){
				calibrateIMU();
				angularPos = 0.0;
				begin = 1;
			}
			if(begin){
			//readAcc(acc_data,10);
			readGyro(angularRate,10);

			// Calculate the time since last measurement
			PORTC ^= ( 1 << PC1 );
			currentTime = TCNT1;
				if (currentTime < previousTime){
					samplingTime = (0xFF -  (previousTime - currentTime))*0.0000005;
				}
				else{
					samplingTime = (currentTime - previousTime)*(0.0000005);
				}
			previousTime = currentTime;

			// Integrate the angular rate to get angular position
			angularPos = angularPos + (float)angularRate[1]*gyro_res*samplingTime;
			error = setReference - angularPos;
			integralError = integralError + error*samplingTime;
			derivativeError = (error - previousError)/samplingTime;
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
		}
	}
	return 0;
}
