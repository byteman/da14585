#include "dispatcher.h"

#define MAX_CB_NR   3
#define MAX_MSG_NR  5
typedef struct cb_item{
	TARGET_ID 		id; 	//模块id
	msg_cb_t  func; //模块消息回调函数.
}cb_item_t;
typedef struct msg_handler{
	 MSG_ID 		cmd; //某个消息
	 cb_item_t cb_func[MAX_CB_NR]; //消息对应的回调函数
	 uint8 		cb_nr; //回调函数的个数.
}msg_handler_t;

static msg_handler_t handlers[MAX_MSG_NR];
static uint8 msg_nr = 0;

void dispatch_send_simple_msg(MSG_ID cmd, uint8 param)
{
	comm_msg_t msg;
	msg.cmd = cmd;
	msg.u8_param = param;
	dispatch_send_msg(&msg);
}
//发送消息
void dispatch_send_msg(comm_msg_t* msg)
{
	uint8 i = 0; 
	uint8 j = 0;
	for(i = 0; i < msg_nr; i++)
	{
		if(handlers[i].cmd == msg->cmd )
		{
			for(j = 0; j < handlers[i].cb_nr;j++)
			{
				if(handlers[i].cb_func[j].func != NULL)
				{
					handlers[i].cb_func[j].func(msg);
				}
			}
		}
	}
}

//注册消息和回调函数.
uint8  dispatch_register_msg(TARGET_ID id, MSG_ID  cmd,  msg_cb_t cb)
{
	uint8 i = 0; 
	uint8 j = 0;
	if(msg_nr >= MAX_MSG_NR)
	{
		return 2;
	}
	
	for(i = 0; i < msg_nr; i++)
	{
		//已经存在了该消息，则添加到后面去.
		if(handlers[i].cmd == cmd )
		{
			//找到消息id了，再看看消息id对应的函数是否超了最大个数,
			if(handlers[i].cb_nr >= MAX_CB_NR)
			{
				return 1;
			}
			//没有超就添加到最后.
			for(j = 0; j < handlers[i].cb_nr; j++)
			{
				if(handlers[i].cb_func[j].id == id){
						//该消息已经被该模块注册过了，重新更新.
						handlers[i].cb_func[j].func = cb;
						return 0;
					
				} 
				
			}
			//还没有注册过.添加一个
			handlers[i].cb_func[handlers[i].cb_nr].func = cb;
			handlers[i].cb_func[handlers[i].cb_nr].id = id;
			handlers[i].cb_nr++;
			return 0;
			
		}
		
	}

	
	//找不到该消息，那么就新建一个消息
	handlers[msg_nr].cmd = cmd;
	handlers[msg_nr].cb_nr = 1;
	handlers[msg_nr].cb_func[0].func = cb;
	handlers[msg_nr].cb_func[0].id = id;
	msg_nr++;
	return 0;
		
}

