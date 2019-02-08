/*
*  文件名称：uart.c
*  文件作用：串口相关初始化、设置、使用函数在这里
*  文件用法：无
*/

#include"STC15.h"
#include"uart.h"


unsigned char uart1_rec;
unsigned char uart3_rec;

bit uart1_transflag, uart3_transflag ;

/*
*  函数名称：uart1_init();
*  函数作用：串口1相关初始化函数 使用定时器2 配置
*  函数用法：无
*/
void uart1_init(void)		//115200bps@30.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0xBF;		//设定定时初值
	T2H = 0xFF;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2 
  
  EA = 1;  //开启全局中断
  ES = 1;  //使能串口1的中断
}


/*
*  函数名称：uart3_init();
*  函数作用：串口3相关初始化函数 使用定时器2 配置
*  函数用法：无
*/
void uart3_init(void) //115200bps@30.000MHz
{
	S3CON = 0x10;		//8位数据,可变波特率
	S3CON |= 0x40;		//串口3选择定时器3为波特率发生器
	T4T3M &= 0xFD;		//定时器3时钟为Fosc/12,即12T
	T3L = 0xBF;		//设定定时初值
	T3H = 0xFF;		//设定定时初值
	T4T3M |= 0x08;		//启动定时器3
  
  EA = 1;
  IE2 |= 0X08; //开启相关中断
}


/*
*  函数名称：uart1_sendbyte();
*  函数作用：串口1数据发送函数，发送一字节的数据
*  函数用法：将待发送数据传递给input
*/
void uart1_sendbyte(unsigned char input)
{
  SBUF = input;
  while(uart1_transflag == 0);  //等待发送完毕才会执行下一个动作
  uart1_transflag = 0;
}


/*
*  函数名称：uart3_sendbyte();
*  函数作用：串口3数据发送函数，发送一字节的数据
*  函数用法：将待发送数据传递给input
*/
void uart3_sendbyte(unsigned char input)
{
  S3BUF = input;
  while(uart3_transflag == 0);  //等待发送完毕才会执行下一个动作
  uart3_transflag = 0;
}


void uart1_sendstring(unsigned char *p)
{
  while(*p > 0)
    {
      uart1_sendbyte(*p);
      p ++;
    }
}


void uart3_sendstring(unsigned char *p)
{
  while(*p > 0)
    {
      uart3_sendbyte(*p);
      p ++;
    }
}


