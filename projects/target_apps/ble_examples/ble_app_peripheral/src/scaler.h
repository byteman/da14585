#ifndef SCALER_H
#define SCALER_H

#include "common.h"
#include "state.h"
#include "channel.h"
#include "types.h"

#define MAX_CHAN 4
typedef struct _scaler_info_t{
    int32_t org_ad; //���һ�μ���õ���AD��
    int32_t filter_ad; //�����˲����AD
    uint8_t adc_soc_err;//1 ad����
		FP32	  wet_zero; //���
		uint8		zeroFlag; 
		uint8		stillFlag;
		uint8		upFlow;
		uint8   downFlow;
		int32_t div_weight; //�ֶ�ֵ���ë��
	  int32_t org_weight; //û�о����ֶ�ֵ���������.	
		int32_t adArrs[MAX_CHAN];
}scaler_info_t;
void    scaler_init();
void    scaler_run();
int			scaler_get_history_record(float* values, uint8 num);
void  	scaler_get_display_weight(char* buffer, uint8 size);
scaler_info_t* 		scaler_get_info(void);
#endif
