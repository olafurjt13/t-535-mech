/*
 * main.c
 *
 *  Created on: 19. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#define FOSC 16000000
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD - 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include "myUSART.h"




int main(){
	asm("sei");
	USART_init(MYUBRR);
	unsigned long c = 50000;


	while(1){
		if(dataReceived){
			USART_Transmit_32_hex(c);
//			for(char i = 0; i < 62; i++){
//				USART_Transmit_int_hex(c[i]);
//			}
			dataReceived = 0;
		}
	}

	return 0;
}
