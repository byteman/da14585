#include "param.h"
#include "i2c_eeprom.h"
#include "user_peripheral.h"
#include "user_periph_setup.h"

//总共256bytes
#define USER_PARA_ADDR 	0    		//offset 0 		size 512bytes
#define LOGIC_PARA_ADDR 80   //offset 80  size 512bytes
#define DEV_PARA_ADDR  	128		//offset  size 1024bytes

const PARA_USER_T g_Default_User_Para = {

		0,						//INT8U		FMD;(0 = IIR2,1 = FIR32,2 = IIR8,3 = IIR4FT,4 = FIR64,5 = FIR64+ MA)
		3,						//INT8U		ICR;(0...7, 600\300\150\75\38\19\9\4)

		100000,						//INT32S	NOV;(0...1599999,Nominal Value)
		50,				//INT32S	CWT;(1000000,20%...120%NOV)
		0,						//INT32S	LDW;(0...+-1599999,zero point)
		1000000,				//INT32S	LWT;(0...+-1599999,full scale)

		0,						//INT32S	MRA;(Multirange switch point)
		1,						//INT32S	RSN;(1, 2, 5, 10, 20, 50, 100 )
		0,						//INT8U		DPT;(0...6,1 = xxxxxx.x,2 = xxxxx.xx,3 = xxxx.xxx)
		20,						//INT8U		ZSE;(0 = deactivated,1 = 2%NOV,2 = 5%,3 = 10%,4 = 20%)
		5,						//INT8U		ZTR;(0 = deactivated,1 = 0.5d,2 = 1.0d,3 = 2.0d,4 = 3.0d)
		5,						//INT8U		MTD;(0 = deactivated,1 = 0.25d/s, 0.5 , 1 , 2 , 3)

	//	{0,1000000,0,0},		//INT32S	LIC[4];
	//	{0,500,2000,10000},		//INT32S	LICI[3];
	//	{0,500,2000,10000},		//INT32S	LICD[3];

		1,						//INT8U		TAS;(0 = net, 1 = gross)
		0,						//INT32S	TAV;(Tare value)

		5,						//ZSEHD    //0, 2,4,10,20,50,100 %
		0.9,					//CalkValue
		10000,				//	INT32S		SensorTotalRg;
		200000,				//INT32S	SensorSen;	
		0						// checksum;

};
const device_param	g_Default_Device_Para = 
{
		//.sensor_nb=4,
		.corn_k= {1,1,1,1,1},
		.mWetUp=0,
		.mWetDown = 0,
		.mTareRang = 0,//皮重范围
    .mTareEN=0,// 皮重允许
		.mBLAuto = 0,
		.mCornCalibWet = 50, //角差标定时的标准重量.
		.mFltLevel = 3,				//INT8U	mFltLevel; //滤波等级
	#if 0
		100,			// 	INT32U	mAdjPcsWet;	//计数标定重量
		50,			// 	INT16U	mAdjPcsCnt;//计数标定片数
		0,			//  	INT16U	mWetUp;		//	重量上限值
		0,			//  	INT16U  mWetDown;	//重量下限值
		0,			//  	INT16U	mPcsUp;		//检数上限值
		0,			//  	INT16U	mPcsDown;	//检数下限值
		0,			//  	INT16U	mBeep;		//蜂鸣模式

		0,			//  	INT8U	mAutoSetTare ;	///是1,否可以自动
		6,			// 	INT8U	mWorkMode;	//打印及累积模式
		0,			// 	INT8U	mPType;		//打印模式
		8,			// 	INT8U  	mLabx ;		//lab x 数据输出模式
		2,			// 	INT8U	mPrtx;		//Prt x 数据输出模式
		0,			// 	INT8U	mLang;		//语言模式
		0,			// 	INT8U	mCalFlag;	//鉴定标志	

		0,			// 	INT8U	mDualNum;	//多量程选择	
		100,			// 	INT16U	mCount	;	//内分度数
		{1,1,1,2},			// 	INT16U	mInc[4];	//分度值
		{0,0,6000,6000},			// 	INT16U	mCap[4];	//满量程
		0,			// 	INT8U	mKeyLock;		//键盘锁定
		0,			// 	INT8U	mAnm;		//动物模式
		UNIT_KG | UNIT_G|UNIT_LB,			// 	INT8U	mUntMask;//启用的单位
		
		0,			//INT8U	mCurUIDIdx;
		0,			//INT8U	mCurIncIdx;
		0,			//INT8U	mTareRang;//皮重范围
		0,			//INT8U	mTareEN;// 皮重允许
		2,			//INT8U	mBLEN;	//背光开关设置
		0,			//INT8U	mBLAuto;//自动关机管理
		{15000,200000,200000,200000,200000,200000},			//INT32S	mLinerPt[4]; //线性标定的点
		{0,6000,6000,6000,6000,6000}, 	//INT32S	mLinerNov[4]; //线性标定的值

		0.974,				//float	mFatLiter; //公升转换值
		3,				//INT8U	mDisSpd;   //显示速度
		0,				//INT8U   mPHoldEn;  //最大值维持功能
		0,				//INT8U	mSleepTmr; //自动睡眠时间
		3,				//INT8U	mFltLevel; //滤波等级
		1,				//INT8U	mZeroTruckTmr;// 零点跟踪速度
		2,				//INT8U	mDefaultUID; //默认单位 gr-lt

		3,			//mCrpValue		//蠕变跟踪默认FilterMini_4
		0,			//mBkLight   		//亮度调节默认为最低
		6,			//mDiv1Num
		6,			//mDiv2Num
		
		32,			//mSlaveID
		1,			//INT8U	mSensorNb;//传感器数量
		0,			//mADSpeed
		{1,1,1,1,1},			//float		mCorlK[5]; //角差系数
		0,			// 	INT8U 	checksum;		
		#endif
};
const logic_param_t	g_logic_default_para = 
{
	.history_weight = {0,0,0}, //最近三条历史记录
	.history_sum = 0, //历史累计和
	.history_count = 0 //历史累计次数.
};
static device_param 	g_param;
static PARA_USER_T  	g_user;
static logic_param_t	g_logic;
static uint8_t g_init =0;


