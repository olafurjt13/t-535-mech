/*
 * main.c
 *
 *  Created on: 20. feb. 2016
 *      Author: olafurjonthoroddsen
 */


#include <avr/io.h>

#include "IMU_communication.h"
#include "myUSART.h"

#define FOSC 16000000
#define BAUD 9600
#define MYUBRR FOSC/BAUD/16 - 1


int main(){
	asm("sei");
	USART_init(MYUBRR);
	SPI_MasterInit();
	IMU_init();
	char acc_data[6];

	while(1){
		IMU_read_acc(acc_data);

		char acc_buffer_L = acc_data[0];
		int acc_buffer_H = ( acc_data[1] << 8 );

		int acc_X = (acc_buffer_H | acc_buffer_L);

		USART_Transmit_16_hex(acc_X);
		USART_Transmit(0x0A);
		USART_Transmit(0x0D);

	}



}
