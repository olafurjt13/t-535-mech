/*
 * led.h
 *
 *  Created on: 14. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#ifndef LED_H_
#define LED_H_


#include <avr/io.h>

void Led_init();
void Led_on();
void Led_off();
void Led_toggle();


#endif /* LED_H_ */
