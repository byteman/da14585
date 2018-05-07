/*******************************************************************************
  * @file    Weighing.c 
  * @author  yangxd
  * @version V 0.01
  * @date    2013.06.14
  * @brief   称重处理模块
  ******************************************************************************
  * @attention
  *
  *
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "Weighing.h"
#include "Timer.h"
#include "param.h"
/* Private macro -------------------------------------------------------------*/

#define		NP_LIN    	4 		//3个加载点
/* Private variables ---------------------------------------------------------*/
//static	INT32S	g_table_RSN[7] = {1,2,5,10,20,50,100};
static	INT32S	g_table_ZSE[5] = {0,2,5,10,20};

static	INT32S	g_wet_input;
static	FP32	g_fwet_zero;
static	FP32	g_Stable_input;
static	FP32	g_Stable_output;
static	FP32 g_Stable_X10 ;
static	INT32S	g_wet_newest;
static	INT32S	g_wet_gross;
static	INT32S	g_wet_net;


static	WET_STATE	g_wet_state;
static 	uWET_ERR	g_wet_err;
///* 补偿计算参数 */

static		FP32	g_FactClb_k;
static		FP32	g_UserClb_k;
static 		FP32	g_LinerClb_k[4];
static 		FP32	g_LinerCoverWet = 0;

static		FP32	g_corr_link[NP_LIN];	//线性补偿系数 

static	INT32S	g_AvgNum = 0;
static	INT32S	g_AvgCount = 0;
static	INT32S	g_AvgSum = 0;
//static	WET_AVG_PROC g_AvgBackFn = NULL;
static		INT32U	g_UserClb_DivCod = 20;

static device_param* g_param;
/* Private functions ---------------------------------------------------------*/
//static	Std_ReturnType	Wet_InitPara(void);
void	Wet_Working(void);

static	INT32S	Wet_Linear(INT32S wt);
//static	INT32S	Wet_StableAD(INT32S wt);
static	void	Wet_StableWt(void);
static	void	Wet_ZeroJudge(FP32	wt);
static	void	Wet_ZeroPowerUp(void);
static	void	Wet_ZeroTracking(void);
static	INT32S	Wet_IncFormat(INT32S wet);
static 	INT32S	Wet_IncFloatFormat(float wet);
static	INT32S	Wet_ParaIsRight(INT32S para,INT32S min,INT32S max);
extern INT32S WetApp_Convert_Data(float inwet);

/******************************************************************************
  * @brief  称重初始化
  * @param  None
  * @retval TRUE: 初始化成功	FALSE: 初始化失败 
******************************************************************************/
Std_ReturnType	Wet_Init(void)
{
	Std_ReturnType rtn;
	TID		pt;
	
	g_wet_err.value = 0;

    if(PARA_ERR_NONE != param_get(&g_param))
    {
        return 0;
    }

    pt = Timer_Regist(LOOP, 2 ,Wet_Working);//定时称重处理  ///g_Para_ICR[g_param->user.ICR]
    if(pt == 0)
    {
        g_wet_err.state.errWorkTask = 1;
    }
    else
    {
        g_wet_err.state.errWorkTask = 0;
    }

		pt = Timer_Regist(LOOP,100,Wet_StableWt);		//定时判稳处理		  
		if(pt == NULL)g_wet_err.state.errStableTask = 1;
		else	g_wet_err.state.errStableTask = 0;

    //g_param->user.ZTR = 3;
        pt = Timer_Regist(LOOP,100,Wet_ZeroTracking);	//定时零点跟踪	  //100
		if(pt == NULL)g_wet_err.state.errZeroTrackTask = 1;
		else	g_wet_err.state.errZeroTrackTask = 0;	
        //rtn = Wet_InitPara();
		if(rtn == FALSE)g_wet_err.state.errpara = 1;
		else	g_wet_err.state.errpara = 0;
		Wet_InitZero();
    FKM_SetFilterGrade(g_param->mFltLevel ,g_UserClb_DivCod,g_UserClb_DivCod*10);


	if(*(INT32U*)&g_wet_err.state != 0)
		return FALSE;
	else	return TRUE;	 
}

