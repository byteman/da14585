#include "key.h"
#include <stdlib.h>
#include "gpio.h"
#include "user_peripheral.h"
#include "user_periph_setup.h"
#include "custs1.h"
#include "custs1_task.h"
typedef struct {
	 uint8 key;
	 uint8 state; //1 pressed 0 relesed 2 long pressed
	 int press_ts; //按下的时间计数起.
	 int releas_ts; //释放的时间.
	 GPIO_PORT port;
	 GPIO_PIN  pin;
}key_state;

#define MAX_KEY_NUM 2
#define KEY_PRESS_TIME 3
#define TIMER_INT 100
#define PRESS_KEY_TIME 2000

static key_state key_states[MAX_KEY_NUM];

static uint8 key_pressed = 0;
static key_event_func_t key_cb = 0;
void  key_register(key_event_func_t cb)
{
	key_cb = cb;
}


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

void send_key_msg(uint8 key, uint8 event)
{
  struct key_msg req;

	req.key = key;
	req.event = event;
 
	key_cb(&req);
}

//按键分析逻辑,定时调用.
void  key_isr()
{
		int i = 0;
	 #if 1
		
		if(GPIO_GetPinStatus(KEY_PWR_PORT,KEY_PWR_PIN) )
		{
					if(key_pressed < 30) return;
					key_states[0].press_ts++;
					//key_states[0].state = KEY_PRESSED;
					if(key_states[0].press_ts > (PRESS_KEY_TIME/TIMER_INT) )
					{
								if(GPIO_GetPinStatus(KEY_ZERO_PORT,KEY_ZERO_PIN))
								{
										send_key_msg(KEY_ALL, KEY_LONG_PRESSED);
										key_states[1].state = KEY_LONG_PRESSED; 
										key_states[1].press_ts = 0;
								}
								else
								{
										send_key_msg(KEY_PWR, KEY_LONG_PRESSED);
								}
								key_states[0].state = KEY_LONG_PRESSED; 
								key_states[0].press_ts = 0;
					}
					else
					{
					
					}
		}
		else
		{
					if(key_pressed < 30)
						key_pressed++;
				
					//LOW LEVEL 按键被释放.
					if(key_states[0].press_ts > KEY_PRESS_TIME && key_states[0].state != KEY_LONG_PRESSED)
					{
							send_key_msg(KEY_PWR, KEY_RELEASE);
												
					}
					key_states[0].state = KEY_RELEASE;
				  key_states[0].press_ts = 0;
					
		}
		
		if(GPIO_GetPinStatus(KEY_ZERO_PORT,KEY_ZERO_PIN))
		{
					key_states[1].press_ts++;
					key_states[1].releas_ts=20;
			
					if(key_states[1].press_ts > (PRESS_KEY_TIME/TIMER_INT) )
					{
								if(GPIO_GetPinStatus(KEY_PWR_PORT,KEY_PWR_PIN))
								{
										send_key_msg(KEY_ALL, KEY_LONG_PRESSED);
										key_states[0].state = KEY_LONG_PRESSED; 
										key_states[0].press_ts = 0;
								}
								else
								{
										send_key_msg(KEY_ZERO, KEY_LONG_PRESSED);
								}
								key_states[1].state = KEY_LONG_PRESSED; 
								key_states[1].press_ts = 0;
						  
					}
		}
		else
		{
					//释放后需要等待2秒才发送按键被释放命令.
					if(key_states[1].releas_ts > 0){
							key_states[1].releas_ts--;
							return;
					}
			//LOW LEVEL 按键被释放.
					if(key_states[1].press_ts > KEY_PRESS_TIME && key_states[1].state != KEY_LONG_PRESSED)
					{
								
								send_key_msg(KEY_ZERO_PORT, KEY_RELEASE);
					}
					key_states[1].state = KEY_RELEASE;
				  key_states[1].press_ts = 0;
		}
	#endif
		
}
uint8 key_power_off(void)
{
	//
		util_delay(20000);
		if(	GPIO_GetPinStatus(KEY_PWR_PORT,KEY_PWR_PIN) == 0)
		{
				return 0;
		}
		GPIO_SetInactive(PWR_OFF_PORT,PWR_OFF_PIN);
		//GPIO_SetInactive(PWR_OFF_PORT,PWR_OFF_PIN);
		while(1);
		
}