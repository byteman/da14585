#ifndef STATE_H
#define STATE_H

#include "common.h"

typedef struct _adc_err_state_t{
    int32_t	NoExcBit:1;			//û�м�����ѹ
    int32_t	OpenBridgeBit:1;	//��·��·
    int32_t	NoAdcBit:1;			// 1 ����û��鵽ת�����
}adc_err_state_t;
typedef struct _state{
    uint8_t err;
}device_state;
#endif
