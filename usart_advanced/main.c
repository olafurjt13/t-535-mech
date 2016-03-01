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

	unsigned char string[60] = "This is a string with multiple characters\0 null null null";
	while(1){
		if(dataReceived){
			myPrint(string,60);
			dataReceived = 0;
		}
	}
	return 0;
}
