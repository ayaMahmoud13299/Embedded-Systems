#include "timer_driver.h"
#include "stm32f4xx.h"



void(*global)(void)=(void*)0;
void Timer_INIT(void)
{
	/*clock enable of TIMER1*/
	RCC->APB1ENR |=(1u<<0);
	/* time base unit*/

	/*  fCK_PSC / (PSC[15:0] + 1) to get timer clock with 1000 HZ(1 ms) */
	TIM2 ->PSC =16000-1;
	/*update counter every 1 second */
	TIM2->ARR = 1000;
	TIM2-> CR1 |=(1<<7);
	/* Counter used as upcounter */
	TIM2-> CR1 &= ~(1<<4);

	/*clear timer counter*/
		//TIM2->CNT = 0;

	/*Enable  timer*/
	TIM2->CR1 |= (1u<<0);

}


void TIM_Start()
{
	/*Enable timer interrupt*/
	TIM2->DIER |= (1<<0);
	/*Enable timer interrupt NVIC*/
	NVIC_EnableIRQ(TIM2_IRQn);

}
void TIM_Stop()
{
	/*Enable timer interrupt NVIC*/
	NVIC_DisableIRQ(TIM2_IRQn);
	/*Enable timer interrupt*/
	TIM2->DIER &= ~(1u<<0);
}
void set_interrupt_fun(void(*ptr)(void))
{
	global=ptr;

}
void TIM2_IRQHandler()
{
if(	(TIM2-> SR >>0) &1)
{
	global();
}
}
