#include"x9c104.h"
#include"stdlib.h"
#include"stc15.h"
#include"INTRINS.h"
#include"delay.h"


//程序需要记住抽头位置，这里记录的是RWB，也就是抽头到B那一部分
unsigned char x9c104_position = 100 - x9c104_initval;

//修复X9C104第一次触发不灵敏的问题
static unsigned char x9c104_udstate = 3;
/*
	重要说明：此函数在正式环境中将在中断中运行
	为保证执行效率step应不大于10
*/
void x9c104_cmd(x9c104cmd_typedef* cmd)
{
	x9c104_cs = 0;
	if(cmd->x9c104_up_down == 1)
	{
		x9c104_ud = 1;
	}
	else
	{
		x9c104_ud = 0;
	}
	delay2us();
	delay1us();
	
	
	//修复X9C104第一次触发不灵敏的问题
	if((cmd->x9c104_up_down != x9c104_udstate) && (x9c104_udstate != 3))
	{
		cmd->x9c104_step += 2;
	}
	
	x9c104_udstate = cmd->x9c104_up_down;
	
	for(;cmd->x9c104_step > 0;cmd->x9c104_step--)
	{
		x9c104_inc = 1;
		delay2us();
		x9c104_inc = 0;
		delay2us();
	}
	
	x9c104_inc = 0;
	delay1us();
	delay2us();
	x9c104_cs = 1;		
}


/*
	X9C104初始化函数，把抽头位置调整到initval处
	需要注意的是没事别乱调用，芯片读写次数只有
	10K次
*/
void x9c104_init(void)
{
	unsigned char temp = 0;
	
	x9c104_cs = 1;
	delayms(20);
	x9c104_cs = 0;
	x9c104_ud = UP;
	delay2us();
	delay1us();
	
	for(;temp < 105;temp ++)
	{
		x9c104_inc = 1;
		delay2us();
		x9c104_inc = 0;
		delay2us();
	}
	
	x9c104_ud = DOWN;
	delay2us();
	delay1us();	
	
	for(temp = 0;temp < x9c104_initval + 1;temp ++)
	{
		x9c104_inc = 1;
		delay2us();
		x9c104_inc = 0;
		delay2us();
	}
	
	x9c104_inc = 1;
	delay2us();
	delay1us();
	x9c104_cs = 1;
}

