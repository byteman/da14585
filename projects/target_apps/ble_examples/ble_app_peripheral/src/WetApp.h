
#ifndef WET_APP_H
#define WET_APP_H

#include "std_types.h"

typedef enum
{ 
	LIMITS_UP = 0x00, 
	LIMITS_OK,
	LIMITS_LOW,
	
}ELIMITS;

typedef enum
{ 
	MODE_CONT_OUT = 0x01,  //�������
	MODE_ST1	= 0x02,		//�����ȶ����һ��
	MODE_STC	= 0x04,		//�ȶ������������
	MODE_PR1	= 0x08,		//����ӡ���1��
	MODE_PR2	= 0x10,		//�ֶ����
	MODE_AUTO	= 0x20,		//�Զ����
	MODE_ASK	= 0x40,		//Զ���������
	MODE_WIREL	= 0x80,
	
}EMODECHECK;


typedef enum
{ 
	SCALE_EMPTY = 0x00, 
	SCALE_CHECK_WET,
	SCALE_CHECK_PCS,
	SCALE_ADDUP_HANDLE,
	SCALE_ADDUP_AUTO,
	SCALE_NORMAL_WET,
	
}ESCALEMODE;

typedef struct 
{
	INT16U			stStable:1;				 
	INT16U			stZero:1;				 
	INT16U			stNet:1;			 
	INT16U			stUpOverLoad:1;		 
	INT16U			stDownOverLoad:1;		 
	INT16U			stWetRang:1;  //���̶�
	INT16U			stWetX10:1;
	INT16U			stMenuMode:1;
	INT16U			stWorkMode:1;//����ģʽ

	INT16U			stUnit_KG:1;//kg
	INT16U			stUnit_g:1;//g
	INT16U			stUnit_lb:1;//lb
	
}WET_STATUE;
typedef union 
{
	WET_STATUE	scaleBit;
	INT16U	value;
}WETSTATUE;

typedef struct {
	INT8U	mWaveOut;
	INT8U	mPeekCath; //
	INT8U	mNet;
	INT8U 	mKeyLockEN;
	INT16U 	mKeyLockCnt;
	INT8U 	mScaleSleepFlag;  //�ӹ���ģ;ʽ
	INT8U 	mScaleCheckStatue;		//����״ֵ̬
	WET_STATUE 	mScaleStatue;
	INT32U mScalePCS;		//������¼
	INT32S mScaleWet;		//���ؼ�¼
	INT32S mScaleNetWet;	//����
	INT32U mScaleWetInter;	//û�зֶȻ�������ֵ
	INT32S mScaleConvertWet; //�л��������
	INT32U mScaleTotalWet;	//�ۻ�����
	INT32U mScaleTotalNum;	//�ۻ�����
	INT32U mScaleSingleWet; //��������
	INT32S mScalePeekWet;/// ���ֵ
}WETAPP_MODE;

extern WETAPP_MODE gScaleAppData;

extern void WetApp_Set_ShowAddPt(INT8U pt);
extern INT8U WetApp_Get_ShowAddPt(void );
extern INT32U WetApp_Get_ShowAddWet(void );

extern INT8U WetApp_Get_ShowPt(void );
extern INT8U WetApp_Get_RealPt(void);
extern INT8U WetApp_Get_StableFlag(void);
extern void WetApp_Printf_Weighing(void);
extern void WetApp_Printf_OutPut(void);
extern INT8U WetApp_Get_AddUp_New(void);
extern void WetApp_Tolerance_Check_Wet(void);
extern void WetApp_Tolerance_Check_Pcs(void);
extern INT8U WetApp_Auto_AddUp_AddWet(void);
extern void WetApp_Handle_AddUp_AddWet(void);
extern void WetApp_AddUp_AddWet(void);
extern void WetApp_Animal_Manage(void);
extern void WetApp_SetKeyLock(INT8U lock);
extern INT8U WetApp_GetKeyLock(void );
extern void WetApp_Cal_Zero(INT32S avg);
extern void	WetApp_Cal_Wet(INT32S avg);
extern void WetApp_Work_En(INT8U en);
extern void WetApp_Tare2Gross(void );
extern void WetApp_CheckNet(void );
extern INT8U WetApp_CheckWetRange(void);
extern void WetApp_SetZero(void );
extern void WetApp_SetTare(void );
extern Std_ReturnType	WetApp_Init(void);
extern void Wet_Service(void);
extern void wet_set_work_mode(INT8U mod);
extern void com_flash(void);

#endif

