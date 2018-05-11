#include "utils.h"
#include <stdlib.h>

void format_weight(char* buffer, int size, INT32S value, uint8 dot)
{
		memset(buffer,0,size);
		switch(dot)
			{

					case 1:
							snprintf(buffer,size,"%0.1f",(float)value/10.0f);
							break;
					case 2:
							snprintf(buffer,size,"%0.2f",(float)value/100.0f);
							break;
					case 3:
							snprintf(buffer,size,"%0.3f",(float)value/1000.0f);
							break;
					case 4:
							snprintf(buffer,size,"%0.4f",(float)value/10000.0f);
							break;
					default:
							snprintf(buffer,size,"%0.1f",value);
							break;
			}
}