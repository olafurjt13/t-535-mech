/*
 * main.c
 *
 *  Created on: 10. feb. 2016
 *      Author: olafurjonthoroddsen
 */


#include <avr/io.h>

#include "led.h"
#include "myDelay.h"

int main(){
	LED_Init();
	while(1){
		delay_milliseconds(1000);
	}
	return 0;
}
