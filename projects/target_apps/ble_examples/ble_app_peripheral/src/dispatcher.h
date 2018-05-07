#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "common.h"
#include "types.h"

#define MAX_MSG_SIZE 32


typedef struct comm_msg{
	 MSG_ID cmd;
	 uint8 u8_param;
	 int   u32_param;
	 float f32_param;
	 char  data[MAX_MSG_SIZE];
	 uint8  size;
}comm_msg_t;
//消息回调函数
typedef void (*msg_cb_t)(comm_msg_t* msg);

//发送消息
void dispatch_send_msg(comm_msg_t* msg);

//注册消息和回调函数.
uint8 dispatch_register_msg(TARGET_ID id, MSG_ID  cmd,  msg_cb_t cb);


#endif


