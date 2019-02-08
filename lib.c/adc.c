#include"adc.h"
#include"INTRINS.h"
#include"STC15.h"

unsigned int adc_on(adc_cmdtypedef* adc_cmd)
{
  unsigned long temp = 0;
  
  ADC_CONTR = 0x80; //速度改进
  switch(adc_cmd->speed)
    {
    case 0 :
      ADC_CONTR |= 0x00;
      break;
    case 1 :
      ADC_CONTR |= 0x20;
      break;
    case 2 :
      ADC_CONTR |= 0x40;
      break;
    case 3 :
      ADC_CONTR |= 0x60;
      break;
    }

  switch(adc_cmd->channel)
    {
    case 0 :
      P1ASF = 0X01;
      ADC_CONTR |= 0x00;
      break;
    case 1 :
      P1ASF = 0X02;
      ADC_CONTR |= 0x01;
      break;
    case 2 :
      P1ASF = 0X04;
      ADC_CONTR |= 0x02;
      break;
    case 3 :
      P1ASF = 0X08;
      ADC_CONTR |= 0x03;
      break;
    case 4 :
      P1ASF = 0X10;
      ADC_CONTR |= 0x04;
      break;
    case 5 :
      P1ASF = 0X20;
      ADC_CONTR |= 0x05;
      break;
    case 6 :
      P1ASF = 0X40;
      ADC_CONTR |= 0x06;
      break;
    case 7 :
      P1ASF = 0X80;
      ADC_CONTR |= 0x07;
      break;
    }

  CLK_DIV |= 0x20;
      
  ADC_CONTR |= 0x08;  //开始转换
  while(!(ADC_CONTR & 0x10));  //非中断类型 等他转换完再执行

  ADC_CONTR = 0x00;
  P1ASF = 0x00;

  if(adc_cmd->cal > 0)
    {

      temp = ADC_RES;
      temp = temp << 8;
      temp += ADC_RESL;

      temp = (1024 * adc_cmd->vcc) / temp;
      ADC_CONTR = 0x00;
      ADC_RES = 0;
      ADC_RESL = 0;
      
      return temp;
    }

  if(adc_cmd->accuracy == 8)
    {
      temp = (ADC_RESL * adc_cmd->vcc) / 256 ;
    }

  if(adc_cmd->accuracy == 10)
    {
      temp = ADC_RES;
      temp = temp << 8;

      temp += ADC_RESL;
      temp = (temp * adc_cmd->vcc) / 1024 ;
    }

  ADC_CONTR = 0x00;
  ADC_RES = 0;
  ADC_RESL = 0;
  
  return temp;
}
