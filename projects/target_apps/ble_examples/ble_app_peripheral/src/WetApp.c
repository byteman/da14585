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

static MenuCmd_t  menuCmd ;
static GuiMsgInfo guiMsgInfo;

static INT32S	gCalLineNov[6];
static INT32S	gCalLinePt[6];
extern u16 g_adj_flag_cnt ;

 

void WetApp_Cal_Liner_P0(INT32S value)
{
	int temp = 0;

	temp = value - g_ParaPlatForm.mLinerPt[0]  ;
	g_ParaPlatForm.mLinerPt[0] = value;
	g_ParaPlatForm.mLinerNov[0]  = 0;

	g_ParaPlatForm.mLinerPt[1] += temp;
	g_ParaPlatForm.mLinerPt[2] += temp;
	g_ParaPlatForm.mLinerPt[3] += temp;
	g_ParaPlatForm.mLinerPt[4] += temp;
	g_ParaPlatForm.mLinerPt[5] += temp;
	
	//gCalDelay = 1;
	g_adj_flag_cnt = 0;
	gCalErrFlag = 0;
	Wet_InitZero();

}
void WetApp_Cal_Liner_P1(INT32S value)
{

	if(g_ParaPlatForm.mLinerNov[1] * 2 > (value  - g_ParaPlatForm.mLinerPt[0] ) ) //10万分子一
		return;
	
	g_ParaPlatForm.mLinerPt[1] = value;
	g_ParaPlatForm.mLinerPt[2] = value;
	g_ParaPlatForm.mLinerPt[3] = value;
	g_ParaPlatForm.mLinerPt[4] = value;
	g_ParaPlatForm.mLinerPt[5] = value;

	g_ParaPlatForm.mLinerNov[0]  = 0;

	g_ParaPlatForm.mLinerNov[2] =  g_ParaPlatForm.mLinerNov[1]; 
	g_ParaPlatForm.mLinerNov[3] =  g_ParaPlatForm.mLinerNov[1]; 
	g_ParaPlatForm.mLinerNov[4] =  g_ParaPlatForm.mLinerNov[1]; 
	g_ParaPlatForm.mLinerNov[5] =  g_ParaPlatForm.mLinerNov[1]; 

	if(g_ParaPlatForm.mLinerPt[0] >= value)
	{
		gCalErrFlag = 1;
		g_adj_flag_cnt = 1;
	}
	g_adj_flag_cnt = 0;
	//gCalDelay = 1;
	Wet_InitZero();
	Para_Save_block(EE_ADDR_PLATFORM,(INT8U*)&g_ParaPlatForm,sizeof(PARA_PLATFORM));
}

void WetApp_Cal_Liner_P2(INT32S value)
{
	if(g_ParaPlatForm.mLinerPt[1] >= value)
	{
		gCalErrFlag = 1;
		g_adj_flag_cnt = 1;
		return ;
	}
	
	g_ParaPlatForm.mLinerPt[2] = value;
	g_ParaPlatForm.mLinerPt[3] = value;
	g_ParaPlatForm.mLinerPt[4] = value;
	g_ParaPlatForm.mLinerPt[5] = value;	

	g_ParaPlatForm.mLinerNov[3] =  g_ParaPlatForm.mLinerNov[2]; 
	g_ParaPlatForm.mLinerNov[4] =  g_ParaPlatForm.mLinerNov[2]; 
	g_ParaPlatForm.mLinerNov[5] =  g_ParaPlatForm.mLinerNov[2]; 
	
	g_adj_flag_cnt = 0;
	gCalEndFlag = 1;
	//gCalDelay = 1;
	Wet_InitZero();
	Beep(3,3);
	Para_Save_block(EE_ADDR_PLATFORM,(INT8U*)&g_ParaPlatForm,sizeof(PARA_PLATFORM));
}	
void WetApp_Cal_Liner_P3(INT32S value)
{

	if(g_ParaPlatForm.mLinerPt[2] >= value)
	{
		gCalErrFlag = 1;
		g_adj_flag_cnt = 1;
		return;
	}
	g_ParaPlatForm.mLinerPt[3] = value;
	g_ParaPlatForm.mLinerPt[4] = value;
	g_ParaPlatForm.mLinerPt[5] = value;	

	g_ParaPlatForm.mLinerNov[4] =  g_ParaPlatForm.mLinerNov[3]; 
	g_ParaPlatForm.mLinerNov[5] =  g_ParaPlatForm.mLinerNov[3]; 
	

	g_adj_flag_cnt = 0;
	gCalEndFlag = 1;
	//gCalDelay = 1;
	Wet_InitZero();
	Beep(3,3);
	Para_Save_block(EE_ADDR_PLATFORM,(INT8U*)&g_ParaPlatForm,sizeof(PARA_PLATFORM));
}	
void WetApp_Cal_Liner_P4(INT32S value)
{
	if(g_ParaPlatForm.mLinerPt[3] >= value)
	{
		gCalErrFlag = 1;
		g_adj_flag_cnt = 1;
		return;
	}
	
	g_ParaPlatForm.mLinerPt[4] = value;
	g_ParaPlatForm.mLinerPt[5] = value;	
	g_ParaPlatForm.mLinerNov[5] =  g_ParaPlatForm.mLinerNov[4]; 

	g_adj_flag_cnt = 0;
	gCalEndFlag = 1;
	//gCalDelay = 1;
	Wet_InitZero();
	Beep(3,3);
	Para_Save_block(EE_ADDR_PLATFORM,(INT8U*)&g_ParaPlatForm,sizeof(PARA_PLATFORM));
}	
void WetApp_Cal_Liner_P5(INT32S value)
{

	if(g_ParaPlatForm.mLinerPt[4] >= value)
	{
		gCalErrFlag = 1;
		g_adj_flag_cnt = 1;
		return;
	}
	
	g_ParaPlatForm.mLinerPt[5] = value;

	g_adj_flag_cnt = 0;
	gCalEndFlag = 1;
//	gCalDelay = 1;
	Wet_InitZero();
	Beep(3,3);
	Para_Save_block(EE_ADDR_PLATFORM,(INT8U*)&g_ParaPlatForm,sizeof(PARA_PLATFORM));
}	

