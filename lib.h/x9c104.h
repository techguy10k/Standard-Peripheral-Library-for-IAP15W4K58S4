#ifndef X9C104_H_
#define X9C104_H_
#include"io.h"


#define UP   1u
#define DOWN 0u
#define x9c104_initval 80u

typedef struct{
								unsigned char x9c104_up_down;
								unsigned char x9c104_step;
							}x9c104cmd_typedef;

void x9c104_cmd(x9c104cmd_typedef* cmd);
void x9c104_init(void);


#endif
