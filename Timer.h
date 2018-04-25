

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMER_H
#define __TIMER_H
/* Includes ------------------------------------------------------------------*/
#include "common.h"

/* Exported types ------------------------------------------------------------*/
typedef void (*TIME_OUT_PROC)(void);

typedef enum
{ 
	ONE_TIME = 0x00, 
	LOOP = 0x01,
}TIMERTYPE;

typedef struct 
{
	INT8U			tid;
	INT8U			used;
	TIMERTYPE		typ; 				//0:ONE_TIME 1:LOOP
	INT8U			suspend; 			//����ı�־
	INT32U			reload_timeout_ms;	// �жϼ��. 
	INT32U			timeout_ms; 		//  
	TIME_OUT_PROC	fn; 				//��ʱ������
}timer,*p_timer;

typedef p_timer TID;
/* Exported constants --------------------------------------------------------*/
#define MAX_TIMER_NUM	16
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern	Std_ReturnType Timer_Init(void);			//1 mS
extern	TID Timer_Regist(TIMERTYPE typ,INT32U ms, TIME_OUT_PROC fn);
extern	Std_ReturnType Timer_Delete(TID pt);
extern	Std_ReturnType Timer_Enable(TID pt);
extern	Std_ReturnType Timer_Disable(TID pt);
extern	void Timer_Server(void);					//������ѭ���е���
/*�벻Ҫ�ڸ��ж������ܶ����飬���������ж�Ƕ��*/
extern	Std_ReturnType Timer6_Init(INT32U us,TIME_OUT_PROC fn);	//��СΪ100uS�����Ϊ6553600uS(6.5536S)
extern	Std_ReturnType Timer6_Close(void);

extern	void Delay(INT32U nClock);					//�������ʱ

#endif /* __TIMER_H */
