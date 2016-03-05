/*
 * main.c
 *
 *  Created on: 1. mar. 2016
 *      Author: olafurjonthoroddsen
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include "clockFuncs.h"
#include "myUSART.h"


unsigned char debounceFlag = 0;

int main(){
	unsigned char waitState = 1;
	//unsigned char  countState = 2;
	//unsigned char printState = 3;
	DDRB = 0x20;

	asm("sei");
	USART_init(9600);
	unsigned char currentState = waitState;
	unsigned char nextState;
	unsigned volatile char currentPinStatus;
	unsigned volatile char lastPinStatus;
	unsigned char counter = 0;

	OCR1B = 781;	// set output compare interrupt at 50ms
	TCNT1 = 0;
	DDRD = 0;
	clock_init();

	while(1){
		if( currentState == waitState ){
			unsigned char reading = (PIND & 0x20);
			if(reading != lastPinStatus){
				TCCR1B  = ( 1 << CS10 ) | ( 1 << CS11 );	// Start the timer
				TCNT1 = 0;									// Reset the counter
			}
			if(debounceFlag){
				if(reading != currentPinStatus){
					currentPinStatus = reading;
				}
				if (currentPinStatus == 0x20){
					counter++;
					USART_Transmit_8_dec(counter);
					TCCR1B = 0;
					nextState = waitState;
				}
				debounceFlag = 0;
			}
			lastPinStatus = reading;;
		}
		currentState = nextState;
	}
	return 0;
}

ISR( TIMER1_COMPB_vect ){
	debounceFlag = 1;
}
