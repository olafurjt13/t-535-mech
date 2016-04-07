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
	unsigned char message[25] = "Motor initialized on pin\0";
	myPrint(message,25);
	USART_Transmit_dec(outputPin);
	USART_Transmit(0x0A);
}


void clock_init(){
	// fastPWM mode, set OC0A and OC0B on compare match and clear at bottom
	TCCR0A = ( 1 << COM0A1 ) | ( 1 << COM0A0 ) | ( 1 << COM0B1 ) | ( 1 << COM0B0 ) | ( 1 << WGM01 ) | ( 1 << WGM00 );
	// Initialize a timer with no prescaling
	TCCR0B =  ( 1 << CS00 );
	// Initialize the counter at 0
	TCNT0 = 0;
	clockInitFlag = 1;
	unsigned char message[25] = "PWM Pins initialized\0";
	myPrintNL(message,25);
}

void setMotorDirection(enum motorID motor,enum direction spin){
	unsigned char pin;
	if(motor == right){pin = 3;}else{pin = 4;}
	if (spin == ccw){
		PORTD |= ( 1 << pin );
	}
	else{
		PORTD &= ~( 1 << pin );
	}
}

void setMotorSpeed(unsigned char intensity, enum motorID motor){
	if (intensity > 255){intensity = 255;};
	if (intensity < 0){intensity = 0;};
	if (motor == right){
		OCR0B = intensity;//((100 - intensity)*255)/100;
	}
	else if (motor == left){
		OCR0A = intensity;//((100 - intensity)*255)/100;
	}

}