void Wet_Filter_Set(int cal)
{
	//g_UserClb_DivCod = 5;
	if(cal == 1)
        FKM_SetFilterGrade(g_param->mFltLevel ,20,20*10);
	else
        FKM_SetFilterGrade(g_param->mFltLevel ,g_UserClb_DivCod,g_UserClb_DivCod*10);
}
/******************************************************************************
  * @brief  称重初始化参数
  * @param  None
  * @retval TRUE: 初始化成功	FALSE: 初始化失败 
******************************************************************************/
Std_ReturnType	Wet_InitPara(void)
{	
	INT32U	i;
	INT32S	err = 0;
	static INT32S tmpDec = 0;
	
    err+= Wet_ParaIsRight(g_param->factory.SZA,-100000,100000);
    err+= Wet_ParaIsRight(g_param->factory.SFA,-1600000,1600000);
    err+= Wet_ParaIsRight(g_param->factory.SFAdigit,-1600000,1600000);
	if(err == 0)
        g_FactClb_k = (FP32)g_param->factory.SFAdigit /(FP32)(g_param->factory.SFA - g_param->factory.SZA);
	else
		g_FactClb_k = 1.0;

// 	err+= Wet_ParaIsRight(g_param->user.LDW,-1600000,4600000);
// 	err+= Wet_ParaIsRight(g_param->user.LWT,-1600000,4600000);
// 	err+= Wet_ParaIsRight(g_param->user.CWT,-1600000,4600000);
	if(err == 0)
        g_UserClb_k = (FP32)g_param->user.CWT /(FP32)(g_param->user.LWT - g_param->user.LDW);
	else
		g_UserClb_k = 1.0;

    if( g_param->mLinerPt[1] ==  g_param->mLinerPt[0])
	{//表示没有标定
		g_LinerClb_k[0] = 1;
		g_LinerClb_k[1] = 1;
	}
	else
	{
        tmpDec = g_param->mLinerPt[1] - g_param->mLinerPt[0];
        g_LinerClb_k[0] = (float)(g_param->mLinerNov[1] - g_param->mLinerNov[0] )/(float)(tmpDec);
		g_LinerClb_k[1] = 1;
	}
			
	if(g_LinerClb_k[0] <= 0.0000001)
	{
		g_LinerClb_k[0] = 1.0;
	}
	
    if(g_param->user.NOV > 0)
	{
		if(g_LinerClb_k[0] < 0.0001)
            g_UserClb_DivCod =(INT32U)g_param->user.RSN  ;
		else
            g_UserClb_DivCod =(INT32U)((FP32)g_param->user.RSN  / g_LinerClb_k[0]  );
	}
	else
		g_UserClb_DivCod = 20;	
	
	if(g_UserClb_DivCod < 20)
		g_UserClb_DivCod = 20;
	
    g_param->user.LICI[0] = 0;
    g_param->user.LICD[0] = 0;
	g_corr_link[0]     = 1.0;
	for(i=1;i<NP_LIN-1;i++)
	{
        err+= Wet_ParaIsRight(g_param->user.LICI[i],g_param->user.LICI[i-1],g_param->user.LICI[i+1]);
        err+= Wet_ParaIsRight(g_param->user.LICD[i],g_param->user.LICD[i-1],g_param->user.LICD[i+1]);
	}
	if(err == 0)
	{
		for(i=1;i<NP_LIN;i++)
            g_corr_link[i] = (FP32)(g_param->user.LICD[i]-g_param->user.LICD[i-1]) / (FP32)(g_param->user.LICI[i]-g_param->user.LICI[i-1]);
	}
	else
	{
		for(i=1;i<NP_LIN;i++)
			g_corr_link[i] = 1.0;
	}
	
//-----------------------------------------------------------------------------
	if(err < 0)return FALSE;
	else	return TRUE;	
}

