#include "gui.h"
#include <stdlib.h>
#include "cornmenu.h"
#include "param.h"
#include "key.h"
#include "scaler.h"
#define MAX_ANGLE 4
static uint8 step = 0;
static device_param* g_para;
static void corn_menu_show_angle(uint8 num)
{

	gui_clear_screen();
	char buf[32] = {0,};
	
	snprintf(buf,32,"CAL.%02d",num);

	
	LCD_CAL(8,1,num-1);
	LCD_P8x16Str(72,5,buf);
	LCD_P16x32Str(W_VALUE+8,1,"20.00");
	LCD_KG(100,1);

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
static void gui_show_weight(scaler_info_t * sif,uint8 update)
{
	static uint8 clear = 0;
	static int32_t old_value = 12345678;
	
	char buf[16]={0,};

	if(sif->upFlow || sif->downFlow || sif->div_weight>=10000){
			LCD_OverLoad(W_VALUE+8,1);
			return;
	}
	format_weight((char*)buf,16,sif->div_weight,1,4);

	LCD_P16x32Str(W_VALUE+8,1,buf);


}

void corn_menu_gui_func(void)
{
	#if 0
		scaler_info_t * sif = scaler_get_info();
		if(sif != NULL){
				if(!sif->ready)
				{
						
				
						return;
				}
				gui_show_weight(sif,1);
		}
	#endif
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
					//LCD_P16x16bmp(110,0,5); //按钮被按下
			}
			else if(msg->event == KEY_PRESS_RLEASED)
			{
					//LCD_P16x16bmp(110,0,4); //按钮被释放
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
					//LCD_P16x16bmp(2,0,5); //按钮被按下
			}
			else if(msg->event == KEY_PRESS_RLEASED)
			{
					//LCD_P16x16bmp(2,0,4); //按钮被释放
			}
			
	}
	
}
