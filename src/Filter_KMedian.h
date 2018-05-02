#ifndef	FILTER_KMEDIAN_H
#define	FILTER_KMEDIAN_H

#include "common.h"

#define	FKM_InnerGradeMax		(6u)		// 0/1/2/3/4

extern	void FKM_InitFilterList(int32_t newData);
extern	void FKM_SetFilterGrade(uint32_t grade, uint32_t mini, uint32_t edge);
extern	int32_t FKM_Filter(int32_t newData);
extern	Std_ReturnType IsFilterDataStable(void);
extern	int32_t creep_ad_deal(int32_t inAD ,uint32_t traceflag);
extern	void creep_init(int trace_value);
#endif	// FILTER_KMEDIAN_H

