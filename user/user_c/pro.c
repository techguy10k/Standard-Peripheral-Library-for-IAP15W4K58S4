/*
*  文件名称：pro.c
*  文件作用：整个工程的主c文件
*  文件用法：无
*/
#include"STC15.h"
#include"INTRINS.h"
#include"string.h"
#include"stdio.h"

#include"io.h"
#include"IAP15W4K32S4_IT.h"

void system_init(void)
{
  io_init();
}

void main(void)
{		
  system_init();
		
	for(;;)
	{
		
	}
}
