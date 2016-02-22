/*
 * spi_com.c
 *
 *  Created on: 20. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#define DDR_SPI DDRB
#define SPI_MOSI DDB3
#define SPI_MISO DDB4
#define SPI_SCK DDB5
#define SPI_SS DDB2

#define IF_ADD_INC 2
#define ACC_Z 5
#define ACC_Y 4
#define ACC_X 3

#define CTRL_REG5_XL 0x1F
#define CTRL_REG6_XL 0x20
#define CTRL_REG7_XL 0x21
#define CTRL_REG8 0x22
#define INT_GEN_SRC_XL 0x25
#define STATUS_REG 0x27

#define WHO_AM_I 0x0F


#include "IMU_communication.h"
// including myUSART for debugging
#include "myUSART.h"

void SPI_MasterInit(void){
	// set MOSI, SCK and SS as output, all others are inputs.
	DDR_SPI = ( 1 << SPI_MOSI ) | ( 1 << SPI_SCK ) | ( 1 << SPI_SS );

	// SPI Control Register
	// SPE bit: SPI Enable
	// MSTR bit: Master select bit
	// SPR0 bit: Clock select bit, sets clock rate to Fosc/16
	SPCR = ( 1 << SPE ) | ( 1 << MSTR ) | ( 1 << SPR0 );
}

void SPI_MasterTransmit(char cData){
	// Start transmission
	// SPI Data Register, transfers data between register file and the SPI shift register
	SPDR = cData;

	// Wait for transmission to complete
	// SPI Status Register
	// SPIF bit: SPI Interrupt Flag, it is set when a serial transfer is complete
	while ( !( SPSR & ( 1 << SPIF ) ) ){};
}

void IMU_init(){
	char ctrlReg5 = ( 1 << ACC_Z ) | ( 1 << ACC_Y ) | ( 1 << ACC_X );
	char regAddress = CTRL_REG5_XL;

	// Pull the SS line low to initiate transmission
	PORTB |= ( 1 << SPI_SS );
	// Send write address
	SPI_MasterTransmit(regAddress);
	// Send ctrlReg5 bit pattern to turn on X,Y,Z acceleration sensors
	SPI_MasterTransmit(ctrlReg5);

	// Pull the SS line high to end transmission
	PORTB &= ~( 1 << SPI_SS );

	char ctrlReg8 = ( 1 << IF_ADD_INC );
	regAddress = ( 0x80 ) | CTRL_REG8;
	// Pull the SS line low to initiate transmission
	PORTB |= ( 1 << SPI_SS );

	// Send write address
	SPI_MasterTransmit(regAddress);
	// Send ctrlReg8 bit pattern to turn auto increment registers on
	SPI_MasterTransmit(ctrlReg8);

	// Pull the SS line high to end transmission
	PORTB &= ~ ( 1 << SPI_SS );
}

void IMU_read_acc(char *accBuffer){
	// char sendingRequest[27] = "Now sending request to IMU\n";
	// char readingFromSPDR[18] = "Reading from SPDR\n";

	char *acc = accBuffer;

	// Select the IMU as an SPI Slave
	PORTB |= ( 1 << SPI_SS );
	// Send "read from acceleration output register" signal
	SPDR = 0b10101000;
	// Wait for the request to be sent
	while( !(SPSR & (1 << SPIF ) ) ){};

	for (unsigned char i = 0; i <= 6; i++){
			*acc = SPDR;
			acc++;
			//while( !(SPSR & (1 << SPIF ) ) ){};
	}
	PORTB &= ~( 1 << SPI_SS );

}
