#ifndef DEBUG_MENU_H
#define DEBUG_MENU_H

#include "types.h"
#include "key.h"

void debug_menu_init_func(uint8 prev);
void debug_menu_gui_func(void);
void debug_menu_key_event(key_msg_t* msg);
#endif

