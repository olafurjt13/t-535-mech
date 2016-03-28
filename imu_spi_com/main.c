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
//#include <stdio.h>

#define BAUD 9600

// USART function for outputting floats to stdout
//int usart_putchar_printf(char var, FILE *stream){
//	if(var == 'n'){USART_Transmit('\r');}
//	USART_Transmit(var);
//	return 0;
//}

// OSTREAM to use for floats
//static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE);


int main(){
	//stdout = &mystdout;
	asm("sei");
	USART_init(BAUD);
	IMU_init();
	motorInit(5);
	motorInit(6);

	int acc_data[3];
	int gyro_data[3];
	float acc_res = 4.0/65536.0;
	float gyro_res = 245.0/32768.0;

	setMotorSpeed(50,right);
	setMotorSpeed(50,left);
//	unsigned char speed1[20] = "Motor speed: 100\0";
//	unsigned char speed2[20] = "Motor speed: 50\0";
//	unsigned char speed3[20] = "Motor speed: 30\0";
//	unsigned char speed4[20] = "Motor speed: 10\0";
	char timerval_right;
	char timerval_left;

	DDRD = ( 1 << PD3) ;
	while(1){
		if(dataReceived){
			PORTD ^= ( 1 << PD3 );
			readAcc(acc_data);
			readGyro(gyro_data);

//			float angX = (float)gyro_data[0]*gyro_res;
//			float angY = (float)gyro_data[1]*gyro_res;
//
//			if(angX < 0){angX = -angX;}
//			if(angY < 0){angY = -angY;}
//			timerval_right = (angX/2.0)*255;
//			timerval_left = (angY/2.0)*255;
//
//			USART_Transmit_dec(timerval_right);
//			USART_Transmit(0x20);
//			USART_Transmit_dec(timerval_left);
//			USART_Transmit(0x0A);
//			setMotorSpeed((char)(timerval_right),right);
//			setMotorSpeed((char)(timerval_left),left);
		}
	}
	return 0;
}


/*
 			rxByte = rxBuffer;
			if (rxByte == '1'){
				setMotorSpeed(100,right);
				myPrintNL(speed1,20);
			}
			else if(rxByte == '2'){
				setMotorSpeed(50,left);
				myPrintNL(speed2,20);
			}
			else if(rxByte == '3'){
				setMotorSpeed(30,right);
				myPrintNL(speed3,20);
			}
			else if(rxByte == '4'){
				setMotorSpeed(10,left);
				myPrintNL(speed4,20);
			}


			dataReceived = 0;
			rxByte = 0;
 */

/*
			printf("%f", acc_res*acc_data[0]);
			USART_Transmit(0x20);
			printf("%f", acc_res*acc_data[1]);
			USART_Transmit(0x20);
			printf("%f", acc_res*acc_data[2]);
			USART_Transmit(0x20);
			printf("%f", gyro_res*gyro_data[0]);
			USART_Transmit(0x20);
			printf("%f", gyro_res*gyro_data[1]);
			USART_Transmit(0x20);
			printf("%f", gyro_res*gyro_data[2]);
			USART_Transmit(0x0A);
			*/
