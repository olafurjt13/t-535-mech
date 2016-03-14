/*
 * myDelay.c
 *
 *  Created on: 11. mar. 2016
 *      Author: olafurjonthoroddsen
 */


#include <avr/io.h>
#include "delayFuncs.h"


void waste_time_ms(unsigned int i){
	unsigned volatile long ulMax, ulCount;

	ulMax = 380*i;
	ulCount = 0;
	while (ulCount != ulMax){
		ulCount++;
	}
}
