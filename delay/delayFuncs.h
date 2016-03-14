/*
 * myDelay.h
 *
 *  Created on: 11. mar. 2016
 *      Author: olafurjonthoroddsen
 */

#ifndef DELAYFUNCS_H_
#define DELAYFUNCS_H_

#define delay_us(microSeconds){	\
	/*unsigned char numNops = microSeconds; */	\
		asm("nop");	\
		asm("nop");	\
		asm("nop");	\
}	\

#endif /* DELAYFUNCS_H_ */
