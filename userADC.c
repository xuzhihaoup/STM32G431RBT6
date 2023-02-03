#include "userADC.h"

double Get_adc_num(ADC_HandleTypeDef *PIN)
{
  uint adc_num=0;
   HAL_ADC_Start(PIN);
   adc_num = HAL_ADC_GetValue(PIN);
   return  adc_num*3.3/4096;
}


