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
	right = 5,
	left = 6,
};

enum direction
{
	cw,
	ccw,
};



void clock_init();
void motorInit(char outputPin, char direction);
void setMotorSpeed(unsigned char intensity, enum motorID motor);
void setMotorDirection(enum motorID motor,enum direction spin);


#endif /* MOTORCONTROL_H_ */
