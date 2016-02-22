/*
 * myUSART.c
 *
 *  Created on: 14. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#include "myUSART.h"


void USART_init(unsigned int br){
	// Settin Baud Rate to br
	UBRR0H = (unsigned char) (br >> 8);
	UBRR0L = (unsigned char) br;

	// Enabling Recieve (RXEN0) and transmit (TXEN0) and
	// the RX/TX Complete Interrupt in the USART
	//Control and Status Register 0B
	UCSR0B = (1 << RXEN0) | ( 1 << TXEN0 ) | ( 1 << RXCIE0);

	// Setting frame format:
		// 2 stop bits	8 bits of data
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}


void USART_Transmit(unsigned char data){
	// Wait while the transmit buffer is not empty (UDRE0 flag in UCSR0A)
	while( !(UCSR0A & (1 << UDRE0)) ){
		;
	}
	// Write the data into the USART Data I/O register
	UDR0 = data;
}


/*
unsigned char USART_Recieve(void){
	// While the reception buffer is not empty (RXC0 flag in UCSR0A)
	while( !(UCSR0A & (1<<RXC0)) ){
		;
	}
	return UDR0;
}
*/

ISR(USART_RX_vect){
	// dataReceived flag to alert the main function of new data
	dataReceived = 1;
	// Write the data from the USART I/O register to a buffer
	rxBuffer = UDR0;
}
