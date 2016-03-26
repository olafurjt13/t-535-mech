/*
 * spi_comm.c
 *
 *  Created on: 5. mar. 2016
 *      Author: olafurjonthoroddsen
 */

#define DDR_SPI DDRB
#define SPI_MOSI DDB3
#define SPI_MISO DDB4
#define SPI_SCK DDB5


#include "spi_comm.h"
#include "myUSART.h"

void SPI_MasterInit(void){
	unsigned char initializingSPI[50] = "Initializing SPI now\0";
	myPrint(initializingSPI,21);
	// set MOSI, SCK and SS as output, all others are inputs.
	DDR_SPI = ( 1 << SPI_MOSI ) | ( 1 << SPI_SCK ) | ( 1 << SPI_SS );

	// Ensure that the SS line is high (no transmission) when the initialization is finished
	SPI_PORT |= ( 1 << SPI_SS );

	// SPI Control Register
	// SPE bit: SPI Enable
	// MSTR bit: Master select bit
	// SPR0 bit: Clock select bit, sets clock rate to Fosc/16, sets the clock polarity such that SCK is high when idle
	SPCR = ( 1 << SPE ) | ( 1 << MSTR ) | ( 1 << CPOL ) | ( 1 << SPR1 );// | ( 1 << SPR0 );

	// Set data sampling at the trailing edge of SCK
	SPCR |= ( 1 << CPHA );
}

char SPI_MasterTransmit(char cData){
	//unsigned char transmitting[50] = "Transmitting over SPI now\0";
	//myPrint(transmitting,30);
	// SPI Data Register, transfers data between register file and the SPI shift register
	//USART_Transmit_8_hex(cData);
	SPDR = cData;

	//asm volatile("nop");	// This is supposed to help

	// Wait for transmission to complete
	// SPI Status Register
	// SPIF bit: SPI Interrupt Flag, it is set when a serial transfer is complete
	while ( !( SPSR & ( 1 << SPIF ) ) );
	if(!(SPCR & ( 1 << MSTR) ) ){SPCR |= ( 1 << MSTR);}

	return SPDR;
}

/*
char SPI_MasterReceive(){
	while ( !( SPSR & (1 << SPIF) ) ){};
	if(!(SPCR & ( 1 << MSTR) ) ){SPCR |= ( 1 << MSTR);}
	return SPDR;
}
*/