void Wet_InitZero(void)
{
	g_fwet_zero = 0.0;
}
/******************************************************************************
  * @brief  判断整型参数正确性
  * @param 			 
  * @retval 0：正确，-1：达到极限值，-2：超出范围
******************************************************************************/
INT32S	Wet_ParaIsRight(INT32S para,INT32S min,INT32S max)
{
	if((para < min)||(para > max))
		return	-2;
	else if((para == min)||(para == max))
		return	-1;
	else
		return	0;
}
/******************************************************************************
  * @brief  读取重量
  * @param  wt：称重状态存放的结构体
  * @retval 实时重量值
******************************************************************************/
INT32S	Wet_Read(WET_STATE* wt)
{
	if(*(INT32U*)&g_wet_err.state != 0)g_wet_state.err = 1;
	else	g_wet_state.err = 0;

	wt->stableAD = g_wet_state.stableAD;	
	wt->stable = g_wet_state.stable;		
	wt->zero = g_wet_state.zero;		
	wt->overADC = g_wet_state.overADC;			
	wt->overGross = g_wet_state.overGross;		
	wt->overNET = g_wet_state.overNET;
	wt->underGross = g_wet_state.underGross;
	wt->busy = g_wet_state.busy;	
	wt->dnew = g_wet_state.dnew;
	wt->wetready = g_wet_state.wetready;
	wt->err = g_wet_state.err;
	wt->range = g_wet_state.range;
	wt->aztflg = g_wet_state.aztflg;		
	
	if(g_wet_state.wetready != 1)return	888888;
				
	g_wet_state.dnew = 0;	
    if(g_param->user.NOV > 0)
	{
        if(g_param->user.TAS == 0)
			return	g_wet_net;
        else if(g_param->user.TAS == 1)
			return	g_wet_gross; 
		else
			return	g_wet_newest;
	}
	else
	{
        if(g_param->user.TAS == 0)
			return	(FP32)g_wet_net * 5.12;		//AD103C算率 
        else if(g_param->user.TAS == 1)
			return	(FP32)g_wet_gross * 5.12;	//AD103C算率 
		else
			return	g_wet_newest;
	}									
}
void Wet_Read_State(uWET_STATE *wts)
{
	
}
/******************************************************************************
  * @brief  读取重量
  * @param  wt：称重状态存放的结构体
  * @retval 实时重量值
******************************************************************************/
void	Wet_Read_Wet(WET_DATA* wt)
{
	if(wt == NULL)
		return;
	wt->mWet_Stable = g_wet_input ;// g_Stable_output;
	wt->mWet_newest = (INT32S )g_Stable_X10;//g_wet_newest;
	wt->mWet_gross	= g_wet_gross;
	wt->mWet_net	= g_wet_net;
}

/******************************************************************************
  * @brief	读取 Wet 模块错误标志  
  * @param  err：返回的结构体指针
  * @retval None
******************************************************************************/
void	Wet_ErrRead(WET_ERR* err)
{
	err->errpara = g_wet_err.state.errpara;				//参数超出允许范围
	err->errAD = g_wet_err.state.errAD;					//ADC初始化失败
	err->errWorkTask = g_wet_err.state.errWorkTask;		//定时称重任务初始化失败
	err->errStableTask = g_wet_err.state.errStableTask;
	err->errZeroTrackTask = g_wet_err.state.errZeroTrackTask;
}
void Wet_Read_Err(uWET_ERR *wts)
{
	
	wts->value = g_wet_err.value;
}

void Wet_Clear_Err(void )
{
	g_wet_err.value = 0;
}
/******************************************************************************
  * @brief	Wet 获取平均值  
  * @param  num：求平均值的AD个数
  			fn：计算完成后的通知函数
  * @retval TRUE: 初始化成功	FALSE: 初始化失败
******************************************************************************/
Std_ReturnType	Wet_StartAvg(INT32U num,WET_AVG_PROC fn)
{
//	if((num > 0)&&(fn != NULL))
//	{
//		g_AvgNum = num;
//		g_AvgCount = num;
//		g_AvgSum = 0;
//		g_AvgBackFn = fn;
//		return TRUE;
//	}
	return FALSE;
}
int AD_CheckNew()
{
    return TRUE;
}
int32_t AD_Read()
{
    return 0;
}
/******************************************************************************
  * @brief  称重服务，请在小于 AD 转换周期的时间周期调用该函数
  * @param  None
  * @retval None 
******************************************************************************/
void	Wet_Working(void)
{
	INT32S	adcode;
//	FP32	temp;

//	AD_ERR	aderr;
//	TEMP_ERR	tmperr;

	if(AD_CheckNew() == TRUE)
	{
		adcode = AD_Read();
//		temp = TEMP_Read();
		Wet_Process(adcode,20.0);
//AVG work----------------------------------------------------------------------
//		if(g_AvgCount > 0)
//		{
//			g_AvgSum += g_wet_input;
//			g_AvgCount--;
//			if(g_AvgCount == 0)
//			{
//				g_AvgSum /= g_AvgNum;
//				if(g_AvgBackFn != NULL)g_AvgBackFn(g_AvgSum);
//				g_AvgCount = 0;
//				g_AvgNum = 0;
//				g_AvgSum = 0;
//				g_AvgBackFn = NULL;
//			}
//		}
	}
    //AD_ErrRead(&aderr);
//	TEMP_ErrRead(&tmperr);

}

