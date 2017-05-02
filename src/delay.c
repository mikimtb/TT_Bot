/*
 * delay.c
 *
 *  Created on: Apr 30, 2017
 *      Author: Miroslav
 */
#include "delay.h"

volatile uint32_t t_delay;
static uint32_t sys_tick_timer = 0;

void SysTick_Handler(void)
{
	/** Check whether delay_ms is set and decrement its value */
	if (t_delay != 0)
	{
		t_delay--;
	}

	/** Increment sys_tick_timer */
	sys_tick_timer++;
}

void delay_init()
{
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	(void) SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
}

void delay_ms(volatile uint32_t value)
{
	t_delay = value;
	while(t_delay != 0);
}

uint32_t millis()
{
	return sys_tick_timer;
}