/*
检测重量或计数在设定范围内。
*/
void WetApp_Tolerance_Check_Pcs(void)
{
	INT32U mTempCheck = 0;
	if(gScaleAppData.mScaleNetWet < g_ParaUser.RSN  ) // <20d表示卸载货物
	{
		return;
	}

	LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S5_HI);
	LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S3_LO);
	LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S4_OK);

	if( (g_ParaPlatForm.mPcsUp == 0) || ( g_ParaPlatForm.mPcsDown == 0) )
	{
		//return ;
	}

	//计数
	if(g_ParaPlatForm.mAdjPcsWet != 0)
		mTempCheck =  ( gScaleAppData.mScaleNetWet * g_ParaPlatForm.mAdjPcsCnt )/ g_ParaPlatForm.mAdjPcsWet;
	else
		mTempCheck = 0;
	gScaleAppData.mScalePCS = mTempCheck;
	if(mTempCheck >= g_ParaPlatForm.mPcsUp )
	{
		LCD_DispSymbl(UM_OPMODE_EN, SYMI_S5_HI);
		gScaleAppData.mScaleCheckStatue = LIMITS_UP;
		if(g_ParaPlatForm.mBeep == 2) 
			Beep(1, 1);
	}
	else if(mTempCheck <= g_ParaPlatForm.mPcsDown)
	{
		LCD_DispSymbl(UM_OPMODE_EN, SYMI_S3_LO);
		gScaleAppData.mScaleCheckStatue = LIMITS_LOW;
		if(g_ParaPlatForm.mBeep == 2) 
			Beep(1, 1);

	}
	else
	{
		LCD_DispSymbl(UM_OPMODE_EN, SYMI_S4_OK);
		gScaleAppData.mScaleCheckStatue = LIMITS_OK;
		if(g_ParaPlatForm.mBeep == 1) 
			Beep(1, 1);
	}	
}

/*
检测重量或计数在设定范围内。
*/
void WetApp_Tolerance_Check_Wet(void)
{
	INT32U mTempCheck = 0;
	if(gScaleAppData.mScaleNetWet < g_ParaUser.RSN  ) // <20d表示卸载货物
	{
		return;
	}

	LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S5_HI);
	LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S3_LO);
	LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S4_OK);
	//检重
	if( (g_ParaPlatForm.mWetUp == 0) || ( g_ParaPlatForm.mWetDown == 0) )
	{
		return ;
	}
	mTempCheck =  gScaleAppData.mScaleNetWet;
	
	if(mTempCheck >= g_ParaPlatForm.mWetUp )
	{
		gScaleAppData.mScaleCheckStatue = LIMITS_UP;
		LCD_DispSymbl(UM_OPMODE_EN, SYMI_S5_HI);
		if(g_ParaPlatForm.mBeep == 2) 
			Beep(1, 1);
	}
	else if(mTempCheck <= g_ParaPlatForm.mWetDown)
	{
		LCD_DispSymbl(UM_OPMODE_EN, SYMI_S3_LO);
		gScaleAppData.mScaleCheckStatue = LIMITS_LOW;
		if(g_ParaPlatForm.mBeep == 2) 
			Beep(1, 1);
	}
	else
	{
		LCD_DispSymbl(UM_OPMODE_EN, SYMI_S4_OK);
		gScaleAppData.mScaleCheckStatue = LIMITS_OK;
		if(g_ParaPlatForm.mBeep == 1) 
			Beep(1, 1);
	}
}

//////////////////// add up /////////////////////////////////

INT8U WetApp_Get_AddUp_New(void)
{
#if USE_ADDUP_FUNC
	static INT8U bf_numm = 0;
	INT8U ret = 0;
	if(bf_numm != gScaleAppData.mScaleTotalNum )
	{
		ret = 1;		

	}
	bf_numm = gScaleAppData.mScaleTotalNum;
	return ret;
#else

	return 0;
#endif

}
static INT8U gAddPtNum = 0;
static float gAddTotal = 0.0;
static INT32U gAddTotalShow  = 0;

void WetApp_Set_ShowAddPt(INT8U pt)
{
	gAddPtNum = pt;
}
INT8U WetApp_Get_ShowAddPt(void )
{
	return gAddPtNum;
}
INT32U WetApp_Get_ShowAddWet(void )
{
	return gAddTotalShow;
}

INT8U WetApp_AddUp_DealTotalWet(INT32U wet)
{
#if USE_ADDUP_FUNC
	float tempadd = (float)wet; 
	char i = 0;
	float mCmpWet = 999999.0;
	char mShowPt = WetApp_Get_ShowPt();

	for(i = 0;i < mShowPt;i++)
	{
		tempadd = tempadd / 10.0;
	}
	for(i = 0;i < gAddPtNum;i++)
	{
		mCmpWet = mCmpWet / 10.0;
	}	
	tempadd = gAddTotal + tempadd;
	if(tempadd > mCmpWet) //小数点移位
	{
		if(gAddPtNum != 0)
			gAddPtNum -= 1;
	}
	if(tempadd >= 999999) //超出最大累计值
		return 0;
	
	gAddTotal = tempadd;
	
	for(i = 0;i < gAddPtNum;i++)
	{
		tempadd = tempadd * 10.0;
	}		
	gAddTotalShow = (INT32U)tempadd;
#endif	
	return 1;
}

