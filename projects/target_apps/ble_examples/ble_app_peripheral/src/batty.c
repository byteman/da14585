#include "batty.h"
#include "battery.h"
#include <stdlib.h>

void  battery_isr()
{

}
uint8  battery_get()
{
	battery_get_lvl(BATT_CR2032);
}

