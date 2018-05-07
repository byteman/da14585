#include "dispatcher.h"

#define MAX_CB_NR   3
#define MAX_MSG_NR  5
typedef struct cb_item{
	TARGET_ID 		id; 	//ģ��id
	msg_cb_t  func; //ģ����Ϣ�ص�����.
}cb_item_t;
typedef struct msg_handler{
	 MSG_ID 		cmd; //ĳ����Ϣ
	 cb_item_t cb_func[MAX_CB_NR]; //��Ϣ��Ӧ�Ļص�����
	 uint8 		cb_nr; //�ص������ĸ���.
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
//������Ϣ
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

//ע����Ϣ�ͻص�����.
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
		//�Ѿ������˸���Ϣ������ӵ�����ȥ.
		if(handlers[i].cmd == cmd )
		{
			//�ҵ���Ϣid�ˣ��ٿ�����Ϣid��Ӧ�ĺ����Ƿ���������,
			if(handlers[i].cb_nr >= MAX_CB_NR)
			{
				return 1;
			}
			//û�г�����ӵ����.
			for(j = 0; j < handlers[i].cb_nr; j++)
			{
				if(handlers[i].cb_func[j].id == id){
						//����Ϣ�Ѿ�����ģ��ע����ˣ����¸���.
						handlers[i].cb_func[j].func = cb;
						return 0;
					
				} 
				
			}
			//��û��ע���.���һ��
			handlers[i].cb_func[handlers[i].cb_nr].func = cb;
			handlers[i].cb_func[handlers[i].cb_nr].id = id;
			handlers[i].cb_nr++;
			return 0;
			
		}
		
	}

	
	//�Ҳ�������Ϣ����ô���½�һ����Ϣ
	handlers[msg_nr].cmd = cmd;
	handlers[msg_nr].cb_nr = 1;
	handlers[msg_nr].cb_func[0].func = cb;
	handlers[msg_nr].cb_func[0].id = id;
	msg_nr++;
	return 0;
		
}

