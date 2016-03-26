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
	int last[3];
	unsigned char X[10] = "X: \0";
	unsigned char Y[10] = "Y: \0";
	unsigned char Z[10] = "Z: \0";
	float acc_res = 4.0/65536.0;
	char firstvalues = 0;


	while(1){
		if(dataReceived){
			if (rxBuffer == '0' ){dataReceived = 0;};
			IMU_read_acc(acc_data);

			printf("%f", acc_res*acc_data[0]);
			USART_Transmit(0x20);
			printf("%f", acc_res*acc_data[1]);
			USART_Transmit(0x20);
			printf("%f", acc_res*acc_data[2]);
			USART_Transmit(0x0A);
		}


	}



}

/*	Diffurstöff

 	 if(firstvalues){
				for (unsigned char i = 0; i < 3; i++){
					last[i] = (float)(~acc_data[i] + 1);
				}
				firstvalues = 0;
				continue;
			}
			//myPrint(X,10);
			USART_Transmit(0x20);
			printf("X: %f", acc_res*acc_data[0] - acc_res*last[0]);
			//USART_Transmit_dec(acc_data[0]);
			USART_Transmit(0x20);
			//myPrint(Y,10);
			USART_Transmit(0x20);
			printf("Y: %f", acc_res*acc_data[1] - acc_res*last[1]);
			//USART_Transmit_dec(acc_data[1]);
			USART_Transmit(0x20);
			//myPrint(Z,10);
			USART_Transmit(0x20);
			printf("Z: %f", acc_res*acc_data[2] - acc_res*last[2]);
			//USART_Transmit_dec(acc_data[2]);
			USART_Transmit(0x0A); //Newline

			for (unsigned char i = 0; i < 3; i++){
				last[i] = (float)(~acc_data[i] + 1);
			}
 */
