/*
 * main.c
 *
 *  Created on: 20. feb. 2016
 *      Author: olafurjonthoroddsen
 */


#include <avr/io.h>

#include "IMU_module.h"
#include "myUSART.h"
#include <stdio.h>

#define BAUD 9600

int usart_putchar_printf(char var, FILE *stream){
	if(var == 'n'){USART_Transmit('\r');}
	USART_Transmit(var);
	return 0;
}
static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE);


int main(){
	stdout = &mystdout;
	asm("sei");
	USART_init(BAUD);
	IMU_init();
	int acc_data[3];
	int gyro_data[3];
	float acc_res = 4.0/65536.0;
	float gyro_res = 245.0/32768.0;


	while(1){
		if(dataReceived){
			if (rxBuffer == '0' ){dataReceived = 0;};
			IMU_read_acc(acc_data);
			IMU_read_gyro(gyro_data);

//			printf("%f", acc_res*acc_data[0]);
//			USART_Transmit(0x20);
//			printf("%f", acc_res*acc_data[1]);
//			USART_Transmit(0x20);
//			printf("%f", acc_res*acc_data[2]);
//			USART_Transmit(0x20);
			printf("%f", gyro_res*gyro_data[0]);
			USART_Transmit(0x20);
			printf("%f", gyro_res*gyro_data[1]);
			USART_Transmit(0x20);
			printf("%f", gyro_res*gyro_data[2]);
			USART_Transmit(0x0A);

		}


	}



}
