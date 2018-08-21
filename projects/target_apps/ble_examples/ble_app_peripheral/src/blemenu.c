#include "gui.h"
#include <stdlib.h>
#include "blemenu.h"

#include "key.h"
#include "scaler.h"
static uint8 zero_press_cnt = 0;
static uint8 pwr_press_cnt  = 0;
static void gui_show_ble_addr(const char* addr)
{
	
	
}
void ble_menu_init_func(uint8 prev)
{
		gui_clear_screen();
		char title[4] = {28,29,30,31}; //标定重量
	
		const char* addr = scaler_get_ble_addr();
		//LCD_P16x16_ZH_Arr(1,0,title,4);

		//LCD_P8x16Str(10,3,addr);
		LCD_BLE(110,5,1);
		pwr_press_cnt = 0;
		zero_press_cnt= 0;
		LCD_P6x8Str(3,13,addr); //128 - 102 = 26/2

}

void ble_menu_gui_func(void)
{

}

void ble_menu_key_event(key_msg_t* msg)
{
  if( (msg->key == KEY_PWR)  )
	{
		  if(msg->event == KEY_PRESSED)
			{
					LCD_P16x16bmp(2,0,5); //按钮被按下
			}
			else if(msg->event == KEY_PRESS_RLEASED)
			{
					LCD_P16x16bmp(2,0,4); //按钮被释放
					gui_show(MENU_MAIN);
			}
			
	}

	else if(msg->key == KEY_ZERO)
	{
			if(msg->event == KEY_PRESSED)
			{
					LCD_P16x16bmp(110,0,5); //按钮被按下
			}
			else if(msg->event == KEY_PRESS_RLEASED)
			{
					LCD_P16x16bmp(110,0,4); //按钮被释放
					gui_show(MENU_MAIN);
				
			}
	}
	else if(msg->key == KEY_ALL)
	{
			if(msg->event == KEY_LONG_PRESSED)
			{
					gui_show(MENU_DEBUG);
			}
	}

	
	
	
}
