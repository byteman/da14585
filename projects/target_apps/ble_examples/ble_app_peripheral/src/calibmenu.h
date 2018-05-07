#ifndef CALIB_MENU_H
#define CALIB_MENU_H

#include "types.h"
#include "key.h"


void calib_menu_init_func(uint8 prev);
void calib_menu_gui_func(void);
void calib_menu_key_event(key_msg_t* msg);


#endif

