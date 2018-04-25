/*******************************************************************************
  * @file    Filter_KMedian.c 
  * @author  yangxd
  * @version V 0.01
  * @date    2015.09.09
  * @brief   中值平均滤波
  ******************************************************************************
  * @attention
  *
  *
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "Filter_KMedian.h"
#include "math.h"
/* Private macro -------------------------------------------------------------*/
#define	FKM_ListSizeMin			(4u)
#define	FKM_ListSizeDelta		(2u)
#define	FKM_GradeMax			FKM_InnerGradeMax
#define	FKM_ListSizeMax			(FKM_ListSizeMin+FKM_ListSizeDelta*FKM_GradeMax)
#define	FKM_ListSizeMaxLimit	(30u)

#if		!(FKM_ListSizeMax<=FKM_ListSizeMaxLimit)
#error	"Invalid FKM_ListSizeMax"
#endif

#define	HYS_RANGE_COUNTER_MAX	(50)
/* Private variables ---------------------------------------------------------*/
static int32_t g_ADValueList[FKM_ListSizeMax] = { 0, };
static uint32_t g_ADListSize = FKM_ListSizeMax;
static uint32_t g_index;

static int32_t FilterMini;
static int32_t FilterMini_2;		// FilterMini / 2
static int32_t FilterMini_4;		// FilterMini / 4
static int32_t FilterMini_5;		// FilterMini / 5
static int32_t Filter_Crp_Value ;

static int32_t FilterEdgeThresValue;

//static int32_t FilterAveResult;
static int32_t FilterHysResult;

static Std_ReturnType FilterDataStableFlag;
/* Private functions ---------------------------------------------------------*/

static unsigned char Filter_Mini_4_flag = 0;

/******************************************************************************
  * @brief	找数组序列中指定位置元素  
  * @param  
  * @retval 
******************************************************************************/
static	int32_t FKM_SortDate(int32_t a[],uint32_t n,uint32_t k)
{
    uint32_t i,j;
    int32_t x;

	for(i=0;i<n-1;i++)
	{
		for(j=i+1;j<n;j++)
		{
			if(a[i]>a[j])
			{
				x=a[i];
				a[i]=a[j];
				a[j]=x;
			}
		}
	}
	return a[k];
}
////////////////////////////////////////////////////////////////////////////////
// interface
/******************************************************************************
  * @brief  
  * @param  
  * @retval 
******************************************************************************/
void FKM_InitFilterList(int32_t newData)
{
    uint32_t i;

	g_index = 0;
	for(i=0; i<g_ADListSize; i++)
		g_ADValueList[i] = newData;	

	FKM_Filter(newData);				
}
/******************************************************************************
  * @brief  
  * @param  
  * @retval 
******************************************************************************/
void FKM_SetFilterGrade(uint32_t grade, uint32_t mini, uint32_t edge)
{
    uint32_t tmp;

    g_ADListSize = (uint32_t)((uint32_t)FKM_ListSizeMin+(uint32_t)FKM_ListSizeDelta*grade);

	if(grade > FKM_GradeMax)
		grade = FKM_GradeMax;

	FilterMini = mini;
	FilterMini_2 = mini >> 1;
	FilterMini_4 = mini >> 2;
	FilterMini_5 = mini/5;

	tmp = mini * grade * g_ADListSize;
	if(edge < tmp)edge = tmp;
    FilterEdgeThresValue = edge;
}
/******************************************************************************
  * @brief  
  * @param  
  * @retval 
******************************************************************************/	

