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

//˯��״̬�͹ر�lcd��adcͨ��.
static void powe_sleep(void)
{
		uint8 i = 0;
		LCD_UnInit();
		channel_all_power(0);
		reset_sleep_tick();
		
}

//˯�߽���һ�����ִ��˯�ߵĲ���.
void sleep_menu_init_func(uint8 prev)
{
		param_get_user(&g_user);
		power_ctrl(PWR_SLEEP);
		reset_sleep_tick();
		
}
static void power_on(void)
{
	//����10��d����
	power_ctrl(PWR_ON);
	//�����������ؽ���.
	gui_show(MENU_MAIN);
}
static void power_check_weight(void)
{
	//��ȡ�������ڴ���������10��d���ͻ���.
		scaler_info_t * sif = scaler_get_info();
		if(sif != NULL){
			if(!sif->ready)
			{
					return;
			}
			if(sif->div_weight > g_user->RSN){
					power_on();
			}else{
					//�������˯��.
					channel_power(1,0);
					
			}
		}
}

//��ʱ�������Ѳ���.
static uint8  power_isr(void)
{
	static int delay_cont = 10;
	
	//ÿ��2s����һ�Σ���ȡadֵ. 1s����ȡ��ǰ����.
	if(0==(GetTick() % 2)){
			channel_power(1,1); //ad 0 �򿪵�Դ.
			delay_cont = 10;
	}
	//�ӳ�1s����ȡ����.
	if(delay_cont > 0)
	{
		delay_cont--;
		if(delay_cont == 0){
				power_check_weight();
		}
	}
	//˯����10������
	if(is_sleep_timeout()){
			power_ctrl(PWR_OFF);
	}
		
	
	
	return 1;
}
void sleep_menu_gui_func(void)
{
		power_isr();
}
//��Ҫ�������Ĳ���.
/*
1.����ģʽ����������ʾ OLED�ر�/�ر������ź�/����������.
2.����ģʽ�¿ɰ����������.
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
					//�յ������,����.
					power_on();	
			}
	}
}
