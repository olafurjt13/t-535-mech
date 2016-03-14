/*
 * clockFuncs.c
 *
 *  Created on: 15. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#include "pwmFuncs.h"

#include <avr/io.h>
#include <avr/interrupt.h>

void pwmInit(){
	// Initialize the timer 0A with "set 0C0A and 0C0B on Compare Match, clear OC0A at Bottom" and sets it to Fast PWM mode
	TCCR0A = 0xF3; //( 1 << COM0A1 ) | ( 1 << COM0A0 ) | ( 1 << COM0B0 ) | ( 1 << COM0B1 ) | ( 1 << WGM00 ) | ( 1 << WGM01 );
	// Initialize a timer at 0B with no prescaling
	TCCR0B = ( 1 << CS00 );
	// Initialize the counter at 0
	TCNT0 = 0;

	// Enable Overflow interrupt
	//TIMSK0 = ( 1 << TOIE0 );
	// Clearing interrupt flag (writing 1  => clearing)
	//TIFR0 = ( 1 << TOV0 );
}
