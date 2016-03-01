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

#define waitState 1
#define countState 2
#define printState 3

unsigned char debounceFlag = 0;

int main(){
	asm("sei");
	unsigned char currentState = waitState;
	unsigned char nextState;
	unsigned char lastState;
	unsigned char currentPinStatus;
	unsigned char lastPinStatus;

	clock_init();
	OCR1B = 156;	// set output compare interrupt at 10ms
	DDRD |= 0x20;
	PORTD |= 0x20;
	TCNT1 = 0;

	while(1){

		if( currentState == waitState ){
			currentPinStatus = (PIND & 0x20);

			if(currentPinStatus != lastPinStatus){
				TCNT1 = 0;
			}
			if(debounceFlag){
				nextState = countState;
				debounceFlag = 0;
			}
		}

		else if( currentState == countState ){

		}

		else if( currentState == printState ){

		}


		currentState = nextState;
	}


	return 0;
}

ISR( TIMER0_COMPB_vect ){
	debounceFlag = 1;
}
