/*******************************************************************************
  * @file    Weighing.c 
  * @author  yangxd
  * @version V 0.01
  * @date    2013.06.14
  * @brief   ���ش���ģ��
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

#define		NP_LIN    	4 		//3�����ص�
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
///* ����������� */

static		FP32	g_FactClb_k;
static		FP32	g_UserClb_k;
static 		FP32	g_LinerClb_k[4];
static 		FP32	g_LinerCoverWet = 0;

//static		FP32	g_corr_link[NP_LIN];	//���Բ���ϵ�� 

static	INT32S	g_AvgNum = 0;
static	INT32S	g_AvgCount = 0;
static	INT32S	g_AvgSum = 0;
static	WET_AVG_PROC g_AvgBackFn = NULL;
static		INT32U	g_UserClb_DivCod = 20;

static device_param* g_param;
static PARA_USER_T*  g_user;
/* Private functions ---------------------------------------------------------*/
//static	Std_ReturnType	Wet_InitPara(void);
void	Wet_Working(void);

static	INT32S	Wet_Linear(INT32S wt);
//static	INT32S	Wet_StableAD(INT32S wt);
void	Wet_StableWt(void);
static	void	Wet_ZeroJudge(FP32	wt);
static	void	Wet_ZeroPowerUp(void);
void	Wet_ZeroTracking(void);
static	INT32S	Wet_IncFormat(INT32S wet);
static 	INT32S	Wet_IncFloatFormat(float wet);
static	INT32S	Wet_ParaIsRight(INT32S para,INT32S min,INT32S max);
extern INT32S WetApp_Convert_Data(float inwet);

/******************************************************************************
  * @brief  ���س�ʼ��
  * @param  None
  * @retval TRUE: ��ʼ���ɹ�	FALSE: ��ʼ��ʧ�� 
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
	if(PARA_ERR_NONE != param_get_user(&g_user))
	{
			return 0;
	}
		rtn = Wet_InitPara();
		if(rtn == FALSE)g_wet_err.state.errpara = 1;
		else	g_wet_err.state.errpara = 0;
#if 0
		pt = Timer_Regist(LOOP,100,Wet_StableWt);		//��ʱ���ȴ���		 100ms 
		if(pt == NULL)g_wet_err.state.errStableTask = 1;
		else	g_wet_err.state.errStableTask = 0;

    pt = Timer_Regist(LOOP,100,Wet_ZeroTracking);	//��ʱ������	  //100
		if(pt == NULL)g_wet_err.state.errZeroTrackTask = 1;
		else	g_wet_err.state.errZeroTrackTask = 0;	
		if(rtn == FALSE)g_wet_err.state.errpara = 1;
		else	g_wet_err.state.errpara = 0;
	
#endif
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
  * @brief  ���س�ʼ������
  * @param  None
  * @retval TRUE: ��ʼ���ɹ�	FALSE: ��ʼ��ʧ�� 
******************************************************************************/
Std_ReturnType	Wet_InitPara(void)
{	
	INT32U	i;
	INT32S	err = 0;
	static INT32S tmpDec = 0;

	g_user->CWT = 200;
 	err+= Wet_ParaIsRight(g_user->LDW,-1600000,4600000);
 	err+= Wet_ParaIsRight(g_user->LWT,-1600000,4600000);
 	err+= Wet_ParaIsRight(g_user->CWT,-1600000,4600000);
	if(err == 0)
    g_UserClb_k = (FP32)g_user->CWT /(FP32)(g_user->LWT - g_user->LDW);
	else
		g_UserClb_k = 1.0;

  g_UserClb_DivCod =(INT32U)g_user->RSN  ;
	
	if(g_UserClb_DivCod < 20)
		g_UserClb_DivCod = 20;
	
//-----------------------------------------------------------------------------
	if(err < 0)return FALSE;
	else	return TRUE;	
}

void Wet_InitZero(void)
{
	g_fwet_zero = 0.0;
}
/******************************************************************************
  * @brief  �ж����Ͳ�����ȷ��
  * @param 			 
  * @retval 0����ȷ��-1���ﵽ����ֵ��-2��������Χ
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
  * @brief  ��ȡ����
  * @param  wt������״̬��ŵĽṹ��
  * @retval ʵʱ����ֵ
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
    if(g_user->NOV > 0)
	{
        if(g_user->TAS == 0)
			return	g_wet_net;
        else if(g_user->TAS == 1)
			return	g_wet_gross; 
		else
			return	g_wet_newest;
	}
	else
	{
        if(g_user->TAS == 0)
			return	(FP32)g_wet_net * 5.12;		//AD103C���� 
        else if(g_user->TAS == 1)
			return	(FP32)g_wet_gross * 5.12;	//AD103C���� 
		else
			return	g_wet_newest;
	}									
}
void Wet_Read_State(uWET_STATE *wts)
{
	
}
/******************************************************************************
  * @brief  ��ȡ����
  * @param  wt������״̬��ŵĽṹ��
  * @retval ʵʱ����ֵ
******************************************************************************/
void	Wet_Read_Wet(WET_DATA* wt)
{
	if(wt == NULL)
		return;
	wt->mWet_Stable = g_wet_input ;// g_Stable_output; û�о����ֶ�ֵ����
	wt->mWet_newest = (INT32S )g_Stable_X10;//g_wet_newest; //�ŵ���10��
	wt->mWet_gross	= g_wet_gross; //�����ֶ�ֵ�����ë��
	wt->mWet_net	= g_wet_net;
}

