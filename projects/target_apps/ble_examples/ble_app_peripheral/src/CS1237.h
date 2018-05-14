
#ifndef __CS1237_H
#define __CS1237_H

#include "types.h"

#include "channel.h"



typedef enum{
	AD_HZ_40= 0,
}AD_HZ;
extern  void CS1237_Init(AD_HZ hz);
extern  int8 CS1237_ReadAD(uint8 chan,int32* ad);
extern  uint8 CS1237_Ready(uint8 chan);
extern  void CS1237_Power(uint8 chan, uint8 pn);

#endif /* __CS1237_H */

