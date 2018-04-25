#ifndef PARAM_H
#define PARAM_H

#include "common.h"
#define MAX_CORN_NUM 5

typedef enum {
  PARA_ERR_NONE=0,
  PARA_ERR_UNINIT, //未初始化
  PARA_ERR_INVALID_PARA, //传入参数无效
  PARA_ERR_READ_TIMEOUT, //读取超时
  PARA_ERR_WRITE_TIMEOUT, //写入超时
  PARA_ERR_READ_CRC, //校验错误
}param_err_t;

typedef struct
{
    INT8U 	serialnum[7+1];
    INT32S	SZA;			//工厂标定零点
    INT32S	SFA;			//工厂标定采样值
    INT32S	SFAdigit;		//工厂标定目标值
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

    INT8U	FMD;			//滤波模式		FMD;(0 = IIR2,1 = FIR32,2 = IIR8,3 = IIR4FT,4 = FIR64,5 = FIR64+ MA)
    INT8U	ICR;			//输出速率		ICR;(0...7, 600\300\150\75\38\19\9\4)

    INT32S	NOV;		//满量程	NOV;(0...1599999,Nominal Value)
    INT32S	CWT;		//标定值	CWT;(1000000,20%...120%NOV)	//	标定砝码值
    INT32S	LDW;		//标定零点	LDW;(0...+-1599999,zero point)   //标定零点AD码
    INT32S	LWT;		//标定加载	LWT;(0...+-1599999,full scale)	 //标定量程ad码
    INT32S	MRA;		//多量程选择点	MRA;(Multirange switch point)
    INT32S	RSN;		//分度值	RSN;(1, 2, 5, 10, 20, 50, 100 )
    INT8U	DPT;		//小数点		DPT;(0...6,1 = xxxxxx.x,2 = xxxxx.xx,3 = xxxx.xxx)
    INT8U	ZSE;		//置零范围		ZSE;(0 = deactivated,1 = 2%NOV,2 = 5%,3 = 10%,4 = 20%)
    INT8U	ZTR;		//零点跟住范围		ZTR;(0 = deactivated,1 = 0.5d,2 = 1.0d,3 = 2.0d,4 = 3.0d)
    INT8U	MTD;		//零点跟踪速度		MTD;(0 = deactivated,1 = 0.25d/s, 0.5 , 1 , 2 , 3)

    //线性修正用的
    INT32S	LIC[4];		//INT32S	LIC[4];
    INT32S	LICI[4];	//INT32S	LICI[3];
    INT32S	LICD[4];	//INT32S	LICD[3];

    INT8U	TAS;		//毛重/皮重		TAS;(0 = net, 1 = gross)
    INT32S	TAV;		//皮重值	TAV;(Tare value)

    INT8U	ZSEHd;		//手动置零范围
    float		CalkValue;
    INT32S		SensorTotalRg;
    INT32S	SensorSen;
    INT8U checksum;		// checksum;
}PARA_USER;
typedef struct
{
    INT16U	target;				//目标值
    INT16U	preact;				//提前量
    INT16U	fine;				//细喂量
    INT16U	coarse;				//粗喂量
    INT16U	ToleranceUp;		//上允差
    INT16U	ToleranceDown;		//下允差
    INT16U	ToleranceZero;		//零允差
    INT16U	DlyFineCop;			//细喂禁比时间	 0.1s
    INT16U	DlyCoarseCop;		//粗喂禁比时间   0.1s
    INT16U	DlyStableCop;		//稳定判断延时   0.1s
    INT16U	SelfCorrection;		//自修正比例     0.01

    INT8U 	checksum;
}PARA_OTHER;

//需要持久化保存的设备参数.
typedef struct _device_param{
    uint8_t sensor_nb; //传感器个数
    float32	corn_k[MAX_CORN_NUM]; //角差系数
    INT32S	mAdjPcsWet;	//计数标定重量
    INT16U	mAdjPcsCnt;//计数标定片数
    INT16U	mWetUp;		//	重量上限值
    INT16U  	mWetDown;	//重量下限值
    INT16U	mPcsUp;		//检数上限值
    INT16U	mPcsDown;	//检数下限值
    INT16U	mBeep;		//蜂鸣模式

    INT8U	mAutoSetTare ;	///是否可以自动
    INT8U	mWorkMode;	//打印及累积模式
    INT8U	mRngType;		//多量程设定
    INT8U  	mLabx ;		//lab x 数据输出模式 tr-232
    INT8U	mPrtx;		//Prt x 数据输出模式 d-out
    INT8U	mLang;		//语言模式
    INT8U	mCalFlag;	//鉴定标志

    INT8U	mDualNum;	//多量程选择
    INT16U	mCount	;	//内分度数
    INT8U	mInc[4];	//分度值
    INT32U	mCap[4];	//满量程
    INT8U	mKeyLock;		//键盘锁定
    INT8U	mAnm;		//动物模式
    INT32U	mUntMask;//启用的单位

    INT8U	mCurUIDIdx;
    INT8U	mCurIncIdx;
    INT8U	mTareRang;//皮重范围
    INT8U   	mTareEN;// 皮重允许
    INT8U	mBLEN;	//背光开关设置
    INT8U	mBLAuto;//自动关机管理
    INT32S  	mLinerPt[6]; //线性标定的点
    INT32S	mLinerNov[6]; //线性标定的值

    float		mFatLiter; //公升转换值
    INT8U	mDisSpd;   //显示速度
    INT8U   	mPHoldEn;  //最大值维持功能
    INT8U	mSleepTmr; //自动睡眠时间
    INT8U	mFltLevel; //滤波等级
    INT8U	mZeroTruckTmr;// 零点跟踪速度
    INT8U	mDefaultUID; //默认单位

    INT8U	mCrpValue; //蠕变跟踪参数
    INT8U	mBkLight ;  // 亮度调节
    INT8U	mDiv2Num; //第一量程的分度值数字
    INT8U	mDiv1Num;//第二量程的分度值数字

    INT8U	mSlaveID;
    INT8U	mSensorNb;//传感器数量
    INT8U     mADSpeed; //ad转换速度
    float		mCorlK[5]; //角差系数
    INT8U 	checksum;

    PARA_TACTORY factory;
    PARA_USER user;
    PARA_OTHER other;
}device_param;
//从芯片中读取参数
param_err_t param_load();
//保存参数到芯片中
param_err_t param_save();


/**
 * @brief 获取内存中全局的参数
 * @param 获取到的内存参数
 * @return 成功与否
 */

param_err_t* param_get(device_param** para);

/**
 * @brief 写入参数到内存中.
 * @param 修改指定的参数.
 * @return
 */
param_err_t param_set(device_param* para);


#endif
