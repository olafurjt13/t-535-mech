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
void debounceTimerInit();


// Reserving space for the moving average filter to store data between calls
int accX_rawDataBuffer[20];
int accY_rawDataBuffer[20];
int accZ_rawDataBuffer[20];

int gyroX_rawDataBuffer[20];
int gyroY_rawDataBuffer[20];
int gyroZ_rawDataBuffer[20];

// Flag to let the MAF know if it needs to accumulate data, defaults to true
unsigned char accumulatingAcc = 1;
unsigned char accumulatingGyro = 1;

// An indexer to know where to put the next measurement value from the IMU
unsigned char accBufferIndexer = 0;
unsigned char gyroBufferIndexer = 0;

// Calibration variables for the gyro and accelerometer
int accCalOffset[3] = {0,0,0};
int gyroCalOffset[3] = {0,0,0};

// Debounce variables
unsigned char debounceCounter = 0;
unsigned char idle = 0;
unsigned char idleCounter = 0;
char lastReading = 0;


void IMU_init(){
	calibrationFlag = 0;

	SPI_MasterInit();
	char identification = whoami();
	if (identification != 0x68){
			unsigned char errStr[30] = "Error connecting to IMU!\0";
			myPrintNL(errStr,30);
			USART_Transmit_8_hex(whoami());
	}
	accInit();
	gyroInit();
	debounceTimerInit();
}

void debounceTimerInit(){
	// Initialize the counter at 0
	TCNT1L = 0;
	TCNT1H = 0;
	// Enable Output Compare Match Interrupt
	TIMSK1 |= ( 1 << OCIE1B );
	// Clearing interrupt flags (writing 1 to them => clearing)
	TIFR1 = ( 1 << OCF1B );
	// Set the output compare to 250 => 1ms with 1/64t prescaling
	OCR1B |= 250;
	// Set the clock to 1/64th prescaling
	TCCR1B |= ( 1 << CS11 ) | ( 1 << CS10 );
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

	SPI_Initiate_Transmission();
		SPI_MasterTransmit( READ | CTRL_REG8 );
		SPI_MasterTransmit(0x00);
	SPI_End_Transmission();


	// Set output data rate to 952Hz
	char setCtrlReg6 = ( 1 << ODR_XL2 ) | ( 1 << ODR_XL1 );
	SPI_Initiate_Transmission();
		SPI_MasterTransmit ( WRITE | CTRL_REG6_XL );
		SPI_MasterTransmit ( setCtrlReg6 );
	SPI_End_Transmission();

	unsigned char message[20] = "ACC initialized\0";
	myPrintNL(message,20);
}


void gyroInit(){
	// Set output data rate (ODR) to 952Hz, Full scale selection (FS) to 245dps and gyro bandwidth (BW) to 33Hz
	unsigned char setCtrlReg1 = ( 1 << ODR_G2 ) | ( 1 << ODR_G1 ) | ( 0 << FS_G1 ) | ( 0 << FS_G0 ) | ( 0 << BW_G1 ) | ( 0 << BW_G0);

	SPI_Initiate_Transmission();
		SPI_MasterTransmit ( WRITE | CTRL_REG1_G );
		SPI_MasterTransmit ( setCtrlReg1 );
	SPI_End_Transmission();

	unsigned char message[20] = "GYRO initialized\0";
	myPrintNL(message,20);
}

/*
 * This function initializes calibration of the gyro and accelerometer
 * For this to work, the IMU must be completely stable
 */


void calibrateIMU(){
	int accBuffer[3];
	int gyroBuffer[3];

	// Using temporary long variables to reserve space, don't need long to store the resulting variable, only intermediately
	long accCalOffset_TMP[3] = {0,0,0};
	long gyroCalOffset_TMP[3];

	IMU_read_acc(accBuffer);
	IMU_read_gyro(gyroBuffer);
	for(unsigned char i = 0; i < 3; i++){
		accCalOffset_TMP[i] = accBuffer[i];
		gyroCalOffset_TMP[i] = gyroBuffer[i];
	}
	// Calulating the cumulative average of 1000 data points
	for (unsigned int i = 0; i < 1000; i++){
		IMU_read_acc(accBuffer);
		for(unsigned char j = 0; j < 3; j++){
			accCalOffset_TMP[j] = (accBuffer[j] + (i+1)*accCalOffset_TMP[j])/(i+2);
		}
	}
	for (unsigned char i = 0; i < 100; i++){
			IMU_read_gyro(gyroBuffer);
			for (unsigned char j = 0; j < 3; j++){
				gyroCalOffset_TMP[j] = (gyroBuffer[j] + (i+1)*gyroCalOffset_TMP[j])/(i+2);

			}
		}
	for(unsigned char i = 0; i < 3; i++){
		// The x axis should be in line with gravity when calibration is performed
		if(i == 0){
			accCalOffset[i] = accCalOffset_TMP[i] + 16384;
			gyroCalOffset[i] = gyroCalOffset_TMP[i];
		}
		else{
			accCalOffset[i] = accCalOffset_TMP[i];
			gyroCalOffset[i] = gyroCalOffset_TMP[i];
		}
	}
	// Calibration done
	calibrationFlag = 0;
	unsigned char message[25] = "Calibration Done\0";
	myPrintNL(message,20);
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

		dataBuff[0] = sum(accX_rawDataBuffer,smoothness)/smoothness - accCalOffset[0];
		dataBuff[1] = sum(accY_rawDataBuffer,smoothness)/smoothness - accCalOffset[1];
		dataBuff[2] = sum(accZ_rawDataBuffer,smoothness)/smoothness - accCalOffset[2];
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

		dataBuff[0] = sum(gyroX_rawDataBuffer,smoothness)/smoothness - gyroCalOffset[0];
		dataBuff[1] = sum(gyroY_rawDataBuffer,smoothness)/smoothness - gyroCalOffset[1];
		dataBuff[2] = sum(gyroZ_rawDataBuffer,smoothness)/smoothness - gyroCalOffset[2];
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
	// Send "read from gyroscope output register" signal
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

ISR(TIMER1_COMPB_vect){
	asm("cli");
	OCR1B += 250;
	if(idle){
		if(idleCounter > 200){idle = 0;}
		idleCounter++;
	}
	else{
		char reading = PIND & ( 1 << PD2);
			if( reading != lastReading ){
				debounceCounter = 0;
			}
			else if(reading ==  ( 1 << PD2 )){
				debounceCounter++;
			}
			if (debounceCounter > 50){
				calibrationFlag = 1;
				idle = 1;
				idleCounter = 0;
				debounceCounter = 0;
			}
			lastReading = reading;
	}
	asm("sei");
}
