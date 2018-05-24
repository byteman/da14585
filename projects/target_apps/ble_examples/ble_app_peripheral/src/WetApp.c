/**
  ************************************************************************* 
 *	Filename :	Print.C
 *
 *	Version	  Author	QQ			Date		Description
 *	V1.0	 	 dtcoff      364283622	  2017.01.16	New
 *
 *	Scope :		 
 *
 *	Function(s)	
 *
 *	COPYRIGHT (C) 2017.	All Rights Reserved. WY
 *
 **************************************************************************
 *                                   Update
 * @Version : V1.0.1
 * @By      : dtcoff      
 * @Date    : 2017 / 7 / 20
 * @Brief   : 
 *
 **************************************************************************
 **/

#include "common.h"
#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "timer.h"
#include "param.h"
#include "Weighing.h"
#include "WetApp.h"
//#include "UnitSwitch.h"

TID		mWetAppID;

WETAPP_MODE gScaleAppData;
WET_STATE	gWetStatue;
WET_DATA	gWetData;

INT8U	g_ERR_ZERO_Value = 1;
INT8U gMenuOprt = 0;


static INT8U gCalEndFlag = 0;
static INT8U gCalErrFlag = 0;
static INT8S gCalDelay = 0;
static INT8U gCalMenuFlag = 0;


static INT32S	gCalLineNov[6];
static INT32S	gCalLinePt[6];
static INT16S	g_adj_flag_cnt=0 ;
static device_param* g_param;
static PARA_USER_T * g_user;


void WetApp_Poweroff_Manage(void)
{
	static	INT16U	mBlOffCnt = 0;
	static 	INT16U	mBlOffTmr = 0;
	INT8U	PowrOffArr[] = {0,3,5,15,30};

	/// 自动关机管理
	if(g_param->mBLAuto >4)
		g_param->mBLAuto = 4;
	mBlOffTmr = PowrOffArr[g_param->mBLAuto] * 10 * 60 ;
	if(mBlOffTmr != 0)
	{
		if((gScaleAppData.mScaleNetWet <  g_user->RSN * 2)  )
		{
			mBlOffCnt++;
			if(mBlOffCnt >= mBlOffTmr)
			{
				mBlOffCnt = 0;
				//Power_OFF();
			}
		}
	}

}


static int g_work_mode = 1;
void wet_set_work_mode(INT8U mod)
{
	g_work_mode = mod;
	CS1237_Power(g_work_mode);
	if(mod == 0)
		gScaleAppData.mScaleStatue.stWorkMode = 1; //睡眠
	else
		gScaleAppData.mScaleStatue.stWorkMode = 0;
}

static INT8U g_led_type = 0;
static INT8U g_com_flash_flag = 0;
static INT8U g_com_flash_cnt = 0;
void com_flash(void)
{
	g_com_flash_flag = 1;
	g_com_flash_cnt = 0;	
}
void	LED_Flash(void)
{


}



/******************************************************************************
  * @brief	Wet 按分度值整数倍输出，多分数值处理  
  * @param  none
  * @retval none
******************************************************************************/
INT32S	WetApp_IncFormat(float wet)
{
	FP32 tempF;
	INT32S	curRSN;
	//curRSN = Unit_Convert_UnitIncr(1);
	curRSN = g_user->RSN;
	tempF = (FP32)wet / curRSN;	
	if (tempF < 0)	tempF -= 0.5;
	else			tempF += 0.5;		

	return ((INT32S)tempF * curRSN);
}

INT32S WetApp_Convert_Data(float inwet)
{
		INT32S CvtWet  = 0;
#if 0
	static float wet = 0;
	INT8U  mdiv = 0;
	INT8U  i = 0;
	
	wet = Unit_Convert_UnitData(inwet,2);
	CvtWet  = (INT32S)wet ;	
	CvtWet = WetApp_IncFormat(CvtWet);

	
	mdiv   = Unit_Get_DivCnt();
	for(i = 0;i < mdiv ;i++)
	{
		CvtWet /= 10;
	}

#else
	CvtWet = WetApp_IncFormat(inwet);
#endif	
	return CvtWet;
	
}

INT8U WetApp_Get_RealPt(void)
{
	return 0;
	//return g_ParaUser.DPT + gScaleAppData.mScaleStatue.stWetX10;
}



