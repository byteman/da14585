//
#ifndef PWR_H
#define PWR_H
#include "types.h"

typedef enum {
		PWR_ON = 0,
		PWR_SLEEP=1,
		PWR_OFF=2
}power_state_t;
void power_enable(uint8 en);
void  power_feed(void);
uint8  power_isr(void);
void  power_ctrl(uint8 state);
void 	power_off(void);
uint8 power_state(void);

#endif

