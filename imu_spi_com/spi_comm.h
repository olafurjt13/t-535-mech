/*
 * spi_comm.h
 *
 *  Created on: 5. mar. 2016
 *      Author: olafurjonthoroddsen
 */

#ifndef SPI_COMM_H_
#define SPI_COMM_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define SPI_PORT PORTB
#define SPI_SS DDB2

void SPI_MasterInit(void);
char SPI_MasterTransmit(char cData);

void SPI_Initiate_Transmission();
void SPI_End_Transmission();

#endif /* SPI_COMM_H_ */
