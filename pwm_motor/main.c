/*
 * main.c
 *
 *  Created on: 15. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#define FOSC 16000000
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD - 1

#include <avr/io.h>
#include <avr/interrupt.h>

#include "myUSART.h"
#include "clockFuncs.h"




int main(){
	asm("sei");
	clock_init();
	USART_init(MYUBRR);
	DDRB = 0x20;
	PORTB |= 0x20;
	setLedPWM(40);

	rxBuffer = 0;
	unsigned char rxByte;
	while(1){
		if(dataReceived){
			rxByte = rxBuffer;

			if(rxByte == '1'){
				setLedPWM(100);
			}
			else if(rxByte == '2'){
				setLedPWM(20);
			}
			else if(rxByte == '3'){
				setLedPWM(30);
			}
			else if(rxByte == '4'){
				setLedPWM(40);
			}
			else if(rxByte == '5'){
				setLedPWM(50);
			}
			else if(rxByte == 'q'){
				setLedPWM(0);
			}
			if(sendBack == 1){
				USART_Transmit(rxByte);
				sendBack = 0;
			}

			rxBuffer = 0;
			dataReceived = 0;
		}
	}
	return 0;
}


