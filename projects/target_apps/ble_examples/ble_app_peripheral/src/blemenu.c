#include "gui.h"
#include <stdlib.h>
#include "blemenu.h"

#include "key.h"
#include "scaler.h"

static void gui_show_ble_addr(const char* addr)
{
	
	
}
void ble_menu_init_func(uint8 prev)
{
		gui_clear_screen();
		char title[4] = {28,29,30,31}; //标定重量
	
		const char* addr = scaler_get_ble_addr();
		LCD_P16x16_ZH_Arr(1,0,title,4);
		
		//LCD_P8x16Str(1,10,addr);
		LCD_P6x8Str(3,13,addr); //128 - 102 = 26/2

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
