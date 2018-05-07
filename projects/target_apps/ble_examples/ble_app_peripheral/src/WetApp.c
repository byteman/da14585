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
 

void WetApp_Cal_Liner_P0(INT32S value)
{
	int temp = 0;

	temp = value - g_param->mLinerPt[0]  ;
	g_param->mLinerPt[0] = value;
	g_param->mLinerNov[0]  = 0;

	g_param->mLinerPt[1] += temp;
	g_param->mLinerPt[2] += temp;
	g_param->mLinerPt[3] += temp;
	g_param->mLinerPt[4] += temp;
	g_param->mLinerPt[5] += temp;
	
	//gCalDelay = 1;
	g_adj_flag_cnt = 0;
	gCalErrFlag = 0;
	Wet_InitZero();

}
void WetApp_Cal_Liner_P1(INT32S value)
{

	if(g_param->mLinerNov[1] * 2 > (value  - g_param->mLinerPt[0] ) ) //10�����һ
		return;
	
	g_param->mLinerPt[1] = value;
	g_param->mLinerPt[2] = value;
	g_param->mLinerPt[3] = value;
	g_param->mLinerPt[4] = value;
	g_param->mLinerPt[5] = value;

	g_param->mLinerNov[0]  = 0;

	g_param->mLinerNov[2] =  g_param->mLinerNov[1]; 
	g_param->mLinerNov[3] =  g_param->mLinerNov[1]; 
	g_param->mLinerNov[4] =  g_param->mLinerNov[1]; 
	g_param->mLinerNov[5] =  g_param->mLinerNov[1]; 

	if(g_param->mLinerPt[0] >= value)
	{
		gCalErrFlag = 1;
		g_adj_flag_cnt = 1;
	}
	g_adj_flag_cnt = 0;
	//gCalDelay = 1;
	Wet_InitZero();
	//Para_Save_block(EE_ADDR_PLATFORM,(INT8U*)&g_ParaPlatForm,sizeof(PARA_PLATFORM));
}

void WetApp_Cal_Liner_P2(INT32S value)
{
	if(g_param->mLinerPt[1] >= value)
	{
		gCalErrFlag = 1;
		g_adj_flag_cnt = 1;
		return ;
	}
	
	g_param->mLinerPt[2] = value;
	g_param->mLinerPt[3] = value;
	g_param->mLinerPt[4] = value;
	g_param->mLinerPt[5] = value;	

	g_param->mLinerNov[3] =  g_param->mLinerNov[2]; 
	g_param->mLinerNov[4] =  g_param->mLinerNov[2]; 
	g_param->mLinerNov[5] =  g_param->mLinerNov[2]; 
	
	g_adj_flag_cnt = 0;
	gCalEndFlag = 1;
	//gCalDelay = 1;
	Wet_InitZero();
	//Beep(3,3);
	//Para_Save_block(EE_ADDR_PLATFORM,(INT8U*)&g_ParaPlatForm,sizeof(PARA_PLATFORM));
}	
void WetApp_Cal_Liner_P3(INT32S value)
{

	if(g_param->mLinerPt[2] >= value)
	{
		gCalErrFlag = 1;
		g_adj_flag_cnt = 1;
		return;
	}
	g_param->mLinerPt[3] = value;
	g_param->mLinerPt[4] = value;
	g_param->mLinerPt[5] = value;	

	g_param->mLinerNov[4] =  g_param->mLinerNov[3]; 
	g_param->mLinerNov[5] =  g_param->mLinerNov[3]; 
	

	g_adj_flag_cnt = 0;
	gCalEndFlag = 1;
	//gCalDelay = 1;
	Wet_InitZero();
	//Beep(3,3);
	//Para_Save_block(EE_ADDR_PLATFORM,(INT8U*)&g_ParaPlatForm,sizeof(PARA_PLATFORM));
}	
void WetApp_Cal_Liner_P4(INT32S value)
{
	if(g_param->mLinerPt[3] >= value)
	{
		gCalErrFlag = 1;
		g_adj_flag_cnt = 1;
		return;
	}
	
	g_param->mLinerPt[4] = value;
	g_param->mLinerPt[5] = value;	
	g_param->mLinerNov[5] =  g_param->mLinerNov[4]; 

	g_adj_flag_cnt = 0;
	gCalEndFlag = 1;
	//gCalDelay = 1;
	Wet_InitZero();
	//Beep(3,3);
	//Para_Save_block(EE_ADDR_PLATFORM,(INT8U*)&g_ParaPlatForm,sizeof(PARA_PLATFORM));
}	
void WetApp_Cal_Liner_P5(INT32S value)
{

	if(g_param->mLinerPt[4] >= value)
	{
		gCalErrFlag = 1;
		g_adj_flag_cnt = 1;
		return;
	}
	
	g_param->mLinerPt[5] = value;

	g_adj_flag_cnt = 0;
	gCalEndFlag = 1;
//	gCalDelay = 1;
	Wet_InitZero();
	//Beep(3,3);
	//Para_Save_block(EE_ADDR_PLATFORM,(INT8U*)&g_ParaPlatForm,sizeof(PARA_PLATFORM));
}	


void WetApp_Start_KeyLock(void)
{
	gScaleAppData.mKeyLockCnt = 0;
	gScaleAppData.mKeyLockEN = 1;
}
void WetApp_SetKeyLock(INT8U lock)
{
	//g_param->mKeyLock = lock;
	gScaleAppData.mKeyLockCnt = 0;
	gScaleAppData.mKeyLockEN = lock;	
}
INT8U WetApp_GetKeyLock(void )
{
	return gScaleAppData.mKeyLockEN;
}


