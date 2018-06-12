#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include "common.h"
typedef void (*DoFunc)(void);
void format_weight(char* buffer, int size, INT32S value, uint8 dot,uint8 max);
void format_total(char* buffer, int size, INT32S value, uint8 dot,uint8 max);
void util_delay(unsigned int ms);
void DelayToDo(const uint32 ms, DoFunc func);
int  filter_high_ad(int in_ad);
#endif

