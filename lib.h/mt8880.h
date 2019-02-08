#ifndef MT8880_H_

#define MT8880_H_

#ifndef set_bit
#define set_bit 1
#endif

#ifndef reset_bit
#define reset_bit 0
#endif

#define mt8880_start_delay_time 3000
#define mt8880_stop_delay_time 1000

#define mt8880_mode_dtmf reset_bit
#define mt8880_mode_call_progress set_bit

#define mt8880_single_dual_tone_generation_single set_bit
#define mt8880_single_dual_tone_generation_dual reset_bit

#define mt8880_burst_mode_enable reset_bit
#define mt8880_burst_mode_disable set_bit

#define mt8880_column_row_tones_column set_bit
#define mt8880_column_row_tones_row reset_bit

#define mt8880_send_high_time 50
#define mt8880_send_low_time 50

#define mt8880_mode_listening 0
#define mt8880_mode_receiving 1
#define mt8880_mode_sending   2
#define mt8880_mode_idle      3

typedef struct
{
  char mt8880_irq;
  char mt8880_trans_data_empty;
  char mt8880_rec_data_full;
  char mt8880_delayed_steering;
} mt8880_status_typedef;


typedef struct
{
  char tone_output;
  char mode_control;
  char interrupt_enable;
  char register_b_selected;
  char burst_mode;
  char test_mode;
  char single_dual_tone_generation;
  char column_row_tones;
} mt8880_control_typedef;



void mt8880_write_transmit_data(unsigned char data_input);
mt8880_status_typedef mt8880_get_status(bit clk_enable);
unsigned char mt8880_read_receive_data(void);
void mt8880_write_control_data(mt8880_control_typedef* mt8880_control);
void mt8880_bootup_setup(void);
void mt8880_send_one_byte(unsigned char data_input);
void mt8880_listen(void);
void mt8880_receive(void);

void mt8880_transmit_start(void);
void mt8880_transmit_stop(void);
void mt8880_ptt_cmd(bit state);

void mt8880_send_string(unsigned char* string);
void mt8880_send_sequence_byte(unsigned char* input,unsigned int lenth);

unsigned char get_mode(unsigned char* input);

#endif
