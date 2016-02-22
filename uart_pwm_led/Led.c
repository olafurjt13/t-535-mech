/*
 * pwmLed.c
 *
 *  Created on: 15. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#include <avr/io.h>

#define LEDPIN 0x10

void Led_init(void){
	DDRB |= LEDPIN;
}

void Led_on(void){
	PORTB |= LEDPIN;
}

void Led_off(void){
	PORTB &= ~LEDPIN;
}

void Led_toggle(void){
	PORTB ^= LEDPIN;
}
