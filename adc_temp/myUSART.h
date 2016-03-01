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

void USART_Transmit_8_hex(unsigned char byte);
void USART_Transmit_16_hex(int data16bit);
void USART_Transmit_32_hex(long data32bit);
void USART_Transmit_8_dec(unsigned char byte);
void USART_Transmit_16_dec(unsigned int word);
void myPrint(unsigned char str[], unsigned char len);

volatile char rxBuffer;
volatile char dataReceived;

//unsigned char USART_Recieve(void);

#endif /* MYUSART_H_ */
