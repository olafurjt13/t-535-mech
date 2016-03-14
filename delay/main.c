/*
 * main.c
 *
 *  Created on: 11. mar. 2016
 *      Author: olafurjonthoroddsen
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "delayFuncs.h"

int main(){
	DDRB = 0x20;

	while(1){
		PORTB ^= 0x20;
		//asm("eor PORTB, r20");
		delay_us(1);
	}
}
