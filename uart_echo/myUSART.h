/*
 * myUSART.h
 *
 *  Created on: 14. feb. 2016
 *      Author: olafurjonthoroddsen
 */

#ifndef MYUSART_H_
#define MYUSART_H_

#include <avr/io.h>
#include <avr/interrupt.h>

void USART_init(unsigned int BaudRate);

void USART_Transmit(unsigned char data);

volatile char rxBuffer;
volatile char dataReceived;

//unsigned char USART_Recieve(void);

#endif /* MYUSART_H_ */
