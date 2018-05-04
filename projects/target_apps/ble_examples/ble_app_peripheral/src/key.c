#include "key.h"
#include <stdlib.h>
#include "gpio.h"
#include "user_peripheral.h"
#include "user_periph_setup.h"
typedef struct {
	 uint8 key;
	 uint8 state; //1 pressed 0 relesed
	 int press_ts; //按下的时间计数起.
	 int releas_ts; //释放的时间.
	 GPIO_PORT port;
	 GPIO_PIN  pin;
}key_state;

#define MAX_KEY_NUM 2
static key_state key_states[MAX_KEY_NUM];

void key_init()
{
	memset(&key_states[0], 0 , sizeof(key_states));
	key_states[0].port = KEY_PWR_PORT;
	key_states[0].pin = KEY_PWR_PIN;
	key_states[0].key = 0;
	key_states[1].port = KEY_ZERO_PORT;
	key_states[1].pin = KEY_ZERO_PIN;
	key_states[1].key = 1;
	
}
void Delay()
{
	key_states[1].key = 1;
}
uint8 i = 0;
//按键分析逻辑,定时调用.
void  key_isr()
{
	 
		if(GPIO_GetPinStatus(KEY_PWR_PORT,KEY_PWR_PIN))
		{
				key_states[0].press_ts = 0;
		}
		else
		{
				key_states[0].press_ts = 1;
		}
		if(GPIO_GetPinStatus(KEY_ZERO_PORT,KEY_ZERO_PIN))
		{
				key_states[1].press_ts = 0;
		}
		else
		{
			key_states[1].press_ts = 1; 
		}
		
	 for(i = 0; i < MAX_KEY_NUM; i++)
	 {
		  //HIGH LEVEL 按键释放.
			if(GPIO_GetPinStatus(key_states[i].port,key_states[i].pin)){
					key_states[i].press_ts = 0;
				  
			}else{
				//LOW LEVEL 按键被按下.
				  key_states[i].press_ts++;
			}
	 }
}