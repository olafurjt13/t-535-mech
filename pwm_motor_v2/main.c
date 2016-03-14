/*
 * main.c
 *
 *  Created on: 15. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#define FOSC 16000000
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD - 1

#include <avr/io.h>
#include <avr/interrupt.h>

#include "myUSART.h"
#include "pwmFuncs.h"
#include "delayFuncs.h"

void setLedPWM(unsigned char intensity){
	if (intensity > 100){intensity = 100;};
	if (intensity < 0){intensity = 0;};
	OCR0A = ((100 - intensity)*255)/100;
}
void setLedPWM2(unsigned char intensity){
	if (intensity > 100){intensity = 100;};
	if (intensity < 0){intensity = 0;};
	OCR0B = ((100 - intensity)*255)/100;
}

int main(){
	asm("sei");
	pwmInit();
	USART_init(MYUBRR);
	DDRD = 0b01100000;

	//setLedPWM(0);
	while(1){
		for (unsigned char i = 0; i < 100; i = i + 1){
			setLedPWM(i);
			setLedPWM2(100 - i);
			waste_time_ms(10);
		}
		for (unsigned char i = 100; i > 0; i = i -1){
			setLedPWM(i);
			setLedPWM2(100 - i);
			waste_time_ms(10);
		}
	}
	return 0;
}


