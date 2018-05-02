#ifndef CHANNEL_H
#define CHANNEL_H

#include "common.h"
#include "state.h"

typedef struct {
    uint8_t nr; //ͨ����
    uint8_t ready; //�Ƿ�ת�����
    int32_t value; //ֵ.
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

/**
 * @brief ��ȡȫ��ͨ����AD
 * @return 0 ��� 1 ��δת�����.
 */

uint8_t channel_read_all();



#endif
