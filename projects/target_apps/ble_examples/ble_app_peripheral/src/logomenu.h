#ifndef LOGO_MENU_H
#define LOGO_MENU_H

#include "types.h"
#include "key.h"

void logo_menu_init_func(uint8 prev);
void logo_menu_gui_func(void);
void logo_menu_key_event(key_msg_t* msg);
#endif