void WetApp_AddUp_AddWet(void)
{
#if USE_ADDUP_FUNC
	if(gScaleAppData.mScaleNetWet < g_ParaUser.RSN * 20) // <20d表示卸载货物
	{
		return;
	}

	if(gScaleAppData.mScaleTotalNum >= 999)
	{
		LCD_Set_ErrShow(ERR_OUT_NUM,10);
		return;
	}	
	
	if(WetApp_AddUp_DealTotalWet(gScaleAppData.mScaleNetWet) == 0)
	{
		LCD_Set_ErrShow(ERR_OUT_TOTAL,10);
		return;
	}

	gScaleAppData.mScaleTotalWet += gScaleAppData.mScaleNetWet;
	gScaleAppData.mScaleTotalNum++;	
	gScaleAppData.mScaleSingleWet = gScaleAppData.mScaleNetWet;


	g_ParaAddUp.mAddWet = gAddTotal;
	g_ParaAddUp.mAddCnt = gScaleAppData.mScaleTotalNum;
	g_ParaAddUp.mAddPtNum = gAddPtNum;
	Para_Save_block(EE_ADDR_ADDUP,(INT8U*)&g_ParaAddUp,sizeof(PARA_ADDUP));
	
	//增加打印功能
	//Rs_Printf_Log();
	if(gScaleAppData.mScaleTotalWet != 0)
	{
		LCD_DispSymbl(UM_OPMODE_EN, SYMI_S_ACC);
	}
#endif
}
static INT8U mAddUpFlag = 1;

void WetApp_Handle_AddUp_AddWet(void)
{
#if USE_ADDUP_FUNC
	if(mAddUpFlag == 0)
	{
		WetApp_AddUp_AddWet();
		mAddUpFlag = 1;
	}
#endif	
}
INT8U WetApp_Auto_AddUp_AddWet(void)
{
#if USE_ADDUP_FUNC
	static INT8U mAddFlag = 1;
	static INT32S mTempWet = 0;
	static INT8U  mUpcnt = 0;
	static INT8U  mUpFlag = 0;
	
	
	if(gScaleAppData.mScaleNetWet < g_ParaUser.RSN * 20) // <20d表示卸载货物
	{
		mAddFlag = 0;
		return 0;
	}
	#if 1
	if( (gWetStatue.stable) && (gScaleAppData.mScaleNetWet > g_ParaUser.RSN * 20)  )
	{
		if(abs(mTempWet - gScaleAppData.mScaleNetWet ) < g_ParaUser.RSN)
		{
			mUpcnt++;
			if(mUpcnt >= 2 )
			{
				mUpcnt = 5;
				mUpFlag = 1;
			}
		}
		else
		{
			mUpFlag = 0;
			mUpcnt = 0;
		}
	}
	else
	{
		mUpFlag = 0;
		mUpcnt = 0;
	}
	mTempWet = gScaleAppData.mScaleNetWet ;
	#else
	if(gScaleAppData.mScaleNetWet >= mTempWet )
	{
		mUpcnt++;
		if(mUpcnt >= 2 )
		{
			mUpcnt = 5;
			mUpFlag = 1;
		}
	}
	else if(gScaleAppData.mScaleNetWet < mTempWet )
	{
		mUpFlag = 0;
		mUpcnt = 0;
	}
	
	#endif
	if( (mUpFlag) && (mAddFlag == 0) && (gWetStatue.stable) )
	{
		mAddFlag = 1;
		WetApp_AddUp_AddWet();
		Beep(3,1);
		return 1;
	}
#endif	
	return 0;
}
void WetApp_AddUp_Reset(void)
{
#if USE_ADDUP_FUNC
	gAddTotal = 0.0;
	gAddTotalShow  = 0;

	gScaleAppData.mScaleTotalWet = 0;
	gScaleAppData.mScaleTotalNum = 0;	


	g_ParaAddUp.mAddWet = 0.0 ;
	g_ParaAddUp.mAddCnt = 0 ;
	gAddPtNum = Unit_Get_ConvertPt();
	g_ParaAddUp.mAddPtNum = gAddPtNum ;
	
	Para_Save_block(EE_ADDR_ADDUP,(INT8U*)&g_ParaAddUp,sizeof(PARA_ADDUP));

	LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S_ACC);
#endif		
}
/******************************************************************************
  * @brief  动物称量模式管理，启动后，蜂鸣，并显示HOLD
  * @param  None
  * @retval None 
  * @ 
******************************************************************************/

void WetApp_Animal_Manage(void)
{
	g_ParaPlatForm.mAnm = ~g_ParaPlatForm.mAnm;
	if(g_ParaPlatForm.mAnm )
	{
		LCD_DispSymbl(UM_OPMODE_EN, SYMI_S9_HOLD);
	}
	else
	{
		LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S9_HOLD);
	}
	Beep(2, 1);

}
INT32S WetApp_Animal_Mode(INT32S inWet)
{
	static INT32S bWet = 0;
	static INT8U  bCnt = 0;
	static INT32S mTotalWet = 0;
	
	if(g_ParaPlatForm.mAnm )
	{
		bCnt++;
		mTotalWet += inWet;
		if(bCnt > 10)
		{
			bWet = mTotalWet/ bCnt;
			bCnt = 1;
			mTotalWet = bWet;
		}
		return bWet;
	}
	return inWet;
}

void WetApp_Start_KeyLock(void)
{
	gScaleAppData.mKeyLockCnt = 0;
	gScaleAppData.mKeyLockEN = 1;
}
void WetApp_SetKeyLock(INT8U lock)
{
	//g_ParaPlatForm.mKeyLock = lock;
	gScaleAppData.mKeyLockCnt = 0;
	gScaleAppData.mKeyLockEN = lock;	
}
INT8U WetApp_GetKeyLock(void )
{
	return gScaleAppData.mKeyLockEN;
}

void WetApp_CheckNet(void )
{
	if( g_ParaUser.TAV > g_ParaUser.RSN)
	{
		LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S_NET_GROSS);
		LCD_DispSymbl(UM_OPMODE_EN, SYMI_S_TARE);
		//LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S7_GROSS);
		gScaleAppData.mNet = 1;
	}
	else
	{
		gScaleAppData.mNet = 0;
		LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S_TARE);
		LCD_DispSymbl(UM_OPMODE_EN, SYMI_S_NET_GROSS);
		//LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S6_NET);
	}
	
}


