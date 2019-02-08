#include"mt8880.h"
#include"STC15.h"
#include"INTRINS.h"
#include"delay.h"
#include"uart.h"
#include"user_timer.h"
#include"stdlib.h"
#include"command.h"
#include"exint.h"
#include"io.h"
#include"pro.h"

#include"string.h"

extern bit mt8880_recflag;
extern unsigned char mt8880_rec_raw, mt8880_rec;
extern xdata unsigned int mt8880_counter;
extern systemstatus_typedef* systemstatus;


unsigned char decode_table(unsigned char input);
unsigned char encode_table(unsigned char input);

void mt8880_write_transmit_data(unsigned char data_input)
{
  clk = 0;

  data_input = data_input << 4;
  P2 = 0x0f;
  rs0 = 0;
  rw = 0;


  P2 |= data_input;
  delay1us();
  cs = 0;
  clk = 1;
  delay1us();
  clk = 0;
  delay1us();
  cs = 1;
}

mt8880_status_typedef mt8880_get_status(bit clk_enable)
{
  unsigned char data_read;
  mt8880_status_typedef mt8880_status;

  timer4_cmd(reset_bit);

  clk = 0;

  P2 = 0Xff;
  rs0 = 1;
  rw = 1;


  cs = 0;
  clk = 1;
  delay1us();
  data_read = P2;
  clk = 0;
  delay1us();
  cs = 1;

  data_read = data_read >> 4;

  if(data_read & 0x01)
    mt8880_status.mt8880_irq = 1;
  if(data_read & 0x02)
    mt8880_status.mt8880_trans_data_empty = 1;
  if(data_read & 0x04)
    mt8880_status.mt8880_rec_data_full = 1;
  if(data_read & 0x08)
    mt8880_status.mt8880_delayed_steering = 1;

  if(clk_enable == 1)
    timer4_cmd(set_bit);

  return mt8880_status;
}

unsigned char mt8880_read_receive_data(void)
{
  unsigned char data_read;

  timer4_cmd(set_bit);

  clk = 0;

  P2 = 0Xff;
  rs0 = 0;
  rw = 1;

  cs = 0;
  clk = 1;
  delay1us();
  data_read = P2;
  clk = 0;
  delay1us();
  cs = 1;

  data_read = data_read >> 4;

  timer4_cmd(set_bit);

  return data_read;
}

void mt8880_write_control_data(mt8880_control_typedef* mt8880_control)
{
  unsigned char data_write_cra = 0x00;
  unsigned char data_write_crb = 0x00;

  if(mt8880_control->tone_output)
    data_write_cra |= 0x01;
  if(mt8880_control->mode_control)
    data_write_cra |= 0x02;
  if(mt8880_control->interrupt_enable)
    data_write_cra |= 0x04;
  if(mt8880_control->register_b_selected)
    data_write_cra |= 0x08;

  if(mt8880_control->burst_mode)
    data_write_crb |= 0x01;
  if(mt8880_control->test_mode)
    data_write_crb |= 0x02;
  if(mt8880_control->single_dual_tone_generation)
    data_write_crb |= 0x04;
  if(mt8880_control->column_row_tones)
    data_write_crb |= 0x08;

  data_write_cra = data_write_cra << 4;
  data_write_crb = data_write_crb << 4;

  clk = 0;


  P2 = 0x0f;
  rs0 = 1;
  rw = 0;

  P2 |= data_write_cra;
  cs = 0;
  clk = 1;
  delay1us();
  clk = 0;
  delay1us();
  cs = 1;

  if(mt8880_control->register_b_selected == 0)
    {
      return;
    }

  P2 = 0x0f;
  rs0 = 1;
  rw = 0;

  P2 |= data_write_crb;
  cs = 0;
  clk = 1;
  delay1us();
  clk = 0;
  delay1us();
  cs = 1;

}


