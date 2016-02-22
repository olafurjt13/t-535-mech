/*
 * myDelay.c
 *
 *  Created on: 10. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "myDelay.h"

#define TU 4e-6

void init_clock(){
	TCCR1B |= (( 1 << CS10 ));
	TCNT1 = 0;
}


void delay_milliseconds(unsigned long timer){
	init_clock();
	unsigned volatile long goal = (timer/1000)/TU;
	OCR1B |= goal;
}

ISR (TIMER1_COMPB_vect){
	PORTB ^= 0x20;
}
