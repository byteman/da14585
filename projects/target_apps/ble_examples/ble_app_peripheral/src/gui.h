#ifndef GUI_H
#define GUI_H

#include "types.h"
#include "key.h"
#define MENU_MAIN 		0
#define MENI_CAL_WET	1
#define MENI_CAL_CORN	2
#define MENU_BLE  		3
#define MENU_DEBUG  	4
#define MENU_LOGO    	5
#define MENU_PWR_OFF	6
#define MENU_SLEEP		7

#define HIS_W_X  9 //历史重量开始处
#define HIS_W_Y  9 //历史重量开始处

#define HIS_W_W				8 //历史重量宽度
#define HIS_DOT				24 
#define W_STATE				36
#define W_VALUE				56
#define D_STATE				110
#define DEV_STATE_OFFSET 104

typedef void (*menu_func_t)(void);
typedef void (*init_func_t)(uint8 prev);



typedef struct {
	init_func_t			 init_func;	
	menu_func_t 		 gui_func;
	key_event_func_t key_func;
}menu_item;

//清屏
void 	gui_clear_screen(void);
//gui模块初始化
void 	gui_init(void);
//定时调用显示逻辑业务
void 	gui_isr(void);
//切换当前显示界面.
void 	gui_show(uint8 index);
//获取当前显示界面id
uint8 gui_current(void);

#endif

