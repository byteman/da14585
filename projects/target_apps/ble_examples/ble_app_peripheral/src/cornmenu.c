#include "gui.h"
#include <stdlib.h>
#include "cornmenu.h"
#include "param.h"
#include "key.h"
#include "commenu.h"
#include "cornfix.h"

#define MAX_ANGLE 4
#define BTN_X 56
#define BTN_Y 5
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

void corn_menu_init_func(uint8 prev)
{	
		step = 0;
		param_get(&g_para);
		corn_menu_show_angle(step+1);		
}


void corn_menu_gui_func(void)
{
	
}

void doCornCalib(void)
{
	//0 1 2 3 
		gui_show_button(0,BTN_X,BTN_Y);
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

void corn_menu_key_event(key_msg_t* msg)
{
	if(msg->key == KEY_ZERO)
	{	
			if(msg->event == KEY_PRESSED)
			{
					gui_show_button(1,BTN_X,BTN_Y);
			}
			else if(msg->event == KEY_PRESS_RLEASED)
			{
					static int ts = 0;
					if( (abs(ts - msg->ts) < 20)){
						
					}
					else{
							DelayToDo(KEY_DELAY_TIME,doCornCalib);	
					}
					ts = msg->ts;
					//DelayToDo(KEY_DELAY_TIME,doCornCalib);
			}
	}
	else if(msg->key == KEY_PWR)
	{
			if(msg->event == KEY_PRESSED)
			{
					gui_show_button(1,BTN_X,BTN_Y);
			}
			else if(msg->event == KEY_PRESS_RLEASED)
			{
					gui_show(MENU_MAIN);
			}
			
	}
	
}