void WetApp_BkLight_Manage(void)
{
	static	INT32U	mBlCnt = 0;
	static 	INT32S	mBkWet =0;
#if 1
	if((gScaleAppData.mScaleNetWet >=  g_ParaUser.RSN ) || (g_ParaPlatForm.mSleepTmr == 0))
	{
		mBlCnt = 0;
		gScaleAppData.mScaleSleepFlag = 0;
	}
	else
	{
		mBlCnt++;
		if(mBlCnt >= g_ParaPlatForm.mSleepTmr * 10 )//2s 后背光关闭
		{
			mBlCnt = 0;
			//gScaleAppData.mScaleSleepFlag = 1;

			wet_set_work_mode(0);
		}
	}
#else
	//屏幕背光管理
	switch(g_ParaPlatForm.mBLEN)
	{
		case 0: //bl on
			Pin_IO_Set(OUT_BL, PinLogic_High);
			break;
		case 1: //bl off
			Pin_IO_Set(OUT_BL, PinLogic_Low);
			break;
		case 2:// bl auto
			//if((gScaleAppData.mScaleNetWet <= mBkWet +  g_ParaUser.RSN * 2) || (!bsp_GetAllKeyState()) )
			if((gScaleAppData.mScaleNetWet >=  g_ParaUser.RSN ) || (bsp_GetAllKeyState()) || (g_ParaPlatForm.mSleepTmr == 0))
			{
				mBlCnt = 0;
				//Pin_IO_Set(OUT_BL, PinLogic_High);
				//TM1628_Set_PowerOff(0);
				gScaleAppData.mScaleSleepFlag = 0;
			}
			else
			{
				mBlCnt++;
				if(mBlCnt >= g_ParaPlatForm.mSleepTmr * 600 )//2s 后背光关闭
				{
					mBlCnt = 0;
					//Pin_IO_Set(OUT_BL, PinLogic_Low);
					//TM1628_Set_PowerOff(1);
					gScaleAppData.mScaleSleepFlag = 1;
				}
			}
			//mBkWet = gScaleAppData.mScaleNetWet;
			break;
	}
#endif
}
void WetApp_Poweroff_Manage(void)
{
	static	INT16U	mBlOffCnt = 0;
	static 	INT16U	mBlOffTmr = 0;
	INT8U	PowrOffArr[] = {0,3,5,15,30};

	/// 自动关机管理
	if(g_ParaPlatForm.mBLAuto >4)
		g_ParaPlatForm.mBLAuto = 4;
	mBlOffTmr = PowrOffArr[g_ParaPlatForm.mBLAuto] * 10 * 60 ;
	if(mBlOffTmr != 0)
	{
		if((gScaleAppData.mScaleNetWet <  g_ParaUser.RSN * 2) || (!bsp_GetAllKeyState()) )
		{
			mBlOffCnt++;
			if(mBlOffCnt >= mBlOffTmr)
			{
				mBlOffCnt = 0;
				Power_OFF();
			}
		}
	}

}
#if 1

static int g_work_mode = 1;
void wet_set_work_mode(INT8U mod)
{
	g_work_mode = mod;
	cs1237_set_power(g_work_mode);
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
	static	TYPE_PinLogic	s_LED_State = PinLogic_Low;
#if 0
	LED_Set(g_led_type,0);
	g_com_flash_flag = 1;
	g_com_flash_cnt = 0;
	LED_Set(g_led_type,1);
#else
	if(s_LED_State == PinLogic_Low)
	{
		LED_Set(g_led_type,0);
		s_LED_State = PinLogic_High;
	}
	else
	{
		LED_Set(g_led_type,1);
		s_LED_State = PinLogic_Low;
	}
#endif	

}
void WetApp_BatteryChange_Manage(void)
{
	static INT8U cnt = 0;
	TYPE_BatStatus gVmStatue;
	static TYPE_BatStatus gVmStatueBF ;	
	static INT8U msg_flag = 0;
	
	static INT8U g_flash_on = 0;
	
	static INT8U g_cnt_s = 0;
	
	gVmStatue =  VM_GetBatStat();

	if(gVmStatueBF != gVmStatue )
		msg_flag = 0;
	
	if(gVmStatue == BatCharging) //充电中，闪烁
	{
		msg_flag = 1;
		g_led_type = 0;
	}
	else if(gVmStatue == BatBeenFull)
	{
		g_led_type = 0;
	}

	if(gVmStatue == BatNormal) // 
	{
		g_led_type = 0;
	}
	else if(gVmStatue == BatLow)
	{
		g_led_type = 2;
	}
	else if(gVmStatue == BatEmpty)
	{
		g_led_type = 1;
	}
	else if(gVmStatue == BatShutDown)
	{
		g_led_type = 1;
	}
	if(g_com_flash_flag ) 
	{
		g_com_flash_cnt++;
		if(g_com_flash_cnt > 40 ) /////4s 没有通讯
		{
			g_com_flash_flag = 0;
			g_com_flash_cnt = 0;
		}
		LED_Flash();
		return;
	}	
	
	cnt++;
	if(cnt < 5) // //1s 检测一次
		return;
	cnt =0;
	
	if(g_work_mode == 1)  //工作模式
	{
		if(g_flash_on == 0) g_flash_on = 1;
		else g_flash_on = 0;
	}
	else if(g_work_mode == 0)   //睡眠模式
	{
		g_cnt_s++;
		if(g_cnt_s >= 6)
		{
			g_cnt_s = 0;
			if(g_flash_on == 0) g_flash_on = 1;
			else g_flash_on = 0;
		}	
	}

	if(g_com_flash_flag == 0) //没有通讯灯需要闪烁
		LED_Set(g_led_type,g_flash_on);
	
	gVmStatueBF = gVmStatue;
}

