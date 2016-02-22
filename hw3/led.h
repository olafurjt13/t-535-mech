/*
 * led.h

 *
 *  Created on: 1. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#ifndef LED_H_
#define LED_H_

#define LEDDIR DDRB
#define LEDPORT PORTB
#define PIN_LED (1<<5)

#define LED_INIT LEDDIR |= PIN_LED
#define LED_ON LEDPORT |= PIN_LED
#define LED_OFF LEDPORT &= ~PIN_LED
#define LED_TOGGLE LEDPORT ^PIN_LED

void LED_Init();
void LED_On();
void LED_Off();
void LED_Toggle();


#endif /* LED_H_ */