static void _enter(void)
{
		
    i2c_eeprom_init(I2C_SLAVE_ADDRESS, I2C_SPEED_MODE, I2C_ADDRESS_MODE, I2C_ADDRESS_SIZE);
	
}
static void _leave(void)
{
		//i2c_eeprom_release();
	
}

/**
 ****************************************************************************************
 * @brief Test write byte.
 * @param[in] address Memory position to write the byte to
 * @param[in] byte    Byte to be written
 * @return void
 ****************************************************************************************
 */
static i2c_error_code _write_byte(uint32_t address, uint8_t byte)
{
    _enter();
    i2c_error_code code = i2c_eeprom_write_byte(address, byte);  
		_leave();
		return code;
}

/**
 ****************************************************************************************
 * @brief Test read byte.
 * @param[in] address  Memory address to read the byte from
 * @param[in|out] byte Pointer to the read byte
 * @return void
 ****************************************************************************************
 */
static i2c_error_code _read_byte(uint32_t address, uint8_t *byte)
{
	
	   _enter();
    i2c_error_code code = i2c_eeprom_read_byte(address, byte);  
		_leave();
		return code;

}
/**
 ****************************************************************************************
 * @brief Test write data.
 * @param[in] data              Pointer to the array of bytes to be written
 * @param[in] address           Starting address of the write process
 * @param[in] size              Size of the data to be written
 * @param[in|out] bytes_written Bytes that were actually written (due to memory size limitation)
 * @return void
 ****************************************************************************************
 */
static i2c_error_code _write_data(uint8_t *data, uint32_t address, uint32_t size, uint32_t *bytes_written)
{
		i2c_error_code code;
		uint32_t index = 0;
		uint32_t left  = size;
		uint32_t written = 0;
		uint32_t total = 0;
		 _enter();
		while(left > 0){
			code = i2c_eeprom_write_data(data + index, address + index, (left > I2C_EEPROM_PAGE)?I2C_EEPROM_PAGE:left, &written);
			if(code != I2C_NO_ERROR){
					return code;
				
			}
			total += written;
			left -= written;
			index+=written;
		}
		*bytes_written = total;
		_leave();
		return code;
}
/**
 ****************************************************************************************
 * @brief Test read data.
 * @param[in] data           Pointer to the array of bytes to be read
 * @param[in] address        Starting memory address
 * @param[in] size           Size of the data to be read
 * @param[in|out] bytes_read Bytes that were actually read (due to memory size limitation)
 * @return void
 ****************************************************************************************
 */
static i2c_error_code _read_data(uint8_t *data, uint32_t address, uint32_t size, uint32_t *bytes_read)
{
	
	 _enter();
    i2c_error_code code = i2c_eeprom_read_data(data, address, size, bytes_read);
   _leave();
		return code;
}


