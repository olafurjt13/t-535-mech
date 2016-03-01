/*
 * adc.c
 *
 *  Created on: 29. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#include "adc.h"

#define initADC(){	\
	ADCSRA |= ( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 1 << ADPS0 );	\
	ADMUX |= ( 1 << REFS0 );	\
	ADCSRA |= ( 1 << ADATE );	\
	ADCSRA |= ( 1 << ADEN );	\
	ADCSRA |= ( 1 << ADSC );	\
}

int analogRead(unsigned char pin){
	initADC();
	ADMUX |= pin;
	int adc_val;
	adc_val = ADCL;
	adc_val |= ( ADCH << 8 );
	return adc_val;
}

/* Disabling the Power Reduction ADC bit in the Power Reduction Register*/
/*Setting the prescaler select to 1/128*/
/*Make sure that the ADC is in free running mode so I can continuously read values from it*/
//	ADCSRB &= ~( ( 1 << ADTS2 ) | ( 1 << ADTS1 ) | ( 1 << ADTS0 ) ;
