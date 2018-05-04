#ifndef KEY_H
#define KEY_H

#include "types.h"

//单击 "开机键” 键开机，硬件自动控制
//按住置零键3秒，显示蓝牙地址
//单击置零键 置零重量 双击置零键 清除累计
//长按开机键3秒,关机.

typedef struct {
	uint8 key;
	uint8 event;
}key_msg;

//关机.
void  key_power_off();
//开关键是否持续被按住n秒
void  is_pwr_key_pressed(uint8 s);
//清零键是否持续被按住n秒
void  is_zero_key_pressed(uint8 s);

void  is_zero_key_clicked(uint8 s);

void  is_zero_key_double_clicked(uint8 s);

void  key_isr();
#endif

