#ifndef _utility_H
#define _utility_H

#include <stdint.h>

#define NULL ((void*)0)

#define ISR_TXE (1<<7) /// transmit data register empty
#define ISR_TC (1<<6) /// transmission complete

#define USART2_BASE 0x40004400
#define GPIOA_BASE 0x48000000
#define GPIOB_BASE 0x48000400
#define GPIOB_ODR 0x48000414
#define RCC_BASE 0x40021000
#define RCC_AHB2ENR 0x4002104C
#define SYSCLOCK 4000000
#define SYSTICK 0xE000E010
#define GPIOA_MODER 0x48000000

#define BAUD 115200

// #define start_end *((volatile uint32_t*)(0x20000100))
// #define miliseconds *((volatile uint32_t*)(0x20000105))
// #define seconds *((volatile uint32_t*)(0x20000110))

extern int start_end;
extern int miliseconds;
extern int seconds;


/**
 * Initialize the USART
 * @param baud specifies the designated baudrate
 */
void usart_init(int baud);

/**
*Sends a single character to the default USART transmitter in blocking mode
*@param c specifies the character to be send
*/
void usart_putc(uint8_t c);
/**
 * Put a string into the USART
 * @param str specifies the string to put into USART
 */
void usart_puts(const char *str);

/**
 * Put the param val as a Hexadecimal into a string and output that into USART.
 * \param val specifies the ASCII/Hexadecimal to put into USART
 */
void usart_putx(uint32_t val);

/**
 * initializes every register for the usage of the led including for SYSTICK as well as the variables for the time shown on the display and opendrain for the display pins
 */

void led_init();
/**
 * either turns on the GPIO or turns it off
 */
void toggle_pin();
/**
 * read buttonpin with debounce and set start_end variable so that stopwatch is able to start and end 
 */
void read_gpio();
/**
 * waits a specific amount of time /delay
 * \param us desired time in microseconds
 */
static void systick_delay_us(uint32_t us);

#endif