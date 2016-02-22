/*
 * delay.h

 *
 *  Created on: 2. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#ifndef DELAY_H_
#define DELAY_H_

#include <avr/io.h>
#include <avr/interrupt.h>

void Delay(char cDelay);

ISR(TIMER0_COMPA_vect);

#endif /* DELAY_H_ */
