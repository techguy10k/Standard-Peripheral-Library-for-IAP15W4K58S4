#ifndef EXINT_H_

#define EXINT_H_

#ifndef set_bit
#define set_bit 1
#endif

#ifndef reset_bit 
#define reset_bit 0
#endif

void exint0_init(void);
void exint1_init(void);
void exint2_init(void);
void exint0_cmd(bit state);
void exint2_cmd(bit state);
void exint1_cmd(bit state);

#endif
