/*******************************************************************************
  * @file    Timer.c 
  * @author  yangxd
  * @version V 0.01
  * @date    2013.05.29
  * @brief   ��ʱ��ģ��
  ******************************************************************************
  * @attention
  *
  *
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "Timer.h"
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static timer timer_list[MAX_TIMER_NUM];
static TIME_OUT_PROC Timer6_fn = NULL;
static	INT32U	g_Flg_T6inited = FALSE;
static INT32U g_second_tick = 0;
/* Private functions ---------------------------------------------------------*/
//static void Timer_SysTickConfig(void);
#define SYSTICK_PERIOD_US   1000000     // period for systick timer in us, so 1000000ticks = 1second
#define SYSTICK_EXCEPTION   1           // generate systick exceptions
void SysTick_Event(void);
void systick_isr(void)
{
		g_second_tick++;
		//SysTick_Event();
}
INT32U GetTick(void)
{
		return g_second_tick;
}

/******************************************************************************
  * @brief  ���� SysTick Ϊ 1 ���붨ʱ�ж�.
  * @param  None
  * @retval None
******************************************************************************/
void Timer_SysTickConfig(void)
{
	 systick_register_callback(systick_isr);
    // Systick will be initialized to use a reference clock frequency of 1 MHz
   systick_start(SYSTICK_PERIOD_US, SYSTICK_EXCEPTION);
}


/******************************************************************************
  * @brief  ��ʼ����ʱ��ģ��
  * @param  None
  * @retval TRUE
******************************************************************************/
Std_ReturnType Timer_Init(void)
{
	timer* p;
	 
	for (p = timer_list ; p < timer_list + MAX_TIMER_NUM ; p++)
	{
	 	p->used = FALSE;
		p->typ = ONE_TIME;
		p->suspend = FALSE;
		p->reload_timeout_ms = 0;
		p->timeout_ms = 0;
		p->fn = NULL;
	}

  Timer_SysTickConfig();
	return TRUE;
}
/******************************************************************************
  * @brief  SysTick �жϵ��ã����¶�ʱ���б�
  * @param  None
  * @retval None
******************************************************************************/
void SysTick_Event(void)
{
	timer* p;
	 
	for (p = timer_list ; p < timer_list + MAX_TIMER_NUM ; p++)
	{
		if((p->used == TRUE)&&(p->reload_timeout_ms != 0))
		{
			if(p->timeout_ms)
			{
				p->timeout_ms--;
			}
			else
			{
				p->suspend = TRUE;
				if(p->typ == LOOP)
				{
					p->timeout_ms = p->reload_timeout_ms;
				}	
			}
		}
	}
}

