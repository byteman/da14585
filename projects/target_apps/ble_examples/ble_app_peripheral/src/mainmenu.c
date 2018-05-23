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
static logic_param_t* g_logic = NULL;
static device_param* g_param;
static PARA_USER_T*  g_user;
static uint8_t g_flag = 0;
static uint8_t g_still_count = 0;
static uint32_t g_tick_count = 0; //主称重界面的定时器. 3分钟未计重进入休眠状态. 10分钟未计重进入关机状态. 未计重的意思，重量一直未超过称重阈值??
static uint32_t g_sleep_count= 0;
/*
1.休眠模式下无重量显示 OLED关闭/关闭蓝牙信号/无语音播报.
2.休眠模式下可按置零键唤醒.
*/
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
static void gui_show_history_weight(void)
{
	  uint8 i = 0;
		for(; i < MAX_HIS_WEIGHT; i++)
		{
			char buf[16] = {0,};
			format_weight(buf,16,g_logic->history_weight[i],1,4);	
			LCD_P8x16Str(0,1 + i*2,buf);
		}
}
static void gui_show_sum(int value, uint8 dot)
{
	char buff[16]={0,};
	
	format_total(buff,16,value,dot,8);
	LCD_P8x16Str(48,5,buff);
	
	
	//LCD_P6x8Str(112,1,kg,2);
}
static void gui_show_unit(void)
{
		//uint8_t kg[3] = {24,25,0};
		
		LCD_KG(100,1);
		//LCD_P8x16Ch(104,0,14);
		//LCD_P8x16Ch(112,0,15);
		
		//LCD_P8x8_ZH_Arr(110,0,kg,2);
}
/*
菜单的更新有以下几种情况

1.当前界面是活动页面，设备发生了状态变化，发送消息给活动页面，活动页面更新图标
2.当前页面从新激活的时候，读取一次设备的当前状态，刷新一次页面.
3.需要向对应的模块注册消息回调.
*/
void main_menu_init_func(uint8 prev)
{
	
	scaler_info_t* sif;
	param_get_logic(&g_logic);
	param_get(&g_param);
	param_get_user(&g_user);
	dispatch_register_msg(TARGET_MAINMENU,MSG_BLE_STATE,menu_menu_msg_cb);
	gui_clear_screen();
	#if 1
	gui_show_history_weight();
	
	scaler_get_display_weight(weight,16);
	
	gui_show_battry_state(battery_get());
	
	sif = scaler_get_info();
	
	gui_show_scaler_state(sif);
	gui_show_sum(g_logic->history_sum,1);
	gui_show_unit();
	g_sleep_count  = 0;
//	gui_show_ble_state(0);
	#endif
}
//存入一条重量，如果超过最大条数了，将最老一条数据推出，放入最近一条到最后
static void logic_push_weight(INT32S value)
{
		
		if(g_logic->history_count < MAX_HIS_WEIGHT){
				g_logic->history_weight[g_logic->history_count] = value;
		}else{
				uint8 i = 0;
				for(i = 0; i < MAX_HIS_WEIGHT-1; i++)
				{
						g_logic->history_weight[i] = g_logic->history_weight[i+1];
				}
				g_logic->history_weight[i] = value;
		}
		g_logic->history_count++; 
		g_logic->history_sum += value;
		param_save(LOGIC_PARA_T);
}

uint8 main_logic_isr(scaler_info_t * sif)
{
		if(abs(sif->div_weight >= 1000)){
			return 0;
		}
		if(
			sif->stillFlag &&  
			sif->div_weight > g_user->RSN && 
			(g_flag==0)
		)
		{
				if(g_still_count++ < 10){
						return 0;
				}
				char buf[16] = {0,};
			
			
				g_flag = 1;
				
				//记录历史重量.
				logic_push_weight(sif->div_weight);
				format_weight(buf,16,sif->div_weight,1,8);	
				strcat(buf,"k");
				//播报重量语言.
				audio_queue_message(buf);
				return 1;
				
		}
		else
		{
				
		}
		
		if(sif->stillFlag &&  
			sif->div_weight < 2*g_user->RSN && 
			(g_flag==1))
		{
				g_flag = 0;
				
		}
		
		return 0;
}
 
