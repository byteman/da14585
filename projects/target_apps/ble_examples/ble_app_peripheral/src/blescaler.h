//称重库调用接口
#include "dispatcher.h"

typedef unsigned char uint8;
typedef int int32;

//称重sdk的错误返回值.
typedef enum {
	ERR_NONE =0, //成功
	
}scaler_err_t;
#if 0
typedef struct scaler_err{
	unsigned ad1:1;
	unsigned ad2:1;
	unsigned ad3:1;
	unsigned ad4:1;	
}scaler_err_t;
#endif


#define MAX_HIST_NUM 3
//重量信息结构体
typedef struct scaler_result{
		uint8	dot; //小数点位数
		uint8	zeroFlag; //重量处于零位标志 0 不是零位 1:零位
		uint8	stillFlag; //重量稳定标志	 0 不稳定	1:稳定
		uint8	upFlow;		//重量值向上溢出，超过阀值
		uint8   downFlow;	//重量值向下溢出，超过阀值
		uint8	error; //设备的错误信息
		int32 	weight; 	//实时重量值
		int32	hist_weight[MAX_HIST_NUM];
		int32   hist_count; //称重次数.
}scaler_result_t;

typedef struct scaler_config{
	uint8 	dot; //小数点位数.目前仅仅支持1
	uint8	div; //分度值. 	  目前仅仅支持1
	int32 default_calib_kg;  //默认标定重量，使用该重量作为默认标定重量.
	
}scaler_config_t;

typedef struct scaler_cmd{
	uint8 action; //1.清零历史重量 2.清零重量 3.待机 4.关机
	int32 param; //与某个命令相关参数，保留
	
}scaler_cmd_t;
typedef void (*weight_ready_event)(int32 value, int dot);
typedef void (*scaler_error_event)(uint8 type);
typedef struct scaler_event{
	//完成了一次称重操作.
	weight_ready_event weight_ready;
	scaler_error_event error_ready;
	//有称重错误产生.
}scaler_event_t;


//称重库初始化，只需要调用一次.
int ble_scaler_init(scaler_config_t* config);

//每隔100ms调用一次
int ble_scaler_run(void);

int ble_scaler_register_event(scaler_event_t evt);

//称重控制接口，执行常用的，清零/清零历史重量
int ble_scaler_exec_cmd(scaler_cmd_t* cmd);

//获取重量和称台状态信息
scaler_result_t* ble_scaler_get_info(void);


//蓝牙信息设置接口

typedef struct ble_info{
	uint8 ble_addr[6]; //蓝牙地址
	uint8 ble_state; //连接状态 0 断开 1 已经连接.
	
}ble_info_t;
/*
该接口在蓝牙状态变化后调用，或者初始化时候，传入蓝牙的地址.
*/
int ble_scaler_set_ble_info(ble_info_t* ble);
int ble_scaler_bel_connected(uint8 state);
