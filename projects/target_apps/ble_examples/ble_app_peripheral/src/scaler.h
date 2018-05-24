#ifndef SCALER_H
#define SCALER_H

#include "common.h"
#include "state.h"
#include "channel.h"
#include "types.h"

#define MAX_CHAN 4
typedef struct _scaler_info_t{
		uint8_t refresh; //�Ƿ�������״̬�и���.
    int32_t org_ad; //���һ�μ���õ���AD��
    int32_t filter_ad; //�����˲����AD
    uint8_t adc_soc_err;//1 ad����
		FP32	  wet_zero; //���
		uint8		zeroFlag; 
		uint8		stillFlag;
		uint8		upFlow;
		uint8   downFlow;
		uint8		ready; //������������������.
		int32_t div_weight; //�ֶ�ֵ���ë��
	  int32_t org_weight; //û�о����ֶ�ֵ���������.	
		int32_t adArrs[MAX_CHAN]; //ad����.
}scaler_info_t;
void    scaler_init();
void    scaler_run();
void		scaler_set_ble_addr(char* addr);
const char* scaler_get_ble_addr(void);
int			scaler_get_history_record(float* values, uint8 num);
void  	scaler_get_display_weight(char* buffer, uint8 size);
scaler_info_t* 		scaler_get_info(void);
void		scaler_reset_history(void);
#endif
