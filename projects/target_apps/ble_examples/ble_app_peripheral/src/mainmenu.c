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
static uint32_t g_tick_count = 0; //�����ؽ���Ķ�ʱ��. 3����δ���ؽ�������״̬. 10����δ���ؽ���ػ�״̬. δ���ص���˼������һֱδ����������ֵ??
static uint32_t g_sleep_count= 0;

static void gui_show_scaler_state(scaler_info_t *sif,uint8 update);
static void gui_show_weight(scaler_info_t * sif,uint8 update);
/*
1.����ģʽ����������ʾ OLED�ر�/�ر������ź�/����������.
2.����ģʽ�¿ɰ����������.
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
		LCD_KG(100,1);
}
/*
�˵��ĸ��������¼������

1.��ǰ�����ǻҳ�棬�豸������״̬�仯��������Ϣ���ҳ�棬�ҳ�����ͼ��
2.��ǰҳ����¼����ʱ�򣬶�ȡһ���豸�ĵ�ǰ״̬��ˢ��һ��ҳ��.
3.��Ҫ���Ӧ��ģ��ע����Ϣ�ص�.
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
	
	gui_show_scaler_state(sif,1);
	
	gui_show_weight(sif,1);

	gui_show_sum(g_logic->history_sum,1);
	gui_show_unit();
	LCD_SUM(W_STATE,5);
	
	g_sleep_count  = 0;
//	gui_show_ble_state(0);
	#endif
}
//����һ�����������������������ˣ�������һ�������Ƴ����������һ�������
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
				
				//��¼��ʷ����.
				logic_push_weight(sif->div_weight);
				format_weight(buf,16,sif->div_weight,1,8);	
				strcat(buf,"k");
				//������������.
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

//�ȶ�����λ��ʾ.
//��������һ����־,����������λ��־���ȶ���־������һ���б仯,�Ϳ���ˢ��.
static void gui_show_scaler_state(scaler_info_t *sif,uint8 update)
{
	static uint8 old_still = 255;
	static uint8 old_zero  = 255;
	if(0 == update)
	{
		if(old_still != sif->stillFlag  ) 
		{
			LCD_P16x16bmp(W_STATE,1,sif->stillFlag?BMP_STILL:BMP_CLEAR);
			old_still = sif->stillFlag;
			return;
		}
		if(old_zero  == sif->zeroFlag ) 
		{
			LCD_P16x16bmp(W_STATE,3,sif->zeroFlag? BMP_ZERO:BMP_CLEAR);
			old_zero  = sif->zeroFlag;
			return;
			
		}
	}
	
	
	LCD_P16x16bmp(W_STATE,1,sif->stillFlag?BMP_STILL:BMP_CLEAR);
	LCD_P16x16bmp(W_STATE,3,sif->zeroFlag? BMP_ZERO:BMP_CLEAR);
	
}
static void gui_show_poweroff(void)
{	
		char text[5] = {32,33,34,35};
	

		
		gui_clear_screen();
		
		LCD_P16x16_ZH_Arr(1,32,text,4);
		
}
//1.���ֻ����ʾ4λ ����С����
//2.���һλ���� �� 8x16 �ַ���ʾ
//3.С���㰴ֻ��ʾ4x4����.
static void lcd_show_weight(char* buf)
{
	//buf[4] = 0; //����4λ�˾ͽ�ȡ.
	LCD_P16x32Str(W_VALUE,1,buf);
}
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
	if(sif->upFlow || sif->downFlow || sif->div_weight>=1000){
			LCD_OverLoad(W_VALUE,1);
			return;
	}
	format_weight((char*)buf,16,sif->div_weight,1,4);

	lcd_show_weight(buf);


}
#include "power.h"
//1.�ر�ȫ��ad
//2.�ر�lcd��Ļ
//3.�ر�����.
//4.���ó�������.���붨ʱ��������
static void goto_sleep(void)
{
	 power_ctrl(PWR_SLEEP);
}
static void main_sleep_handle(scaler_info_t * sif)
{
	#define ZERO_DELAY_CNT 5
		static int8 cnt = ZERO_DELAY_CNT;
		//180 sleep
		if(sif->zeroFlag){
			cnt = ZERO_DELAY_CNT;
			//��λ״̬���м���������λ����3���ӣ���������״̬
			if(g_sleep_count++ >= 1800){
					goto_sleep();
					g_sleep_count = 0;
					
			}
		}else{
				if(cnt-- > 0)
				{
						return;
				}
				cnt = ZERO_DELAY_CNT;
				g_sleep_count  = 0;
		}
		
}
void main_menu_debug()
{
	char buf[16] = {0,};
	if(g_tick_count % 10 == 0)
	{
		snprintf(buf,16,"%8d",g_tick_count/10);
		LCD_P8x16Str(48,5,buf);

	}
	
}
void main_menu_gui_func(void)
{

		
	scaler_info_t * sif = scaler_get_info();
	if(sif != NULL){
			if(!sif->ready)
			{
					
			
					return;
			}
			gui_show_weight(sif, 0);
			gui_show_scaler_state(sif,0);
			if(main_logic_isr(sif))
			{
					gui_show_history_weight();
					gui_show_sum(g_logic->history_sum,1);
			}
			main_sleep_handle(sif);
			//main_menu_debug();
		
	}
	if((g_tick_count++ % 5) == 0){
		
			gui_show_battry_state(battery_get());
	}
	
	

	
}
//������ʷ����.
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
				
			}
			else 	if(msg->event == KEY_LONG_PRESSED)
			{
					scaler_reset_history();
			}
			else if(msg->event == KEY_PRESSED)
			{
					//LCD_BLE(108,5,0);
					LCD_BLE(108,5,2); //��ť������
			}
			else if(msg->event == KEY_PRESS_RLEASED)
			{
					scaler_set_zero();
					g_tick_count = 0;
					LCD_BLE(108,5,ble_scaler_get_ble_state());
			}
	}
	else if(msg->key == KEY_PWR)
	{
			if(msg->event == KEY_RELEASE){
					
			}else if(msg->event == KEY_LONG_PRESSED){
					gui_show_poweroff();
					if(key_power_off() == 0){
							gui_show(MENU_MAIN);
					}
			}
			else if(msg->event == KEY_PRESSED)
			{
					//LCD_BLE(108,5,0);
					LCD_BLE(108,5,2); //��ť������ //��ť������
			}
			else if(msg->event == KEY_PRESS_RLEASED)
			{
					LCD_BLE(108,5,ble_scaler_get_ble_state());
					gui_show(MENU_BLE);
					//LCD_P16x16bmp(108,5,4); //��ť���ͷ�
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
