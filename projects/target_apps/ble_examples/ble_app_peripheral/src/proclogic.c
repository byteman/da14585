#include "gui.h"
#include <stdlib.h>
#include "proclogic.h"
#include "scaler.h"
#include "batty.h"
#include "key.h"
#include "dispatcher.h"
#include "param.h"
//#include "WetApp.h"
#include "scaler.h"
#include "audio.h"

static device_param* g_param;
static PARA_USER_T*  g_user;
static logic_param_t*  g_logic;
static uint8_t g_flag = 0;


void logic_init()
{
		param_get(&g_param);
		param_get_user(&g_user);
		param_get_logic(&g_logic);
	
}
//存入一条重量，如果超过最大条数了，将最老一条数据推出，放入最近一条到最后
void logic_push_weight(INT32S value)
{
		
		if(g_logic->history_count < MAX_HIS_WEIGHT){
				g_logic->history_weight[g_logic->history_count] = value;
		}else{
				uint8 i = 0;
				for(; i < MAX_HIS_WEIGHT-1; i++)
				{
						g_logic->history_weight[i] = g_logic->history_weight[i+1];
				}
				g_logic->history_weight[i] = value;
		}
		g_logic->history_count++; 
		g_logic->history_sum += value;
		param_save(LOGIC_PARA_T);
}
void logic_isr(void)
{
		scaler_info_t * sif = scaler_get_info();
		if(
			sif->stillFlag &&  
			sif->div_weight > g_user->RSN && 
			(g_flag==0)
		)
		{
				char buf[16] = {0,};
			
			
				g_flag = 1;
				
				//记录历史重量.
				logic_push_weight(sif->div_weight);
				format_weight(buf,16,sif->div_weight,1);	
				//播报重量语言.
				audio_queue_message(buf);
				
		}
		else
		{
				
		}
		
		if(sif->stillFlag &&  
			sif->div_weight < g_user->RSN && 
			(g_flag==1))
		{
				
				g_flag = 0;
		}
}

