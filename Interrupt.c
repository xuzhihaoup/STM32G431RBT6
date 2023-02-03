#include "Interrupt.h"
#include "lcd.h"
#include "LED.h"
#include "adc.h"
#include "tim.h"
#include "userADC.h"
#include "usart.h"
#include "stdio.h"
#include <string.h>
uint Key_flag = 0 ;
int  key_get=0;//按键消抖标志
u16  time_clc_ms = 0;
u16  time_clc_s = 0;
u16  sum_time= 0;
int light_flag=0;
extern int down_time_open;
char RX_data[60];
uint8_t rx_data;
uchar rx_pointer=0;
uint data_get_ok=0;
uint STSTART = 0,STPWM=0,STMON_OFF=0,STEND=0;
uint STTIM_hour=0,STTIM_min=0,STTIM_miao=0,STK_keeptime=0;
//中断回调函数  中断服务函数调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{   
  if(htim->Instance==(TIM17))
  {  

     if(down_time_open)
	 {		 
		  time_clc_ms++;
		  if(time_clc_ms==100)
		  {  
			 time_clc_ms=0;
			 time_clc_s++;//转换为S
			 sprintf(Show_Text,"Remaining time:%dS ",sum_time-time_clc_s);
	         LCD_DisplayStringLine(Line6, (uint8_t *)Show_Text);
			 if(time_clc_s==sum_time)
			 {
			   if(light_flag==1||STMON_OFF==1)
			   {
				 LED_LD8(4);
			   }
			   else
			   {
				 LED_LD8(5);
			   }
			    down_time_open=0;
				 time_clc_ms=0;
			     time_clc_s=0;
			     sum_time = 0;
			     light_flag = 0;
			     STMON_OFF = 0;
			 }
		  }
     } 
	    Key_flag = Key_scan(0);
	  if(Key_flag)
	  {
	    key_get++;
        if(key_get>=1)
     	  key_get =1;
	  }		  
	    
  }
        

}
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance==(TIM3))
  {     
   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
  }
}
uint tim2_time = 0,tim3_time=0;
uint frq = 0,frq2 = 0;
int  Cap_flag = 1;
u16 get_time = 0;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==(TIM2))
  { 
   tim2_time = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
   __HAL_TIM_SetCounter(htim,TIM_CHANNEL_1);  
   frq = 1000000/tim2_time;
   HAL_TIM_IC_Start(htim,TIM_CHANNEL_1);
  }
  if(htim->Instance==(TIM16))
  {   
	 
	if(Cap_flag==1)
	{  
		//get_time++;
	   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);
	   HAL_GPIO_WritePin(GPIOC,0x80<<8,GPIO_PIN_RESET);
       HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
       HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
		
    __HAL_TIM_SET_CAPTUREPOLARITY(&htim16,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//通道 1 设置为下降沿捕获
       Cap_flag = 2;
	}
	else if(Cap_flag == 2)
	{  
	   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);
	   HAL_GPIO_WritePin(GPIOC,0x80<<8,GPIO_PIN_SET);
       HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
       HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
		//get_time++;
    __HAL_TIM_SET_CAPTUREPOLARITY(&htim16,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//通道 1 设置为上升沿捕获
		Cap_flag = 1;
	}
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *hurat)
{   
	  
	if(data_get_ok==0)
	 {
		 RX_data[rx_pointer++] = rx_data;
         HAL_UART_Receive_IT(&huart1,&rx_data,1);
	 }
	 if(rx_pointer!=0)
	 {
	  if(RX_data[rx_pointer-2]=='\r'&&RX_data[rx_pointer-1]=='\n')
	 {
	  data_get_ok=1;
	  Uart_data_process();
	  memset(RX_data,0,sizeof(RX_data));
	  rx_pointer=0;
	  data_get_ok=0;
	  
	 }
     }

	 
}

void Uart_data_process()
{ 
  static char  last_data = 0;
  if(RX_data[0]=='S'&&RX_data[1]=='T')
  {
    switch(RX_data[2])
	{
		case 'S':
		{
		     STSTART  = 1;
			 last_data ='S';
			 LCD_DisplayStringLine(Line8, (uint8_t *)"Remote Control:ON   ");
			 break;
		}
	    case 'P':
		{
		     STPWM = (RX_data[5]-'0')*100+(RX_data[6]-'0')*10+(RX_data[7]-'0');
			 last_data ='P'; 
			break;
		}
		case 'M':
		{
			if(RX_data[4]=='N')
		       STMON_OFF=1;
			else
			   STMON_OFF=2;
			 
			last_data ='M';
			break;
		}
		case 'T':
		{
             STTIM_hour=(RX_data[5]-'0')*10+(RX_data[6]-'0');
			 STTIM_min =(RX_data[8]-'0')*10+(RX_data[9]-'0');
			 STTIM_miao=(RX_data[11]-'0')*10+(RX_data[12]-'0');
			 last_data ='T';
			break;
		}
		case 'K':
		{
             STK_keeptime=(RX_data[3]-'0')*10+(RX_data[4]-'0');
			 last_data ='K';
			 break;
		}
		case 'E':
		{
			 STEND=1;
			 if(last_data=='K'||last_data=='T'||last_data=='M'||last_data=='P')
			 {
			  switch(last_data)
			  {
				  case 'P':
				  {
					 if(STPWM==0)
				         LED_LD8(5);
					 else if(STPWM==200)
						 LED_LD8(1);
					 else if(STPWM==500)
						 LED_LD8(2);
					 else if(STPWM==800)
						 LED_LD8(3);
					 else if(STPWM==1000)
						 LED_LD8(4);
					 last_data = 0;
					 break;
				  }
				  case 'K':
				  {
					  down_time_open=1;
					  sum_time = STTIM_hour*60*60+STTIM_min*60+STTIM_miao+STK_keeptime*60;
					  STTIM_hour=0;
					  STTIM_min=0;
					  STTIM_miao=0;
					  STK_keeptime=0;
					  last_data = 0;
					  break;
				  }
			  
			  }
			 }
			 LCD_DisplayStringLine(Line8, (uint8_t *)"Remote Control:OFF  ");
			 break;
		}
	}
  }
}






