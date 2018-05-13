#include "gui.h"
#include <stdlib.h>
#include "cornmenu.h"
#include "param.h"
#include "key.h"

#define MAX_ANGLE 4
static uint8 step = 0;
static device_param* g_para;
static void corn_menu_show_angle(uint8 num)
{
	uint8 arr[] = {0,1,2,3};
	uint8 arr2[] = {4,5,7,8,20,9,16,17,10,11};
	gui_clear_screen();
	char buf[32] = {0,};
	
	snprintf(buf,32,"%d",num);
	//LCD_P8x16Str(48,1,"corn");
	//LCD_P16x16_ZH(0,1,1);
	
	LCD_P16x16_ZH_Arr(1,0,arr,4);
	LCD_P16x16_ZH_Arr(1,3,arr2,5);
	LCD_P16x16_ZH_Arr(1,5,arr2+5,5);
}
static void corn_menu_show_complete()
{
	gui_clear_screen();
	LCD_P8x16Str(48,5,"ok");
	step = 0;
}
void corn_menu_init_func(uint8 prev)
{	
		step = 0;
		param_get(&g_para);
		corn_menu_show_angle(step+1);		
}

void corn_menu_gui_func(void)
{
		
}

void corn_menu_key_event(key_msg_t* msg)
{
	if(msg->key == KEY_ZERO)
	{	
			if(msg->event == KEY_RELEASE)
			{
				//0 1 2 3 
					if(step < MAX_ANGLE)
					{
							ad_put_corl_adx(step++);
							if(step == MAX_ANGLE){
								ad_calib_corl_k(g_para);
								param_save(USER_PARA_T);
								gui_show(MENI_CAL_WET);
							}
							else
							{
								corn_menu_show_angle(step+1);
							}
							
					}
			}
	}
	else if(msg->key == KEY_PWR)
	{
			gui_show(MENU_MAIN);
	}
	
}