void WetApp_Poweroff_Manage(void)
{
	static	INT16U	mBlOffCnt = 0;
	static 	INT16U	mBlOffTmr = 0;
	INT8U	PowrOffArr[] = {0,3,5,15,30};

	/// �Զ��ػ�����
	if(g_param->mBLAuto >4)
		g_param->mBLAuto = 4;
	mBlOffTmr = PowrOffArr[g_param->mBLAuto] * 10 * 60 ;
	if(mBlOffTmr != 0)
	{
		if((gScaleAppData.mScaleNetWet <  g_param->RSN * 2)  )
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
		gScaleAppData.mScaleStatue.stWorkMode = 1; //˯��
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
  * @brief	Wet ���ֶ�ֵ����������������ֵ����  
  * @param  none
  * @retval none
******************************************************************************/
INT32S	WetApp_IncFormat(float wet)
{
	FP32 tempF;
	INT32S	curRSN;
	//curRSN = Unit_Convert_UnitIncr(1);
	curRSN = g_param->RSN;
	tempF = (FP32)wet / curRSN;	
	if (tempF < 0)	tempF -= 0.5;
	else			tempF += 0.5;		

	return ((INT32S)tempF * curRSN);
}

INT32S WetApp_Convert_Data(float inwet)
{
	INT32S CvtWet  = 0;
	CvtWet = WetApp_IncFormat(inwet);
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
		g_param->LDW = avg;
		Wet_InitPara();
		Wet_InitZero();
		//rtn = Para_Save_block(EE_ADDR_USER,(INT8U*)&g_ParaUser,sizeof(PARA_USER));
	}
}

void	WetApp_Cal_Wet(INT32S avg)
{
	Std_ReturnType	rtn;
	if(avg > g_param->LDW)
	{
		g_param->LWT = avg;
		Wet_InitPara();
		Wet_InitZero();
		//rtn = Para_Save_block(EE_ADDR_USER,(INT8U*)&g_ParaUser,sizeof(PARA_USER));
	}
	

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



void WetApp_SetZero(void )
{
	if(!gScaleAppData.mNet )
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
  * @brief	��ʼ��
  * @param	None
  * @retval TRUE: ��ʼ���ɹ�	FALSE: ��ʼ��ʧ�� 
******************************************************************************/
Std_ReturnType	WetApp_Init(void)
{
	int i = 0;
	float tempadd = 0.0;
	//gAddTotal = 0.0;
	//gAddTotalShow  = 0;

	gScaleAppData.mWaveOut = 0;
	gScaleAppData.mScaleTotalWet = 0;
	gScaleAppData.mScaleTotalNum = 0;	

	//LCD_DispSymbl(UM_OPMODE_EN, SYMI_S17_SCALE_LABLE);
	//LCD_DispSymbl(UM_OPMODE_EN, SYMI_S18_1_SCALE);
	//LCD_DispSymbl(UM_OPMODE_EN, SYMI_S12_1_SEC);

	gScaleAppData.mScaleSleepFlag = 0;
	g_param->mKeyLock = 0;
	//g_param->TAV = 0;
	//WetApp_CheckNet();

	
	//gAddTotalShow = (INT32U)tempadd;
	
	//mWetAppID = Timer_Regist(LOOP,100,WetApp_Working);//��ʱ���ش���  0.1s
	//WetApp_Work_En(1);
	
	return TRUE;
}

extern void AD_Working(void);
extern void	Wet_Working(void);

void Wet_Service(void)
{
	//AD_Working();
#if 1	
	//Wet_Working();

	Wet_Read(&gWetStatue);
	
	if(gWetStatue.dnew )
	{
		gWetStatue.dnew = 0;
		Wet_Read_Wet(&gWetData);
		
		if(gScaleAppData.mScaleStatue.stWetX10)
		{
			//gWetData.mWet_newest; //�������������ʾ�Ŵ�10��������
			gScaleAppData.mScaleConvertWet	= WetApp_Convert_Data(gWetData.mWet_newest); //gWetData.mWet_newest
			gScaleAppData.mScaleNetWet 	= gScaleAppData.mScaleConvertWet - WetApp_Convert_Data(g_param->TAV * 10);//gWetData.mWet_net;	//����
		}
		else
		{
			gScaleAppData.mScaleConvertWet =  gWetData.mWet_gross;//WetApp_Animal_Mode(gWetData.mWet_gross);
			//gScaleAppData.mScaleConvertWet	= WetApp_Convert_Data(gScaleAppData.mScaleConvertWet); //gWetData.mWet_newest
			gScaleAppData.mScaleNetWet 	= gWetData.mWet_net;//gScaleAppData.mScaleConvertWet - WetApp_Convert_Data(g_ParaUser.TAV);//gWetData.mWet_net;	//����

		}	
		gScaleAppData.mScaleWet 	= gScaleAppData.mScaleConvertWet;   //ë��
		gScaleAppData.mScaleWetInter	= gWetData.mWet_Stable;

		//���³�״̬
		gScaleAppData.mScaleStatue.stStable = gWetStatue.stable;
		gScaleAppData.mScaleStatue.stZero = gWetStatue.zero;
		//gScaleAppData.mScaleStatue.stNet = gScaleAppData.mNet ;  //ͨ���ⲿ�����л�
		gScaleAppData.mScaleStatue.stUpOverLoad= gWetStatue.overGross;
		gScaleAppData.mScaleStatue.stDownOverLoad = gWetStatue.underGross;
		//gScaleAppData.mScaleStatue.stWetX10 = ;
		gScaleAppData.mScaleStatue.stMenuMode = 0;
		
		if(gScaleAppData.mScaleNetWet < g_param->RSN * 2) // <20d��ʾж�ػ���
		{
			//mAddUpFlag = 0;
		}

	}
	#endif
}

