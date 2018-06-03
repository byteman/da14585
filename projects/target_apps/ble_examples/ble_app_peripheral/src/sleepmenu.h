#ifndef SLEEP_MENU_H
#define SLEEP_MENU_H

#include "types.h"
#include "gui.h"

void sleep_menu_init_func(uint8 prev);
void sleep_menu_gui_func(void);
void sleep_menu_key_event(key_msg_t* msg);

#endif

