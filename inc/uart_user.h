/*
 * uart_user.h
 *
 *  Created on: Mar 30, 2017
 *      Author: Miroslav Bozic
 */

#ifndef UART_USER_H_
#define UART_USER_H_

#include <stm32f4xx.h>
#include <stm32f4xx_usart.h>
#include <stdbool.h>
#include "definitions.h"

/**
 * Constant definitions
 */
#define UART_RX_TIMOUT_ENABLE	0		/** Defines whether timeout is enabled or disabled */
#define UART_RX_TIMEOUT			1		/** Defines USART timeout period in milliseconds */
#define UART_BUFFER_SIZE		64		/** Defines USART buffer maximal length */
/**
 * Type definitions
 */
typedef struct buffer
{
	uint8_t uart_buffer[UART_BUFFER_SIZE];
	uint8_t next_in;
	uint8_t next_out;
} t_buffer;

/**
 * USART triggered interrupts handler function
 */
void USART2_IRQHandler(void);

/**
 * Function initialize UART2 port connected on pins pack 1: PA2->TX, PA3->RX
 * @param[in] baudrate selected communication speed
 * @return	error code, 0 for successful configuration, -1 for fail
 */
int uart_init(uint32_t baudrate);

/**
 * Function returns one byte from UART input buffer
 * @return first byte that is written to the uart input buffer
 */
uint8_t uart_bgetc();

/**
 * Function write one byte into uart output buffer
 * @param c[in] the byte that should be written
 */
void uart_bputc(uint8_t c);

/**
 * Function check whether the bytes are existing inside input buffer
 * @return true if data are existing, false if there is no data in the buffer
 */
bool uart_bkbhit();

#endif /* UART_USER_H_ */

