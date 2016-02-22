#include <avr/io.h>

int main()
{
	DDRB=0x20;
	while(1)
	{
		PORTB=0x20;
		PORTB=0;
	}
	return 0;
}
