/**
  ************************************************************************* 
 *	Filename :	Print.C
 *
 *	Version	  Author	QQ			Date		Description
 *	V1.0	 	 dtcoff      364283622	  2017.01.16	New
 *
 *	Scope :		 
 *
 *	Function(s)	
 *
 *	COPYRIGHT (C) 2017.	All Rights Reserved. WY
 *
 **************************************************************************
 *                                   Update
 * @Version : V1.0.1
 * @By      : dtcoff      
 * @Date    : 2017 / 7 / 20
 * @Brief   : 
 *
 **************************************************************************
 **/

#include "Common.h" 


#define MIN_STEP_VAL		1 //0.0001

/*
0.0001, 	// 0
0.0002, 	// 1
0.0005, 	// 2
0.001,		// 3
0.002,		// 4
0.005,		// 5
0.01,		// 6
0.02,		// 7
0.05,		// 8
0.1,		// 9
0.2,		// 10
0.5,		// 11

*/

float incrTBL[18] =		
{

	1,		// 12
	2,		// 13
	5,		// 14
	10,		// 15
	20,		// 16
	50,		// 17
};

/*
    UNIT_KG = 0,
    UNIT_G,    
    UNIT_LB,
    UNIT_OZ,
    UNIT_TJ,
    UNIT_HJ,
//    P,
    UNIT_PERC,  
    UNIT_TOTAL_NUM

*/
float gramsPerUnit[] =
{
	1.0,			// kg
	1000, //1000.0,			// GRAMS PER KILOGRAM, g
	2.20462262, 		// GRAMS PER POUND, lb
	35.2739619,		// GRAMS PER OUNCE, oz
	0.9,
};

/*
	不同分度值对应的切换分度值
*/
static Unit2Unit_T gUnitConvInc[] = 
{
//	kg		g		lb		oz		tj
	{1,		1,		2,		50,		1},
	{2,		2,		5,		50,		2},
	{5,		5,		10,		200,		5},
	{10,	10,		20,		200,		10},
	{20,	20,		50,		500,		20},
};
static UnitMgrInfo_t mUnitMgrInfo;	

float MT_fabs( float value )
//----------------------------------------------------------------------------
{
    	if ( value < 0 ) return -value;
    	return value;
}

float ConvertFromUnitToUnit(tUnitSymbol srcUnit, tUnitSymbol destUnit, const float *weightPtr)
{
	static float tmflt = 0.0;
	if (srcUnit != destUnit)
	{
		tmflt = *weightPtr * gramsPerUnit[srcUnit];
		tmflt = tmflt / gramsPerUnit[destUnit];
		return tmflt;
	}
	else
		return *weightPtr;
}

float RoundToNearest1_2_5(float inc, unsigned char switchUnit)
{
	float new_inc[4];
	float diff[4];
	float diff_8, diff_4;
	unsigned char i, j;
	
	if (inc == 0.0)
		new_inc[0] = 0.0;
	else
	{
		new_inc[0] = 1000.0;
		while (new_inc[0] > inc)
			new_inc[0] *= 0.1;
	}

	new_inc[1] = new_inc[0] * 2.0;
	new_inc[2] = new_inc[0] * 5.0;
	new_inc[3] = new_inc[0] * 10.0;

	diff[0] = MT_fabs(new_inc[0] - inc);
	diff[1] = MT_fabs(new_inc[1] - inc);
	diff[2] = MT_fabs(new_inc[2] - inc);
	diff[3] = MT_fabs(new_inc[3] - inc);

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
			if (diff[i] > diff[j])
				break;
		
		if (j == 4)
			break;
	}
// #if 0
// handle 4oz, 8oz, and 6ozt increments. dpb 06-apr-95

	switch ( switchUnit ) 
	{
								// for Unit oz
		case 4:
			diff_8 = MT_fabs( (float) 8.0 - inc);
			diff_4 = MT_fabs( (float) 4.0 - inc);
			if ((diff_8 <= diff[i]) && (diff_8 <= diff_4))
				return (float) 8.0;
			if ((diff_4 <= diff[i]) && (diff_4 <= diff_8))
				return (float) 4.0;
			break;
/*								// for Unit ozt
		case 9:
			diff_6 = fabs ( (float) 6.0 - inc);
			if ( diff_6 <= diff[i] )
				return (float) 6.0;
			break;
*/
		default:
			break;

	}  // switch
// #endif
	return new_inc[i];
}

signed char GetIncrIndex(float incr)
{
    signed char i, start, end;
    // float x1, x2;
    float result;
    
    start = 0;
    end = MAX_INCR_NUM - 1;
    
    while (end >= start)
    {
    	i = (start + end) >> 1;
    	result = incr - incrTBL[i];
    	
    	if (result > MIN_STEP_VAL)
    		start = i + 1;
    	else if (result < -MIN_STEP_VAL)
    		end = i - 1;
    	else
    		return i;
    }
    
    return -1;
}




