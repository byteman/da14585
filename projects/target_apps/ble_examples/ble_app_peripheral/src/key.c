#include "key.h"
#include <stdlib.h>
#include "gpio.h"
#include "user_peripheral.h"
#include "user_periph_setup.h"
#include "custs1.h"
#include "custs1_task.h"
#include "power.h"

typedef struct {
	 uint8 key;
		uint8 state; //1 pressed 0 relesed 2 long pressed
		int press_stamp;
		int press_ts; //���µ�ʱ�������.
		int releas_ts; //�ͷŵ�ʱ��.
		int relase_stamp; //��һ���ͷŵ�ʱ���.
	
	 GPIO_PORT port;
	 GPIO_PIN  pin;
	
}key_state;

#define MAX_KEY_NUM 2
#define KEY_PRESS_TIME 3
#define KEY_ZERO_PRESS_TIME 5
#define TIMER_INT 100
#define PRESS_ALL_KEY_TIME 4000
#define PRESS_LONG_TIME 2000
#define PWR_DELAY_NUM 10
#define ZERO_RLEASE_DELAY_NUM 10
#define PRESS_TIME_TICK 2
static key_state key_states[MAX_KEY_NUM];
static int32 g_ts = 0;
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
	key_states[0].key = KEY_PWR;
	key_states[0].state = KEY_PRESS_RLEASED;
	key_states[1].port = KEY_ZERO_PORT;
	key_states[1].pin = KEY_ZERO_PIN;
	key_states[1].key = KEY_ZERO;
	key_states[1].state = KEY_PRESS_RLEASED;
}

