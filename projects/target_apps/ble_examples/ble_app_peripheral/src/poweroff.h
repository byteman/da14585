#ifndef PWR_MENU_H
#define PWR_MENU_H

#include "types.h"
#include "key.h"

void pwr_menu_init_func(uint8 prev);
void pwr_menu_gui_func(void);
void pwr_menu_key_event(key_msg_t* msg);
#endif

