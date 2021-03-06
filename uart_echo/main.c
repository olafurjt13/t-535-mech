/*
 * main.c
 *
 *  Created on: 14. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#define FOSC 16000000
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD - 1


#include <avr/io.h>
#include "myUSART.h"

int main(){
	USART_init(MYUBRR);
	asm("sei");
	while(1){
		if(dataReceived){
			USART_Transmit(rxBuffer);
			dataReceived = 0;
			rxBuffer = 0;
		}
	}
	return 0;
}
