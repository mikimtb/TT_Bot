/*
 * uart_user.c
 *
 *  Created on: Mar 30, 2017
 *      Author: Miroslav Bozic
 */
#include "uart_user.h"

/**
 * Global variables
 */
static t_buffer in = {{0}, 0, 0};			/** UART input buffer */
static t_buffer out = {{0}, 0, 0};			/** UART output buffer */

void USART2_IRQHandler(void)
{
	/** Check whether USART2 IRQ handler is triggered by receive interrupt */
	if (USART_GetITStatus(USART2, USART_IT_RXNE))
	{
		uint8_t t;

		/** Clear receive interrupt flag */
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		/** Receive incoming character */
		uint16_t result_word = USART_ReceiveData(USART2);
		/**
		 * User code starting here
		 */
		in.uart_buffer[in.next_in] = (uint8_t)result_word;
		t = in.next_in;
		in.next_in = (in.next_in + 1) % UART_BUFFER_SIZE;
		if (in.next_in == in.next_out)
		{
			in.next_in = t;									/** Buffer is full */
		}
		/**
		 * User code ends here
		 */
	}
	/** Check whether USART2 IRQ handler is triggered by transmit interrupt */
	else if (USART_GetITStatus(USART2, USART_IT_TXE))
	{
		/** Clear transmit interrupt flag */
		USART_ClearITPendingBit(USART2, USART_IT_TXE);

		/**
		 * User code starting here
		 */
		if (out.next_in != out.next_out)
		{
			USART_SendData(USART2, out.uart_buffer[out.next_out]);
			out.next_out = (out.next_out + 1) % UART_BUFFER_SIZE;
		}
		else
		{
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
		}
		/**
		 * User code ends here
		 */
	}
}

int uart_init(uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStruct; 		/** this is for the GPIO pins used as TX and RX */
	USART_InitTypeDef USART_InitStruct; 	/** this is for the USART2	 initialization */
	NVIC_InitTypeDef NVIC_InitStructure; 	/** this is used to configure the NVIC (nested vector interrupt controller) */

	/**
	 * Enable the peripheral clock for the pins used by
	 * USART2, PA2 for TX and PA3 for RX
	 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/**
	 * The RX and TX pins are now connected to their AF
	 * so that the USART2 can take over control of the pins
	 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	/**
	 * This sequence sets up the TX and RX pins
	 * so they work correctly with the USART2 peripheral
	 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; /** Pins 2 (TX) and 3 (RX) are used */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; 			/** the pins are configured as alternate function so the USART peripheral has access to them */
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		/** this defines the IO speed and has nothing to do with the baudrate */
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;			/** this defines the output type as push pull mode (as opposed to open drain) */
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			/** this activates the pull up resistors on the IO pins */
	GPIO_Init(GPIOA, &GPIO_InitStruct);					/** now all the values are passed to the GPIO_Init() function which sets the GPIO registers */

	/**
		 * Enable APB1 peripheral clock for USART2
		 */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/**
	 * Now the USART_InitStruct is used to define the properties of USART2
	 */
	USART_InitStruct.USART_BaudRate = baudrate;										/** the baudrate is set to the value we passed into this init function */
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;						/** we want the data frame size to be 8 bits (standard) */
	USART_InitStruct.USART_StopBits = USART_StopBits_1;								/** we want 1 stop bit (standard) */
	USART_InitStruct.USART_Parity = USART_Parity_No;								/** we don't want a parity bit (standard) */
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 	/** we don't want flow control (standard) */
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 					/** we want to enable the transmitter and the receiver */
	USART_Init(USART2, &USART_InitStruct);											/** again all the properties are passed to the USART_Init
																					 *  function which takes care of all the bit setting */
	/**
	 * Here the USART2 receive interrupt is enabled
	 * and the interrupt controller is configured
	 * to jump to the USART2_IRQHandler() function
	 * if the USART2 receive interrupt occurs
	 */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		 						/** we want to configure the USART2 interrupts */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;						/** this sets the priority group of the USART2 interrupts */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 						/** this sets the subpriority inside the group */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 						/** the USART2 interrupts are globally enabled */
	NVIC_Init(&NVIC_InitStructure);							 						/** the properties are passed to the NVIC_Init */

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 									/** enable the USART2 receive interrupt */

	// finally this enables the complete USART2 peripheral
	USART_Cmd(USART2, ENABLE);

	return NO_ERROR;
}

uint8_t uart_bgetc()
{
	uint8_t c = in.uart_buffer[in.next_out];
	in.next_out = (in.next_out + 1) % UART_BUFFER_SIZE;

	return c;
}

void uart_bputc(uint8_t c)
{
	bool restart;
	uint8_t ni;

	restart = out.next_in == out.next_out;
	out.uart_buffer[out.next_in] = c;
	ni = (out.next_in + 1) % UART_BUFFER_SIZE;
	while (ni == out.next_out);
	out.next_in = ni;
	if (restart)
	{
		USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	}
}

bool uart_bkbhit()
{
	return in.next_in!=in.next_out;
}