void mt8880_bootup_setup(void)
{
  mt8880_control_typedef* mt8880_control = NULL;
  mt8880_control = malloc(sizeof(mt8880_control_typedef));
  memset(mt8880_control,0,sizeof(mt8880_control_typedef));

  mt8880_control->tone_output = reset_bit;
  mt8880_control->mode_control = reset_bit;
  mt8880_control->interrupt_enable = reset_bit;
  mt8880_control->register_b_selected = reset_bit;
  mt8880_control->burst_mode = reset_bit;
  mt8880_control->test_mode = reset_bit;
  mt8880_control->single_dual_tone_generation = reset_bit;
  mt8880_control->column_row_tones = reset_bit;


  mt8880_get_status(reset_bit);
  mt8880_write_control_data(mt8880_control);
  mt8880_write_control_data(mt8880_control);
  mt8880_control->register_b_selected = set_bit;
  mt8880_write_control_data(mt8880_control);
  mt8880_control->register_b_selected = reset_bit;
  mt8880_write_control_data(mt8880_control);
  mt8880_get_status(reset_bit);

  free(mt8880_control);

}


/*
  先发低四位 再发高四位
*/
void mt8880_send_one_byte(unsigned char data_input)
{
  mt8880_control_typedef*  mt8880_control;

  mt8880_control = malloc(sizeof(mt8880_control_typedef));

  if(mt8880_control == NULL)
    {
      send_string_to_console("ERROR CODE 2011!\nSystem halt.\n");
      while(1);
    }


  mt8880_control->tone_output = reset_bit;
  mt8880_control->mode_control = mt8880_mode_dtmf;
  mt8880_control->interrupt_enable = reset_bit; //disable interrupts when its sending data
  mt8880_control->register_b_selected = set_bit;
  mt8880_control->burst_mode = mt8880_burst_mode_disable;
  mt8880_control->single_dual_tone_generation = mt8880_single_dual_tone_generation_dual;
  mt8880_control->test_mode = reset_bit;
  mt8880_control->column_row_tones = reset_bit;

  exint2_cmd(reset_bit);
  timer4_cmd(reset_bit);
  clk = 0;

  mt8880_bootup_setup();
  mt8880_write_control_data(mt8880_control);

  mt8880_write_transmit_data(data_input);

  mt8880_control->tone_output = set_bit;
  mt8880_write_control_data(mt8880_control);


  mt8880_counter = 0;
  while((mt8880_counter < mt8880_send_high_time) && (1));

  mt8880_control->tone_output = reset_bit;
  mt8880_write_control_data(mt8880_control);

  mt8880_counter = 0;
  while((mt8880_counter < mt8880_send_low_time) && (1));
  \
  mt8880_get_status(reset_bit);

  data_input >>= 4;

  mt8880_control->tone_output = set_bit;
  mt8880_write_control_data(mt8880_control);
  mt8880_write_transmit_data(data_input);

  mt8880_counter = 0;
  while((mt8880_counter < mt8880_send_high_time) && (1));

  mt8880_control->tone_output = reset_bit;
  mt8880_write_control_data(mt8880_control);

  mt8880_counter = 0;
  while((mt8880_counter < mt8880_send_low_time) && (1));

  mt8880_get_status(reset_bit);

  free(mt8880_control);

}

void mt8880_listen(void)
{
  xdata mt8880_control_typedef*  mt8880_control;

  mt8880_control = malloc(sizeof(mt8880_control_typedef));

  if(mt8880_control == NULL)
    {
      send_string_to_console("ERROR CODE 2012!\nSystem halt.\n");
      while(1);
    }

  mt8880_control->tone_output = reset_bit;
  mt8880_control->mode_control = mt8880_mode_dtmf;
  mt8880_control->interrupt_enable = set_bit;
  mt8880_control->register_b_selected = set_bit;
  mt8880_control->burst_mode = mt8880_burst_mode_disable;
  mt8880_control->single_dual_tone_generation = mt8880_single_dual_tone_generation_dual;
  mt8880_control->test_mode = reset_bit;
  mt8880_control->column_row_tones = reset_bit;
  mt8880_bootup_setup();
  mt8880_write_control_data(mt8880_control);

  free(mt8880_control);
  timer4_cmd(set_bit);
  exint2_cmd(set_bit);
}

void mt8880_receive(void)
{
  unsigned char rec = 0;
  unsigned char temp = 10;
  unsigned char i = 0;

  if(mt8880_recflag != 0)
    {
      mt8880_counter = 0x00;

      LED_RX = 0;

      while(mt8880_counter < 25);
      rec = mt8880_read_receive_data();

      while(mt8880_counter < 125);
      temp = mt8880_read_receive_data();

      while(mt8880_counter < 220);

      temp = temp << 4;

      rec |= temp;

      send_byte_to_console(rec);

      mt8880_recflag = 0;
      LED_RX = 1;
      systemstatus->system_state = system_state_listen;
    }
}

