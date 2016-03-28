/*
 * clockFuncs.c
 *
 *  Created on: 15. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#include "motorControl.h"
#include "myUSART.h"
#include <avr/io.h>

unsigned char motorPin;
unsigned char clockInitFlag = 0;


void motorInit(char pin){
	motorPin = ( 1 << pin);
	DDRD |= motorPin;
	if (!clockInitFlag){
		clock_init();
	}
}


void clock_init(){
	unsigned char message[20] = "Clock init\0";
	myPrintNL(message,20);
	// fastPWM mode, set OC0A and OC0B on compare match and clear at bottom
	TCCR0A = ( 1 << COM0A1 ) | ( 1 << COM0A0 ) | ( 1 << COM0B1 ) | ( 1 << COM0B0 ) | ( 1 << WGM01 ) | ( 1 << WGM00 );
	// Initialize a timer with no prescaling
	TCCR0B =  ( 1 << CS00 );
	// Initialize the counter at 0
	TCNT0 = 0;

	/*// Enable Output Compare Match and Overflow interrupts
	TIMSK0 = ( 1 << OCIE0B ) | ( 1 << TOIE0 );
	// Clearing interrupt flags (writing 1 to them => clearing)
	TIFR0 = ( 1 << OCF0B ) | ( 1 << TOV0 );
	*/
	clockInitFlag = 1;
}

void setMotorSpeed(unsigned char intensity, enum motorID motor){
	//unsigned char message[20] = "Set speed\0";
	//myPrintNL(message,20);
	if (intensity > 100){intensity = 100;};
	if (intensity < 0){intensity = 0;};
	if (motor == right){
		OCR0B = ((100 - intensity)*255)/100;
	}
	else if (motor == left){
		OCR0A = ((100 - intensity)*255)/100;
	}

}


/*
ISR(TIMER0_COMPB_vect){
	motorPin |= 0x20;
}

ISR(TIMER0_OVF_vect){
	motorPin ^= 0x20;
}
*/
