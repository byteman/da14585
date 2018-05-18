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
			format_weight(buf,16,g_logic->history_weight[i],1);	
			LCD_P8x16Str(-5,1 + i*2,buf);
		}
}
static void gui_show_sum(int value, uint8 dot)
{
	char buff[16]={0,};
	
	format_weight(buff,16,value,dot);
	LCD_P8x16Str(48,5,buff);
	
	
	//LCD_P6x8Str(112,1,kg,2);
}
static void gui_show_unit(void)
{
		uint8_t kg[3] = {24,25,0};
		LCD_P8x16Ch(110,0,14);
		LCD_P8x16Ch(118,0,15);
		
		//LCD_P8x8_ZH_Arr(110,0,kg,2);
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
	
	gui_show_scaler_state(sif);
	gui_show_sum(g_logic->history_sum,1);
	gui_show_unit();
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
				format_weight(buf,16,sif->div_weight,1);	
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
static void gui_show_scaler_state(scaler_info_t *sif)
{
	
	LCD_P16x16bmp(32,1,sif->stillFlag?BMP_STILL:BMP_CLEAR);
	LCD_P16x16bmp(32,3,sif->zeroFlag? BMP_ZERO:BMP_CLEAR);
	LCD_P16x16bmp(32,5,0);

}
static void gui_show_poweroff(void)
{	
		char text[5] = {32,33,34,35};
	

		
		gui_clear_screen();
		
		LCD_P16x16_ZH_Arr(1,3,text,4);
		
}
//1.���ֻ����ʾ4λ ����С����
//2.���һλ���� �� 8x16 �ַ���ʾ
//3.С���㰴ֻ��ʾ4x4����.
static void lcd_show_weight(char* buf)
{
	buf[4] = 0; //����4λ�˾ͽ�ȡ.
	LCD_P16x32Str(48,1,buf);
}
static void gui_show_weight(scaler_info_t * sif)
{
	static uint8 clear = 0;
	char buf[16]={0,};
	
	if(sif->upFlow || sif->downFlow){
			LCD_P16x32Str(48,1,"----");
			clear = 0;
			return;
	}
	//if(sif->div_weight < 0 ) sif->div_weight = 0;
	if(clear == 0){
			LCD_P16x32Str(48,1,"    ");
			//LCD_P16x32Str(48,5,"    ");
			clear = 1; 
	}
	format_weight((char*)buf,16,sif->div_weight,1);
	lcd_show_weight(buf);


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
	if((g_tick_count++ % 50) == 0){
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
					key_power_off();
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
