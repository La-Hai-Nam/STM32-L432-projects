#include "utility.h"

start_end = 0;
miliseconds = 0;
seconds = 0;

void usart_init(int baud)
{

	*(uint32_t*)(RCC_BASE  + 0x4C) |= (1 << 0); // set RCC GPIOAEN
	*(uint32_t*)(RCC_BASE + 0x58) |= (1 << 17); // set RCC USART2EN
	/*
	*(uint32_t*)(GPIOA_BASE  + 0x00) |= (2 << 4); //Set PA2 moder
	*(uint32_t*)(GPIOA_BASE  + 0x00) |= (2 << 6); //set PA3 moder
	*/
	*(uint32_t*)(GPIOA_BASE) = 0xabffffa3;//0xabffffaf; //set PA3, PA2 and PA1 moder ( both USART RX/TX and Button gpio)

	*(uint32_t*)(GPIOA_BASE  + 0x20) |= (7 << 8); //Set PA2 alternet function
	*(uint32_t*)(GPIOA_BASE  + 0x20) |= (7 << 12); //set PA3 alterate function


	*(uint32_t*)(USART2_BASE  + 0x00) |= 0x00; //Set all bits = 0
	*(uint32_t*)(USART2_BASE  + 0x00) |= (1 << 0); //set UE=1
	*(uint32_t*)(USART2_BASE  + 0x0C) |= (3 << 0); //set baudrate fraction ???
	*(uint32_t*)(USART2_BASE  + 0x0C) |= (2 << 4); //set baudrate mantissa ???

	*(uint32_t*)(USART2_BASE  + 0x00) |= (1 << 2); //enable receiver
	*(uint32_t*)(USART2_BASE  + 0x00) |= (1 << 3); //enable transmitter
}


void usart_putc(uint8_t c)
{

	while (!(*(uint32_t*)(USART2_BASE + 0x1C) & ISR_TXE));
	*(uint32_t*)(USART2_BASE + 0x28) = c;
	while (!(*(uint32_t*)(USART2_BASE + 0x1C) & ISR_TC));
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
void led_init(){
	*(uint32_t*)(RCC_BASE  + 0x4C) |= (1 << 1); // set RCC GPIOBEN
	*(uint32_t*)(GPIOB_BASE  + 0x00) = 0xfffffe7f; // set GPIOB_PIN_3 as output (1)0xfffffe7f (2)0xffff5e7f
	*(uint32_t*)(SYSTICK + 0x00) |= (1 << 0); // enable counter
	*(uint32_t*)(SYSTICK + 0x00) |= (1 << 1); // enable systick interrupt request
	*(uint32_t*)(SYSTICK + 0x00) |= (1 << 2); // set clocksource to processorclock (4 Mhz)
	start_end = 0;
	miliseconds = 0;
	seconds = 0;

}

void toggle_pin(){

	*(uint32_t*)(GPIOB_BASE + 0x14) ^= (1<<3);// toggle GPIOB_PIN3 depending on on off state (xor)
}

void read_gpio(){

	if((*(uint32_t*)(0x48000010) & (1<<1))){ // first button press
		*(uint32_t*)(SYSTICK + 0x04) = 4000000; //set reload value 4Mhz
		*(uint32_t*)(SYSTICK + 0x08) = 0; // set current systick value as 0
		for(int button_time = *(uint32_t*)(SYSTICK + 0x08); button_time>=3940000; button_time = *(uint32_t*)(SYSTICK + 0x08)){} // 15 ms delay
		if(!(*(uint32_t*)(0x48000010) & (1<<1))){ // debounce second press release

			switch (start_end){
				case 0:
					start_end = 1;
					usart_puts("Timer Start\r\n");
					usart_puts("\r\n");
					toggle_pin();
					*(uint32_t*)(SYSTICK + 0x04) = 40000; //set reload value 40 khz for 10ms systick interrupts
					*(uint32_t*)(SYSTICK + 0x08) = 0; // set current systick value as 0
					break;
				case 1:
					start_end = 0;
					miliseconds = 0;
					seconds = 0;
					toggle_pin();
					usart_puts("Timer Stop\r\n");
					break;
			}
		}
	}
}


