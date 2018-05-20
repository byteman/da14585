#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include "common.h"

void format_weight(char* buffer, int size, INT32S value, uint8 dot,uint8 max);
void format_total(char* buffer, int size, INT32S value, uint8 dot,uint8 max);
#endif

