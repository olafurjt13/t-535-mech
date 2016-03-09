/*
 * main.c
 *
 *  Created on: 20. feb. 2016
 *      Author: olafurjonthoroddsen
 */


#include <avr/io.h>

#include "IMU_module.h"
#include "myUSART.h"

#define BAUD 9600


int main(){
	asm("sei");
	USART_init(BAUD);
	IMU_init();
	int acc_data[3];

	while(1){
		if(dataReceived){
			//if (rxBuffer == 'q') {dataReceived = 0;};
			IMU_read_acc(acc_data);
			USART_Transmit_16_dec(acc_data[0]);
			dataReceived = 0;
		}


	}



}
