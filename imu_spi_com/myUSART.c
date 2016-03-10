/*
 * myUSART.c
 *
 *  Created on: 14. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#include "myUSART.h"


#define convert2ASCII_hex(byte){	\
	if(byte <= 0x09){byte += 0x30;}			\
	else if(byte <= 0x0F){byte += 0x37;};	\
}

void USART_Transmit(unsigned char data){
	/* While the transmit buffer is not empty (UDRE0 flag in UCSR0A)	*/
	while( !(UCSR0A & (1 << UDRE0)) ){};
	/* Write the data into the USART Data I/O register	*/
	UDR0 = data;
}


void USART_init(unsigned int br){
	unsigned int ubrr = 16000000/16/br - 1;
	// Settin Baud Rate to br
	UBRR0H = (unsigned char) (ubrr >> 8);
	UBRR0L = (unsigned char) ubrr;

	// Enabling Receive (RXEN0) and transmit (TXEN0) and
	// the RX Complete Interrupt in the USART
	//Control and Status Register 0B
	UCSR0B = (1 << RXEN0) | ( 1 << TXEN0 ) | ( 1 << RXCIE0);

	// Setting frame format:
			// 2 stop bits		8 bit
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}


void USART_Transmit_32_hex(long data32bit){
	unsigned char N[8];
	unsigned char *ptr = &N[0];
	for (uint8_t i = 0; i < 8; i++){
		N[i] = ((data32bit >> 4*i) & 0xF);
	}
	for (unsigned char i = 0; i < 8; i++){
		convert2ASCII_hex(*(ptr+7))	// The MSN is last in the memory so we loop backwards
		USART_Transmit(*(ptr+7));
		ptr--;
	}
	USART_Transmit(0x0A);
}


void USART_Transmit_16_hex(int data16bit){
	unsigned char N_1 = (data16bit & 0x000F);
	unsigned char N_2 = ((data16bit >> 4 ) & 0x000F);
	unsigned char N_3 = ((data16bit >> 8 ) & 0x000F);
	unsigned char N_4 = ((data16bit >> 12 ) & 0x000F);

	convert2ASCII_hex(N_1);
	convert2ASCII_hex(N_2);
	convert2ASCII_hex(N_3);
	convert2ASCII_hex(N_4);

	USART_Transmit('0');
	USART_Transmit('x');

	USART_Transmit(N_4);
	USART_Transmit(N_3);
	USART_Transmit(N_2);
	USART_Transmit(N_1);
	USART_Transmit(0x0A);
}



void USART_Transmit_8_hex(unsigned char byte){
	unsigned char LSN = (byte & 0x0F);
	unsigned char MSN = ((byte >> 4) & 0x0F);

	convert2ASCII_hex(LSN);
	convert2ASCII_hex(MSN);

	USART_Transmit('0');
	USART_Transmit('x');
	USART_Transmit(MSN);
	USART_Transmit(LSN);
	USART_Transmit(0x0A);
}

void USART_Transmit_8_dec(unsigned char byte){
	if(byte < 10){
		USART_Transmit(byte + 48);
		USART_Transmit(0x0A);
	}
	else if(byte < 100){
		USART_Transmit((byte / 10) + 48);
		USART_Transmit((byte % 10) + 48);
		USART_Transmit(0x0A);
	}
	else if(byte >= 100){
		USART_Transmit((byte / 100) + 48);
		USART_Transmit(((byte - (byte/100)*100) / 10) + 48);
		USART_Transmit(((byte - (byte/100)*100) % 10) + 48);
		USART_Transmit(0x0A);
	}
}

void USART_Transmit_16_dec(unsigned int word){
	if(word < 10){
		USART_Transmit(word + 48);
		USART_Transmit(0x0A);
	}
	else if(word < 100){
		USART_Transmit((word / 10) + 48);
		USART_Transmit((word % 10) + 48);
		USART_Transmit(0x0A);
	}
	else if(word < 1000){
		USART_Transmit((word / 100) + 48);
		USART_Transmit(((word - (word/100)*100) / 10) + 48);
		USART_Transmit(((word - (word/100)*100) % 10) + 48);
		USART_Transmit(0x0A);
	}
	else if(word < 10000){
		USART_Transmit((word / 1000) + 48);
		USART_Transmit(((word - (word/1000)*1000) / 100) + 48);
		USART_Transmit((((word - (word/1000)*1000) % 100) / 10) + 48);
		USART_Transmit((( (word - (word/1000)*1000) % 100) - (word - ((word/1000)*1000) % 100)/10)*10 +48);
		USART_Transmit(0x0A);
	}
	else{
		USART_Transmit('q');
	}
}


void myPrint(unsigned char str[], unsigned char len){
	for(unsigned char i = 0; i < len; i++){
		if(str[i] == '\0'){
			break;
		}
		USART_Transmit(str[i]);
	}
	USART_Transmit(0x0A);
}


ISR(USART_RX_vect){
	// dataReceived variable to allow the main function to know when new data is available
	dataReceived = 1;
	// Write the data from the USART I/O register to a buffer
	rxBuffer = UDR0;
}

