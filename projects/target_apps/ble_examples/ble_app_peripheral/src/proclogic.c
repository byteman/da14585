#include "gui.h"
#include <stdlib.h>
#include "proclogic.h"
#include "scaler.h"
#include "batty.h"
#include "key.h"
#include "dispatcher.h"
#include "param.h"
#include "WetApp.h"

static device_param* g_param;
static PARA_USER_T*  g_user;
static uint8_t g_flag = 0;
void logic_init()
{
		param_get(&g_param);
		param_get_user(&g_user);
	
}

void logic_isr(void)
{
		if(gScaleAppData.mScaleStatue.stStable &&  gScaleAppData.mScaleWet > g_user->RSN )
		{
				g_flag = 1;
		}
		else
		{
				
		}
		
		if(gScaleAppData.mScaleStatue.stZero && g_flag)
		{
				
				g_flag = 0;
		}
}