#if 1

FP32 Wet_MultiRange_Dela(INT32S inWet)
{
	static FP32 mTempWet = 0.1;
	static FP32 mTempCoverWet = 0.0;
	static INT32S tmpDec = 0;
	static INT8U index = 0;		
	INT8U i = 0;

    if(g_param->user.LIC[0] == 0 )  //表示采用标定值计算
	{
		for(i = 1;i < 6;i++)
		{
            if( inWet <= g_param->mLinerPt[i])
			{
				index = i;
				break;
			}
		}	
		if(i >= 6)
			i = 5;
        mTempCoverWet = g_param->mLinerNov[index] - g_param->mLinerNov[index- 1];
        tmpDec = g_param->mLinerPt[index] - g_param->mLinerPt[index - 1];
		if(tmpDec != 0)
            mTempWet = (FP32)(g_wet_input - g_param->mLinerPt[index - 1])*(float)mTempCoverWet/(float)(tmpDec) + g_param->mLinerNov[index- 1];
		else
			mTempWet = g_wet_input * 0.001;
	}
	else
	{
        mTempWet = (FP32)(g_wet_input - g_param->mLinerPt[0])* (float)( g_param->user.SensorTotalRg)  / g_param->user.SensorSen ;
		mTempWet = mTempWet * 2.0f / 10.0f;
	}
	return mTempWet;
}

INT32S WetApp_Convert_Data(float inwet)
{

}

/******************************************************************************
  * @brief  称重处理
  * @param 	adcode：AD转换采集的原始数据
 			temp：实时温度数据 			 
  * @retval None
******************************************************************************/
void	Wet_Process(INT32S adcode,FP32 temp)
{
	static	INT32U	s_POdecrease = 20;		
	static		INT32S	swet;
	static		FP32	fwet;

	g_wet_state.busy = 1;

	if(s_POdecrease)s_POdecrease--;									//上电后先处理20次
	else g_wet_state.wetready = 1;

	if((adcode > 4000000)||((adcode < -4000000)))					//ADC超载
		g_wet_state.overADC = 1;
	else
		g_wet_state.overADC = 0; 	
    //swet = (adcode - g_param->factory.SZA)*g_FactClb_k;					//工厂标定
//	g_wet_input = Wet_StableAD(swet);								//稳定处理
	g_wet_input = adcode;
	 												 
    //fwet = (FP32)(g_wet_input - g_param->user.LDW)*g_UserClb_k;		//用户标定
	fwet = Wet_MultiRange_Dela(g_wet_input);
	g_Stable_X10 = (fwet - g_fwet_zero) * 10.0;	//
	g_Stable_input = fwet - g_fwet_zero;							//清除零点
	Wet_ZeroJudge(g_Stable_input);									//零位判断
    if(g_param->user.ZSE > 0)Wet_ZeroPowerUp();						//开机清零
    //if(g_param->user.ZTR > 0)Wet_ZeroTracking();						//零点跟踪
	
	g_wet_newest = (INT32S)( g_Stable_input + 0.5 );//Wet_Linear(g_Stable_input); 					//线性处理			
	g_wet_gross = WetApp_Convert_Data(g_Stable_input);						//毛重,分度值格式化，多量程
    g_wet_net   = g_wet_gross - WetApp_Convert_Data(g_param->user.TAV);						//净重

    if(g_wet_newest > (g_param->user.NOV + 9*g_param->user.RSN))					//毛重超载
			g_wet_state.overGross = 1;
	else 	g_wet_state.overGross = 0;
    if(g_wet_newest < (0 - 20*g_param->user.RSN))						//毛重欠载
			g_wet_state.underGross = 1;
	else	g_wet_state.underGross = 0;
    if(g_wet_net > ( g_param->user.NOV + 9*g_param->user.RSN))	//1000000				//净重超载
			g_wet_state.overNET = 1;
	else	g_wet_state.overNET = 0;
    if(g_param->user.MRA > 0)											//双量程判断
    {	if(g_wet_newest > g_param->user.MRA)g_wet_state.range = 1;
		else g_wet_state.range = 0;}
	else	g_wet_state.range = 0;
																			   
	g_wet_state.dnew = 1;
	g_wet_state.busy = 0;
}


