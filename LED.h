#ifndef _LED_H_
#define _LED_H_
#include "main.h"

void LED_Show_On_num(int led_num);
void LED_Show(uchar led_num);
void LED_LD8(int mode);
void TIME_SET(int mode);

extern char  Show_Text[];//LCDÏÔÊ¾»º´æÊý×é
extern uint  Pwm_PA6_duty;
#endif

