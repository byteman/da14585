#include "gui.h"
#include <stdlib.h>
#include "oled.h"
#include "mainmenu.h"
#include "calibmenu.h"
#include "blemenu.h"
#include "debugmenu.h"
#include "cornmenu.h"
#include "logomenu.h"
#include "poweroff.h"
#include "sleepmenu.h"
#include "key.h"



	
static menu_item menu_itmes[] = {
	{
		main_menu_init_func,
		main_menu_gui_func,
		main_menu_key_event
	},
	{
		calib_menu_init_func,
		calib_menu_gui_func,
		calib_menu_key_event
	},
	{
		corn_menu_init_func,
		corn_menu_gui_func,
		corn_menu_key_event
	},
	{
		ble_menu_init_func,
		ble_menu_gui_func,
		ble_menu_key_event
	},
	{
		debug_menu_init_func,
		debug_menu_gui_func,
		debug_menu_key_event
	},
	{
		logo_menu_init_func,
		logo_menu_gui_func,
		logo_menu_key_event
	},
	{
		pwr_menu_init_func,
		pwr_menu_gui_func,
		pwr_menu_key_event
	},
	{
		sleep_menu_init_func,
		sleep_menu_gui_func,
		sleep_menu_key_event
	}
	
};
static uint8 menu_index = 0;

//切换当前界面.
void gui_show(uint8 index)
{
		menu_itmes[index].init_func(menu_index);
		menu_index = index;
}

void gui_clear_screen(void)
{
	LCD_Fill(0x00);  //初始清屏
	LCD_Set_Pos(0,0); 
}
//定时
void gui_isr(void)
{
		menu_itmes[menu_index].gui_func();
		
}
void key_event(key_msg_t* msg)
{
		//char buf[16] = {0,};
		//snprintf(buf,16,"key=%d.%d",msg->key,msg->event);
		//LCD_P8x16Str(0,0,buf);
		
		menu_itmes[menu_index].key_func(msg);
		
}
uint8 gui_current(void)
{
	return menu_index;
}
void gui_init(void)
{
		LCD_Init();
		key_register(key_event);
		gui_show(MENU_LOGO);
		//gui_show(MENU_DEBUG);
}

