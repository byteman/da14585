#include "blescaler.h"
static uint8 ble_state = 0;
//称重库初始化，只需要调用一次.
int ble_scaler_init(scaler_config_t* config)
{
		//custom_GPIO_reservations();
		//custom_set_pad_functions();
		user_app_start();
}

//每隔100ms调用一次
int ble_scaler_run(void)
{

}

int ble_scaler_register_event(scaler_event_t evt)
{

}

int ble_scaler_ble_connected(uint8 state)
{
		ble_state = state;
		dispatch_send_simple_msg(MSG_BLE_STATE, state);
}

//称重控制接口，执行常用的，清零/清零历史重量
int ble_scaler_exec_cmd(scaler_cmd_t* cmd)
{

}

//获取重量和称台状态信息
scaler_result_t* ble_scaler_get_info(void)
{

}

/*
该接口在蓝牙状态变化后调用，或者初始化时候，传入蓝牙的地址.
*/
int ble_scaler_set_ble_info(ble_info_t* ble)
{

}
uint8 ble_scaler_get_ble_state(void)
{
	return ble_state;
}
