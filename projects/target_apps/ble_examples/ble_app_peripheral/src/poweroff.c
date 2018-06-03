#include "gui.h"
#include <stdlib.h>
#include "poweroff.h"
#include "scaler.h"
#include "key.h"
#include "dispatcher.h"


static int32 tick = 0;


void pwr_menu_init_func(uint8 prev)
{
		char title[4] = {12,13,2,3};
		char text[5] = {32,33,34,35};
		char text2[3] = {4,26,27};
		gui_clear_screen();
		//LCD_P16x16_ZH_Arr(1,0,title,4);
		//LCD_P16x16_ZH_Arr(33,2,text,4);
		//LCD_P16x16_ZH_Arr(33,4,text2,3);
		tick = 1;
		LCD_P16x32_OFF(40,2);
		
		
}


void pwr_menu_gui_func(void)
{
			if(tick-- < 0){
					if(key_power_off() == 0){
							
					}
			}
}
void pwr_menu_key_event(key_msg_t* msg)
{

  if(msg->key == KEY_PWR)
	{
			if(msg->event == KEY_PRESS_RLEASED)
			{
				//gui_show(MENU_MAIN);
			
			}
			
	}
}
