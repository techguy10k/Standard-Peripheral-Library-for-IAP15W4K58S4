/*
*  文件名称：uart.h
*  文件作用：串口初始化、设置、使用相关声明放这里
*  文件用法：无
*/

#ifndef UART_H_

#define UART_H_

void uart1_init(void);
void uart3_init(void);
void uart1_sendbyte(unsigned char input);
void uart3_sendbyte(unsigned char input);
void uart1_sendstring(unsigned char *p);
void uart3_sendstring(unsigned char *p);


#endif

