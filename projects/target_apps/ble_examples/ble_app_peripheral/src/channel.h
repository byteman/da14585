#ifndef CHANNEL_H
#define CHANNEL_H

#include "common.h"
#include "state.h"


#define 		CAP_N   10 	
#define		  CAP_M   4  //�ĸ��ǲ��. 	

typedef struct {

	INT32S	m_Value[CAP_N+1]; 
	INT32S 	data_sum;
	INT32S  m_index;	
	INT8U		m_ad_err;
	INT8U		m_ad_ready;
	

}__CHANNEL_VALUE;
typedef struct {
    uint8_t nr; //ͨ����
    uint8_t ready; //�Ƿ�ת�����
		int32_t org; //δ�ƶ�ǰԭʼad
    int32_t value; //ֵ.
		int32_t filter; //�˲����ֵ.
    adc_err_state_t state; //��ǰ��оƬ״̬.
}ad_channel_t;


typedef enum {
    CHAN_ERR_NONE=0, //�ɹ�
    CHAN_ERR_BUSY,//ת��δ�ɹ�
}ad_channel_err_t;
/**
 * @brief ��ʼ��ͨ��
 * @param ͨ����Ŀ
 * @return
 */
int8_t  channel_init(uint8_t nr);

/**
 * @brief ��ȡ��ĳ��ͨ������Ϣ
 * @param ָ����ͨ��
 * @param ͨ����Ϣ
 * @return 0 �ɹ� ����ʧ��
 */
int8_t  channel_read(uint8_t chan,ad_channel_t* info);

/**
 * @brief ��ȡĳ·ͨ�����һ�ε�����.
 * @param chan ͨ�����
 * @return ͨ����Ϣ.
 */
ad_channel_t* channel_get(uint8_t chan);

INT32S channel_get_filter_ad(uint8_t chan);

/**
 * @brief ��ȡȫ��ͨ����AD
 * @return 0 ��� 1 ��δת�����.
 */

uint8_t channel_read_all(void);

uint8_t channel_all_power(uint8_t en);

uint8_t channel_power(uint8_t chan, uint8_t en);

#endif
