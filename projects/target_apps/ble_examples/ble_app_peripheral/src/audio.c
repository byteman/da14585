#include "audio.h"
#include <stdlib.h>


typedef struct audio_msg{
	char buffer[16];
	uint8 index;
	uint8 cmd;
}audio_msg_t;
#define MAX_MSG_NUM 10
typedef struct audio_msg_queue{
	audio_msg_t queue[MAX_MSG_NUM];
	uint8 count;
	uint8 rd_index; //当前播放到第几条
	uint8 wr_index; //当前写到第几条.
}audio_msg_queue_t;

static audio_msg_queue_t q;


void  audio_queue_digtal(const char* text)
{
		memset(q.queue[q.wr_index].buffer,0,16);
		strcpy(q.queue[q.wr_index].buffer,text);
		q.queue[q.wr_index].cmd = 0;
		q.wr_index = (++q.wr_index)%MAX_MSG_NUM;
		if(q.wr_index == q.rd_index){
			q.rd_index++;
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

}
void  audio_queue_message(audio_message_t msg)
{
		memset(q.queue[q.wr_index].buffer,0,16);
		strcpy(q.queue[q.wr_index].buffer,text);
		q.wr_index = (++q.wr_index)%MAX_MSG_NUM;
		if(q.wr_index == q.rd_index){
			q.rd_index++;
		}
}
void  audio_isr(void)
{
		if(audio_queue_empty())
		{
				return;
		}
		
		audio_msg_t* msg = audio_get_front();
		if(msg != NULL)
		{
				if(msg->buffer[msg->index] != 0)
				{
						//数据还没有播放完
						sc5040b_play(msg->buffer[msg->index++]);
				}
				else
				{
					//数据已经播放到最后一个字符.
					 audio_pop_front();
				}
		}
		

}