/******************************************************************************
  * @brief	��ȡ Wet ģ������־  
  * @param  err�����صĽṹ��ָ��
  * @retval None
******************************************************************************/
void	Wet_ErrRead(WET_ERR* err)
{
	err->errpara = g_wet_err.state.errpara;				//������������Χ
	err->errAD = g_wet_err.state.errAD;					//ADC��ʼ��ʧ��
	err->errWorkTask = g_wet_err.state.errWorkTask;		//��ʱ���������ʼ��ʧ��
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
  * @brief	Wet ��ȡƽ��ֵ  
  * @param  num����ƽ��ֵ��AD����
  			fn��������ɺ��֪ͨ����
  * @retval TRUE: ��ʼ���ɹ�	FALSE: ��ʼ��ʧ��
******************************************************************************/
Std_ReturnType	Wet_StartAvg(INT32U num,WET_AVG_PROC fn)
{
	if((num > 0)&&(fn != NULL))
	{
		g_AvgNum = num;
		g_AvgCount = num;
		g_AvgSum = 0;
		g_AvgBackFn = fn;
		return TRUE;
	}
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
void calib_prase_handler(){
		if(g_AvgCount > 0)
		{
			g_AvgSum += g_wet_input;
			g_AvgCount--;
			if(g_AvgCount == 0)
			{
				g_AvgSum /= g_AvgNum;
				if(g_AvgBackFn != NULL)g_AvgBackFn(g_AvgSum);
				g_AvgCount = 0;
				g_AvgNum = 0;				
				g_AvgSum = 0;
				g_AvgBackFn = NULL;	
			}
		}					
}
/******************************************************************************
  * @brief  ���ط�������С�� AD ת�����ڵ�ʱ�����ڵ��øú���
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
	

	}
    //AD_ErrRead(&aderr);
//	TEMP_ErrRead(&tmperr);

}

#if 0

FP32 Wet_MultiRange_Dela(INT32S inWet)
{
	static FP32 mTempWet = 0.1;
	static FP32 mTempCoverWet = 0.0;
	static INT32S tmpDec = 0;
	static INT8U index = 0;		
	INT8U i = 0;

    if(g_user->LIC[0] == 0 )  //��ʾ���ñ궨ֵ����
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
        mTempWet = (FP32)(g_wet_input - g_param->mLinerPt[0])* (float)( g_user->SensorTotalRg)  / g_user->SensorSen ;
		mTempWet = mTempWet * 2.0f / 10.0f;
	}
	return mTempWet;
}
#endif

/******************************************************************************
  * @brief  ���ش���
  * @param 	adcode��ADת���ɼ���ԭʼ����
 			temp��ʵʱ�¶����� 			 
  * @retval None
******************************************************************************/
void	Wet_Process(INT32S adcode,FP32 temp)
{
	static	INT32U	s_POdecrease = 20;		
	static		INT32S	swet;
	static		FP32	fwet;

	g_wet_state.busy = 1;

	if(s_POdecrease)s_POdecrease--;									//�ϵ���ȴ���20��
	else g_wet_state.wetready = 1;

	if((adcode > 4000000)||((adcode < -4000000)))					//ADC����
		g_wet_state.overADC = 1;
	else
		g_wet_state.overADC = 0; 	
	g_wet_input = adcode;
	 												 
    fwet = (FP32)(g_wet_input - g_user->LDW)*g_UserClb_k;		//�û��궨
	//fwet = Wet_MultiRange_Dela(g_wet_input);
	g_Stable_X10 = (fwet - g_fwet_zero) * 10.0;	//
	g_Stable_input = fwet - g_fwet_zero;							//������
	Wet_ZeroJudge(g_Stable_input);									//��λ�ж�
  if(g_user->ZSE > 0)
		Wet_ZeroPowerUp();						//��������
  //if(g_param->ZTR > 0)Wet_ZeroTracking();						//������
	
	g_wet_newest = (INT32S)( g_Stable_input + 0.5 );//Wet_Linear(g_Stable_input); 					//���Դ���			
	g_wet_gross = WetApp_Convert_Data(g_Stable_input);						//ë��,�ֶ�ֵ��ʽ����������
    g_wet_net   = g_wet_gross - WetApp_Convert_Data(g_user->TAV);						//����

    if(g_wet_newest > (g_user->NOV + 9*g_user->RSN))					//ë�س���
		g_wet_state.overGross = 1;
	else 	
		g_wet_state.overGross = 0;
    if(g_wet_newest < (0 - 20*g_user->RSN))						//ë��Ƿ��
		g_wet_state.underGross = 1;
	else	
		g_wet_state.underGross = 0;
    if(g_wet_net > ( g_user->NOV + 9*g_user->RSN))	//1000000				//���س���
		g_wet_state.overNET = 1;
	else	
		g_wet_state.overNET = 0;
    if(g_user->MRA > 0)											//˫�����ж�
    {	
    	if(g_wet_newest > g_user->MRA)g_wet_state.range = 1;
		else g_wet_state.range = 0;
	}
	else	
		g_wet_state.range = 0;
																			   
	g_wet_state.dnew = 1;
	g_wet_state.busy = 0;
}





/******************************************************************************
  * @brief  �ж�ʾֵ�Ƿ��ȶ�(0.1�봦��һ�Σ�������1��)
  * @input 		g_Stable_input,g_user->MTD,g_user->RSN
  * @output		g_wet_state.stable 				   
******************************************************************************/
#define		NUM_STBUF    		6 		//1��
#define		NUM_BREAK			1		//��ʱ

void	Wet_StableWt(void)
{
	g_wet_state.stable = IsFilterDataStable();	 //KM�˲��п��Ի�ȡ�ȶ�
}
/******************************************************************************
  * @brief  �ж�ʾֵ�Ƿ��ȶ�(0.1�봦��һ�Σ�������1��)
  * @input 		g_Stable_input,g_user->RSN,g_wet_state.stable
  * @output		g_wet_state.zero 				   
******************************************************************************/
void	Wet_ZeroJudge(FP32	wt)
{
	FP32	linmitMAX,linmitMIN;	

	if(g_wet_state.stable == 1)
	{
        linmitMAX = 0.25 * (FP32)g_user->RSN;
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
  * @brief	Wet �������  
  * @param  none
  * @retval TRUE: ��ʼ���ɹ�	FALSE: ��ʼ��ʧ��
******************************************************************************/
Std_ReturnType	Wet_Zeroing(void)
{
	if(g_wet_state.stable == 1)
	{
        if(g_user->NOV > 0)
		{
            //if(g_wet_newest < (g_user->NOV /50 ))	//< 2%
            if(g_wet_newest <(g_user->NOV / 100 * g_user->ZSEHd) )
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
  * @brief	Wet �����������  
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
            if(g_user->NOV > 0)
			{
                if(g_wet_newest < (g_user->NOV / 100 * g_user->ZSE ) )//g_table_ZSE[g_user->ZSE]))
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
                if(g_wet_newest < (10000 *  g_user->ZSE) )//g_table_ZSE[g_user->ZSE]))
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
  * @brief	Wet ������  
  * @param  none
  * @retval none
******************************************************************************/
#define		NUM_ZTBUF    		10 		//1��

void	Wet_ZeroTracking(void)
{
static	INT32U	s_flg_inited = 1;
static	FP32	s_wt_bufer[NUM_ZTBUF];
static	INT32U	s_wt_BufPtr = 0;
		FP32	wtMAX,wtMIN,linmit;			//,wtAVG;
		INT32U	i;

    if(g_user->ZTR == 0)
		goto pr_AZT_out;
    else if(g_user->ZTR < 100)
        linmit = g_user->ZTR * (FP32)g_user->RSN /10; //g_param->mZeroTruckTmr
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
  * @brief	Wet ȥƤ����  
  * @param  none
  * @retval TRUE: ��ʼ���ɹ�	FALSE: ��ʼ��ʧ��
******************************************************************************/
Std_ReturnType	Wet_Taring(void)
{
	if(g_wet_state.stable == 1)
	{
        if(g_user->NOV > 0)
		{
            if(g_wet_newest < g_user->NOV)	//< ������
			{	
				if(g_wet_newest <= 0)
                    g_user->TAV  = 0;
				else
                    g_user->TAV = g_wet_newest;
				return TRUE;
			}
			else
				return FALSE;	
		}
		else
		{
            g_user->TAV = g_wet_newest;
			return TRUE;
		}
	}
	else
		return FALSE;
}
/******************************************************************************
  * @brief	Wet ���ֶ�ֵ����������������ֵ����  
  * @param  none
  * @retval none
******************************************************************************/
INT32S	Wet_IncFormat(INT32S wet)
{
	FP32 tempF;
	INT32S	curRSN;
	
    if((g_user->MRA > 0)&&(wet > g_user->MRA))
	{
        switch (g_user->RSN)
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
                curRSN = g_user->RSN;
				break;
		}
	}
	else
        curRSN = g_user->RSN;

	tempF = (FP32)wet / curRSN;	
	if (tempF < 0)	tempF -= 0.5;
	else			tempF += 0.5;		

	return ((INT32S)tempF * curRSN);
}

INT32S	Wet_IncFloatFormat(float wet)
{
	FP32 tempF;
	INT32S	curRSN;
	
    curRSN = g_user->RSN;

	tempF = (FP32)(wet ) / curRSN;	
	if (tempF < 0)	tempF -= 0.5;
	else			tempF += 0.5;		

	return ((INT32S)tempF * curRSN);
}



