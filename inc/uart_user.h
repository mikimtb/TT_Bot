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
 * @param c[out] return value
 * @return true if getc is successful, false if not
 */
bool uart_bgetc(uint8_t *c);

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

/**
 * Function un-stuff bytes and write them into input buffer
 * The data are stuffed with Consistent Overhead Byte Stuffing (COBS) algorithm
 * @param data[in] the data that should be un-stuffed
 * @return true if un-stuffed byte is written into input buffer, false if not
 */
bool unstuff_data(uint8_t data);

#endif /* UART_USER_H_ */