int32_t FKM_Filter(int32_t newData)
{
    int32_t	tempArray[FKM_ListSizeMax];
    int32_t	median;
    int32_t	FilterResult;
    int32_t FilCmpLow ;
    int32_t FilCmpHigh ;

    int32_t	sum;		//!Thomas
    int32_t i, count;
    static	int32_t	OldFilteredValue,iNewComer=0;
    static	uint32_t hysRangeCounter = 0;

	FilterDataStableFlag = 0;

	g_ADValueList[g_index++] = newData;
	if(g_index >= g_ADListSize)
		g_index = 0;

	if((newData <= OldFilteredValue - FilterEdgeThresValue)||(newData >= OldFilteredValue + FilterEdgeThresValue))
	{
		OldFilteredValue = newData;
		iNewComer=1;
		FilterDataStableFlag = 0;
		FilterHysResult = newData;
		return FilterHysResult;
	}

	if ((iNewComer>=1) && ( iNewComer<=(g_ADListSize>>1)))
	{
		iNewComer++;
		OldFilteredValue=(OldFilteredValue+newData)>>1;
		FilterHysResult = OldFilteredValue;
		return FilterHysResult;
	}
   // iNewComer=0; memcpy
	memcpy(tempArray, g_ADValueList, g_ADListSize*sizeof(*g_ADValueList));
	median = FKM_SortDate(tempArray, g_ADListSize, (g_ADListSize>>1));

	for(sum=0, count=0, i=0; i<g_ADListSize; i++)
	{
		FilCmpHigh = g_ADValueList[i] + FilterMini;
		FilCmpLow = g_ADValueList[i] - FilterMini;
		if((median <= FilCmpHigh)&&(median >= FilCmpLow))
		{
			count++;
			sum+= g_ADValueList[i];
		}
	}
	if(count == 0)
		count = 1;
	FilterResult = sum/count;
	Filter_Mini_4_flag  = 0;
	if((FilterResult <= (OldFilteredValue + FilterMini_4))&&(FilterResult >= (OldFilteredValue - FilterMini_4)))
	{
		Filter_Mini_4_flag  = 1;
		FilterDataStableFlag = 1;
		if(++hysRangeCounter>=HYS_RANGE_COUNTER_MAX)
		{
			hysRangeCounter = 0;
			OldFilteredValue= FilterResult;
			FilterHysResult = FilterResult;
			return FilterHysResult;
		}
		else
		{
			FilterResult=OldFilteredValue ;
			//OldFilteredValue= FilterResult;
			FilterHysResult = FilterResult;
			return FilterHysResult;
		}
	}
	else if((FilterResult <= (OldFilteredValue+FilterMini_2))&&(FilterResult >= (OldFilteredValue - FilterMini_2)))
	{
		FilterDataStableFlag = 1;		
		FilterResult=OldFilteredValue;	
		//OldFilteredValue= FilterResult;
	}
	else
	{
		FilterDataStableFlag = 0;
		OldFilteredValue= FilterResult;		
	}
	hysRangeCounter = 0;
	
	FilterHysResult = FilterResult;
	return FilterHysResult;
}
/******************************************************************************
  * @brief  
  * @param  
  * @retval 
******************************************************************************/
Std_ReturnType IsFilterDataStable(void)
{
	
	return FilterDataStableFlag;
}



/////////////////////////////////////////////////

float ZeroTrackAver;

#define STATIC_SAMPLE_HZ 10
#define	MCREEPFOLLOWTIME	1   //s?
#define	MCREEPFOLLOWDIV	3   //s?

