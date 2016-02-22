/*
 * delay.c

 *
 *  Created on: 2. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#include "delay.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile char cDelayDone;

void Delay(char cDelay){
	cDelayDone = 0;
	OCR0A=cDelay;
	TCNT0=0;
	TIMSK0=0x02;
	while(cDelayDone==0){
		;
	}
	TIMSK0=0;
}

ISR(TIMER0_COMPA_vect)
{
	cDelayDone=1;
}
