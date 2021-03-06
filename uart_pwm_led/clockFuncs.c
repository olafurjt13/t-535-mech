/*
 * clockFuncs.c
 *
 *  Created on: 15. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#include "clockFuncs.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void clock_init(){
	// Initialize a timer at 0B with 1/64th prescaling
	TCCR0B =  ( 1 << CS01 ) | ( 1 << CS00 );
	// Initialize the counter at 0
	TCNT0 = 0;
	// Enable Output Compare Match and Overflow interrupts
	TIMSK0 = ( 1 << OCIE0B ) | ( 1 << TOIE0 );
	// Clearing interrupt flags (writing 1 to them => clearing)
	TIFR0 = ( 1 << OCF0B ) | ( 1 << TOV0 );
}
