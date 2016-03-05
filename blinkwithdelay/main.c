/*
 * main.c
 *
 *  Created on: 8. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#define LEDPIN 0x20;

#include <avr/io.h>


int main(){
	DDRB |= LEDPIN;			// Set pin 0x20 to be an output
	TCCR1B |= (( 1 << CS12 ) | ( 1 << CS10 ));	// Initialize a clock source with prescaling of 1024 at TimerCounterControlRegister 1B
	PORTB = LEDPIN;			// Turn on the LED
	while(1){
		if(TCNT1 > 1563){		// If the TimerCounter register 1 reaches 15625 (1 second)
			PORTB ^= LEDPIN;	// Toggle the LED
			TCNT1 = 0;		// Reset the timer value
		}
	}
	return 0;
}



