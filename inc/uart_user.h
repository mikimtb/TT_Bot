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

#define ERROR -1
#define NO_ERROR 0

/**
 * Function initialize UART2 port connected on pins pack 1: PA2->TX, PA3->RX
 * @param[in] baudrate selected communication speed
 * @return	error code, 0 for successful configuration, -1 for fail
 */
int uart_init(uint32_t baudrate);

/**
 * Function print text into the console
 * @param[in] USARTx serial port that should be used
 * @param[in] s pointer to string that should be sent
 * @return	error code, 0 for successful configuration, -1 for fail
 */
void uart_printf(USART_TypeDef *USARTx, char *s);

/**
 * Function read byte from UART port
 * @param USARTx selected UART port
 * @return received byte from UART
 */
uint8_t uart_ReadByte(USART_TypeDef *USARTx);

/**
 * Function send single byte to UART port
 * @param USARTx selected UART port
 * @param data byte that should be sent
 */
void uart_WriteByte(USART_TypeDef *USARTx, uint8_t data);

#endif /* UART_USER_H_ */

