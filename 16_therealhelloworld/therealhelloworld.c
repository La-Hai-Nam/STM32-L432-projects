#include "usart.h"


void _start()
{
	usart_init(BAUD);
	while(1)
	{
		usart_puts("Hello World\n\r");
		usart_putx(0xDEADBEEF);
		usart_puts("\r\n");
	}
}

