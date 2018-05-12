#include "gui.h"
#include <stdlib.h>
#include "calibmenu.h"
#include "common.h"
#include "key.h"
#include "channel.h"
#include "param.h"
#include "Weighing.h"

static PARA_USER_T *g_user = NULL;
static uint8 step = 0;

static void calib_menu_show_zero(void)
{
	gui_clear_screen();
	char buf[32] = {0,};
	
	LCD_P8x16Str(1,5,"clear scaler");
	LCD_P8x16Str(1,1,"calibrate");
	
}
static void calib_menu_show_weight(void)
{
	gui_clear_screen();
	
	LCD_P8x16Str(1,5,"load weight");
	LCD_P8x16Str(1,1,"calibrate");
	
}
static void calib_state_show()
{
	gui_clear_screen();
	LCD_P8x16Str(1,5,"please wait");
}

void calib_menu_init_func(uint8 prev)
{
		step = 0;
		param_get_user(&g_user);
		calib_menu_show_zero();
		
}

void calib_menu_gui_func(void)
{
	
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
			if(msg->event == KEY_RELEASE)
			{
					if(step == 0)
					{
						calib_state_show();
						Wet_StartAvg(20,Cal_Zero_Callback); //600
						
					}
					else
					{		
						calib_state_show();						
						Wet_StartAvg(20,Cal_Wet_Callback); //600
						
					}
			}
	}
	else if(msg->key == KEY_PWR)
	{
			gui_show(MENU_MAIN);
	}
}
