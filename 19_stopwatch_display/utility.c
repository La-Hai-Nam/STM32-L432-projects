#include "utility.h"
#include "tm1637.h"

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
	*(uint32_t*)(USART2_BASE  + 0x0C) |= (3 << 0); //set baudrate fraction 
	*(uint32_t*)(USART2_BASE  + 0x0C) |= (2 << 4); //set baudrate mantissa 

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

	long int num_decimal , remainder , quotient ;
	int a = 1 , b , var ;
	char hexanum_decimal[ 100 ] ;
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

		usart_puts(hexanum_decimal);
	
}
void led_init(){
	*(uint32_t*)(RCC_BASE  + 0x4C) |= (1 << 1); // set RCC GPIOBEN
	*(uint32_t*)(GPIOB_BASE  + 0x00) = 0xffff5e7f; // set GPIOB_PIN_3 as output (1)0xfffffe7f (2)0xffff5e7f
	*(uint32_t*)(SYSTICK + 0x00) |= (1 << 0); // enable counter
	*(uint32_t*)(SYSTICK + 0x00) |= (1 << 1); // enable systick interrupt request
	*(uint32_t*)(SYSTICK + 0x00) |= (1 << 2); // set clocksource to processorclock (4 Mhz)
	start_end = 0;
	miliseconds = 0;
	seconds = 0;
	*(uint32_t*)(GPIOB_BASE  + 0x04) |= (1<<7); //opendrain PB7 
	*(uint32_t*)(GPIOB_BASE  + 0x04) |= (1<<6); //opendrain PB6
}

void toggle_pin(){

	*(uint32_t*)(GPIOB_BASE + 0x14) ^= (1<<3);// toggle GPIOB_PIN3 depending on on off state (xor)
}

void read_gpio(){

	if((*(uint32_t*)(0x48000010) & (1<<1))){ // first button press
		// *(uint32_t*)(SYSTICK + 0x04) = 4000000; //set reload value 4Mhz
		// *(uint32_t*)(SYSTICK + 0x08) = 0; // set current systick value as 0
		// for(int button_time = *(uint32_t*)(SYSTICK + 0x08); button_time>=3940000; button_time = *(uint32_t*)(SYSTICK + 0x08)){} // 15 ms delay
		systick_delay_us(15000);
		if(!(*(uint32_t*)(0x48000010) & (1<<1))){ // debounce second press release

			switch (start_end){
				case 0:
					start_end = 1;
					usart_puts("Timer Start\r\n");
					usart_putx(0xDEADBEEF);
					toggle_pin();
					*(uint32_t*)(SYSTICK + 0x04) = 400000; //set reload value 40 khz for 10ms systick interrupts
					*(uint32_t*)(SYSTICK + 0x08) = 0; // set current systick value as 0
					/*reset 7 segment display*/
					clear_display();
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

static void systick_delay_us(uint32_t us)
{
  uint32_t systick_clock = SYSCLOCK;

  /* Recall that the systick value is a counter that is perodically decremented by the hardware until 0 is reached. Then
   * it is reset to the load value. The idea of this implementation is to get the current systick value and then to
   * calculate the systick value that would be the current one if the given amount of time has passed. We then busy wait
   * until this happens.
   *
   * Note that this is not really the way the systick should be used. Normally, you just poll against the value becoming
   * zero, which can be safely captured by reading out the appropriate flag of the systicks's control register. We
   * cannot use this safe approach here, as the systick is already used by someone else.
   *
   * For our implementation, we have at least two problems:
   *  a) depending on the clock of the time, we may not hit the exact projected value
   *  b) the counter may wrap (e.g., because the initial current value is already near the zero value)
   *
   * Note that alternatively, we could use a second timer (e.g., another harware timer or a trace timer).
   */

  uint32_t systick_val = *(uint32_t*)(SYSTICK + 0x08);
  uint32_t systick_load = *(uint32_t*)(SYSTICK + 0x04);
  uint32_t systick_delta = us * systick_clock / 1000000;
  int32_t systick_goal = systick_val - systick_delta;

  /* Becaue of b) the systick_goal can be negative, that is, we expect a single wrap. The purpose of the following is
   * to wait for the wrap to happen. */
  if (systick_goal < 0)
  {
    /* Fix the goal as we are now going to wait until the wrap happens */
    systick_goal += systick_load;

    /* Wait for the wrap to happen. We observe a wrap if the newly read out value is less than a previous
     * value (note that we have a problem here if this previous value is high, but then we assume that the delay is
     * so small that we would not wrap in the first place) */
    while (*(uint32_t*)(SYSTICK + 0x08) < systick_val);
  }

  /* The goal value is now in range, so we expect no wrap */
  while (1)
  {
    uint32_t systick_cur_val = *(uint32_t*)(SYSTICK + 0x08);

    /* Bail out if we reach the goal. Because of a) we use <= */
    if (systick_cur_val <= systick_goal)
      break;

    /* Also bail out, if the counter wrapped, which can be the case if the goal was near zero */
    if (systick_cur_val > systick_val)
      break;
  }
}