#else
void WetApp_BatteryChange_Manage(void)
{
	static INT8U cnt = 0;
	TYPE_BatStatus gVmStatue;
	static TYPE_BatStatus gVmStatueBF ;
	static INT8U err_flag = 0;
	static INT8U msg_flag = 0;
	
	cnt++;
	if(cnt < 10)  //1s 检测一次
		return;
	cnt =0;

	LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S21_BAT);

	gVmStatue =  VM_GetBatStat();

	if(gVmStatueBF != gVmStatue )
		msg_flag = 0;
	
	if(gVmStatue == BatCharging) //充电中，闪烁
	{
		//if(msg_flag == 0)
		//	LCD_Set_ErrShow(MSG_ACC_IN, 20);
		msg_flag = 1;
		LCD_DispSymbl(UM_OPMODE_REVE, SYMI_S2_Flash);
	}
	else if(gVmStatue == BatBeenFull)
	{
		LCD_DispSymbl(UM_OPMODE_EN, SYMI_S2_Flash);
	}
	else
	{
		LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S2_Flash);
	}

	if(gVmStatue == BatNormal) // 
	{
		if(err_flag )
			LCD_Clear_Err();
		LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S21_BAT);
	}
	else if(gVmStatue == BatLow)
	{
		err_flag = 1;
		LCD_Set_ErrShow(ERR_BAT_LOW, 252);
		//Beep(2, 1);
		LCD_DispSymbl(UM_OPMODE_EN, SYMI_S21_BAT);
	}
	else if(gVmStatue == BatEmpty)
	{
		err_flag = 1;
		LCD_Set_ErrShow(ERR_BAT_LOW, 252);
		//Beep(2, 1);
		LCD_DispSymbl(UM_OPMODE_REVE, SYMI_S21_BAT);
	}
	else if(gVmStatue == BatShutDown)
	{
		err_flag = 1;
		LCD_Set_ErrShow(ERR_BAT_LOW, 252);
		//Beep(2, 1);
	}
	gVmStatueBF = gVmStatue;
}
#endif

void WetApp_Auto_Tare_Manage(void )
{
	static INT8U mSetTareFlag = 0;
	static INT8U mStableCnt  = 0 ;
	static INT32S mTareSum = 0;
	if( g_ParaPlatForm.mAutoSetTare )
	{
		if(!mSetTareFlag)  //置皮
		{
			if(gWetData.mWet_gross > 0 ) //( g_ParaUser.RSN  * 10 )
			{
				if(gWetStatue.stable )
				{
					mStableCnt++;
					mTareSum += gWetData.mWet_gross;
					if(mStableCnt >= 8)
					{
						mStableCnt = 0;
						g_ParaUser.TAV = mTareSum / 8;
						gScaleAppData.mNet = 1;	
						mSetTareFlag = 1;
						WetApp_CheckNet();
					}
				}
				else
				{
					mStableCnt = 0;
					mTareSum = 0;
				}
			}
		}
		else
		{
			if(gWetData.mWet_gross  <= 0) //皮重去下 < ( g_ParaUser.RSN  * 2 )
			{
				if(gWetStatue.stable )
				{
					mStableCnt++;
					mTareSum += gWetData.mWet_gross;
					if(mStableCnt >= 8)
					{
						mStableCnt = 0;
						g_ParaUser.TAV = 0;//mTareSum / 30;
						gScaleAppData.mNet = 0; 
						mSetTareFlag = 0;
						mTareSum = 0;
						WetApp_CheckNet();
					}
				}
				else
				{
					mStableCnt = 0;
					mTareSum = 0;
				}

			}
		}
			
	}
}
/*
处理多量程多分度值的管理

问题:何时启用分段功能
*/
#if 0
void WetApp_MultiRang_Manage(void)
{
	//g_ParaPlatForm.mDualNum ;
	static INT8U mUpFlag = 0;
	static INT32S mBfRsn = 0;
	
	if(g_ParaPlatForm.mDualNum == 0)
		g_ParaUser.RSN = g_ParaPlatForm.mInc[0];
	if(g_ParaPlatForm.mDualNum == 1)
	{

		if(gWetData.mWet_gross < 0)
		{
			g_ParaUser.RSN = g_ParaPlatForm.mInc[0];
			LCD_DispSymbl(UM_OPMODE_EN, SYMI_S12_1_SEC);
			LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S13_2_SEC);
		}	
		else if(gWetData.mWet_gross > g_ParaPlatForm.mCap[0]) //大于分界点
		{
			g_ParaUser.RSN = g_ParaPlatForm.mInc[1];
			LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S12_1_SEC);
			LCD_DispSymbl(UM_OPMODE_EN, SYMI_S13_2_SEC);
		}
		else
		{
			g_ParaUser.RSN = g_ParaPlatForm.mInc[0];
			LCD_DispSymbl(UM_OPMODE_EN, SYMI_S12_1_SEC);
			LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S13_2_SEC);
		}
		
	}
	if(g_ParaPlatForm.mDualNum == 2)
	{
		if(gWetData.mWet_gross < 0)
		{
			if(mUpFlag )
			{
				if(gWetData.mWet_gross  <= g_ParaUser.RSN)
				{
					g_ParaUser.RSN = g_ParaPlatForm.mInc[0];
					mUpFlag = 0;
				}
			}
			else
				g_ParaUser.RSN = g_ParaPlatForm.mInc[0];

			LCD_DispSymbl(UM_OPMODE_EN, SYMI_S12_1_SEC);
			LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S13_2_SEC);
		}		
		else if(gWetData.mWet_gross > g_ParaPlatForm.mCap[0]) //大于分界点
		{
			g_ParaUser.RSN = g_ParaPlatForm.mInc[1];
			LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S12_1_SEC);
			LCD_DispSymbl(UM_OPMODE_EN, SYMI_S13_2_SEC);
			mUpFlag = 1;
		}
		else
		{
			LCD_DispSymbl(UM_OPMODE_EN, SYMI_S12_1_SEC);
			LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S13_2_SEC);
			if(mUpFlag )
			{
				if(gWetData.mWet_gross  <= g_ParaUser.RSN)
				{
					g_ParaUser.RSN = g_ParaPlatForm.mInc[0];
					mUpFlag = 0;
				}
			}
			else
				g_ParaUser.RSN = g_ParaPlatForm.mInc[0];
		}
	}	

	if(mBfRsn != g_ParaUser.RSN)
	{
		Unit_SetBaseIncr(g_ParaUser.RSN);
		Unit_Convert_Incr(0);
		mBfRsn = g_ParaUser.RSN;
	}
}
#else
void WetApp_MultiRang_Manage(void)
{
	//g_ParaPlatForm.mDualNum ;
	static INT8U mUpFlag = 0;
	static INT32S mBfRsn = 0;
	
	//if(g_ParaPlatForm.mDualNum == 1)
	if( g_ParaPlatForm.mRngType == 2)
	{
		if(gWetData.mWet_gross < g_ParaPlatForm.mCap[0])
		{
			g_ParaUser.RSN = g_ParaPlatForm.mInc[0];
			LCD_DispSymbl(UM_OPMODE_EN, SYMI_S12_1_SEC);
			LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S13_2_SEC);
		}	
		else if(gWetData.mWet_gross < g_ParaPlatForm.mCap[1]) //大于分界点
		{
			g_ParaUser.RSN = g_ParaPlatForm.mInc[1];
			LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S12_1_SEC);
			LCD_DispSymbl(UM_OPMODE_EN, SYMI_S13_2_SEC);
		}
		else  
		{
			g_ParaUser.RSN = g_ParaPlatForm.mInc[2];
			LCD_DispSymbl(UM_OPMODE_EN, SYMI_S12_1_SEC);
			LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S13_2_SEC);
		}
	}
	else if( g_ParaPlatForm.mRngType == 1)
	{
		if(gWetData.mWet_gross < g_ParaPlatForm.mCap[1]) //大于分界点
		{
			g_ParaUser.RSN = g_ParaPlatForm.mInc[1];
			LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S12_1_SEC);
			LCD_DispSymbl(UM_OPMODE_EN, SYMI_S13_2_SEC);

			gScaleAppData.mScaleStatue.stWetRang = 0;
		}
		else  
		{
			g_ParaUser.RSN = g_ParaPlatForm.mInc[2];
			LCD_DispSymbl(UM_OPMODE_EN, SYMI_S12_1_SEC);
			LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S13_2_SEC);

			gScaleAppData.mScaleStatue.stWetRang = 1;
		}
	}
	else
	{
		g_ParaUser.RSN = g_ParaPlatForm.mInc[2];
		LCD_DispSymbl(UM_OPMODE_EN, SYMI_S12_1_SEC);
		LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S13_2_SEC);		
		gScaleAppData.mScaleStatue.stWetRang = 1;
	}
	
	if(mBfRsn != g_ParaUser.RSN)
	{
#if USE_UNIT_CONV_FUNC	
		Unit_SetBaseIncr(g_ParaUser.RSN);
		Unit_Convert_Incr(0);
#endif		
		mBfRsn = g_ParaUser.RSN;
	}
}
#endif

