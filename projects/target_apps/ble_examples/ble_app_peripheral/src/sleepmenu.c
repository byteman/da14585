#include "gui.h"
#include <stdlib.h>
#include "sleepmenu.h"
#include "scaler.h"
#include "key.h"
#include "dispatcher.h"
#include "power.h"

static int32 g_sleep_count = 0;

//˯��״̬�͹ر�lcd��adcͨ��.
static void powe_sleep(void)
{
		uint8 i = 0;
		LCD_UnInit();
		channel_all_power(0);
		g_sleep_count = 0;
		
}

//˯�߽���һ�����ִ��˯�ߵĲ���.
void sleep_menu_init_func(uint8 prev)
{
		power_ctrl(PWR_SLEEP);
		
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
			if(sif->div_weight>=10){
					power_on();
			}else{
					//�������˯��.
					channel_power(0,0);
					
			}
		}
}
#define PWR_DOWN_TICK 600
//��ʱ�������Ѳ���.
static uint8  power_isr(void)
{
	static int delay_cont = 10;
	
	//ÿ��2s����һ�Σ���ȡadֵ. 1s����ȡ��ǰ����.
	if(0==(++g_sleep_count % 20)){
			channel_power(0,1); //ad 0 �򿪵�Դ.
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
	if(g_sleep_count > PWR_DOWN_TICK){
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
