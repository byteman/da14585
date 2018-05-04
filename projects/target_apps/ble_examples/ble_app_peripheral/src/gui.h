#ifndef GUI_H
#define GUI_H

#include "types.h"

void gui_show_history_weight(float* values,uint8 num, uint8 dot);

void gui_show_scaler_state(uint8 zero, uint8 still);

void gui_show_sum(int value, uint8 dot);

void gui_show_ble_state(uint8 state);

void gui_show_ble_addr(uint8 *addr, uint8 len);


void gui_show_battry_state(uint8 value);

void gui_show_weight(int value, uint8 dot, uint8 unit);

//定时调用显示逻辑业务
void gui_isr();
#endif

