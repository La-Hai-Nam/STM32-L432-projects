#include "usart.h"
#include "math.h"


static volatile uint32_t * const usart2_cr1 = (uint32_t*)(USART2_BASE + 0x00);
static volatile uint32_t * const usart2_isr = (uint32_t*)(USART2_BASE + 0x1C);
static volatile uint32_t * const usart2_tdr = (uint32_t*)(USART2_BASE + 0x28);


 
void usart_init(int baud)
{
	
	*(uint32_t*)(RCC_BASE  + 0x4C) |= (1 << 0); // set RCC GPIOAEN
	*(uint32_t*)(RCC_BASE + 0x58) |= (1 << 17); // set RCC USART2EN
	/*
	*(uint32_t*)(GPIOA_BASE  + 0x00) |= (2 << 4); //Set PA2 moder
	*(uint32_t*)(GPIOA_BASE  + 0x00) |= (2 << 6); //set PA3 moder
	*/
	*(uint32_t*)(GPIOA_BASE) = 0xabffffaf; //set PA3 and PA2 moder somehow its not possible to set it per shifting but only like this

	*(uint32_t*)(GPIOA_BASE  + 0x20) |= (7 << 8); //Set PA2 alternet function
	*(uint32_t*)(GPIOA_BASE  + 0x20) |= (7 << 12); //set PA3 alterate function


	*(uint32_t*)(USART2_BASE  + 0x00) |= 0x00; //Set all bits = 0
	*(uint32_t*)(USART2_BASE  + 0x00) |= (1 << 0); //set UE=1
	*(uint32_t*)(USART2_BASE  + 0x0C) |= (3 << 0); //set baudrate fraction ??? freq = 4000000
	*(uint32_t*)(USART2_BASE  + 0x0C) |= (2 << 4); //set baudrate mantissa ???

	*(uint32_t*)(USART2_BASE  + 0x00) |= (1 << 2); //enable receiver
	*(uint32_t*)(USART2_BASE  + 0x00) |= (1 << 3); //enable transmitter
}

/**
 * \brief Put a character to the USART
 * \param c the character to put into USART
 */
void usart_putc(uint8_t c)
{
	while (!(*(usart2_isr) & ISR_TXE));
	*usart2_tdr = c;
	while (!(*(usart2_isr) & ISR_TC));
}


void usart_puts(const char *str)
{
	char c;
	while((c = *str++)){
		usart_putc(c);
	}
}

void usart_putx(uint32_t val)
{
	uint32_t num_decimal , remainder , quotient ;
	int a = 1 , b , var ;
	unsigned char hexanum_decimal[ 10 ] ;
	quotient = val ;
	while( quotient != 0 ) {
		var = quotient % 16 ;
		if( var < 10 )
			var = var + 48 ;
		else
			var = var + 55 ;
			hexanum_decimal[ a++ ]= var ;
			quotient = quotient / 16;
	}
	usart_puts("0x");
	for(int i = a-1; i>0;i--){
		usart_putc(hexanum_decimal[i]);
	}
	
}