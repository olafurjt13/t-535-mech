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
#define ODR_G2 7
#define ODR_G1 6
#define FS_G1 4
#define FS_G0 3
#define BW_G1 1
#define BW_G0 0

#define CTRL_REG1_G 0x10

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

void IMU_read_acc(int *accBuffer);
void IMU_read_gyro(int *gyroBuffer);
long sum(int *array,char size);


// Reserving space for the moving average filter to store data between calls
int accX_rawDataBuffer[10];
int accY_rawDataBuffer[10];
int accZ_rawDataBuffer[10];

int gyroX_rawDataBuffer[10];
int gyroY_rawDataBuffer[10];
int gyroZ_rawDataBuffer[10];

// Flag to let the MAF know if it needs to accumulate data, defaults to true
unsigned char accumulatingAcc = 1;
unsigned char accumulatingGyro = 1;

// An indexer to know where to put the next measurement value from the IMU
unsigned char accBufferIndexer = 0;
unsigned char gyroBufferIndexer = 0;

// Calibration variables for the gyro and accelerometer
int accCalibration[3] = {0,0,0};
int gyroCalibration[3] = {0,0,0};


void IMU_init(){

	PCICR |= ( 1 << PCIE2 );
	// Enabling pin change interrupt on PD2
	PCMSK2 |= ( 1 << PCINT18 );
	calibrationFlag = 0;

	SPI_MasterInit();

	if (whoami() != 0x68){
			unsigned char errStr[30] = "Error connecting to IMU!\0";
			myPrint(errStr,10);
	}
	accInit();
	gyroInit();
}

void accInit(){
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


void gyroInit(){
	// Set output data rate (ODR) to 952Hz, Full scale selection (FS) to 245dps and gyro bandwidth (BW) to 33Hz
	unsigned char setCtrlReg1 = ( 1 << ODR_G2 ) | ( 1 << ODR_G1 ) | ( 0 << FS_G1 ) | ( 0 << FS_G0 ) | ( 0 << BW_G1 ) | ( 0 << BW_G0);

	SPI_Initiate_Transmission();
		SPI_MasterTransmit ( WRITE | CTRL_REG1_G );
		SPI_MasterTransmit ( setCtrlReg1 );
	SPI_End_Transmission();

}

/*
 * This function initializes calibration of the gyro and accelerometer
 * For this to work, the IMU must be completely stable
 */

void calibrateIMU(){
	int accBuffer[3];
	int gyroBuffer[3];

	// Using temporary long variables to reserve space, don't need long to store the resulting variable, only intermediately
	long accCalibration_TMP[3] = {0,0,0};
	long gyroCalibration_TMP[3] = {0,0,0};

	IMU_read_acc(accBuffer);
	IMU_read_gyro(gyroBuffer);
	for(unsigned char i = 0; i < 3; i++){
		accCalibration[i] = accBuffer[i];
		gyroCalibration[i] = gyroBuffer[i];
	}

	for (unsigned int i = 0; i < 1000; i++){
		IMU_read_acc(accBuffer);
		IMU_read_gyro(gyroBuffer);
		for(unsigned char j = 0; j < 3; j++){
			accCalibration_TMP[j] = (accBuffer[j] + (i+1)*accCalibration_TMP[j])/(i+2);
			gyroCalibration_TMP[j] = (gyroBuffer[j] + (i+1)*gyroCalibration_TMP[j])/(i+2);
		}
	}
	for(unsigned char i = 0; i < 3; i++){
		accCalibration[i] = accCalibration_TMP[i];
		gyroCalibration[i] = gyroCalibration_TMP[i];
	}
	// Calibration done
	calibrationFlag = 0;
}

/*
 * This function serves as a get function for acceleration data with a moving average filter.
 * The smoothness parameter is the number of points in the moving avg filter.
 */
void readAcc(int *dataBuff,char smoothness){
	int rawdata[3];

	if (accumulatingAcc){
		for(unsigned char i = 0; i < smoothness; i++){
			IMU_read_acc(rawdata);
			accX_rawDataBuffer[i] = rawdata[0];
			accY_rawDataBuffer[i] = rawdata[1];
			accZ_rawDataBuffer[i] = rawdata[2];
		}
		dataBuff[0] = sum(accX_rawDataBuffer,smoothness)/smoothness;
		dataBuff[1] = sum(accY_rawDataBuffer,smoothness)/smoothness;
		dataBuff[2] = sum(accZ_rawDataBuffer,smoothness)/smoothness;

		accumulatingAcc = 0;
	}
	else{
		IMU_read_acc(rawdata);
		accX_rawDataBuffer[accBufferIndexer] = rawdata[0];
		accY_rawDataBuffer[accBufferIndexer] = rawdata[1];
		accZ_rawDataBuffer[accBufferIndexer] = rawdata[2];
		accBufferIndexer = accBufferIndexer + 1;
		if (accBufferIndexer == (smoothness - 1)){accBufferIndexer = 0;} // Reset the bufferIndexer

		dataBuff[0] = sum(accX_rawDataBuffer,smoothness)/smoothness - accCalibration[0];
		dataBuff[1] = sum(accY_rawDataBuffer,smoothness)/smoothness - accCalibration[1];
		dataBuff[2] = sum(accZ_rawDataBuffer,smoothness)/smoothness - (accCalibration[2] - 16384);
	}
}


/*
 * This function serves as a get function for angular rate data with built in 10 point moving average
 */
void readGyro(int *dataBuff,char smoothness){
	int rawdata[3];

	if(accumulatingGyro){
		for(unsigned char i = 0; i < smoothness; i++){
			IMU_read_gyro(rawdata);
			gyroX_rawDataBuffer[i] = rawdata[0];
			gyroY_rawDataBuffer[i] = rawdata[1];
			gyroZ_rawDataBuffer[i] = rawdata[2];
		}
		dataBuff[0] = sum(gyroX_rawDataBuffer,smoothness)/smoothness;
		dataBuff[1] = sum(gyroY_rawDataBuffer,smoothness)/smoothness;
		dataBuff[2] = sum(gyroZ_rawDataBuffer,smoothness)/smoothness;
		accumulatingGyro = 0;
	}
	else{
		IMU_read_gyro(rawdata);
		gyroX_rawDataBuffer[gyroBufferIndexer] = rawdata[0];
		gyroY_rawDataBuffer[gyroBufferIndexer] = rawdata[1];
		gyroZ_rawDataBuffer[gyroBufferIndexer] = rawdata[2];
		gyroBufferIndexer = gyroBufferIndexer + 1;
		if (gyroBufferIndexer == (smoothness - 1)){gyroBufferIndexer = 0;} // Reset the bufferIndexer

		dataBuff[0] = sum(gyroX_rawDataBuffer,smoothness)/smoothness - gyroCalibration[0];
		dataBuff[1] = sum(gyroY_rawDataBuffer,smoothness)/smoothness - gyroCalibration[1];
		dataBuff[2] = sum(gyroZ_rawDataBuffer,smoothness)/smoothness - gyroCalibration[2];
	}

}

long sum(int *array,char size){
	long output = 0;
	for(unsigned char i = 0; i < size; i++){
		output = output + (long)array[i];
	}
	return output;
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

ISR(PCINT2_vect){
	calibrationFlag = 1;
}
