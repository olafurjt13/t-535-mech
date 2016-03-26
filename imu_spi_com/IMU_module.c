/*
 * spi_com.c
 *
 *  Created on: 20. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#define IF_ADD_INC 2
#define ACC_Z 5
#define ACC_Y 4
#define ACC_X 3
#define ODR_XL2 7
#define ODR_XL1 6

#define CTRL_REG5_XL 0x1F
#define CTRL_REG6_XL 0x20
#define CTRL_REG7_XL 0x21
#define CTRL_REG8 0x22
#define INT_GEN_SRC_XL 0x25
#define STATUS_REG 0x27
#define WHO_AM_I 0x0F

#define OUT_X_XL 0x28
#define OUT_Y_XL 0x2A
#define OUT_Z_XL 0x2C
#define OUT_X_G 0x18
#define OUT_Y_G 0x1A
#define OUT_Z_G 0x1C


#define READ 0x80
#define WRITE 0x00

#define MOSI_PIN PORTB


#include "spi_comm.h"
#include "IMU_module.h"
#include "myUSART.h"


void IMU_init(){
	SPI_MasterInit();

	if (whoami() != 0x68){
			unsigned char errStr[10] = "Error!\0";
			myPrint(errStr,10);
	}

	// The bit pattern to turn on acceleration sensing in the Z,Y and X directions
	char setCtrlReg5 = ( 1 << ACC_Z ) | ( 1 << ACC_Y ) | ( 1 << ACC_X );

	// Send write address
	SPI_Initiate_Transmission();
	SPI_MasterTransmit( WRITE | CTRL_REG5_XL );

	// Send ctrlReg5 bit pattern to turn on X,Y,Z acceleration sensors
	SPI_MasterTransmit( setCtrlReg5 );
	SPI_End_Transmission();


	char setCtrlReg8 = ( 1 << IF_ADD_INC );

	// Send write address
	SPI_Initiate_Transmission();
		SPI_MasterTransmit( WRITE | CTRL_REG8 );
		// Send setCtrlReg8 bit pattern to turn auto increment registers on
		SPI_MasterTransmit( setCtrlReg8 );
	SPI_End_Transmission();
	//unsigned char doneIMU[50] = "Finished initializing IMU\0";
	//myPrint(doneIMU,30);

	// Set output data rate to 952Hz
	char setCtrlReg6 = ( 1 << ODR_XL2 ) | ( 1 << ODR_XL1 );

	SPI_Initiate_Transmission();
		SPI_MasterTransmit ( WRITE | CTRL_REG6_XL );
		SPI_MasterTransmit ( setCtrlReg6 );
	SPI_End_Transmission();


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
	for (unsigned char i = 0; i < 6; i++){
			spiBuffer[i] = SPI_MasterTransmit(0x00);
	}
	SPI_End_Transmission();

	acc[0] = (spiBuffer[1] << 8 | spiBuffer[0]);
	acc[1] = (spiBuffer[3] << 8 | spiBuffer[2]);
	acc[2] = (spiBuffer[5] << 8 | spiBuffer[4]);
}


void IMU_read_gyro(int *gyroBuffer){
	// char sendingRequest[27] = "Now sending request to IMU\n";
	// char readingFromSPDR[18] = "Reading from SPDR\n";

	char spiBuffer[6];
	int *ang_rate = gyroBuffer;

	// Select the IMU as an SPI Slave

	SPI_Initiate_Transmission();


	// Send "read from acceleration output register" signal
	SPI_MasterTransmit( READ | OUT_X_G );
	for (unsigned char i = 0; i < 6; i++){
			spiBuffer[i] = SPI_MasterTransmit(0x00);
	}
	SPI_End_Transmission();

	ang_rate[0] = (spiBuffer[1] << 8 | spiBuffer[0]);
	ang_rate[1] = (spiBuffer[3] << 8 | spiBuffer[2]);
	ang_rate[2] = (spiBuffer[5] << 8 | spiBuffer[4]);
}



char whoami(void){
	char buff;
	SPI_Initiate_Transmission();
	SPI_MasterTransmit( READ | WHO_AM_I );	// Request Identification
	buff = SPI_MasterTransmit(0x00);	// Store Identification
	SPI_End_Transmission();

	return buff;
}
