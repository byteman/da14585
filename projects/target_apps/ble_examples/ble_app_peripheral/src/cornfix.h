
#ifndef __CORN_FIX_H
#define __CORN_FIX_H

#include "types.h"
#include "common.h"
#include "param.h"

#define 		CAP_N   10 	
#define		  CAP_M   4  //四个角差点. 	

typedef struct {

	INT32S	m_Value[CAP_N+1]; //??????????
	INT32S 	data_sum;
	INT32S  	m_index;	
	INT8U	m_ad_err;
	INT8U	m_ad_ready;
	

}__CHANNEL_VALUE;


#define		MAX_SNSR_NUM				6
typedef struct {
	int				total;
	unsigned char 	addr[MAX_SNSR_NUM];
	float 			Adx[MAX_SNSR_NUM][MAX_SNSR_NUM]; //每个角上标定的ad值.
	float				Bx[MAX_SNSR_NUM]; //每个角上标定的重量
}CORL_DEF;

void ad_put_corl_adx(char index);
void ad_calib_corl_k(device_param* parm);

#endif /* __CS1237_H */

