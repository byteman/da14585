#ifndef BLE_MENU_H
#define BLE_MENU_H

#include "types.h"
#include "key.h"


void ble_menu_init_func(uint8 prev);
void ble_menu_gui_func(void);
void ble_menu_key_event(key_msg_t* msg);


#endif

