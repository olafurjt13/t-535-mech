/*
 * led.c

 *
 *  Created on: 1. feb. 2016
 *      Author: olafurjonthoroddsen
 */


#include "led.h"
#include <avr/io.h>

void LED_Init(){
	LEDDIR |= PIN_LED;	// make the pin an output
}

void LED_On(){
	LEDPORT |= PIN_LED;
}

void LED_Off(){
	LEDPORT &= ~PIN_LED;
}

void LED_Toggle(){
	LEDPORT ^= PIN_LED;
}
