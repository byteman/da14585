#ifndef PARAM_H
#define PARAM_H

#include "common.h"
#define MAX_CORN_NUM 5

typedef enum {
  PARA_ERR_NONE=0,
  PARA_ERR_UNINIT, //δ��ʼ��
  PARA_ERR_INVALID_PARA, //���������Ч
  PARA_ERR_READ_TIMEOUT, //��ȡ��ʱ
  PARA_ERR_WRITE_TIMEOUT, //д�볬ʱ
  PARA_ERR_READ_CRC, //У�����
}param_err_t;
//��Ҫ�־û�������豸����.
typedef struct _device_param{
    uint8_t sensor_nb; //����������
    float32	corn_k[MAX_CORN_NUM]; //�ǲ�ϵ��

}device_param;
//��оƬ�ж�ȡ����
param_err_t param_load();
//���������оƬ��
param_err_t param_save();


/**
 * @brief ��ȡ�ڴ���ȫ�ֵĲ���
 * @param ��ȡ�����ڴ����
 * @return �ɹ����
 */

param_err_t* param_get(device_param** para);

/**
 * @brief д��������ڴ���.
 * @param �޸�ָ���Ĳ���.
 * @return
 */
param_err_t param_set(device_param* para);


#endif
