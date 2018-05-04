#ifndef KEY_H
#define KEY_H

#include "types.h"

//���� "�������� ��������Ӳ���Զ�����
//��ס�����3�룬��ʾ������ַ
//��������� �������� ˫������� ����ۼ�
//����������3��,�ػ�.

typedef struct {
	uint8 key;
	uint8 event;
}key_msg;

//�ػ�.
void  key_power_off();
//���ؼ��Ƿ��������סn��
void  is_pwr_key_pressed(uint8 s);
//������Ƿ��������סn��
void  is_zero_key_pressed(uint8 s);

void  is_zero_key_clicked(uint8 s);

void  is_zero_key_double_clicked(uint8 s);

void  key_isr();
#endif

