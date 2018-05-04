
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
	MODE_CONT_OUT = 0x01,  //连续输出
	MODE_ST1	= 0x02,		//重量稳定输出一次
	MODE_STC	= 0x04,		//稳定重量连续输出
	MODE_PR1	= 0x08,		//按打印输出1此
	MODE_PR2	= 0x10,		//手动求和
	MODE_AUTO	= 0x20,		//自动求和
	MODE_ASK	= 0x40,		//远程命令访问
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
	INT16U			stWetRang:1;  //量程段
	INT16U			stWetX10:1;
	INT16U			stMenuMode:1;
	INT16U			stWorkMode:1;//工作模式

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
	INT8U 	mScaleSleepFlag;  //秤工作模;式
	INT8U 	mScaleCheckStatue;		//监测的状态值
	WET_STATUE 	mScaleStatue;
	INT32U mScalePCS;		//检数记录
	INT32S mScaleWet;		//检重记录
	INT32S mScaleNetWet;	//净重
	INT32U mScaleWetInter;	//没有分度化的重量值
	INT32S mScaleConvertWet; //切换后的重量
	INT32U mScaleTotalWet;	//累积总重
	INT32U mScaleTotalNum;	//累积总数
	INT32U mScaleSingleWet; //单次重量
	INT32S mScalePeekWet;/// 最大值
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