void send_key_msg(uint8 key, uint8 event)
{
  struct key_msg req;

	req.key = key;
	req.event = event;
	req.ts = g_ts;
	key_cb(&req);
}
void key_press_handle()
{

}
void  key_isr()
{
	int i = 0;
	g_ts++;
	for(i = 0; i < 2; i++)
	{
		if(GPIO_GetPinStatus(key_states[i].port,key_states[i].pin) )
		{
					if(key_states[i].state == KEY_LONG_PRESSED)
					{
							continue;
					}
					//key_states[i].press_stamp++;
					key_states[i].press_ts++;
					
					//key_states[0].state = KEY_PRESSED;
					if(key_states[i].press_ts > (PRESS_LONG_TIME/TIMER_INT) )
					{
						//��Դ����ס3����
								uint8 j = (i==0?1:0);
								if(GPIO_GetPinStatus(key_states[j].port,key_states[j].pin))
								{
										if(key_states[i].press_ts > (PRESS_ALL_KEY_TIME/TIMER_INT))
										{
												//�����Ҳ�ǰ�ס��
												key_states[j].state = KEY_LONG_PRESSED; 
												send_key_msg(KEY_ALL, KEY_LONG_PRESSED);
										}else{
												continue;
										}
									
								}
								else
								{
										send_key_msg(key_states[i].key, KEY_LONG_PRESSED);
								}
								key_states[i].state = KEY_LONG_PRESSED; 
								key_states[0].press_ts = 0; //���㰴�����¼�����.
								key_states[1].press_ts = 0; //���㰴�����¼�����.
								continue; //�Ѿ������˰����¼�
					}
					else if(key_states[i].press_ts >= PRESS_TIME_TICK && key_states[i].state != KEY_PRESSED)
					{
							//�̰��¼�����.
							send_key_msg(key_states[i].key, KEY_PRESSED);
							key_states[i].state = KEY_PRESSED; 
					}
		}
		else
		{
					//�������ͷ��¼�������Ҫ�а���ʱ�䣬�Ų����ͷ��¼�.
					if(key_states[i].press_ts >= PRESS_TIME_TICK &&  key_states[i].state != KEY_PRESS_RLEASED)
					{
							//�������������ͷ���
							
							send_key_msg(key_states[i].key, KEY_PRESS_RLEASED);						
					}
					key_states[i].state = KEY_PRESS_RLEASED;
				  key_states[i].press_ts = 0;
					
		}
	}
}
//���������߼�,��ʱ����.
void  key_isr2()
{
		
	int i = 0;
		
	 #if 1
		g_ts++;
		if(GPIO_GetPinStatus(KEY_PWR_PORT,KEY_PWR_PIN) )
		{
					if(key_states[0].state == KEY_LONG_PRESSED)
					{
							return;
					}
					key_states[0].press_ts++;
					
					//key_states[0].state = KEY_PRESSED;
					if(key_states[0].press_ts > (PRESS_ALL_KEY_TIME/TIMER_INT) )
					{
						//��Դ����ס3����
								if(GPIO_GetPinStatus(KEY_ZERO_PORT,KEY_ZERO_PIN))
								{
									//�����Ҳ�ǰ�ס��
										send_key_msg(KEY_ALL, KEY_LONG_PRESSED);
								}
								else
								{
										send_key_msg(KEY_PWR, KEY_LONG_PRESSED);
								}
								key_states[0].state = KEY_LONG_PRESSED; 
								key_states[0].press_ts = 0; //���㰴�����¼�����.
								key_states[1].press_ts = 0; //���㰴�����¼�����.
								return; //�Ѿ������˰����¼�
					}
					else if(key_states[0].press_ts >= PRESS_TIME_TICK && key_states[0].state != KEY_PRESSED)
					{
							//�̰��¼�����.
							send_key_msg(KEY_PWR, KEY_PRESSED);
							key_states[0].state = KEY_PRESSED; 
					}
		}
		else
		{
					//�������ͷ��¼�������Ҫ�а���ʱ�䣬�Ų����ͷ��¼�.
					if(key_states[0].press_ts >= PRESS_TIME_TICK &&  key_states[0].state != KEY_PRESS_RLEASED)
					{
							//�������������ͷ���
							send_key_msg(KEY_PWR, KEY_PRESS_RLEASED);						
					}
					key_states[0].state = KEY_PRESS_RLEASED;
				  key_states[0].press_ts = 0;
					
		}

		if(GPIO_GetPinStatus(KEY_ZERO_PORT,KEY_ZERO_PIN))
		{
					if(key_states[1].state == KEY_LONG_PRESSED)
					{
							return;
					}
					key_states[1].press_ts++;
					
					if(key_states[1].press_ts > (PRESS_ALL_KEY_TIME/TIMER_INT) )
					{
								if(GPIO_GetPinStatus(KEY_PWR_PORT,KEY_PWR_PIN))
								{
										send_key_msg(KEY_ALL, KEY_LONG_PRESSED);		
								}
								else
								{
										send_key_msg(KEY_ZERO, KEY_LONG_PRESSED);
								}
								key_states[1].state = KEY_LONG_PRESSED; 
								key_states[1].press_ts = 0;
								key_states[0].press_ts = 0; //���㰴�����¼�����.
								return;
						  
					}
					else if(key_states[1].press_ts >= PRESS_TIME_TICK && key_states[1].state != KEY_PRESSED)
					{
							//�̰��¼�����.
							send_key_msg(KEY_ZERO, KEY_PRESSED);
							key_states[1].state = KEY_PRESSED; 
					}
			
					
		}
		else
		{
					if(key_states[1].press_ts >= PRESS_TIME_TICK && key_states[1].state != KEY_PRESS_RLEASED)
					{
							//�������������ͷ���
							send_key_msg(KEY_ZERO, KEY_PRESS_RLEASED);
							
					}
					key_states[1].state = KEY_PRESS_RLEASED;				
				  key_states[1].press_ts = 0;
		}
	#endif
		
}
uint8 key_power_off(void)
{
	//
		//util_delay(20000);
		
		if(	GPIO_GetPinStatus(KEY_PWR_PORT,KEY_PWR_PIN) == 0)
		{
			//����ɿ��������򲻹ػ�.
				//return 0;
		}
		power_ctrl(PWR_OFF);
		
}