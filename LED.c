#include "LED.h"
#include "lcd.h"
#include  "stdio.h"
#include "tim.h"

extern int light_flag;
void LED_Show(uchar led_num)
{  
   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOC,led_num<<8,GPIO_PIN_RESET);
   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
	
}
void TIME_SET(int mode)
{  
	if(mode)
	{
 		light_flag = 1;
		LCD_DisplayStringLine(Line4, (uint8_t *)"Machine ON/OFF:ON   ");
	    
	}
	else
	{   
		light_flag = 0;
		LCD_DisplayStringLine(Line4, (uint8_t *)"Machine ON/OFF:OFF  ");
	}
  
}




void LED_LD8(int mode)
{
  switch(mode)
  {
	  case 1: 		
		 Pwm_PA6_duty=200;
		  __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,Pwm_PA6_duty);
		 sprintf(Show_Text,"Brightness:%3d%% ",Pwm_PA6_duty/10);
		 LCD_DisplayStringLine(Line4, (uint8_t *)Show_Text);
	     break;
      case 2: 		
		 Pwm_PA6_duty=500;
		  __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,Pwm_PA6_duty);
		 sprintf(Show_Text,"Brightness:%3d%% ",Pwm_PA6_duty/10);
		 LCD_DisplayStringLine(Line4, (uint8_t *)Show_Text);
	     break;
	  case 3: 		
		 Pwm_PA6_duty=800;
		  __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,Pwm_PA6_duty);
		 sprintf(Show_Text,"Brightness:%3d%% ",Pwm_PA6_duty/10);
		 LCD_DisplayStringLine(Line4, (uint8_t *)Show_Text);
	     break;
	  case 4:
		  Pwm_PA6_duty=1000;
		  __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,Pwm_PA6_duty);
		 sprintf(Show_Text,"Brightness:%3d%%  ",Pwm_PA6_duty/10);
		 LCD_DisplayStringLine(Line4, (uint8_t *)Show_Text);
	     break;
	  case 5:
		  Pwm_PA6_duty=0;
		  __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,Pwm_PA6_duty);
		 sprintf(Show_Text,"Brightness:%3d%%  ",Pwm_PA6_duty/10);
		 LCD_DisplayStringLine(Line4, (uint8_t *)Show_Text);
	     break;
  
  }


}



void LED_Show_On_num(int led_num)
{ 
   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);
   switch(led_num)
   {
	   case 1: HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);break;  
       case 2: HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);break; 
       case 3: HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);break;
       case 4: HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET);break; 
       case 5: HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET);break; 
       case 6: HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);break; 
       case 7: HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_RESET);break; 
       case 8: HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_RESET);break; 
       default: break;
   }
   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
	
}


