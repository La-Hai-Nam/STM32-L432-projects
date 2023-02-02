#ifndef THEREALHELLOWORLD_USART_H
#define THEREALHELLOWORLD_USART_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "math.h"

#define null NULL

#define ISR_TXE (1<<7) /// transmit data register empty
#define ISR_TC (1<<6) /// transmission complete

#define USART2_BASE 0x40004400
#define GPIOA_BASE 0x48000000
#define RCC_BASE 0x40021000
#define RCC_AHB2ENR 0x4002104C
#define SYSCLOCK 32000000
#define GPIOA_MODER 0x48000000

#define BAUD 115200


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

#endif