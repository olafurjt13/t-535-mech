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
	TCCR1B =  0;
	// Initialize the counter at 0
	TCNT1 = 0;
	// Enable Output Compare Match
	TIMSK1 = ( 1 << OCIE1B );
	// Clearing interrupt flags (writing 1 to them => clearing)
	TIFR1 = ( 1 << OCF1B );
}