////////////////////////////////////////////////////////////

/*
	开关单位
*/
static INT32U Unit_SetUnitBitOp(INT32U *bitGroup, INT8U opMode, INT32U maskSet)
{
	switch(opMode)
	{
	case UM_OPMODE_EN:		*bitGroup |= maskSet;	break;
	case UM_OPMODE_DIS:		*bitGroup &= ~maskSet;	break;
	case UM_OPMODE_REVE:	*bitGroup ^= maskSet;	break;
	default:				 
		break;
	}

	return *bitGroup;
}
void	Unit_SetUnitEn(INT8U opMode, INT8U unitID)
{
	Unit_SetUnitBitOp(&mUnitMgrInfo.mAvaiMaskFact,opMode,UNITM(unitID));
	
}

INT8U Unit_IsUnitAvailable(INT8U unitID)
{
	INT32U  avaiMask;

	if(unitID >= UNIT_TOTAL_NUM)return FALSE;

	avaiMask = mUnitMgrInfo.mAvaiMaskFact;
	return (avaiMask & UNITM(unitID))?(TRUE):(FALSE);
}

INT8U Unit_SetCurUnit(INT8U unitID)
{
	if(unitID<UNIT_TOTAL_NUM)
	{
		mUnitMgrInfo.mCurUIDIdx = unitID;
	}
	return mUnitMgrInfo.mCurUIDIdx;
}

//==============================================================================

INT8U Unit_GetUnitID(void)
{
	return mUnitMgrInfo.mCurUIDIdx;
}


/*
	按单位切换数值
	type:2 ---- kg to another
	type:1 ---- another to kg
	
*/
float Unit_Convert_UnitData(float srcData,INT8U type)
{
	static float mTempf = 0.0;
	switch(type)
	{
		case 1:
			mTempf = ConvertFromUnitToUnit(UNIT_KG, mUnitMgrInfo.mCurUIDIdx, &srcData);
			break;
		case 2:
			mTempf = ConvertFromUnitToUnit(mUnitMgrInfo.mCurUIDIdx,UNIT_KG,  &srcData);
			break;
		case 3:
			break;
		case 4:
			break;
	}
	
	return mTempf;
}
void Unit_Set_BasePt(INT8U pt )
{
	mUnitMgrInfo.mBasePtNum = pt;
}
INT8U Unit_Get_ConvertPt(void)
{
	return mUnitMgrInfo.mCvtPtNum;
}
INT8U Unit_Get_DivCnt(void)
{
	return mUnitMgrInfo.mBasePtNum - mUnitMgrInfo.mCvtPtNum;
}
INT8U Unit_Convert_Pnt( INT8U type)
{
	INT8U	mCvtNum = 0 ,mBaseNum = 0;

	if(mUnitMgrInfo.mCvtIncValue <10)
		mCvtNum = 1;
	else if(mUnitMgrInfo.mCvtIncValue <100)
		mCvtNum = 2;
	else if(mUnitMgrInfo.mCvtIncValue <1000)
		mCvtNum = 3;

	if(mUnitMgrInfo.mCurUIDIdx == UNIT_G)
		mCvtNum = 4;
	if(mUnitMgrInfo.mBaseIncValue <10)
		mBaseNum = 1;
	else if(mUnitMgrInfo.mBaseIncValue <100)
		mBaseNum = 2;
	else if(mUnitMgrInfo.mBaseIncValue <1000)
		mBaseNum = 3;

	mCvtNum = mCvtNum - mBaseNum;
	if(mUnitMgrInfo.mBasePtNum > mCvtNum)
		mUnitMgrInfo.mCvtPtNum = mUnitMgrInfo.mBasePtNum - mCvtNum;
	else
		mUnitMgrInfo.mCvtPtNum = 0;

}

/*
	返回转换后的分度值
*/
INT8U	Unit_Get_ConvertIncr(void )
{
	return mUnitMgrInfo.mCvtIncValue; 
}