//载荷下跟踪
int32_t  static_creep_zero_track(int32_t ADcode)
{
        uint32_t		i =0 ;
    static uint32_t  		mCmpTmr = 0;
	
    static uint32_t	data_point=0;
    static int32_t		data_buf[MCREEPFOLLOWTIME * STATIC_SAMPLE_HZ]= {0,};
    static int32_t		data_sum = 0;
    static int32_t 		Filter_Crp_Cmp_Value = 0;
    static int32_t		beforADaver = 0;
    static int32_t		diff_adaver = 0;

	switch(Filter_Crp_Value)
		{
		case 0:
			Filter_Crp_Cmp_Value = 0;
			break;
		case 1:
			Filter_Crp_Cmp_Value = FilterMini_5;
			break;
		case 2:
			Filter_Crp_Cmp_Value = FilterMini_4;
			break;
		case 3:
			Filter_Crp_Cmp_Value = FilterMini_2;
			break;
		case 4:
			Filter_Crp_Cmp_Value = FilterMini;
			break;
		default:
			Filter_Crp_Cmp_Value = FilterMini_2;
			break;
	}
	data_point++;
	if(data_point >= (MCREEPFOLLOWTIME * STATIC_SAMPLE_HZ))
		data_point=0;
	data_buf[data_point]= ADcode;

	mCmpTmr++;
	if(mCmpTmr < MCREEPFOLLOWTIME * STATIC_SAMPLE_HZ)
	{
	    return ZeroTrackAver ;
	}
	mCmpTmr = 0;
	data_sum = 0;
	for(i=0;i<(MCREEPFOLLOWTIME * STATIC_SAMPLE_HZ);i++)
	{
	  data_sum += data_buf[i];
	}
	data_sum = data_sum/(MCREEPFOLLOWTIME * STATIC_SAMPLE_HZ);
	
	diff_adaver =  ( data_sum - beforADaver );

	if( fabs(diff_adaver) <  Filter_Crp_Cmp_Value)	//FilterMini_2
	{
			ZeroTrackAver += diff_adaver;  //保存需要跟踪的值	
	}
	
	beforADaver = data_sum;

	return ZeroTrackAver ;
}


////////////////////////////////////
/*
相当于右移动了5次
*/
int32_t creep_ad_deal(int32_t inAD ,uint32_t traceflag)
{
    static int32_t mTraceWet = 0.0;
    static int32_t mAFTraceWet = 0.0;

	if(traceflag == 0)
		mTraceWet = 0;
	else 
		mTraceWet = static_creep_zero_track(inAD); //
		
	mAFTraceWet = inAD - mTraceWet ;
	
	return mAFTraceWet;
}


void creep_init(int trace_value)
{
	Filter_Crp_Value = trace_value;
}

/*
// function:	partition array A[0..N-1] into two part.
//				A[0..PivotIndex-1] are no bigger than pivot value and
//				A[PivotIndex+1..N-1] are no smaller than pivot value.
//				A[N/2] is the pivot.
// A:			start address of array A
// N:			element number of array A
// return:		index of pivot
static uint32_t partition(int32_t *A, uint32_t N)
{
    int32_t *p, *q;
    int32_t pivot;

	assert(A);
	assert(N>0);

	if(N==1) return 0;

	p = A;
	q = A+N-1;
	pivot = A[N>>1];
	A[N>>1] = A[0];

	while(p<q)
	{
		while(p<q&&*q>=pivot) --q;
		if(p<q) *p++ = *q;
		while(p<q&&*p<=pivot) ++p;
		if(p<q) *q-- = *p;
	}

	*p = pivot;

    return (uint32_t)(p-A);
}

// function:	find the Kth element of array A
// A:			start address of array A
// N:			element number of array A
// K:			ordinal number (counted from 0)
// return:		value of the Kth element
static int32_t FindKMedian(int32_t *A, uint32_t N, uint32_t K)
{
    int32_t pivot;
    uint32_t index;

	assert(A);
	assert(N>0);
	assert(K<N);

	while(1)
	{
		index = partition(A, N);
		pivot = A[index];
		if(index==K)
		{
			return pivot;
		}
		else if(index>K)
		{
			while(index>K&&A[index]==pivot)
				--index;

			if(A[index]==pivot)
			{
				return pivot;
			}
			else
			{
				N = index + 1;
			}
		}
		else // index<K
		{
			while(index<K&&A[index]==pivot)
				++index;

			if(A[index]==pivot)
			{
				return pivot;
			}
			else
			{
				A += index;
				N -= index;
				K -= index;
			}
		}
	}
}*/

