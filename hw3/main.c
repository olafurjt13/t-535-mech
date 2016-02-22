/*
 * main.c

 *
 *  Created on: 2. feb. 2016
 *      Author: olafurjonthoroddsen
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include "led.h"
#include "delay.h"


int main()
{
	OCR0A=0xFF;
	TCCR0A=0x00;
	TCCR0B=0x05;
	TIFR0=0x03;
	TIMSK0=0;
	TIFR0=0x03;
	sei();

	LED_Init();
	while(1){
		LED_On();
		Delay(15);
		LED_Off();
	}
}