/******************************************************************************
  * @brief	��ʱ��������ã�������ѭ���е��øú������Ӷ�ִ�����ж�ʱ���¼�  
  * @param  None
  * @retval None
******************************************************************************/
void Timer_Server(void)
{
	timer* p;
	 
	for (p = timer_list ; p < timer_list + MAX_TIMER_NUM ; p++)
	{
		if((p->used == TRUE)&&(p->fn != NULL))
		{
			if(p->suspend == TRUE)
			{
				p->fn();			//
				p->suspend = FALSE;
				if(p->typ == ONE_TIME)
				{
					p->used = FALSE;
					//p->fn = NULL;	
				}
			}
		}
	}
}
/******************************************************************************
  * @brief	ע��һ����ʱ��  
  * @param  typ������ / ѭ��
  			ms����ʱ�����ڣ���λΪ ����
			fn����ʱ���¼��ص�����   NULL��ִ�к���
  * @retval ��ʱ��ָ��	NULL: ע��ʧ��
******************************************************************************/
TID Timer_Regist(TIMERTYPE typ,INT32U ms, TIME_OUT_PROC fn)
{
	timer* p;

	if(ms == 0)
		return NULL;	//���������� 
	 
	for (p = timer_list ; p < timer_list + MAX_TIMER_NUM ; p++)
	{
		if((p->used == FALSE)&&(p->reload_timeout_ms == 0))
		{
			p->typ = typ;
			p->suspend = FALSE;
			p->reload_timeout_ms = ms;
			p->timeout_ms = ms;
			p->fn = fn;
			p->used = TRUE;
			
			return p;		
		}
	}
	return NULL;		//��ʱ������
}
/******************************************************************************
  * @brief	ɾ��һ����ʱ��  
  * @param  ��ʱ��ָ��
  * @retval TRUE: ɾ���ɹ�	FALSE: û�иö�ʱ��
******************************************************************************/
Std_ReturnType Timer_Delete(TID pt)
{
	if(pt == NULL)
		return FALSE;
	if((pt->used == TRUE)&&(pt->reload_timeout_ms != 0))
	{
		pt->used = FALSE;
		pt->typ = ONE_TIME;
		pt->suspend = FALSE;
		pt->reload_timeout_ms = 0;
		pt->timeout_ms = 0;
		pt->fn = NULL;
		return TRUE;
	}
	return FALSE;		
}
/******************************************************************************
  * @brief	�򿪶�ʱ��  
  * @param  ��ʱ��ָ��
  * @retval TRUE: ɾ���ɹ�	FALSE: û�иö�ʱ��
******************************************************************************/
Std_ReturnType Timer_Enable(TID pt)
{
	if(pt == NULL)
		return FALSE;	
	if(pt->reload_timeout_ms != 0)
	{
		pt->timeout_ms = pt->reload_timeout_ms;
		pt->suspend = FALSE;
		pt->used = TRUE;
		return TRUE;
	}
	return FALSE;
}
/******************************************************************************
  * @brief	�رն�ʱ��  
  * @param  ��ʱ��ָ��
  * @retval TRUE: ɾ���ɹ�	FALSE: û�иö�ʱ��
******************************************************************************/
Std_ReturnType Timer_Disable(TID pt)
{
	if(pt == NULL)
		return FALSE;
	if(pt->reload_timeout_ms != 0)
	{
		pt->used = FALSE;
		pt->suspend = FALSE;
		pt->timeout_ms = 0;			
		return TRUE;		
	}
	return FALSE;
}
#if 0
/******************************************************************************
  * @brief  ��ʱ�� 6 ��ʼ�������ڻ�����ʱ������СΪ1uS�����Ϊ65536uS(65.536 mS)
  * @param  us: ��ʱ����Ϊ us ��΢�
  			fn����ʱ���¼��ص�����
  * @retval TRUE����ʼ���ɹ�
  			FALSE��������������⡢��ö�ʱ���Ѿ���ʼ��
******************************************************************************/
Std_ReturnType Timer6_Init(INT32U us,TIME_OUT_PROC fn)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_ClocksTypeDef RCC_Clocks;
	INT16U Prescaler;

	if(g_Flg_T6inited == TRUE)
		return	FALSE;

	RCC_GetClocksFreq(&RCC_Clocks);
	Prescaler = RCC_Clocks.SYSCLK_Frequency / 1000000;		//Ԥ��Ƶ�� 1uS
	if(us < 1)return FALSE;		//̫С
	if(us > 65536)return FALSE;		//̫��
	if(fn == NULL)return FALSE;		//�¼���Ч
	Timer6_fn = fn;
	

	/* TIM6 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	/* Enable the TIM3 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = 1;//TIM6_DAC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = (us - 1);				//���65535
	TIM_TimeBaseStructure.TIM_Prescaler = (Prescaler - 1);		//Ԥ��Ƶ�� 1uS
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//  
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);				//  

	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM6, ENABLE);  									//������ʹ��
	
	g_Flg_T6inited = TRUE;
	return TRUE;
}
/******************************************************************************
  * @brief  �رն�ʱ�� 6 
  * @param  None
  * @retval TRUE���ɹ�
  			FALSE��
******************************************************************************/
Std_ReturnType Timer6_Close(void)
{
	TIM_Cmd(TIM6, DISABLE);
	Timer6_fn = NULL;
	g_Flg_T6inited = FALSE;
	
	return TRUE;	
}
/******************************************************************************
  * @brief  Timer6 �¼���������ֻ������Ҫ���ݲɼ�
  * @param  None
  * @retval None
******************************************************************************/
void Timer6_Event(void)
{
	/*�벻Ҫ��������̫�������*/
	if(Timer6_fn != NULL)Timer6_fn();
}
#endif
/******************************************************************************
  * @brief  �����ʱ Delay = (n*4 + 49)(1/19.6608) us
  * @param  nTime:	
  			1:2.69us 	100:22.837us	1000:205.942us		
			10000:2.037ms	100000:20.347ms		1000000:203.453ms
  * @retval None
******************************************************************************/
//void Delay(__IO INT32U nClock)
//{
//	for(; nClock > 0; nClock--);
//}



