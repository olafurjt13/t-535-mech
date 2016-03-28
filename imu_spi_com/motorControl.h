/*
 * clockFuncs.h
 *
 *  Created on: 15. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#ifndef MOTORCONTROL_H_
#define MOTORCONTROL_H_


enum motorID
{
	right = 0,
	left,
};



void clock_init();
void motorInit(char pin);
void setMotorSpeed(unsigned char intensity, enum motorID motor);


#endif /* MOTORCONTROL_H_ */
