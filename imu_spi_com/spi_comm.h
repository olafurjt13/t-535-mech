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

	/* Pull the SS line low to initiate transmission */
#define SPI_Initiate_Transmission(){SPI_PORT &= ~( 1 << SPI_SS );}

	/* Pull the SS line high to end transmission */
#define SPI_End_Transmission(){SPI_PORT |= ( 1 << SPI_SS );}

void SPI_MasterInit(void);
char SPI_MasterTransmit(char cData);
//char SPI_MasterReceive();

/*
void SPI_Initiate_Transmission();
void SPI_End_Transmission();
*/

#endif /* SPI_COMM_H_ */
