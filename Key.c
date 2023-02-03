#include "Key.h"
#include "tim.h"
#include "i2c - hal.h"
#include "stdbool.h"
// Mode : 0:不支持长按  1：长按


int Key_scan(int Mode)
{
  static int  key_up=1;//按键松开标志
  if(Mode==1) key_up=1;//支持长按 
  if(key_up==1&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))
  {
    key_up=0;
	if(KEY1==0)        return KEY1_Get;
	else if(KEY2==0)   return KEY2_Get;
	else if(KEY3==0)   return KEY3_Get;
	else if(KEY4==0)   return KEY4_Get;
  }
  else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)
	  key_up = 1;
  return 0;//无按键按下
}
int Light_class = 5;
bool on_off = 0;
bool time_on_off = 0;
int time_set_flag = 0;
int time_set_flag_page1 = 0;
int hour=0,min = 0,miao=0;
extern u16  sum_time;
extern int light_flag;
int down_time_open = 0;
uchar Pwm_PA6_duty_h = 0;
uchar Pwm_PA6_duty_l = 0;
void Key_User(void)
{   
	  static int key_num = 0;
      static int time_set = 1;
    if(Key_flag ==1&&key_get==1)
	{        
		     time_set_flag = 0;
			 LCD_Init_Show();
		     if((Pwm_PA6_duty!=1000||on_off==1)&&(Pwm_PA6_duty!=0||on_off==0))
		        on_off = !on_off;
			  if(on_off&&Light_class!=1&&Light_class!=2&&Light_class!=3)
				Light_class = 4;
			  else
				Light_class = 5;
		  LED_LD8(Light_class);
		  key_get = 0;
		  key_num = 1;
	}
	else if(Key_flag ==2&&key_get==1)
	{  
		if(key_num!=3&&time_set_flag!=1)	
		{ 
			if(key_num!=2&&key_num!=1)
			     LCD_Init_Show();
			 Light_class +=1;
			if(Light_class>3)
				Light_class = 1;
			LED_LD8(Light_class);
		}
		else if(time_set_flag==1)
		{  
			if(time_set_flag_page1 == 1)
			{
			  time_set++;
			  if(time_set==4)
			  {
				LCD_Time_Show(3);				  
				sprintf(Show_Text,"Keep Time:%02d:%02d:%02d",hour,min,miao);
		        LCD_DisplayStringLine(Line4, (uint8_t *)Show_Text);
			  }
			  if(time_set==5)
			  {
			   time_set = 1;
			   time_set_flag_page1 = 0;
			   time_set_flag = 0;
			   sum_time = hour*60*60+min*60+miao;
			   hour=0;min=0;miao=0;
			   down_time_open=1;
			   LCD_Init_Show();
			  }
			}
           else	
		   {			   
		      time_set_flag_page1 = 1;
	          LCD_Time_Show(2);//设置时间页面
		   }
		}
		key_get = 0;
		key_num = 2;
	}
	else if(Key_flag ==3&&key_get==1)
	{   
		
		if(time_set_flag_page1 != 1)
		{
			time_on_off = !time_on_off;
			time_set_flag = 1;
			if(key_get==1||key_get==2&&time_set_flag_page1 != 1)			
				 LCD_Time_Show(1);
			if(time_set_flag_page1==0)
			{  
			   TIME_SET(time_on_off);			   
			}
	    }
        if(time_set_flag_page1 == 1)
        {
		  switch(time_set)
		  {
			  case 1: 
				      hour++;
			          if(hour>23) 
						  hour=23;
					  sprintf(Show_Text,"Timer Start:%02d:%02d:%02d",hour,min,miao);
		              LCD_DisplayStringLine(Line4, (uint8_t *)Show_Text);
					  break;
			  case 2: 
				      min++;
			          if(min>59) 
						  min=59;
					  sprintf(Show_Text,"Timer Start:%02d:%02d:%02d",hour,min,miao);
		              LCD_DisplayStringLine(Line4, (uint8_t *)Show_Text);
					  break;
              case 3: 
				      miao++;
			          if(miao>59) 
						  miao=59;
					  sprintf(Show_Text,"Timer Start:%02d:%02d:%02d",hour,min,miao);
		              LCD_DisplayStringLine(Line4, (uint8_t *)Show_Text);
					  break;
              case 4: 
				      hour+=30;
			          if(hour>65534) 
						  hour=65530;
					  sprintf(Show_Text,"Keep Time:%02d:%02d:%02d",hour,min,miao);
		              LCD_DisplayStringLine(Line4, (uint8_t *)Show_Text);
					  break;					  
		  }
		}			
		key_get = 0;
		key_num = 3;
	}
	else if(Key_flag ==4&&key_get==1)
	{   		
		if(time_set_flag_page1 == 1)
        {
		  switch(time_set)
		  {
			  case 1: 
				      hour--;
			          if(hour<0) 
						  hour=0;
					  sprintf(Show_Text,"Timer Start:%02d:%02d:%02d",hour,min,miao);
		              LCD_DisplayStringLine(Line4, (uint8_t *)Show_Text);
					  break;
			  case 2: 
				      min--;
			          if(min<0) 
						  min=0;
					  sprintf(Show_Text,"Timer Start:%02d:%02d:%02d",hour,min,miao);
		              LCD_DisplayStringLine(Line4, (uint8_t *)Show_Text);
					  break;
              case 3: 
				      miao--;
			          if(miao<0) 
						  miao=0;
					  sprintf(Show_Text,"Timer Start:%02d:%02d:%02d",hour,min,miao);
		              LCD_DisplayStringLine(Line4, (uint8_t *)Show_Text);
					  break;
              case 4: 
				      min-=30;
			          if(min<0) 
						  min=0;
					  sprintf(Show_Text,"Keep Time:%02d:%02d:%02d",hour,min,miao);
		              LCD_DisplayStringLine(Line4, (uint8_t *)Show_Text);
					  break;					  
		  }
		}
		else
		{  
	      Pwm_PA6_duty_h = (Pwm_PA6_duty)>>8;
		  Pwm_PA6_duty_l = (Pwm_PA6_duty)&0xff;
		  eeprom_write(2,Pwm_PA6_duty_l);
		  HAL_Delay(10);
	      eeprom_write(1,Pwm_PA6_duty_h);		
		}
		key_get = 0;
		key_num = 4;
	}
}




