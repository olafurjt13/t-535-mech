/*
 * main.c
 *
 *  Created on: 29. feb. 2016
 *      Author: olafurjonthoroddsen
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"
#include "myUSART.h"


int main(){
	asm("sei");
	USART_init(9600);

	while(1){
		if (dataReceived){
			int adc = analogRead(0);
			unsigned int temperature = (adc*500)/1024;	// scaling to 8 bits
			USART_Transmit_16_dec(temperature);
			dataReceived = 0;
		}
	}
	return 0;
}


//			if(rxBuffer == '0'){dataReceived = 0;}; // To stop the transmit, send a 0
