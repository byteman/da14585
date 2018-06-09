#include "gui.h"
#include <stdlib.h>
#include "calibmenu.h"
#include "common.h"
#include "key.h"
#include "channel.h"
#include "param.h"
#include "Weighing.h"
#include "commenu.h"
#include "utils.h"
#include "oled.h"

#define BTN_X 56
#define BTN_Y 5
static PARA_USER_T *g_user = NULL;
static uint8 step = 0;


static void calib_menu_show_zero(void)
{
	
	gui_clear_screen();
	char buf[32] = {0,};
	
	snprintf(buf,32,"CAL.%02d",5);

	LCD_CAL(8,1,4);
	LCD_P8x16Str(72,5,(unsigned char*)buf);
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
	gui_show_button(0,BTN_X,BTN_Y);
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
		//show_button(0);
		gui_show(MENU_MAIN);
		
	}
	

}

void doCalib(void)
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
void calib_menu_key_event(key_msg_t* msg)
{
	
	if(msg->key == KEY_ZERO)
	{	
			if(msg->event == KEY_PRESSED)
			{
					gui_show_button(1,BTN_X,BTN_Y);
			}
			else if(msg->event == KEY_PRESS_RLEASED)
			{
					DelayToDo(KEY_DELAY_TIME,doCalib);
			}
	}
	else if(msg->key == KEY_PWR)
	{
			if(msg->event == KEY_PRESSED)
			{
				gui_show_button(1,BTN_X,BTN_Y);
					//LCD_P16x16bmp(2,0,5); //按钮被按下
			}
			else if(msg->event == KEY_PRESS_RLEASED)
			{
					gui_show(MENU_MAIN);
					//LCD_P16x16bmp(2,0,4); //按钮被释放
			}
			
	}
}
