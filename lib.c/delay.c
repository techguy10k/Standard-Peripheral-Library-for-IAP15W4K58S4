#include"STC15.h"
#include"delay.h"
#include"INTRINS.h"


void delayms(unsigned int a)	//@30.000MHz
{
  unsigned char i, j;
  while(a > 0)
    {
      i = 30;
      j = 43;
      do
        {
          while (--j);
        }
      while (--i);
      a --;
    }
}


void delay1us(void)  //@30Mhz
{
  unsigned char i;

  i = 5;
  while (--i);
}

void delay2us(void) //2us @30MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 12;
	while (--i);
}