void WetApp_Cal_Zero(INT32S avg)
{
	Std_ReturnType	rtn;
	//if((avg > -1000000)&&(avg < 1000000))
	{
		g_user->LDW = avg;
		Wet_InitPara();
		Wet_InitZero();
		//rtn = Para_Save_block(EE_ADDR_USER,(INT8U*)&g_ParaUser,sizeof(PARA_USER));
	}
}

void	WetApp_Cal_Wet(INT32S avg)
{
	Std_ReturnType	rtn;
	if(avg > g_user->LDW)
	{
		g_user->LWT = avg;
		Wet_InitPara();
		Wet_InitZero();
		//rtn = Para_Save_block(EE_ADDR_USER,(INT8U*)&g_ParaUser,sizeof(PARA_USER));
	}
	

}

void WetApp_Work_En(INT8U en)
{
	if(en )
	{
		//Timer_Enable(mWetAppID);
	}
	else
	{
		//Timer_Disable(mWetAppID);
	}
}



void WetApp_SetZero(void )
{
	//if(!gScaleAppData.mNet )
	{
		g_ERR_ZERO_Value = Wet_Zeroing();
	}
		// gScaleAppData.mScaleStatue.stClearZeroFail ;
}

INT8U WetApp_Get_StableFlag(void)
{
	return gWetStatue.stable;
}
/******************************************************************************
  * @brief	初始化
  * @param	None
  * @retval TRUE: 初始化成功	FALSE: 初始化失败 
******************************************************************************/
Std_ReturnType	WetApp_Init(void)
{
	int i = 0;
	float tempadd = 0.0;
	//gAddTotal = 0.0;
	//gAddTotalShow  = 0;
	param_get_user(&g_user);
	param_get(&g_param);
	gScaleAppData.mWaveOut = 0;
	gScaleAppData.mScaleTotalWet = 0;
	gScaleAppData.mScaleTotalNum = 0;	
	gScaleAppData.mScaleSleepFlag = 0;
	
	
	return TRUE;
}

extern void AD_Working(void);
extern void	Wet_Working(void);

void Wet_Service(void)
{
	Wet_StableWt();
	Wet_ZeroTracking();
#if 1	
	//Wet_Working();

	Wet_Read(&gWetStatue);
	
	if(gWetStatue.dnew )
	{
		gWetStatue.dnew = 0;
		Wet_Read_Wet(&gWetData);
		
		if(gScaleAppData.mScaleStatue.stWetX10)
		{
			//gWetData.mWet_newest; //借用这个变量显示放大10倍的重量
			gScaleAppData.mScaleConvertWet	= WetApp_Convert_Data(gWetData.mWet_newest); //gWetData.mWet_newest
			gScaleAppData.mScaleNetWet 	= gScaleAppData.mScaleConvertWet - WetApp_Convert_Data(g_user->TAV * 10);//gWetData.mWet_net;	//净重
		}
		else
		{
			gScaleAppData.mScaleConvertWet =  gWetData.mWet_gross;//WetApp_Animal_Mode(gWetData.mWet_gross);
			//gScaleAppData.mScaleConvertWet	= WetApp_Convert_Data(gScaleAppData.mScaleConvertWet); //gWetData.mWet_newest
			gScaleAppData.mScaleNetWet 	= gWetData.mWet_net;//gScaleAppData.mScaleConvertWet - WetApp_Convert_Data(g_ParaUser.TAV);//gWetData.mWet_net;	//净重

		}	
		gScaleAppData.mScaleWet 	= gScaleAppData.mScaleConvertWet;   //毛重
		gScaleAppData.mScaleWetInter	= gWetData.mWet_Stable;
		gScaleAppData.mScaleStatue.stReady = gWetStatue.wetready;
		//更新秤状态
		gScaleAppData.mScaleStatue.stStable = gWetStatue.stable;
		gScaleAppData.mScaleStatue.stZero = gWetStatue.zero;
		//gScaleAppData.mScaleStatue.stNet = gScaleAppData.mNet ;  //通过外部操作切换
		gScaleAppData.mScaleStatue.stUpOverLoad= gWetStatue.overGross;
		gScaleAppData.mScaleStatue.stDownOverLoad = gWetStatue.underGross;
		//gScaleAppData.mScaleStatue.stWetX10 = ;
		gScaleAppData.mScaleStatue.stMenuMode = 0;
		
		if(gScaleAppData.mScaleNetWet < g_user->RSN * 2) // <20d表示卸载货物
		{
			//mAddUpFlag = 0;
		}

	}
	calib_prase_handler();
	#endif
}

