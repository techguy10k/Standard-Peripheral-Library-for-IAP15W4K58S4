#include"STC15.h"
#include"eeprom.h"

#define command_error 2
#define write_error 1
#define ok 0

unsigned char eeprom_readbyte(unsigned int addr)
{
  unsigned int addr_temp;
  unsigned char IAP_DATA_RES;
  
  IAP_CONTR = 0x00;
  IAP_DATA = 0x00;
  
  addr_temp = addr;
  IAP_ADDRL = addr_temp; 
  IAP_ADDRH = (addr_temp >> 8);
  
  IAP_CMD = 0x01; 
  IAP_CONTR |= 0x80;  
  IAP_TRIG = 0x5a;
  IAP_TRIG = 0xa5;
  
    
  IAP_DATA_RES = IAP_DATA;
  
  IAP_CONTR &= 0x10;
  IAP_TRIG = 0x00;
  IAP_CMD = 0x00;
  IAP_ADDRL = 0x00;
  IAP_ADDRH = 0x00;
  IAP_DATA = 0x00;
  
  return IAP_DATA_RES;
}


unsigned char eeprom_writebyte(unsigned int addr,unsigned char data_write)
{
  unsigned int addr_temp;
  unsigned char ret;
  
  IAP_CONTR = 0x00;
  IAP_DATA = 0x00;
  
  addr_temp = addr;
  IAP_ADDRL = addr_temp;
  IAP_ADDRH = (addr_temp >> 8);
  
  IAP_DATA = data_write;
  
  IAP_CMD = 0x02; 
  IAP_CONTR |= 0x80;
  
  IAP_TRIG = 0x5a;
  IAP_TRIG = 0xa5;
  if(IAP_CONTR & 0x90)
  {
    ret = command_error;   //命令错误
    IAP_CONTR = 0x00;
    IAP_TRIG = 0x00;
    IAP_CMD = 0x00;
    return ret;
  }
  
  if(eeprom_readbyte(addr) != data_write)
  {
    ret = write_error;   //写入错误
    IAP_CONTR = 0x00;
    IAP_TRIG = 0x00;
    IAP_CMD = 0x00;
    return ret;
  }          
  
  IAP_CONTR = 0x00;
  IAP_TRIG = 0x00;
  IAP_CMD = 0x00;
    
  ret = ok;   //正常返回
  return ret;
}