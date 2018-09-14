#include "gui.h"
#include <stdlib.h>
#include "commenu.h"
#include "key.h"
#include "channel.h"
#include "param.h"
#include "Weighing.h"
#include "scaler.h"


void gui_show_weight(scaler_info_t * sif,uint8 update,uint8 x, uint8 y)
{

	char buf[16]={0,};


	if(sif->upFlow || sif->downFlow){
			LCD_OverLoad(x,y);
			return;
	}
	format_weight((char*)buf,16,sif->div_weight,1,4);

	LCD_P16x32Str(x,y,buf);


}
void gui_show_button(uint8 show,uint8 x, uint8 y)
{
		if(show)
				LCD_P16x16bmp(x,y,5); //按钮被按下
		else
				LCD_P16x16bmp(x,y,4); //按钮被按下
}