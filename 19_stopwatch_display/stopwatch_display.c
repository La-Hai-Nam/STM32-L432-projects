#include "utility.h"
#include "tm1637.h"

static tm1637_t disp;
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

static void tm_delay_us(struct tm1637 *tm, uint32_t us)
{
	// int delay = (SYSCLOCK/1000000) * us;
	// for(int delay_time = *(uint32_t*)(SYSTICK + 0x08); delay_time>=delay; delay_time = *(uint32_t*)(SYSTICK + 0x08)){} // us delay
   systick_delay_us(us);
}

static void tm_write_clk(struct tm1637 *tm, unsigned int val)
{
   // HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, val?GPIO_PIN_SET:GPIO_PIN_RESET);
    	(val) ? (*(uint32_t*)(GPIOB_BASE + 0x18) |= (1<<6)):(*(uint32_t*)(GPIOB_BASE + 0x18) |= (1<<22)); //set or unset PB6 CLK

}

static void tm_write_dio(struct tm1637 *tm, unsigned int val)
{
   // HAL_GPIO_WritePin(SDIO_GPIO_Port, SDIO_Pin, val?GPIO_PIN_SET:GPIO_PIN_RESET);
    (val) ? (*(uint32_t*)(GPIOB_BASE + 0x18)) |= (1<<7):(*(uint32_t*)(GPIOB_BASE + 0x18) |= (1<<23)); // set or unset PB7 DIO
}

static unsigned int tm_read_dio(struct tm1637 *tm)
{

    return *(uint32_t*)(GPIOB_BASE + 0x10) & (1<<7);
}

void _start()
{
	/*inits*/
	usart_init(BAUD);
	led_init();
	tm1637_init(&disp, tm_delay_us, tm_write_clk, tm_read_dio, tm_write_dio, NULL);
	/*print out nAM to 7 segment display*/
	static const uint8_t seg_name[] =  {0x54, 0x77, 0x37};
	tm1637_write_segment(&disp, seg_name, 4, 1);
	while(1){
		read_gpio();
	}
}

void wfi()
{
	__asm__ volatile ("wfi" : : : "memory");
}


void systick()
{
	int ms_counter = miliseconds;
	int sec_counter = seconds;
	char str_ms[3];
	char str_s[3];
	int time;
	static const uint8_t seg_EMPTY[] = {0, 0, 0, 0};
	/*depending on start_end state prints out tiem or not*/
	switch (start_end){
	case 0:
		break;
	case 1:
		ms_counter++;
		miliseconds = ms_counter;
		if(miliseconds == 10){
			miliseconds = 0;
			sec_counter++;
			seconds = sec_counter;
		}
		if(sec_counter == 60){
			start_end = 0;
			miliseconds = 0;
			seconds = 0;
			toggle_pin();
			usart_puts("Timer Stop\r\n");
			break;
		}
		/*print time on display depending on time state so that it looks right*/
		if(seconds < 1){
			time = miliseconds*10;
			tm1637_write_uint(&disp,time, 2);
		}
		else if(seconds < 10){
			time = seconds*100+miliseconds*10;
			tm1637_write_uint(&disp,time, 1);
		}else{
			time = seconds*100+miliseconds*10;	
			tm1637_write_uint(&disp,time, 0);
		}
		/*print the time to 7 segment display*/
		//tm1637_write_uint(&disp,miliseconds*10, 2);
		//tm1637_write_uint(&disp,time, 3);
		/*print the time to usart*/
		// sprintf(str_ms,"%d\r\n",miliseconds);
		// sprintf(str_s,"%d",seconds);
		// strcat(str_s, ",");
		// strcat(str_s,str_ms);
		// usart_puts(str_s);
	}

}	

void clear_display(){
	static const uint8_t seg_EMPTY[] = {0, 0, 0, 0};
	tm1637_write_segment(&disp, seg_EMPTY, 4, 0);

}