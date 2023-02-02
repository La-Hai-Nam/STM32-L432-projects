#include "utility.h"

int new_value[2];
char buf_temp[2];

void _start()
{
	usart_init(BAUD);
	led_init();
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
	// if(seconds == 0){
	// 	sec_counter = 0;
	// 	seconds = 0;
	// }else{
	// 	sec_counter = seconds;
	// }
	char str_ms[2];
	char str_s[2];
	switch (start_end){
	case 0:
		break;
	case 1:
		ms_counter++;
		miliseconds = ms_counter;
		if(miliseconds == 100){
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

		int2string(seconds);
		usart_putc(':');
		int2string(miliseconds);
		usart_puts("\r\n");
		break;
	}
	
}	


void int2string(int val){

    for (int x = 1; x >= 0; x--) {
        new_value[x] = val % 10;
        val = val / 10;
        buf_temp[x] = new_value[x] + 48;
        }
		usart_puts(buf_temp);
}