void WetApp_Printf_OutPut(void)
{
	INT32S prt_data = gScaleAppData.mScaleNetWet;

	Rs_Data_Printf_Mode(prt_data,1,1);
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
#if USE_UNIT_CONV_FUNC	
	curRSN = Unit_Get_ConvertIncr();
#else
	curRSN = g_ParaUser.RSN;
#endif
	tempF = (FP32)wet / curRSN;	
	if (tempF < 0)	tempF -= 0.5;
	else			tempF += 0.5;		

	return ((INT32S)tempF * curRSN);
}

INT32S WetApp_Convert_Data(float inwet)
{
	INT32S CvtWet  = 0;
#if USE_UNIT_CONV_FUNC
	static float wet = 0;
	INT8U  mdiv = 0;
	INT8U  i = 0;
	
	wet = Unit_Convert_UnitData(inwet,2);
	CvtWet  = (INT32S)wet ;	
	CvtWet = WetApp_IncFormat(CvtWet);

	#if 1
	
	#else
	if(Unit_GetUnitID() != UNIT_KG )
	{
		mdiv = g_ParaUser.DPT;
		for(i = 0;i < mdiv ;i++)
		{
			CvtWet /= 10;
		}	
	}
	#endif
	//if(Unit_GetUnitID() != UNIT_G )
	{
		mdiv   = Unit_Get_DivCnt();
		for(i = 0;i < mdiv ;i++)
		{
			CvtWet /= 10;
		}
	}
#else
	CvtWet = WetApp_IncFormat(inwet);
#endif	
	return CvtWet;
}

#if (USE_UNIT_CONV_FUNC == 0)

INT8U Unit_Get_ConvertPt(void)
{
	return WetApp_Get_ShowPt();
}


