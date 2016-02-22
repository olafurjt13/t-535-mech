/*
 * led.c
 *
 *  Created on: 14. feb. 2016
 *      Author: olafurjonthoroddsen
 */


#include "led.h"

#define LEDPIN 0x10

void Led_init(){
	DDRB |= LEDPIN;
}

void Led_on(){
	PORTB |= LEDPIN;
}

void Led_off(){
	PORTB &= ~LEDPIN;
}

void Led_toggle(){
	PORTB ^= LEDPIN;
}
