#include "gui.h"
#include <stdlib.h>
#include "oled.h"
#include "mainmenu.h"
#include "calibmenu.h"
#include "blemenu.h"

#include "key.h"

typedef void (*menu_func_t)(void);
typedef void (*init_func_t)(uint8 prev);



typedef struct {
	init_func_t			 init_func;	
	menu_func_t 		 gui_func;
	key_event_func_t key_func;
}menu_item;
	
static menu_item menu_itmes[3] = {
	{
		main_menu_init_func,
		main_menu_gui_func,
		main_menu_key_event
	},
	{
		calib_menu_init_func,
		calib_menu_gui_func,
		calib_menu_key_event
	},
	{
		ble_menu_init_func,
		ble_menu_gui_func,
		ble_menu_key_event
	}
};
static uint8 menu_index = 0;

//切换当前界面.
void gui_show(uint8 index)
{
		menu_itmes[index].init_func(menu_index);
		menu_index = index;
}
void format_weight(char* buffer, int size, float value, uint8 dot)
{
		memset(buffer,0,size);
		switch(dot)
			{

					case 1:
							snprintf(buffer,size,"%0.1f",(float)value/10.0f);
							break;
					case 2:
							snprintf(buffer,size,"%0.2f",(float)value/100.0f);
							break;
					case 3:
							snprintf(buffer,size,"%0.3f",(float)value/1000.0f);
							break;
					case 4:
							snprintf(buffer,size,"%0.4f",(float)value/10000.0f);
							break;
					default:
							snprintf(buffer,size,"%0.1f",value);
							break;
			}
}
void gui_show_history_weight(float* wf,uint8 num, uint8 dot)
{
	  uint8 i = 0;

		char buf[16] = {0,};
		format_weight(buf,16,wf[0],dot);	
		LCD_P8x16Str(02,1,buf);
		format_weight(buf,16,wf[1],dot);	
		LCD_P8x16Str(0,3,buf);
		format_weight(buf,16,wf[2],dot);	
		LCD_P8x16Str(0,5,buf);
		
		
}

void gui_show_battry_state(uint8 value)
{
		//14 level
	uint8 level = (value / 18)%14;
	
	LCD_P8x16Ch(112,3,level);
	 
}
void gui_show_sum(int value, uint8 dot)
{
	LCD_P8x16Str(48,5,"80196.2");
}
void gui_show_ble_state(uint8 state)
{
	 if(state)
	 {
			//param_save();
			LCD_P16x16bmp(112,5,3);
	 }
	 else
	 {
			LCD_P16x16bmp(112,5,4);
	 }
	
}

void gui_show_ble_addr(uint8 *addr, uint8 len)
{
	gui_clear_screen();
	LCD_P6x8Str(1,3,"00:11:33:44:55:66");
}
void gui_show_scaler_state(uint8 zero, uint8 still)
{
	LCD_P16x16bmp(32,1,1);
	LCD_P16x16bmp(32,3,2);
	LCD_P16x16bmp(32,5,0);
}

void gui_show_weight(int value, uint8 dot, uint8 unit)
{
	char buf[16]={0,};
	format_weight(buf,16,value,dot);
	LCD_P16x32Str(48,1,buf);


}
void gui_clear_screen(void)
{
	LCD_Fill(0x00);  //初始清屏
	LCD_Set_Pos(0,0); 
}
//定时
void gui_isr()
{
		menu_itmes[menu_index].gui_func();
		
}
void key_event(key_msg_t* msg)
{
		char buf[16] = {0,};
		snprintf(buf,16,"key=%d.%d",msg->key,msg->event);
		//LCD_P8x16Str(0,0,buf);
		
		menu_itmes[menu_index].key_func(msg);
		
}

void gui_show_error(const char* err)
{
	gui_clear_screen();
	LCD_P6x8Str(10,4,(unsigned char*)err);
}
uint8 gui_current()
{
	return menu_index;
}
void gui_init()
{
		LCD_Init();
		key_register(key_event);
		gui_show(MENU_MAIN);
	
}