INT8U checksum(INT8U *str,INT32U len)
{
	INT32U i;
	INT8U chksum = 0xAA;

	for(i=0; i<len; i++)
	{	
		chksum += *str;
		str++;
	}
	return chksum;	
}

void param_test(void)
{

}
param_err_t param_init(void)
{
   
		uint32_t bytes_read = 0,bytes_written = 0;

		i2c_error_code code = _read_data((uint8_t*)&g_param, DEV_PARA_ADDR, sizeof(g_param), &bytes_read);
		if(code != I2C_NO_ERROR)
		{
				memcpy((void*)&g_param, (const void*)&g_Default_Device_Para,sizeof(g_param));
				//return PARA_ERR_READ_TIMEOUT;
		}
		
		if((g_param.checksum) != checksum((uint8_t*)&g_param,sizeof(g_param)-1))
		{
				memcpy((void*)&g_param, (const void*)&g_Default_Device_Para,sizeof(g_param));
				
				//return PARA_ERR_READ_CRC;
		}
		
		code = _read_data((uint8_t*)&g_user, USER_PARA_ADDR, sizeof(g_user), &bytes_read);
		if(code != I2C_NO_ERROR)
		{
			memcpy((void*)&g_user, (void*)&g_Default_User_Para,sizeof(g_user));
				
				//return PARA_ERR_READ_TIMEOUT;
		}
		
		if((g_user.checksum) != checksum((uint8_t*)&g_user,sizeof(g_user)-1))
		{
			memcpy((void*)&g_user, (void*)&g_Default_User_Para,sizeof(g_user));
				//return PARA_ERR_READ_CRC;
		}
		code = _read_data((uint8_t*)&g_logic, LOGIC_PARA_ADDR, sizeof(g_logic), &bytes_read);
		if(code != I2C_NO_ERROR)
		{
			memcpy((void*)&g_logic, (void*)&g_logic_default_para,sizeof(g_logic));	
				//return PARA_ERR_READ_TIMEOUT;
		}
		
		if((g_logic.checksum) != checksum((uint8_t*)&g_logic,sizeof(g_logic)-1))
		{
			memcpy((void*)&g_logic, (void*)&g_logic_default_para,sizeof(g_logic));
			
				//return PARA_ERR_READ_CRC;
		}
		
		g_init = 1;
		
		return PARA_ERR_NONE;
}

param_err_t param_save(uint8_t t)
{
	//uint8_t *data, uint32_t address, uint32_t size, uint32_t *bytes_written
		uint32_t bytes_written = 0,bytes_read = 0;
		
		if(t == DEV_PARA_T)
		{
				g_param.checksum = checksum((uint8_t*)&g_param, sizeof(g_param) - 1);
				
				_write_data((uint8_t*)&g_param, DEV_PARA_ADDR, sizeof(g_param),&bytes_written);
		}
		else if(t ==  USER_PARA_T ){
				
				g_user.checksum = checksum((uint8_t*)&g_user, sizeof(g_user) - 1);
				
				_write_data((uint8_t*)&g_user, USER_PARA_ADDR, sizeof(g_user),&bytes_written);
				_read_data((uint8_t*)&g_user, USER_PARA_ADDR, sizeof(g_user), &bytes_read);
				if((g_user.checksum) != checksum((uint8_t*)&g_user,sizeof(g_user)-1))
				{
					memcpy((void*)&g_user, (void*)&g_Default_User_Para,sizeof(g_user));
					
						//return PARA_ERR_READ_CRC;
				}
		}
		else if(t == LOGIC_PARA_T){
				g_logic.checksum = checksum((uint8_t*)&g_logic, sizeof(g_logic) - 1);
				
				_write_data((uint8_t*)&g_logic, LOGIC_PARA_ADDR, sizeof(g_logic),&bytes_written);
		}
		return PARA_ERR_NONE;
}

param_err_t param_get(device_param **para)
{
    if(!g_init) return PARA_ERR_UNINIT;
    if(para == 0) return PARA_ERR_INVALID_PARA;
    *para = &g_param;
    return PARA_ERR_NONE;
}
param_err_t param_get_user(PARA_USER_T** para)
{
		if(!g_init) return PARA_ERR_UNINIT;
    if(para == 0) return PARA_ERR_INVALID_PARA;
    *para = &g_user;
    return PARA_ERR_NONE;
}
param_err_t param_get_logic(logic_param_t** para)
{
		if(!g_init) return PARA_ERR_UNINIT;
    if(para == 0) return PARA_ERR_INVALID_PARA;
    *para = &g_logic;
    return PARA_ERR_NONE;
}
