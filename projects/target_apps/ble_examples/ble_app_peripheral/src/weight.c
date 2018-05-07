
#include "param.h"
#include "adc.h"
#include "channel.h"
#include "weight.h"
#include "scaler.h"
#include "Filter_KMedian.h"
static device_param*  g_param;
static scaler_info_t* g_sinfo;
static PARA_USER		g_ParaUser;
#define		NP_LIN    	4 		


static	INT32S	g_wet_newest;
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

static		FP32	g_FactClb_k;
static		FP32	g_UserClb_k;
static 		FP32	g_LinerClb_k[4];
static 		FP32	g_LinerCoverWet = 0;

static		FP32	g_corr_link[NP_LIN];	//

static	INT32S	g_AvgNum = 0;
static	INT32S	g_AvgCount = 0;
static	INT32S	g_AvgSum = 0;
//static	WET_AVG_PROC g_AvgBackFn = NULL;
static		INT32U	g_UserClb_DivCod = 20;

//开机清零操作.
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
			if(g_param->NOV > 0)
			{
				if(g_wet_newest < (g_param->NOV / 100 * g_param->ZSE ) )//g_table_ZSE[g_ParaUser.ZSE]))	
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
				if(g_wet_newest < (10000 *  g_ParaUser.ZSE) )//g_table_ZSE[g_ParaUser.ZSE]))	
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
  * @brief  判断当前是否是零位
  * @input 		g_Stable_input,g_ParaUser.RSN,g_wet_state.stable		 
  * @output		g_wet_state.zero 				   
******************************************************************************/


void	Wet_ZeroJudge(FP32	wt)
{
	FP32	linmitMAX,linmitMIN;	

	if(g_wet_state.stable == 1)
	{
		linmitMAX = 0.25 * (FP32)g_param->RSN;
		linmitMIN = 0 - linmitMAX;
		if((wt > linmitMIN)&&(wt < linmitMAX))
			g_wet_state.zero = 1;
		else
			g_wet_state.zero = 0;
	}
	else
		g_wet_state.zero = 0;	
}


FP32 Wet_MultiRange_Dela(INT32S inWet)
{
	static FP32 mTempWet = 0.1;
	static FP32 mTempCoverWet = 0.0;
	static INT32S tmpDec = 0;
	static INT8U index = 0;		
	INT8U i = 0;

	//第一段重量时候记录的重量值
	mTempCoverWet = g_param->mLinerNov[1] - g_param->mLinerNov[0];
	//第一段重量时候记录的AD值
	tmpDec = g_param->mLinerPt[1] - g_param->mLinerPt[0];

	
	mTempWet = (FP32)(inWet - g_param->mLinerPt[0])*(float)mTempCoverWet/(float)(tmpDec) + g_param->mLinerNov[0];
	

	return mTempWet;
}