#else

#endif
/******************************************************************************
  * @brief  线性修正
  * @param 	wt：原始重量		 
  * @retval 修正后的重量
******************************************************************************/
INT32S	Wet_Linear(INT32S wt)
{
	INT32U	i;
	FP32	rwt;

	if(wt <= 0)
	{
		//wt = (FP32)wt * g_corr_link[1];				
		return (INT32S)wt;
	}
	for(i=1;i<NP_LIN;i++)
	{
        if((wt > g_param->user.LICI[i-1])&&(wt <= g_param->user.LICI[i]))
		{
            rwt = (FP32)(wt - g_param->user.LICI[i-1]) * g_corr_link[i];
            rwt = rwt + g_param->user.LICD[i-1];
			return (INT32S)rwt;
		}						
	}
    if(wt > g_param->user.LICI[NP_LIN-1])
	{
        rwt = (FP32)(wt - g_param->user.LICI[NP_LIN-1]) * g_corr_link[NP_LIN-1];
        rwt = rwt + g_param->user.LICD[NP_LIN-1];
		return (INT32S)rwt;
	}		
	return wt;		
}

/******************************************************************************
  * @brief  判断示值是否稳定(0.1秒处理一次，处理长度1秒)
  * @input 		g_Stable_input,g_param->user.MTD,g_param->user.RSN
  * @output		g_wet_state.stable 				   
******************************************************************************/
#define		NUM_STBUF    		6 		//1秒
#define		NUM_BREAK			1		//及时