#define BMP_SUM 0
#define BMP_STILL 1
#define BMP_ZERO 2
#define BMP_BLE  3
#define BMP_CLEAR 4

//稳定和零位显示.
//可以设置一个标志,重量或者零位标志，稳定标志有其中一个有变化,就可以刷新.
static void gui_show_scaler_state(scaler_info_t *sif)
{
	
	LCD_P16x16bmp(W_STATE,1,sif->stillFlag?BMP_STILL:BMP_CLEAR);
	LCD_P16x16bmp(W_STATE,3,sif->zeroFlag? BMP_ZERO:BMP_CLEAR);
	LCD_SUM(W_STATE,5);

}
static void gui_show_poweroff(void)
{	
		char text[5] = {32,33,34,35};
	

		
		gui_clear_screen();
		
		LCD_P16x16_ZH_Arr(1,32,text,4);
		
}
//1.最多只能显示4位 包括小数点
//2.最后一位数字 按 8x16 字符显示
//3.小数点按只显示4x4像素.
static void lcd_show_weight(char* buf)
{
	//buf[4] = 0; //超过4位了就截取.
	LCD_P16x32Str(W_VALUE,1,buf);
}
static void gui_show_weight(scaler_info_t * sif)
{
	static uint8 clear = 0;
	char buf[16]={0,};
	
	if(sif->upFlow || sif->downFlow || sif->div_weight>=1000){
			//LCD_P16x32Str(W_VALUE,1,"--.1");
			//LCD_OverLoad(W_VALUE,1);
			clear = 0;
			return;
	}
	//if(sif->div_weight < 0 ) sif->div_weight = 0;
	if(clear == 0){
			//LCD_P16x32Str(W_VALUE,1,"   ");
			//LCD_P16x32Str(48,5,"    ");
			clear = 1; 
	}
	format_weight((char*)buf,16,sif->div_weight,1,4);
	if(sif->div_weight >= 1000){
			//clear = 0;
	}
	lcd_show_weight(buf);


}
#include "power.h"
//1.关闭全部ad
//2.关闭lcd屏幕
//3.关闭蓝牙.
//4.禁用称重流程.进入定时唤醒流程
static void goto_sleep(void)
{
	 power_ctrl(PWR_SLEEP);
}
static void main_sleep_handle(scaler_info_t * sif)
{
		static uint8 cnt = 3;
		//180 sleep
		if(sif->zeroFlag){
			cnt = 3;
			//零位状态进行计数，在零位超过3分钟，进入休眠状态
			if(g_sleep_count++ >= 1800){
					goto_sleep();
					
			}
		}else{
				g_sleep_count  = 0;
		}
		
}
void main_menu_gui_func(void)
{

	scaler_info_t * sif = scaler_get_info();
	if(sif != NULL){
			gui_show_weight(sif);
			gui_show_scaler_state(sif);
			if(main_logic_isr(sif))
			{
					gui_show_history_weight();
					gui_show_sum(g_logic->history_sum,1);
			}
		
	}
	if((g_tick_count++ % 5) == 0){
		
			gui_show_battry_state(battery_get());
	}
	

	
}
//清零历史重量.
static void		scaler_reset_history(void)
{
		g_logic->history_sum = 0;
		param_save(LOGIC_PARA_T);	
		gui_show_sum(g_logic->history_sum,1);
}
void main_menu_key_event(key_msg_t* msg)
{
	if(msg->key == KEY_ZERO)
	{
			
			if(msg->event == KEY_RELEASE)
			{
				//gui_show(MENU_DEBUG);
					scaler_set_zero();
			}
			else 	if(msg->event == KEY_LONG_PRESSED)
			{
					scaler_reset_history();
			}
	}
	else if(msg->key == KEY_PWR)
	{
			if(msg->event == KEY_RELEASE){
					gui_show(MENU_BLE);
			}else if(msg->event == KEY_LONG_PRESSED){
					gui_show_poweroff();
					if(key_power_off() == 0){
							gui_show(MENU_MAIN);
					}
			}
			
	}
	else if(msg->key == KEY_ALL)
	{
			if(msg->event == KEY_LONG_PRESSED)
			{
					gui_show(MENI_CAL_CORN);
			}
	}
}
