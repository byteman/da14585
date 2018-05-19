#ifndef GUI_H
#define GUI_H

#include "types.h"
#include "scaler.h"

#define MENU_MAIN 		0
#define MENI_CAL_WET	1
#define MENI_CAL_CORN	2
#define MENU_BLE  		3
#define MENU_DEBUG  	4

#define DEV_STATE_OFFSET 104

void gui_show_scaler_state(scaler_info_t* sif);

void gui_show_sum(int value, uint8 dot);

void gui_show_ble_state(uint8 state);

//void gui_show_ble_addr(uint8 *addr, uint8 len);


void gui_show_battry_state(uint8 value);

//void gui_show_weight(int value, uint8 dot, uint8 unit);

void gui_clear_screen(void);
void gui_init(void);
//��ʱ������ʾ�߼�ҵ��
void gui_isr(void);

void gui_show(uint8 index);
uint8 gui_current(void);
void gui_show_error(const char* err);

#endif

