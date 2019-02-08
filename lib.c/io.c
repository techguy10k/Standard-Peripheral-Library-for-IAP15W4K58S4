#include"STC15.h"
#include"io.h"
#include"mt8880.h"
#include"delay.h"

void io_init(void)
{
  P2M0 = 0x00;
  P2M1 = 0x00;

  P0M0 = 0x14;
  P0M1 = 0x10;

  P3M0 = 0x00;
  P3M1 = 0x00;
  
  P4M0 = 0x00;
  P4M1 = 0x00;
  
  P1M0 = 0x40;
  P1M1 = 0xa0;
}


