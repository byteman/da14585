#include "gui.h"
#include <stdlib.h>
#include "mainmenu.h"
#include "scaler.h"
#include "batty.h"
#include "key.h"
#include "dispatcher.h"
#include "param.h"
#include "WetApp.h"
#include "proclogic.h"
#include "audio.h"

char weight[16];
static logic_param_t* g_logic_para = NULL;
static void menu_menu_msg_cb(comm_msg_t* msg)
{
	switch(msg->cmd)
	{
		case MSG_BLE_STATE:
		{
				if(gui_current() == MENU_MAIN)
				{
						
						gui_show_ble_state(msg->u8_param);
				}
		}
	}
}
/*
菜单的更新有以下几种情况

1.当前界面是活动页面，设备发生了状态变化，发送消息给活动页面，活动页面更新图标
2.当前页面从新激活的时候，读取一次设备的当前状态，刷新一次页面.
3.需要向对应的模块注册消息回调.
*/
void main_menu_init_func(uint8 prev)
{
	float values[3];
	scaler_info_t* sif;
	param_get_logic(&g_logic_para);
	
	dispatch_register_msg(TARGET_MAINMENU,MSG_BLE_STATE,menu_menu_msg_cb);
	
	gui_show_history_weight((INT32S*)&g_logic_para->history_weight[0],3,1);
	
	scaler_get_display_weight(weight,16);
	
	gui_show_battry_state(battery_get());
	
	sif = scaler_get_info();
	
	gui_show_scaler_state(sif);
	gui_show_sum(10001,1);
	gui_show_ble_state(0);
	
}

 
void main_menu_gui_func(void)
{
	static int i = 100;
	i++;
	if(i>999) i  = 100;
	gui_show_weight(i,1,0);
	
	logic_isr();
	
	
}
void main_menu_key_event(key_msg_t* msg)
{
	if(msg->key == KEY_ZERO)
	{
			if(msg->event == KEY_LONG_PRESSED)
			{
					if(msg->event == KEY_ALL)
					{
							gui_show(MENI_CAL_CORN);
					}
			}
			else if(msg->event == KEY_RELEASE)
			{
				
			}
	}
	else if(msg->key == KEY_PWR)
	{
			audio_queue_message("123.4k");
			gui_show(MENU_BLE);
	}
}
