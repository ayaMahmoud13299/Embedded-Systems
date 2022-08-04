

#ifndef TIM_DRIVER_H
#define TIM_DRIVER_H


void Timer_INIT(void);
void TIM_Start(void);
void TIM_Stop(void);
void set_interrupt_fun(void(*ptr)(void));
void TIM2_IRQHandler(void);




#endif
