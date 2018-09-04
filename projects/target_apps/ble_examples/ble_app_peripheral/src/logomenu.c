#include "gui.h"
#include <stdlib.h>
#include "logomenu.h"
#include "scaler.h"
#include "key.h"
#include "dispatcher.h"


static uint32 tick = 0;


void logo_menu_init_func(uint8 prev)
{
		char title[4] = {12,13,2,3};
		char text[5] = {32,33,36,37,38};
		char text2[3] = {4,26,27};
		gui_clear_screen();
		//LCD_P16x16_ZH_Arr(1,0,title,4);
		//LCD_P16x16_ZH_Arr(16,2,text,5);
		//LCD_P16x16_ZH_Arr(33,4,text2,3);
		//LCD_P16x32_ExS(8,1);
		LCD_P16x32_XM(8,1);
		LCD_P16x32Str(54,1,"5000");
		LCD_Vesion(25,5);
		LCD_P8x16Str(40,5,"2018-09-04");

		
}


void logo_menu_gui_func(void)
{

	scaler_info_t * sif = scaler_get_info();
	if(sif != NULL){
			//gui_show_ad(sif);
			if(sif->ready)
			{
					gui_show(MENU_MAIN);
			}
	}

	
}
void logo_menu_key_event(key_msg_t* msg)
{
	
}
