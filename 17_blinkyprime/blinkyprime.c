#include "usart.h"


void _start()
{
	usart_init(BAUD);
	led_init();
	while(1)
	{
		usart_puts("Blinkyprime\n\r");
		usart_putx(0xDEADBEEF);
		usart_puts("\r\n");
		wfi();
	}
}


void wfi()
{
	__asm__ volatile ("wfi" : : : "memory");
}


void systick()
{
	toggle_pin();	//systick interrupt calls toggle pin every second
}