#ifndef WEIGHT_H
#define WEIGHT_H

#include "common.h"
#include "state.h"
#include "channel.h"
#include "types.h"

typedef struct 
{
	INT32U			stableAD:1;		//AD??
	INT32U			stable:1;		//??
	INT32U			zero:1;			//??
	INT32U			overADC:1;		//ADC??
	INT32U			overGross:1;	//????
	INT32U			overNET:1;		//????
	INT32U			underGross:1;	//????
	INT32U			dnew:1;			//???
	INT32U			err:1;			//??
	INT32U			busy:1;			// ?
	INT32U			wetready:1;		//??????
	INT32U			range:1;		//????,0=range1,1=range2
	INT32U			aztflg:1;		//?????,1=activated 
	
}WET_STATE;

typedef struct 
{
	INT32S	mWet_Stable;	//?????????
	INT32S	mWet_newest;	//?????
	INT32S	mWet_gross;		//?????
	INT32S	mWet_net;		//????
}WET_DATA;

typedef struct 
{
	INT32U			errpara:1;				// ????????
	INT32U			errAD:1;				// ADC?????
	INT32U			errWorkTask:1;			// ???????????
	INT32U			errStableTask:1;		// ???????????
	INT32U			errZeroTrackTask:1;		// ???????????

	INT32U			err_poweron_set_zero:1;
	INT32U			err_set_zero:1;
	INT32U			err_ad_range:1;
	INT32U			err_wet_stable:1;
	INT32U			err_com:1;
	INT32U			err_set_tare:1;
	INT32U			err_adjustment:1;
	INT32U			err_printf:1;
	INT32U			err_battery_low:1;	
	
}WET_ERR;

typedef union 
{
	WET_ERR 	state;
	INT32U 		value; 
}uWET_ERR;

FP32 Wet_MultiRange_Dela(INT32S inWet);

#endif
