#include "gui.h"
#include <stdlib.h>
#include "oled.h"





void format_weight(char* buffer, int size, float value, uint8 dot)
{
		memset(buffer,0,size);
		switch(dot)
			{

					case 1:
							snprintf(buffer,64,"%0.1f",(float)value/10.0f);
							break;
					case 2:
							snprintf(buffer,64,"%0.2f",(float)value/100.0f);
							break;
					case 3:
							snprintf(buffer,64,"%0.3f",(float)value/1000.0f);
							break;
					case 4:
							snprintf(buffer,64,"%0.4f",(float)value/10000.0f);
							break;
					default:
							snprintf(buffer,64,"%0.1f",(float)value/10.0f);
							break;
			}
}
void gui_show_history_weight(float* wf,uint8 num, uint8 dot)
{
	  uint8 i = 0;

		char buf[64] = {0,};
		format_weight(buf,64,wf[0],dot);	
		LCD_P8x16Str(10,0,buf);
		format_weight(buf,64,wf[1],dot);	
		LCD_P8x16Str(10,2,buf);
		format_weight(buf,64,wf[2],dot);	
		LCD_P8x16Str(10,4,buf);
				

	 LCD_P16x16Ch(80,0,32); 
	LCD_P16x16Ch(80,2,33);
		LCD_P16x16Ch(80,4,34);
		
}

