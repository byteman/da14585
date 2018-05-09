#ifndef MENUMENU_H
#define MENUMENU_H

#include "types.h"
#include "key.h"

void main_menu_init_func(uint8 prev);
void main_menu_gui_func(void);
void main_menu_key_event(key_msg_t* msg);
#endif