#endif
INT8U WetApp_Get_ShowPt(void )
{
#if USE_UNIT_CONV_FUNC
	return Unit_Get_ConvertPt()+ gScaleAppData.mScaleStatue.stWetX10;
#else
	return g_ParaUser.DPT + gScaleAppData.mScaleStatue.stWetX10;
#endif
}
INT8U WetApp_Get_RealPt(void)
{
	return g_ParaUser.DPT + gScaleAppData.mScaleStatue.stWetX10;
}
void WetApp_PeekCatch(void)
{
	static INT8U mPeekCnt = 0;
	mPeekCnt++;
	if(gWetStatue.stable == 0)
	{
		mPeekCnt = 0;
	}	
	if(g_ParaPlatForm.mPHoldEn == 1)
	{
		
		if(gWetStatue.stable )
		{
			if( (gScaleAppData.mScalePeekWet == 0) && (mPeekCnt > 5) )
			{
				mPeekCnt = 0;
				if(gScaleAppData.mScaleNetWet >  gScaleAppData.mScalePeekWet )
					gScaleAppData.mScalePeekWet = gScaleAppData.mScaleNetWet;
			}	
		}
		else
		{
			mPeekCnt = 0;
		}
	}
	else if(g_ParaPlatForm.mPHoldEn == 2)
	{
		if(gScaleAppData.mScaleNetWet >  gScaleAppData.mScalePeekWet )
			gScaleAppData.mScalePeekWet = gScaleAppData.mScaleNetWet;	
	} 
	else if(g_ParaPlatForm.mPHoldEn == 3) // 在加载过程中开不到中间变化值
	{
		if( (mPeekCnt > 3) && (gWetStatue.stable ) )
		{
			mPeekCnt = 0;
			gScaleAppData.mScalePeekWet = gScaleAppData.mScaleNetWet;
			gScaleAppData.mPeekCath = 1;
		}
	}
	
}

/******************************************************************************
  * @brief	发送重量值，用于调试  
  * @param  None
  * @retval None
******************************************************************************/
void WetApp_SendWet(INT32S wet)
{
	INT8U s[8] = {0xFE,0x7F};

	s[3] = wet>>24;
	s[4] = wet>>16;
	s[5] = wet>>8;
	s[6] = wet;

	RS422_SendData(s,8);
}


/******************************************************************************
  * @brief  定值称重服务，
  * @param  None
  * @retval None 
  * @ 需要实现电池检测功能，动态滤波功能，背光管理功能。
******************************************************************************/
void	WetApp_Working(void)
{
	static INT8U delay_deal = 0;
	static INT8U x10Cnt = 0;

	if(gScaleAppData.mScaleStatue.stWetX10)
	{	
		x10Cnt++;
		if(x10Cnt > 100)
		{
			x10Cnt = 0;
			gScaleAppData.mScaleStatue.stWetX10 = 0;
		}
	}
	//电池管理
	WetApp_BatteryChange_Manage();
	WetApp_BkLight_Manage();
	WetApp_MultiRang_Manage();
#if 0	
	if( gWetStatue.stable )
		LCD_DispSymbl(UM_OPMODE_EN, SYMI_S_STABLE);
	else
		LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S_STABLE);
	
	//WetApp_PeekCatch();

	/// 自动关机管理
	//WetApp_Poweroff_Manage();
	//电池管理
	WetApp_BatteryChange_Manage();

       //WetApp_PrintOut_Manage();
	
	if(gMenuOprt == 0)
	{
		//屏幕背光管理
		WetApp_BkLight_Manage();	
		//自动值皮功能
		WetApp_Auto_Tare_Manage();

		//多量程多分度处理
		WetApp_MultiRang_Manage();
	}
#endif	
}


void WetApp_Cal_Zero(INT32S avg)
{
	Std_ReturnType	rtn;
#if 0
	if((avg > -100000)&&(avg < 100000))
	{
		g_ParaFact.SZA = avg;
		g_ParaUser.LDW = 0;
		g_ParaUser.LWT = 1000000;
		g_ParaUser.CWT = 1000000;
		Wet_InitPara();

		rtn = Para_Save_block(EE_ADDR_TACTORY,(INT8U*)&g_ParaFact,sizeof(PARA_TACTORY));

	}
#else
	//if((avg > -1000000)&&(avg < 1000000))
	{
		g_ParaUser.LDW = avg;
		Wet_InitPara();
		Wet_InitZero();
		rtn = Para_Save_block(EE_ADDR_USER,(INT8U*)&g_ParaUser,sizeof(PARA_USER));
	}
#endif
}

void	WetApp_Cal_Wet(INT32S avg)
{
	Std_ReturnType	rtn;
#if 0
	if(((avg > -1600000)&&(avg < -600000))||((avg > 600000)&&(avg < 1600000)))
	{
		g_ParaFact.SFA = avg;
		g_ParaFact.SFAdigit	= 1000000;
		g_ParaUser.LDW = 0;
		g_ParaUser.LWT = 1000000;
		g_ParaUser.CWT = 1000000;
		Wet_InitPara();

		rtn = Para_Save_block(EE_ADDR_TACTORY,(INT8U*)&g_ParaFact,sizeof(PARA_TACTORY));
	}
#else
	if(avg > g_ParaUser.LDW)
	{
		g_ParaUser.LWT = avg;
		Wet_InitPara();
		Wet_InitZero();
		rtn = Para_Save_block(EE_ADDR_USER,(INT8U*)&g_ParaUser,sizeof(PARA_USER));
	}
	else
	{
		//Form_CalNoLin_Err();
	}

#endif
}

void WetApp_Work_En(INT8U en)
{
	if(en )
	{
		Timer_Enable(mWetAppID);
	}
	else
	{
		Timer_Disable(mWetAppID);
	}
}
void WetApp_Tare2Gross(void )
{
	gScaleAppData.mNet = !gScaleAppData.mNet; 
	//WetApp_CheckNet();
	if( gScaleAppData.mNet )
	{
		LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S_NET_GROSS);
		//LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S7_GROSS);
	}
	else
	{
		LCD_DispSymbl(UM_OPMODE_EN, SYMI_S_NET_GROSS);
		//LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S6_NET);
	}	
	 gScaleAppData.mScaleStatue.stNet = gScaleAppData.mNet;

}

