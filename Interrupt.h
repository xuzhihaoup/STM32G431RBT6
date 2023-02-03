#ifndef _Interrupt_H_
#define _Interrupt_H_
#include "main.h"


extern char  Show_Text[];//LCDÏÔÊ¾»º´æÊý×é
extern int Key_scan(int Mode);
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void Uart_data_process();


#endif

