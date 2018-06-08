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
	uint8 arr2[] = {4,18,7,8,num+20-1,9,16,17,10,11};
	gui_clear_screen();
	char buf[32] = {0,};
	
	//snprintf(buf,32,"%d",num);
	//LCD_P8x16Str(48,1,"corn");
	//LCD_P16x16_ZH(0,1,1);
	
	LCD_CAL(8,1,num);
	
	//LCD_P16x16_ZH_Arr(28,0,arr,4);
	//LCD_P16x16_ZH_Arr(9,3,arr2,6);
	//LCD_P16x16_ZH_Arr(20,5,arr2+6,4);
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
			if(msg->event == KEY_RELEASE_2S)
			{
				//0 1 2 3 
					if(step < MAX_ANGLE)
					{
							ad_put_corl_adx(step++);
							if(step == MAX_ANGLE){
								ad_calib_corl_k(g_para);
								param_save(DEV_PARA_T);
								gui_show(MENI_CAL_WET);
							}
							else
							{
								corn_menu_show_angle(step+1);
							}
							
					}
			}
			else if(msg->event == KEY_PRESSED)
			{
					LCD_P16x16bmp(110,0,5); //按钮被按下
			}
			else if(msg->event == KEY_PRESS_RLEASED)
			{
					LCD_P16x16bmp(110,0,4); //按钮被释放
			}
	}
	else if(msg->key == KEY_PWR)
	{
			if(msg->event == KEY_RELEASE_2S)
			{
				gui_show(MENU_MAIN);
			}	
			else if(msg->event == KEY_PRESSED)
			{
					LCD_P16x16bmp(2,0,5); //按钮被按下
			}
			else if(msg->event == KEY_PRESS_RLEASED)
			{
					LCD_P16x16bmp(2,0,4); //按钮被释放
			}
			
	}
	
}
