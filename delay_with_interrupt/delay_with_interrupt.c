/*
 * delay_with_interrupt.c
 *
 *  Created on: 1. feb. 2016
 *      Author: olafurjonthoroddsen
 */


#include <avr/io.h>
#include <avr/interrupt.h>

volatile char cDelayDone = 0;

void Delay(char cDelay){
	cDelayDone = 0;
	OCR0A=cDelay;
	TCNT0=0;
	TIMSK0=0x02;
	while(cDelayDone == 0)
		;
		TIMSK0 = 0;
}

int main(){
	DDRB = 0x20;
	OCR0A = 0xFF;
	TCCR0A = 0x00;
	TCCR0B = 0x05;
	TIFR0 = 0x03;
	TIMSK0 = 0;
	TIFR0 = 0x03;

	sei();

	while(1){
		PORTB = 0x20;
		Delay(15);
		PORTB = 0;
	}
}

ISR (TIMER0_COMPA_vect){
	cDelayDone = 1;
}

ISR (TIMER0_OVF_vect){

}
