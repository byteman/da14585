#ifndef KEY_H
#define KEY_H

#include "types.h"

//单击 "开机键” 键开机，硬件自动控制
//按住置零键3秒，显示蓝牙地址
//单击置零键 置零重量 双击置零键 清除累计
//长按开机键3秒,关机.

typedef enum{
	KEY_ZERO=0,
	KEY_PWR,
	KEY_ALL
}key_code_t;
#define KEY_PRESSED 1
#define KEY_LONG_PRESSED 2
#define KEY_RELEASE_2S 0
#define KEY_PRESS_RLEASED 3
#define KEY_RELASE_TWICE 4
typedef struct key_msg{
	uint8 key;
	uint8 event;
	uint8 invalid;
}key_msg_t;

typedef void(*key_event_func_t)(key_msg_t* event);

//关机.
uint8  key_power_off(void);
//开关键是否持续被按住n秒
void  is_pwr_key_pressed(uint8 s);
//清零键是否持续被按住n秒
void  is_zero_key_pressed(uint8 s);

void  is_zero_key_clicked(uint8 s);

void  is_zero_key_double_clicked(uint8 s);

void  key_isr(void);
void  key_register(key_event_func_t cb);

uint8  key_get_msg(key_msg_t* msg);
#endif

