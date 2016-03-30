/*
 * clockFuncs.c
 *
 *  Created on: 15. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#include "motorControl.h"
#include "myUSART.h"
#include <avr/io.h>

unsigned char clockInitFlag = 0;


void motorInit(char outputPin, char direction){
	setMotorSpeed(0,right);
	setMotorSpeed(0,left);
	unsigned char directionPin = ( 1 << direction );
	unsigned char motorPin = ( 1 << outputPin);
	DDRD |= ( directionPin | motorPin );
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
	clockInitFlag = 1;
}

void setMotorDirection(enum motorID motor,enum direction spin){
	if (spin == ccw){
		PORTD |= ( 1 << motor );
	}
	else{
		PORTD &= ~( 1 << motor );
	}
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