void mt8880_transmit_start(void)
{
  systemstatus->system_state = system_state_transmit;
  mt8880_counter = 0;
  mt8880_ptt_cmd(set_bit);
  while((mt8880_counter < mt8880_start_delay_time) && (1));
}

void mt8880_transmit_stop(void)
{
  mt8880_counter = 0;
  while((mt8880_counter < mt8880_stop_delay_time) && (1));
  mt8880_ptt_cmd(reset_bit);
  mt8880_listen();
  systemstatus->system_state = system_state_listen;
}



void mt8880_send_string(unsigned char* string)
{
//  hc05_cmd(reset_bit);
  mt8880_transmit_start();

  while(*string)
    {
      mt8880_send_one_byte(*string);
      string ++;
      delayms(100);
    }

  mt8880_transmit_stop();
  hc05_cmd(set_bit);
}

void mt8880_send_sequence_byte(unsigned char* input,unsigned int lenth)
{
  unsigned int i;

  mt8880_transmit_start();
  for(i = 0; i < lenth; i ++)
    {
      mt8880_send_one_byte(input[i]);
    }
  mt8880_transmit_stop();
}

unsigned char get_mode(unsigned char* input)
{
  xdata unsigned char number;
  xdata unsigned char row;
  xdata unsigned char more1;
  xdata unsigned char* same_count = NULL;

  same_count = calloc(5, sizeof(unsigned char));

  for(row = 0; row < 5; row ++)
    {
      for(number = 0; number < 5; number ++)
        {
          if(input[row] == input[number])
            {
              same_count[row] ++;
            }
        }
    }

  if(same_count[0] > same_count[1])
    {
      more1 = 0;
    }
  else
    {
      more1 = 1;
    }

  if(same_count[more1] > same_count[2])
    {

    }
  else
    {
      more1 = 2;
    }

  if(same_count[more1] > same_count[3])
    {

    }
  else
    {
      more1 = 3;
    }

  if(same_count[more1] > same_count[4])
    {

    }
  else
    {
      more1 = 4;
    }

  return input[more1];
}

unsigned char decode_table(unsigned char input)
{
  switch(input)
    {
    case 0x81 :
      return ' ';
    case 0x82 :
      return '-';
    case 0x83 :
      return '/';
    case 0x84 :
      return '=';
    case 0x85 :
      return '>';
    case 0x86 :
      return '?';
    case 0x87 :
      return '@';
    case 0x88 :
      return 'M';
    case 0x89 :
      return 'N';
    case 0x8a :
      return 'O';
    case 0x8b :
      return ']';
    case 0x8c :
      return '^';

    case 0x91 :
      return '_';
    case 0x92 :
      return '`';
    case 0x93 :
      return 'm';
    case 0x94 :
      return 'n';
    case 0x95 :
      return 'o';
    case 0x96 :
      return 'p';
    case 0x97 :
      return '}';
    case 0x98 :
      return '~';
    case 0x99 :
      return 0x7f;

    default :
      return input;
    }
}

unsigned char encode_table(unsigned char input)
{
  switch(input)
    {
    case ' ' :
      return 0x81;
    case '-' :
      return 0x82;
    case '/' :
      return 0x83;
    case '=' :
      return 0x84;
    case '>' :
      return 0x85;
    case '?' :
      return 0x86;
    case '@' :
      return 0x87;
    case 'M' :
      return 0x88;
    case 'N' :
      return 0x89;
    case 'O' :
      return 0x8a;
    case ']' :
      return 0x8b;
    case '^' :
      return 0x8c;

    case '_' :
      return 0x91;
    case '`' :
      return 0x92;
    case 'm' :
      return 0x93;
    case 'n' :
      return 0x94;
    case 'o' :
      return 0x95;
    case 'p' :
      return 0x96;
    case '}' :
      return 0x97;
    case '~' :
      return 0x98;
    case 0x7f :
      return 0x99;

    default :
      return input;
    }
}
