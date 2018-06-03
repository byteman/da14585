#include "gui.h"
#include <stdlib.h>
#include "sleepmenu.h"
#include "scaler.h"
#include "key.h"
#include "dispatcher.h"
#include "power.h"
#include "param.h"
static int32 g_sleep_count = 0;
static PARA_USER_T*  g_user;

#define PWR_DOWN_TICK (600-180)
static void reset_sleep_tick(void)
{
		g_sleep_count = GetTick();
}
static uint8 is_sleep_timeout(void)
{
		INT32U diff = abs(GetTick() - g_sleep_count);
		return (diff >= PWR_DOWN_TICK)?1:0;
}

//睡眠状态就关闭lcd和adc通道.
static void powe_sleep(void)
{
		uint8 i = 0;
		LCD_UnInit();
		channel_all_power(0);
		reset_sleep_tick();
		
}

//睡眠界面一进入就执行睡眠的操作.
void sleep_menu_init_func(uint8 prev)
{
		param_get_user(&g_user);
		power_ctrl(PWR_SLEEP);
		reset_sleep_tick();
		
}
static void power_on(void)
{
	//超过10个d唤醒
	power_ctrl(PWR_ON);
	//并进入主称重界面.
	gui_show(MENU_MAIN);
}
static void power_check_weight(void)
{
	//读取重量，期待重量超过10个d，就唤醒.
		scaler_info_t * sif = scaler_get_info();
		if(sif != NULL){
			if(!sif->ready)
			{
					return;
			}
			if(sif->div_weight > g_user->RSN){
					power_on();
			}else{
					//否则继续睡眠.
					channel_power(1,0);
					
			}
		}
}

//定时来做唤醒操作.
static uint8  power_isr(void)
{
	static int delay_cont = 10;
	
	//每隔2s唤醒一次，读取ad值. 1s后再取当前重量.
	if(0==(GetTick() % 2)){
			channel_power(1,1); //ad 0 打开电源.
			delay_cont = 10;
	}
	//延迟1s后再取重量.
	if(delay_cont > 0)
	{
		delay_cont--;
		if(delay_cont == 0){
				power_check_weight();
		}
	}
	//睡眠了10分钟了
	if(is_sleep_timeout()){
			power_ctrl(PWR_OFF);
	}
		
	
	
	return 1;
}
void sleep_menu_gui_func(void)
{
		power_isr();
}
//还要处理按键的操作.
/*
1.休眠模式下无重量显示 OLED关闭/关闭蓝牙信号/无语音播报.
2.休眠模式下可按置零键唤醒.
*/
void sleep_menu_key_event(key_msg_t* msg)
{

  if(msg->key == KEY_PWR)
	{
			if(msg->event == KEY_PRESS_RLEASED)
			{
				//gui_show(MENU_MAIN);
			
			}
			
	}
	else  if(msg->key == KEY_ZERO)
	{
			if(msg->event == KEY_PRESS_RLEASED)
			{
					//收到置零键,唤醒.
					power_on();	
			}
	}
}
