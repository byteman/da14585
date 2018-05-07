#include "gui.h"
#include <stdlib.h>
#include "blemenu.h"

#include "key.h"

void ble_menu_init_func(uint8 prev)
{
		gui_show_ble_addr("00:11:22:33:44:55",17);
}

void ble_menu_gui_func(void)
{

}

void ble_menu_key_event(key_msg_t* msg)
{
  if(msg->key == KEY_PWR)
	{
			gui_show(MENU_MAIN);
	}
}
