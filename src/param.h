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

typedef struct
{
    INT8U 	serialnum[7+1];
    INT32S	SZA;			//�����궨���
    INT32S	SFA;			//�����궨����ֵ
    INT32S	SFAdigit;		//�����궨Ŀ��ֵ
    //INT8U AddrChr[2+1];

    INT8U 	checksum;
}PARA_TACTORY;

typedef struct
{
    INT8U devicename[15+1];	//devicename[15+1];
    INT8U password[7+1];	//password[7+1];
    INT8U dLCAddr;			//dLCAddr;
    INT8U AddrChr[2+1];		//AddrChr[2+1];

    INT32U baudRate;		//baudRate;
    INT8U wordLength;		//wordLength;
    INT8U parityBit;		//parityBit;

    INT8U	FMD;			//�˲�ģʽ		FMD;(0 = IIR2,1 = FIR32,2 = IIR8,3 = IIR4FT,4 = FIR64,5 = FIR64+ MA)
    INT8U	ICR;			//�������		ICR;(0...7, 600\300\150\75\38\19\9\4)

    INT32S	NOV;		//������	NOV;(0...1599999,Nominal Value)
    INT32S	CWT;		//�궨ֵ	CWT;(1000000,20%...120%NOV)	//	�궨����ֵ
    INT32S	LDW;		//�궨���	LDW;(0...+-1599999,zero point)   //�궨���AD��
    INT32S	LWT;		//�궨����	LWT;(0...+-1599999,full scale)	 //�궨����ad��
    INT32S	MRA;		//������ѡ���	MRA;(Multirange switch point)
    INT32S	RSN;		//�ֶ�ֵ	RSN;(1, 2, 5, 10, 20, 50, 100 )
    INT8U	DPT;		//С����		DPT;(0...6,1 = xxxxxx.x,2 = xxxxx.xx,3 = xxxx.xxx)
    INT8U	ZSE;		//���㷶Χ		ZSE;(0 = deactivated,1 = 2%NOV,2 = 5%,3 = 10%,4 = 20%)
    INT8U	ZTR;		//����ס��Χ		ZTR;(0 = deactivated,1 = 0.5d,2 = 1.0d,3 = 2.0d,4 = 3.0d)
    INT8U	MTD;		//�������ٶ�		MTD;(0 = deactivated,1 = 0.25d/s, 0.5 , 1 , 2 , 3)

    //���������õ�
    INT32S	LIC[4];		//INT32S	LIC[4];
    INT32S	LICI[4];	//INT32S	LICI[3];
    INT32S	LICD[4];	//INT32S	LICD[3];

    INT8U	TAS;		//ë��/Ƥ��		TAS;(0 = net, 1 = gross)
    INT32S	TAV;		//Ƥ��ֵ	TAV;(Tare value)

    INT8U	ZSEHd;		//�ֶ����㷶Χ
    float		CalkValue;
    INT32S		SensorTotalRg;
    INT32S	SensorSen;
    INT8U checksum;		// checksum;
}PARA_USER;
typedef struct
{
    INT16U	target;				//Ŀ��ֵ
    INT16U	preact;				//��ǰ��
    INT16U	fine;				//ϸι��
    INT16U	coarse;				//��ι��
    INT16U	ToleranceUp;		//���ʲ�
    INT16U	ToleranceDown;		//���ʲ�
    INT16U	ToleranceZero;		//���ʲ�
    INT16U	DlyFineCop;			//ϸι����ʱ��	 0.1s
    INT16U	DlyCoarseCop;		//��ι����ʱ��   0.1s
    INT16U	DlyStableCop;		//�ȶ��ж���ʱ   0.1s
    INT16U	SelfCorrection;		//����������     0.01

    INT8U 	checksum;
}PARA_OTHER;

//��Ҫ�־û�������豸����.
typedef struct _device_param{
    uint8_t sensor_nb; //����������
    float32	corn_k[MAX_CORN_NUM]; //�ǲ�ϵ��
    INT32S	mAdjPcsWet;	//�����궨����
    INT16U	mAdjPcsCnt;//�����궨Ƭ��
    INT16U	mWetUp;		//	��������ֵ
    INT16U  	mWetDown;	//��������ֵ
    INT16U	mPcsUp;		//��������ֵ
    INT16U	mPcsDown;	//��������ֵ
    INT16U	mBeep;		//����ģʽ

    INT8U	mAutoSetTare ;	///�Ƿ�����Զ�
    INT8U	mWorkMode;	//��ӡ���ۻ�ģʽ
    INT8U	mRngType;		//�������趨
    INT8U  	mLabx ;		//lab x �������ģʽ tr-232
    INT8U	mPrtx;		//Prt x �������ģʽ d-out
    INT8U	mLang;		//����ģʽ
    INT8U	mCalFlag;	//������־

    INT8U	mDualNum;	//������ѡ��
    INT16U	mCount	;	//�ڷֶ���
    INT8U	mInc[4];	//�ֶ�ֵ
    INT32U	mCap[4];	//������
    INT8U	mKeyLock;		//��������
    INT8U	mAnm;		//����ģʽ
    INT32U	mUntMask;//���õĵ�λ

    INT8U	mCurUIDIdx;
    INT8U	mCurIncIdx;
    INT8U	mTareRang;//Ƥ�ط�Χ
    INT8U   	mTareEN;// Ƥ������
    INT8U	mBLEN;	//���⿪������
    INT8U	mBLAuto;//�Զ��ػ�����
    INT32S  	mLinerPt[6]; //���Ա궨�ĵ�
    INT32S	mLinerNov[6]; //���Ա궨��ֵ

    float		mFatLiter; //����ת��ֵ
    INT8U	mDisSpd;   //��ʾ�ٶ�
    INT8U   	mPHoldEn;  //���ֵά�ֹ���
    INT8U	mSleepTmr; //�Զ�˯��ʱ��
    INT8U	mFltLevel; //�˲��ȼ�
    INT8U	mZeroTruckTmr;// �������ٶ�
    INT8U	mDefaultUID; //Ĭ�ϵ�λ

    INT8U	mCrpValue; //�����ٲ���
    INT8U	mBkLight ;  // ���ȵ���
    INT8U	mDiv2Num; //��һ���̵ķֶ�ֵ����
    INT8U	mDiv1Num;//�ڶ����̵ķֶ�ֵ����

    INT8U	mSlaveID;
    INT8U	mSensorNb;//����������
    INT8U     mADSpeed; //adת���ٶ�
    float		mCorlK[5]; //�ǲ�ϵ��
    INT8U 	checksum;

    PARA_TACTORY factory;
    PARA_USER user;
    PARA_OTHER other;
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
