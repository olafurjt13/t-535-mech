/*
 * spi_com.c
 *
 *  Created on: 20. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#define IF_ADD_INC 5
#define ACC_Z 2
#define ACC_Y 3
#define ACC_X 4

#define CTRL_REG5_XL 0x1F
#define CTRL_REG6_XL 0x20
#define CTRL_REG7_XL 0x21
#define CTRL_REG8 0x22
#define INT_GEN_SRC_XL 0x25
#define STATUS_REG 0x27
#define OUT_X_XL 0x28
#define OUT_Y_XL 0x2A
#define OUT_Z_XL 0x2C

#define WHO_AM_I 0x0F

#define READ 0x80
#define WRITE 0x00

#define MOSI_PIN PORTB


#include "spi_comm.h"
#include "IMU_module.h"
// including myUSART for debugging
#include "myUSART.h"



void IMU_init(){
	//unsigned char initializingIMU[50] = "Initializing IMU now\0";
	//myPrint(initializingIMU,21);

	SPI_MasterInit();

	// The bit pattern to turn on acceleration sensing in the Z,Y and X directions
	char setCtrlReg5 = ( 1 << ACC_Z ) | ( 1 << ACC_Y ) | ( 1 << ACC_X );

	// Send write address
	SPI_Initiate_Transmission();
	SPI_MasterTransmit( WRITE | CTRL_REG5_XL );

	// Send ctrlReg5 bit pattern to turn on X,Y,Z acceleration sensors
	SPI_MasterTransmit( setCtrlReg5 );
	SPI_End_Transmission();

	//asm("nop");
	char setCtrlReg8 = ( 1 << IF_ADD_INC );

	// Send write address
		SPI_Initiate_Transmission();

	SPI_MasterTransmit( WRITE | CTRL_REG8 );
	// Send setCtrlReg8 bit pattern to turn auto increment registers on
	SPI_MasterTransmit( setCtrlReg8 );
		SPI_End_Transmission();
	//unsigned char doneIMU[50] = "Finished initializing IMU\0";
	//myPrint(doneIMU,30);
}

void IMU_read_acc(int *accBuffer){
	// char sendingRequest[27] = "Now sending request to IMU\n";
	// char readingFromSPDR[18] = "Reading from SPDR\n";

	char spiBuffer[6];
	int *acc = accBuffer;

	// Select the IMU as an SPI Slave
	SPI_Initiate_Transmission();
	// Send "read from acceleration output register" signal
	SPI_MasterTransmit( READ | OUT_X_XL );
	for (unsigned char i = 0; i < 2; i++){
			spiBuffer[i] = SPI_MasterTransmit(0x00);
	}
	SPI_End_Transmission();

	SPI_Initiate_Transmission();
	SPI_MasterTransmit( READ | OUT_Y_XL );
	for (unsigned char i = 2; i < 4; i++){
			spiBuffer[i] = SPI_MasterTransmit(0x00);
	}
	SPI_End_Transmission();

	SPI_Initiate_Transmission();
	SPI_MasterTransmit( READ | OUT_Z_XL );
	for (unsigned char i = 4; i < 6; i++){
		spiBuffer[i] = SPI_MasterTransmit(0x00);
	}
	SPI_End_Transmission();

	acc[0] = (spiBuffer[1] << 8 | spiBuffer[0]);
	acc[1] = (spiBuffer[3] << 8 | spiBuffer[2]);
	acc[2] = (spiBuffer[5] << 8 | spiBuffer[4]);
}
