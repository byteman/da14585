#include "audio.h"
#include <stdlib.h>
#include <string.h>

typedef struct audio_msg{
	char buffer[16];
	uint8 index;
}audio_msg_t;
#define MAX_MSG_NUM 10
typedef struct audio_msg_queue{
	audio_msg_t queue[MAX_MSG_NUM];
	uint8 count;
	uint8 rd_index; //��ǰ���ŵ��ڼ���
	uint8 wr_index; //��ǰд���ڼ���.
}audio_msg_queue_t;

static audio_msg_queue_t q;
static audio_soc_t *_soc = NULL;

void  audio_queue_init(audio_soc_t *soc)
{
		_soc = soc;
    memset(&q, 0, sizeof(q));
}
void  audio_queue_message(const char* text)
{

		memset(&q.queue[q.wr_index],0,sizeof(audio_msg_t));
		strcpy(q.queue[q.wr_index].buffer,text);
		q.wr_index = (++q.wr_index)%MAX_MSG_NUM;

		if(q.wr_index == q.rd_index){
            q.rd_index = (++q.rd_index)%MAX_MSG_NUM;
		}
		
}
uint8_t audio_queue_empty(void)
{
	return (q.rd_index == q.wr_index)?1:0;
}
audio_msg_t* audio_get_front(void)
{
    if(audio_queue_empty())
		{
            return NULL;
		}
		return &q.queue[q.rd_index];
		
}
void audio_pop_front(void)
{
    if(!audio_queue_empty()){
        q.rd_index = (++q.rd_index)%MAX_MSG_NUM;
    }

}
//��Ƶ�����߼�.
void  audio_isr(void)
{
		static uint8 cnt;
		if(cnt++%5 != 0) return;
		if(audio_queue_empty())
		{
				return;
		}
		
		audio_msg_t* msg = audio_get_front();
		if(msg != NULL)
		{
				if(msg->buffer[msg->index] != 0)
				{
						//���ݻ�û�в�����
            char c = msg->buffer[msg->index];
						
						//�п��ܻ���busy,�ȴ���һ�λ��Ქ��.
						if(_soc->play!=NULL){
								int err = _soc->play(c);
								if(err == 0)
								{
									//���ųɹ����ۼӲ���ָ��.
										msg->index++;
								}
						}
						else
						{
								msg->index++;
						}
						
				}
				else
				{
					//�����Ѿ����ŵ����һ���ַ�.
					 audio_pop_front();
				}
		}
		

}
