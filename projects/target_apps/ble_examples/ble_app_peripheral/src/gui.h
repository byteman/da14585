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

#define HIS_W_X  9 //��ʷ������ʼ��
#define HIS_W_Y  9 //��ʷ������ʼ��

#define HIS_W_W				8 //��ʷ�������
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

//����
void 	gui_clear_screen(void);
//guiģ���ʼ��
void 	gui_init(void);
//��ʱ������ʾ�߼�ҵ��
void 	gui_isr(void);
//�л���ǰ��ʾ����.
void 	gui_show(uint8 index);
//��ȡ��ǰ��ʾ����id
uint8 gui_current(void);

#endif

