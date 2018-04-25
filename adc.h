#ifndef ADC_H
#define ADC_H

#include "common.h"

typedef enum {
    ADC_CHIP_CS1237=0,
}adc_chip_type_t;
typedef enum {
    ADC_ERR_NONE=0,
    ADC_ERR_SOC, //оƬ����.
    ADC_ERR_BRIGE, //��·�Ͽ�.
    ADC_ERR_BUSY, //��δת���ɹ�.
}adc_err_t;
typedef struct _adc_config{
    uint8_t hz; //ת������
    uint8_t type; //adc���� 0 cs1237

}adc_config_t;
//��ȡĳһ·ͨ����ADֵ
adc_err_t adc_read(uint8_t chan,int32_t *value);
adc_err_t adc_config(uint8_t chan,adc_config_t* config);

/**
 * @brief ĳ·adת���Ƿ����.
 * @return
 */
adc_err_t adc_ready(uint8_t chan);
#endif
