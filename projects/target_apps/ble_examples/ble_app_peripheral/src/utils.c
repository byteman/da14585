#include "utils.h"
#include <stdlib.h>

void util_delay(unsigned int ms)
{                         
	unsigned int a;
	while(ms)
	{
	a=1800;
	while(a--);
	ms--;
	}
	return;
}
//固定位数为4位，包括小数点.
void format_weight(char* buffer, int size, INT32S value, uint8 dot,uint8 max)
{
		memset(buffer,0,size);
		dot++;
		max++;
		switch(dot)
			{

					case 1:
							snprintf(buffer,size,"%4.1f",(float)value/10.0f);
							break;
					case 2:
							snprintf(buffer,size,"%5.2f",(float)value/100.0f);
							break;
					case 3:
							snprintf(buffer,size,"%4.3f",(float)value/1000.0f);
							break;
					case 4:
							snprintf(buffer,size,"%4.4f",(float)value/10000.0f);
							break;
					default:
							snprintf(buffer,size,"%4.1f",value);
							break;
			}
		
			buffer[max] = 0;
}

void format_total(char* buffer, int size, INT32S value, uint8 dot,uint8 max)
{
		memset(buffer,0,size);
		dot++;
		max++;
	
		switch(dot)
			{

					case 1:
							snprintf(buffer,size,"%7.1f",(float)value/10.0f);
							break;
					case 2:
							snprintf(buffer,size,"%8.2f",(float)value/100.0f);
							break;
					case 3:
							snprintf(buffer,size,"%7.3f",(float)value/1000.0f);
							break;
					case 4:
							snprintf(buffer,size,"%7.4f",(float)value/10000.0f);
							break;
					default:
							snprintf(buffer,size,"%7f",value);
							break;
			}
		
			buffer[max] = 0;
}
#include "app_easy_timer.h"
void DelayToDo(const uint32 ms, DoFunc func)
{
	app_easy_timer(ms,func);
}