//
#ifndef SC5040B_H
#define SC5040B_H
#include "types.h"

void  sc5040b_init(void);
uint8 sc5040b_play(char label);
//��ʾ����,�з��ţ���С����.
void sc5040b_play_digital(int value,unsigned char dot);
#endif

