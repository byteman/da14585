
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WEIGHING_H
#define __WEIGHING_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"

/* Exported types ------------------------------------------------------------*/
typedef struct 
{
	INT32U			stableAD:1;		//AD稳定
	INT32U			stable:1;		//稳定
	INT32U			zero:1;			//零位
	INT32U			overADC:1;		//ADC超载
	INT32U			overGross:1;	//毛重超载
	INT32U			overNET:1;		//净重超载
	INT32U			underGross:1;	//毛重欠载
	INT32U			dnew:1;			//新数据
	INT32U			err:1;			//错误
	INT32U			busy:1;			// 忙
	INT32U			wetready:1;		//称重模块就绪
	INT32U			range:1;		//量程指示，0=range1，1=range2
	INT32U			aztflg:1;		//零跟踪指示，1=activated 
	
}WET_STATE;

typedef union 
{
	WET_STATE 	state;
	INT32U 		value; 
}uWET_STATE;


typedef struct 
{
	INT32S	mWet_Stable;	//重量化后稳定处理了
	INT32S	mWet_newest;	//线性化处理
	INT32S	mWet_gross;		//分度值处理
	INT32S	mWet_net;		//去除皮重
}WET_DATA;

typedef struct 
{
	INT32U			errpara:1;				// 参数超出允许范围
	INT32U			errAD:1;				// ADC初始化失败
	INT32U			errWorkTask:1;			// 定时称重任务初始化失败
	INT32U			errStableTask:1;		// 定时稳定任务初始化失败
	INT32U			errZeroTrackTask:1;		// 定时零跟任务初始化失败

	INT32U			err_poweron_set_zero:1;
	INT32U			err_set_zero:1;
	INT32U			err_ad_range:1;
	INT32U			err_wet_stable:1;
	INT32U			err_com:1;
	INT32U			err_set_tare:1;
	INT32U			err_adjustment:1;
	INT32U			err_printf:1;
	INT32U			err_battery_low:1;	
	
}WET_ERR;

typedef union 
{
	WET_ERR 	state;
	INT32U 		value; 
}uWET_ERR;


typedef void (*WET_AVG_PROC)(INT32S);
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern  void Wet_InitZero(void);
extern	Std_ReturnType	Wet_Init(void);
extern	Std_ReturnType	Wet_InitPara(void);
extern	INT32S			Wet_Read(WET_STATE* wt);
extern	void			Wet_Read_Wet(WET_DATA* wt);
extern	void			Wet_ErrRead(WET_ERR* err);
extern  void 			Wet_Read_Err(uWET_ERR *wts);
extern	Std_ReturnType	Wet_StartAvg(INT32U num,WET_AVG_PROC fn);
extern  void	Wet_Process(INT32S adcode,FP32 temp);	
extern	Std_ReturnType	Wet_Zeroing(void);
extern	Std_ReturnType	Wet_Taring(void);
extern	void Wet_Filter_Set(int cal);
//extern	Std_ReturnType	Wet_SetTare(INT32S tare);
//extern	INT32S			Wet_ReadTare(void);
//extern	Std_ReturnType	Wet_SetGorN(INT32S gorn);
//extern	INT32S			Wet_ReadGorN(void);

#endif /* __WEIGHING_H */