void	Wet_StableWt(void)
{
static	INT32U	s_flg_inited = 1;
static	FP32	s_wt_bufer[NUM_STBUF];
static	INT32U	s_wt_BufPtr = 0;
		FP32	wtMAX,wtMIN,wtAVG,linmit;
		INT32U	i;
static	INT32U	s_flg_Stable = 0;
static	FP32	s_wtLOCK;
static	INT32U	s_AvgUp_counter = 0,s_AvgDown_counter = 0;
//DebugPtf("g_Stable_input = %f\r\n",g_Stable_input);
	g_wet_state.stable = IsFilterDataStable();	
	//my_printf("%d \r\n",g_wet_state.stable);
	return;
	
	
    if(g_param->user.MTD == 0)
	{
		g_Stable_output = g_Stable_input;
		
		g_wet_state.stable = IsFilterDataStable();//1;
		return;
	}
	else 
        linmit = g_param->user.MTD * (FP32)g_param->user.RSN / 2;
			 
	if(s_flg_inited)
	{
		for(i=0;i<NUM_STBUF;i++)
			s_wt_bufer[i] = g_Stable_input;
		s_flg_inited = 0;	
	}

	s_wt_bufer[s_wt_BufPtr] = g_Stable_input; 
	s_wt_BufPtr++;
	if(s_wt_BufPtr >= NUM_STBUF)s_wt_BufPtr=0;

	wtMAX = s_wt_bufer[0];
	for(i=1;i<NUM_STBUF;i++)
		if(s_wt_bufer[i] > wtMAX)wtMAX = s_wt_bufer[i];
	wtMIN = s_wt_bufer[0];
	for(i=1;i<NUM_STBUF;i++)
		if(s_wt_bufer[i] < wtMIN)wtMIN = s_wt_bufer[i];

	wtAVG = 0;
	for(i=0;i<NUM_STBUF;i++)
		wtAVG += s_wt_bufer[i];
	wtAVG /= NUM_STBUF;

	if(s_flg_Stable)
	{
		if(wtAVG > s_wtLOCK)		
		{
			if((wtAVG - s_wtLOCK) > linmit)
			{
				s_AvgDown_counter = NUM_BREAK;
				if(s_AvgUp_counter)s_AvgUp_counter--;
				else  s_flg_Stable = 0;  
			}
			else
			{
				s_AvgUp_counter = NUM_BREAK;
				s_AvgDown_counter = NUM_BREAK;
			}
		}
		else					
		{
			if((s_wtLOCK - wtAVG) > linmit)
			{
				s_AvgUp_counter = NUM_BREAK;
				if(s_AvgDown_counter)s_AvgDown_counter --;
				else  s_flg_Stable = 0;
			}
			else
			{
				s_AvgUp_counter = NUM_BREAK;
				s_AvgDown_counter = NUM_BREAK;
			}
		}
	}
	else
	{
		if((wtMAX - wtMIN) < linmit)
		{
			s_flg_Stable = 1;	
			s_wtLOCK = wtAVG;	
			s_AvgUp_counter = NUM_BREAK;
			s_AvgDown_counter = NUM_BREAK;
		}
	}
	if(s_flg_Stable)
	{
		g_wet_state.stable = 1;
		g_Stable_output = wtAVG;
	}
	else 
	{
		g_wet_state.stable = 0;
		g_Stable_output = g_Stable_input;
	}	
	g_wet_state.stable = IsFilterDataStable();
}
/******************************************************************************
  * @brief  判断示值是否稳定(0.1秒处理一次，处理长度1秒)
  * @input 		g_Stable_input,g_param->user.RSN,g_wet_state.stable
  * @output		g_wet_state.zero 				   
******************************************************************************/
void	Wet_ZeroJudge(FP32	wt)
{
	FP32	linmitMAX,linmitMIN;	

	if(g_wet_state.stable == 1)
	{
        linmitMAX = 0.25 * (FP32)g_param->user.RSN;
		linmitMIN = 0 - linmitMAX;
		if((wt > linmitMIN)&&(wt < linmitMAX))
			g_wet_state.zero = 1;
		else
			g_wet_state.zero = 0;
	}
	else
		g_wet_state.zero = 0;	
}
/******************************************************************************
  * @brief	Wet 清零操作  
  * @param  none
  * @retval TRUE: 初始化成功	FALSE: 初始化失败
******************************************************************************/
Std_ReturnType	Wet_Zeroing(void)
{
	if(g_wet_state.stable == 1)
	{
        if(g_param->user.NOV > 0)
		{
            //if(g_wet_newest < (g_param->user.NOV /50 ))	//< 2%
            if(g_wet_newest <(g_param->user.NOV / 100 * g_param->user.ZSEHd) )
			{	
				g_fwet_zero += g_Stable_input;
				g_wet_err.state.err_set_zero = 0;
				return TRUE;
			}
			else
			{
				g_wet_err.state.err_set_zero = 1;
				return FALSE;	
			}	
		}
		else
		{
			g_wet_err.state.err_set_zero = 0;
			g_fwet_zero += g_Stable_input;
			return TRUE;
		}
	}
	else
	{
		g_wet_err.state.err_set_zero = 1;
		return FALSE;
	}
}
/******************************************************************************
  * @brief	Wet 开机清零操作  
  * @param  none
  * @retval none
******************************************************************************/
void	Wet_ZeroPowerUp(void)
{
	static 	INT32S	cnt = 0;
	static	INT32S	s_pzflged = FALSE;

	if(s_pzflged == TRUE)return;
	else
	{
		cnt++;
		if(cnt < 30)
			return;
		cnt =0;
		if((g_wet_state.stable == 1)&&(g_wet_state.wetready == 1))
		{
            if(g_param->user.NOV > 0)
			{
                if(g_wet_newest < (g_param->user.NOV / 100 * g_param->user.ZSE ) )//g_table_ZSE[g_param->user.ZSE]))
				{
					g_fwet_zero += g_Stable_input;
					g_wet_err.state.err_poweron_set_zero = 0;
				}
				else
				{
					g_wet_err.state.err_poweron_set_zero = 1;
				}
				s_pzflged = TRUE;
			}
			else
			{
                if(g_wet_newest < (10000 *  g_param->user.ZSE) )//g_table_ZSE[g_param->user.ZSE]))
				{
					g_fwet_zero += g_Stable_input;
					g_wet_err.state.err_poweron_set_zero = 0;
				}
				else
				{
					g_wet_err.state.err_poweron_set_zero = 1;
				}
				s_pzflged = TRUE;
			}
		}			
	} 
}
/******************************************************************************
  * @brief	Wet 零点跟踪  
  * @param  none
  * @retval none
******************************************************************************/
#define		NUM_ZTBUF    		10 		//1秒