/*
	按单位切换数值
	type:1 ----inc cov kg to another
	type:2----- inc another to kg	
	
*/
INT8U Unit_Convert_Incr( INT8U type)
{
	INT8U i = 0;
	INT8U mNumb = ARRAY_LEN(gUnitConvInc);
	
	//mUnitMgrInfo.mCvtIncValue = mUnitMgrInfo.mBaseIncValue;
	//return mUnitMgrInfo.mBaseIncValue;
	
	for(i =0;i<mNumb;i++)
	{
		if( mUnitMgrInfo.mBaseIncValue == gUnitConvInc[i].tBaseKg )
		{
			switch(mUnitMgrInfo.mCurUIDIdx)
			{
				case 0:
					mUnitMgrInfo.mCvtIncValue = gUnitConvInc[i].tBaseKg;
					break;
				case 1:
					mUnitMgrInfo.mCvtIncValue = gUnitConvInc[i].tConvG;
					break;
				case 2:
					mUnitMgrInfo.mCvtIncValue = gUnitConvInc[i].tConvlb;
					break;
				case 3:
					mUnitMgrInfo.mCvtIncValue = gUnitConvInc[i].tConvoz;
					break;
				default:
					mUnitMgrInfo.mCvtIncValue = gUnitConvInc[i].tBaseKg;
					break;
			}			
		}
	}
	return mUnitMgrInfo.mCvtIncValue;
}

float Unit_Convert_UnitIncr( INT8U type)
{
	float incr;
	signed char incrIndex;
	float mTempf = 0.0;
	
	switch(type)
	{
		case 1:
			incr = ConvertFromUnitToUnit(UNIT_KG, mUnitMgrInfo.mCurUIDIdx, &incrTBL[mUnitMgrInfo.mCurIncIdx]);
			incr = RoundToNearest1_2_5(incr, mUnitMgrInfo.mCurUIDIdx);
			break;
		case 2:
			incr = ConvertFromUnitToUnit(mUnitMgrInfo.mCurUIDIdx, UNIT_KG, &incrTBL[mUnitMgrInfo.mCurIncIdx]);
			incr = RoundToNearest1_2_5(incr, UNIT_KG);
			break;
	}
	incrIndex = GetIncrIndex(incr);
	if(incrIndex == -1)
		incrIndex = 0;
	mUnitMgrInfo.mCurIncIdx = incrIndex;
	return incr;

}

/*
	进行单位切换
*/
INT8U Unit_SwitchUnit(void)
{
	INT8U  oriUIDGroupIdx;

	oriUIDGroupIdx = mUnitMgrInfo.mUnitIdx;
	while(1)
	{
		mUnitMgrInfo.mUnitIdx++;
		if(mUnitMgrInfo.mUnitIdx >= UNIT_TOTAL_NUM)
		{
			mUnitMgrInfo.mUnitIdx = UNIT_KG;
		}
		if(Unit_IsUnitAvailable(mUnitMgrInfo.mUnitIdx))
		{
			mUnitMgrInfo.mCurUIDIdx = mUnitMgrInfo.mUnitIdx ;
			Unit_Convert_Incr(0);
			Unit_Convert_Pnt(0);
			return TRUE;		
		}
		if(mUnitMgrInfo.mUnitIdx== oriUIDGroupIdx)
			return FALSE;		// To avoid dead loop
	}

}

INT32U Unit_GetUnitMask(void)
{
	return mUnitMgrInfo.mAvaiMaskFact;
}
void Unit_SetUnitMask(INT8U unitmask)
{
	mUnitMgrInfo.mAvaiMaskFact = unitmask;
}

void Unit_SetBaseIncr(INT8U inc)
{
	mUnitMgrInfo.mBaseIncValue = inc;	
}
//////////////////////////////////////

void Unit_Set_DefUnit(INT8U unit)
{
	//mUnitMgrInfo.mRWBCurUnit = unit;
}
void Unit_Set_ValidUnit(INT8U valid)
{
	mUnitMgrInfo.mRWBValidUnit = valid;
}
////////////////////////////////////////
void Unit_Set_UnitVonvert_FAT(INT8U unitid,float ltfat )
{
	gramsPerUnit[unitid] = ltfat;
}
void Unit_Init(INT32U unitmask,INT8U curUnit,INT8U curInc)
{
	if( ( curUnit != UNIT_KG) && ( curUnit != UNIT_LB) && ( curUnit != UNIT_LT) )
	{

		Unit_SetUnitMask(0);

		Unit_SetUnitEn(UM_OPMODE_EN,UNIT_KG);
		Unit_SetUnitEn(UM_OPMODE_EN,UNIT_LT);
		Unit_SetCurUnit(UNIT_KG);

		curUnit = UNIT_KG;
	}  	
	else
		mUnitMgrInfo.mAvaiMaskFact = unitmask;

	Unit_Set_DefUnit(UNIT_KG);
	
	mUnitMgrInfo.mAvaiMaskUser = mUnitMgrInfo.mAvaiMaskFact ;
	mUnitMgrInfo.mCurUIDIdx	 = curUnit;//UNIT_KG;
	mUnitMgrInfo.mUnitIdx =  mUnitMgrInfo.mCurUIDIdx;
	mUnitMgrInfo.mCurIncIdx	 = curInc;//2;

	mUnitMgrInfo.mBaseIncValue = curInc;
	Unit_Convert_Incr(0);
	Unit_Convert_Pnt(0);
}

