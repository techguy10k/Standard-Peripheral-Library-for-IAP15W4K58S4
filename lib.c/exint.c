#include"STC15.h"
#include"exint.h"


void exint2_init(void)
{
  EA = 1;
  INT_CLKO |= 0x10;
}

void exint0_init(void)
{
  P32 = 1; //下降沿
  IT0 = 1;
  EX0 = 1;
  EA = 1;
}

void exint1_init(void)
{
  P33 = 1; //下降沿
  IT1 = 1;
  EX1 = 1;
  EA = 1;
}


void exint2_cmd(bit state)
{
  if(state == set_bit)
  {
    INT_CLKO |= 0x10;
  }
  else
  {
    INT_CLKO &= 0xef;
  }
}

void exint0_cmd(bit state)
{
  if(state == set_bit)
  {
    EX0 = 1;
  }
  else
  {
    EX0 = 0;
  }
}

void exint1_cmd(bit state)
{
  if(state == set_bit)
  {
    EX1 = 1;
  }
  else
  {
    EX1 = 0;
  }
}

