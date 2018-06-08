#include "gui.h"
#include <stdlib.h>
#include "calibmenu.h"
#include "common.h"
#include "key.h"
#include "channel.h"
#include "param.h"
#include "Weighing.h"
#include "scaler.h"

static PARA_USER_T *g_user = NULL;
static uint8 step = 0;
static void gui_show_weight(scaler_info_t * sif,uint8 update)
{
	static uint8 clear = 0;
	static int32_t old_value = 12345678;
	
	char buf[16]={0,};
	if(!update)
	{
		if(sif->div_weight == old_value)
		{
				return;
		}
	}

	old_value = sif->div_weight;
	if(sif->upFlow || sif->downFlow || sif->div_weight>=10000){
			LCD_OverLoad(W_VALUE+8,1);
			return;
	}
	format_weight((char*)buf,16,sif->div_weight,1,4);

	LCD_P16x32Str(W_VALUE+8,1,buf);


}

static void calib_menu_show_zero(void)
{
	
	gui_clear_screen();
	char buf[32] = {0,};
	
	snprintf(buf,32,"CAL.%02d",5);

	LCD_CAL(8,1,4);
	LCD_P8x16Str(72,5,buf);
	LCD_KG(100,1);
	LCD_P16x32Str(W_VALUE+8,1,"0.00");
	
}
static void calib_menu_show_weight(void)
{
	/*
	
	*/
	gui_clear_screen();
	char buf[32] = {0,};
	
	snprintf(buf,32,"CAL.%02d",6);

	LCD_CAL(8,1,5);
	LCD_P8x16Str(72,5,buf);
	LCD_KG(100,1);
	LCD_P16x32Str(W_VALUE+8,1,"20.00");

	
}
static void calib_state_show()
{
	gui_clear_screen();
	//char title[4] = {12,13,10,11}; //标定重量
	char text[7] = {32,33,2,3,4,26,27}; 
	//LCD_P16x16_ZH_Arr(1,0,title,4);
	LCD_P16x16_ZH_Arr(1,3,text,7);
}

void calib_menu_init_func(uint8 prev)
{
		step = 0;
		param_get_user(&g_user);
		calib_menu_show_zero();
		
}

void calib_menu_gui_func(void)
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
static void Cal_Zero_Callback(INT32S avg)
{
	Std_ReturnType	rtn;
	//if((avg > -1000000)&&(avg < 1000000))
	{
		g_user->LDW = avg;
		Wet_InitPara();
		Wet_InitZero();
		param_save(USER_PARA_T);
		step = 1;
		calib_menu_show_weight();
	}
}
static void	 Cal_Wet_Callback(INT32S avg)
{
	//if(avg > g_user->LDW)
	{
		g_user->LWT = avg;
		Wet_InitPara();
		Wet_InitZero();
		param_save(USER_PARA_T);
		step = 2;
		gui_show(MENU_MAIN);
	}
	

}

void calib_menu_key_event(key_msg_t* msg)
{
	
	if(msg->key == KEY_ZERO)
	{	
			if(msg->event == KEY_RELEASE_2S)
			{
					if(step == 0)
					{
						//calib_state_show();
						Wet_StartAvg(20,Cal_Zero_Callback); //600
						
					}
					else
					{		
						//calib_state_show();						
						Wet_StartAvg(20,Cal_Wet_Callback); //600
						
					}
			}
			else if(msg->event == KEY_PRESSED)
			{
					//LCD_P16x16bmp(112,0,5); //按钮被按下
			}
			else if(msg->event == KEY_PRESS_RLEASED)
			{
					//LCD_P16x16bmp(112,0,4); //按钮被释放
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
