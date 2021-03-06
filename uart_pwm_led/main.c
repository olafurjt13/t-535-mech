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

#include "Led.h"
#include "myUSART.h"
#include "clockFuncs.h"


ISR(TIMER0_COMPB_vect){
	Led_on();
}

ISR(TIMER0_OVF_vect){
	Led_off();
}

void setLedPWM(unsigned char intensity){
	if (intensity > 100){intensity = 100;};
	if (intensity < 0){intensity = 0;};
	OCR0B = ((100 - intensity)*255)/100;
}

int main(){
	asm("sei");
	clock_init();
	USART_init(MYUBRR);
	Led_init();
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


