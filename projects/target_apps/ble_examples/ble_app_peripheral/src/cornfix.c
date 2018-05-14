
#include "cornfix.h"
#include "gpio.h"
#include "user_peripheral.h"
#include "user_periph_setup.h"
#include "matrix.h"
#include "channel.h"
//static 	__CHANNEL_VALUE ad_value[CAP_M];  
//INT32S			AD_filter[CAP_M];
CORL_DEF		corl_ad;


//存放重量
void ad_put_corl_bx(int wet)
{
	char i = 0;
	for(i =0;i< CAP_M;i++)
	{
		corl_ad.Bx[i] = wet ;
	}
}

//角差标点，对每个角的标定操作,支持存放ad值
void ad_put_corl_adx(char index)
{
	char i = 0;
	for(i =0;i< CAP_M;i++)
	{
			corl_ad.Adx[index][i] = channel_get_filter_ad(i);
	}
}
//角差标点，计算标定结果
void ad_calib_corl_k(device_param* parm)
{
	char i = 0;
	float res[MAX_SNSR_NUM];
	float min_k = 0;
	//ad_put_corl_bx( parm->mLinerNov[1]) ;
	//ad_put_corl_bx(parm->mCornCalibWet);
	ad_put_corl_bx(50);
	linear_equation_group(0, corl_ad.Adx, corl_ad.Bx,CAP_M, res);	

	for(i =0;i< CAP_M;i++)
	{
		parm->corn_k[i] = res[i] /res[0] ; 
		if(parm->corn_k[i]  < 0.8f || parm->corn_k[i]  > 1.2f)
		{
			//parm->corn_k[i]  = 1.0;
		}
	}	
}

