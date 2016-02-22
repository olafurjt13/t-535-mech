/*
 * spi_com.h
 *
 *  Created on: 20. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#ifndef IMU_COMMUNICATION_H_
#define IMU_COMMUNICATION_H_

#include <avr/io.h>

void SPI_MasterInit(void);
void SPI_MasterTransmit(char cData);
void IMU_read_acc(char *accBuffer);

#endif /* IMU_COMMUNICATION_H_ */