INT8U WetApp_CheckWetRange(void)
{
	INT8U tpOverGross = 0;
	INT8U tpUnderGross =0;
	
	if((gWetStatue.overGross) || (gWetData.mWet_gross > (g_ParaUser.NOV + 9*g_ParaUser.RSN))	)				//毛重超载
			tpOverGross = 1;
	else 	tpOverGross = 0;
	if((gWetStatue.underGross) || (gWetData.mWet_gross < (0 - 20*g_ParaUser.RSN))	)					//毛重欠载
			tpUnderGross = 1;
	else	tpUnderGross = 0;

	if(tpOverGross )
	{
		LCD_DispStr(0,0,"  HIGH");
	}
	if(tpUnderGross)
	{
		LCD_DispStr(0,0,"   LOW");
	}
	if(tpOverGross ||  tpUnderGross)
	{
		return 1;
	}
	return 0;
}

void WetApp_SetTare(void )
{
	//if(gWetData.mWet_gross < g_ParaPlatForm.mTareRang)
	{
		Wet_Taring();
		WetApp_CheckNet();
	}
	gScaleAppData.mScaleStatue.stNet = gScaleAppData.mNet;
	
	return ;
}

void WetApp_SetZero(void )
{
	if(!gScaleAppData.mNet )
	{
		g_ERR_ZERO_Value = Wet_Zeroing();
		// gScaleAppData.mScaleStatue.stClearZeroFail ;

		if(LCD_Get_Err_Flag() )
		{
			g_ERR_ZERO_Value = 1;
		}		
	}
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
	gAddTotal = 0.0;
	gAddTotalShow  = 0;

	gScaleAppData.mWaveOut = 0;
	gScaleAppData.mScaleTotalWet = 0;
	gScaleAppData.mScaleTotalNum = 0;	

	//LCD_DispSymbl(UM_OPMODE_EN, SYMI_S17_SCALE_LABLE);
	LCD_DispSymbl(UM_OPMODE_EN, SYMI_S18_1_SCALE);
	LCD_DispSymbl(UM_OPMODE_EN, SYMI_S12_1_SEC);

	gScaleAppData.mScaleSleepFlag = 0;
	g_ParaPlatForm.mKeyLock = 0;
	g_ParaUser.TAV = 0;
	WetApp_CheckNet();

#if USE_ADDUP_FUNC
	if(g_ParaAddUp.mAddCnt == 0)
		WetApp_Set_ShowAddPt(Unit_Get_ConvertPt());
	else
		gAddPtNum = g_ParaAddUp.mAddPtNum;
	gAddTotal = g_ParaAddUp.mAddWet ;
	gScaleAppData.mScaleTotalNum = g_ParaAddUp.mAddCnt ;
		
	if(gScaleAppData.mScaleTotalNum != 0)
		LCD_DispSymbl(UM_OPMODE_EN, SYMI_S_ACC);
	else
		LCD_DispSymbl(UM_OPMODE_DIS, SYMI_S_ACC);

#endif
	tempadd = gAddTotal;
	for(i = 0;i < gAddPtNum;i++)
	{
		tempadd = tempadd * 10.0;
	}		
	gAddTotalShow = (INT32U)tempadd;
	
	mWetAppID = Timer_Regist(LOOP,100,WetApp_Working);//定时称重处理  0.1s
	WetApp_Work_En(1);
	
	return TRUE;
}

extern void AD_Working(void);
extern void	Wet_Working(void);

void Wet_Service(void)
{
	AD_Working();
#if 1	
	Wet_Working();

	Wet_Read(&gWetStatue);
	
	if(gWetStatue.dnew )
	{
		gWetStatue.dnew = 0;
		Wet_Read_Wet(&gWetData);
		
		if(gScaleAppData.mScaleStatue.stWetX10)
		{
			//gWetData.mWet_newest; //借用这个变量显示放大10倍的重量
			gScaleAppData.mScaleConvertWet	= WetApp_Convert_Data(gWetData.mWet_newest); //gWetData.mWet_newest
			gScaleAppData.mScaleNetWet 	= gScaleAppData.mScaleConvertWet - WetApp_Convert_Data(g_ParaUser.TAV * 10);//gWetData.mWet_net;	//净重
		}
		else
		{
			gScaleAppData.mScaleConvertWet =  gWetData.mWet_gross;//WetApp_Animal_Mode(gWetData.mWet_gross);
			//gScaleAppData.mScaleConvertWet	= WetApp_Convert_Data(gScaleAppData.mScaleConvertWet); //gWetData.mWet_newest
			gScaleAppData.mScaleNetWet 	= gWetData.mWet_net;//gScaleAppData.mScaleConvertWet - WetApp_Convert_Data(g_ParaUser.TAV);//gWetData.mWet_net;	//净重

		}	
		gScaleAppData.mScaleWet 	= gScaleAppData.mScaleConvertWet;   //毛重
		gScaleAppData.mScaleWetInter	= gWetData.mWet_Stable;

		//更新秤状态
		gScaleAppData.mScaleStatue.stStable = gWetStatue.stable;
		gScaleAppData.mScaleStatue.stZero = gWetStatue.zero;
		//gScaleAppData.mScaleStatue.stNet = gScaleAppData.mNet ;  //通过外部操作切换
		gScaleAppData.mScaleStatue.stUpOverLoad= gWetStatue.overGross;
		gScaleAppData.mScaleStatue.stDownOverLoad = gWetStatue.underGross;
		//gScaleAppData.mScaleStatue.stWetX10 = ;
		gScaleAppData.mScaleStatue.stMenuMode = 0;
		
		if(gScaleAppData.mScaleNetWet < g_ParaUser.RSN * 2) // <20d表示卸载货物
		{
			mAddUpFlag = 0;
		}

		if(gScaleAppData.mWaveOut == 1 )
			WetApp_SendWet(gScaleAppData.mScaleNetWet);
		else if(gScaleAppData.mWaveOut == 2)
			WetApp_Printf_OutPut();
		else if(gScaleAppData.mWaveOut == 3)
			AD_Send_Wave(0);
		else if(gScaleAppData.mWaveOut == 4)
			AD_Send_Wave(1);
		else if(gScaleAppData.mWaveOut == 5)
			AD_Send_Wave(2);		
	}
	#endif
}

