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
	// Initialize a timer at 1B with 1/1024th prescaling
	TCCR1B =  ( 1 << CS02 ) | ( 1 << CS00 );
	// Initialize the counter at 0
	TCNT1L = 0;
	TCNT1H = 0;
	// Enable Output Compare Match and Overflow interrupts
	TIMSK1 = ( 1 << OCIE1B ) | ( 1 << TOIE1 );
	// Clearing interrupt flags (writing 1 to them => clearing)
	TIFR1 = ( 1 << OCF1B ) | ( 1 << TOV1 );
}