void	Wet_ZeroTracking(void)
{
static	INT32U	s_flg_inited = 1;
static	FP32	s_wt_bufer[NUM_ZTBUF];
static	INT32U	s_wt_BufPtr = 0;
		FP32	wtMAX,wtMIN,linmit;			//,wtAVG;
		INT32U	i;

    if(g_param->user.ZTR == 0)
		goto pr_AZT_out;
    else if(g_param->user.ZTR < 100)
        linmit = g_param->user.ZTR * (FP32)g_param->user.RSN /10; //g_param->mZeroTruckTmr
	else	
		goto pr_AZT_out;
			 
	if(s_flg_inited)
	{
		for(i=0;i<NUM_ZTBUF;i++)
			s_wt_bufer[i] = g_Stable_input;					//g_Stable_output;
		s_flg_inited = 0;	
	}

	s_wt_bufer[s_wt_BufPtr] = g_Stable_input;				//g_Stable_output; 
	s_wt_BufPtr++;
	if(s_wt_BufPtr >= NUM_ZTBUF)s_wt_BufPtr=0;

	wtMAX = s_wt_bufer[0];
	for(i=1;i<NUM_ZTBUF;i++)
		if(s_wt_bufer[i] > wtMAX)wtMAX = s_wt_bufer[i];
	wtMIN = s_wt_bufer[0];
	for(i=1;i<NUM_ZTBUF;i++)
		if(s_wt_bufer[i] < wtMIN)wtMIN = s_wt_bufer[i];

	if((wtMAX < linmit)&&(wtMIN > (0 - linmit)))
	{
		if(g_wet_state.stable == 1)
		{
			g_fwet_zero += g_Stable_input;
			g_wet_state.aztflg = 1;
			return;
		}
	}
pr_AZT_out:
	g_wet_state.aztflg = 0;
}
/******************************************************************************
  * @brief	Wet 去皮操作  
  * @param  none
  * @retval TRUE: 初始化成功	FALSE: 初始化失败
******************************************************************************/
Std_ReturnType	Wet_Taring(void)
{
	if(g_wet_state.stable == 1)
	{
        if(g_param->user.NOV > 0)
		{
            if(g_wet_newest < g_param->user.NOV)	//< 满量程
			{	
				if(g_wet_newest <= 0)
                    g_param->user.TAV  = 0;
				else
                    g_param->user.TAV = g_wet_newest;
				return TRUE;
			}
			else
				return FALSE;	
		}
		else
		{
            g_param->user.TAV = g_wet_newest;
			return TRUE;
		}
	}
	else
		return FALSE;
}
/******************************************************************************
  * @brief	Wet 按分度值整数倍输出，多分数值处理  
  * @param  none
  * @retval none
******************************************************************************/
INT32S	Wet_IncFormat(INT32S wet)
{
	FP32 tempF;
	INT32S	curRSN;
	
    if((g_param->user.MRA > 0)&&(wet > g_param->user.MRA))
	{
        switch (g_param->user.RSN)
		{
			case 1:
				curRSN = 2;
				break;
			case 2:
				curRSN = 5;
				break;
			case 5:
				curRSN = 10;
				break;
			case 10:
				curRSN = 20;
				break;
			case 20:
				curRSN = 100;
				break;
			default:
                curRSN = g_param->user.RSN;
				break;
		}
	}
	else
        curRSN = g_param->user.RSN;

	tempF = (FP32)wet / curRSN;	
	if (tempF < 0)	tempF -= 0.5;
	else			tempF += 0.5;		

	return ((INT32S)tempF * curRSN);
}

INT32S	Wet_IncFloatFormat(float wet)
{
	FP32 tempF;
	INT32S	curRSN;
	
    curRSN = g_param->user.RSN;

	tempF = (FP32)(wet ) / curRSN;	
	if (tempF < 0)	tempF -= 0.5;
	else			tempF += 0.5;		

	return ((INT32S)tempF * curRSN);
}



