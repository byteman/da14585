#ifndef CORN_MENU_H
#define CORN_MENU_H

#include "types.h"
#include "key.h"


void corn_menu_init_func(uint8 prev);
void corn_menu_gui_func(void);
void corn_menu_key_event(key_msg_t* msg);


#endif

