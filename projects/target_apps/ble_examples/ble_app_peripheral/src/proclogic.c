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


