#ifndef _KEY_H_
#define _KEY_H_
#include "main.h"
#include "lcd.h"
#include "Interrupt.h"
#include "LED.h"
#include  <stdio.h>

#define KEY1   HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)
#define KEY2   HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)
#define KEY3   HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)
#define KEY4   HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)

#define KEY1_Get   1
#define KEY2_Get   2
#define KEY3_Get   3
#define KEY4_Get   4


extern char  Show_Text[];
extern uint Key_flag;
extern int  key_get;//按键消抖标志
extern uint Pwm_PA6_duty;//PA6占空比 
int Key_scan(int Mode);
void Key_User(void);

#endif
