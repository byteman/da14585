#ifndef COMMON_H
#define COMMON_H

#include "Std_Types.h"

typedef unsigned char uint8_t;
typedef signed char   int8_t;
typedef float   float32;
typedef signed int int32_t;
typedef unsigned int uint32_t;
//typedef int Std_ReturnType;
#define NULL 0

typedef enum{
	MSG_BLE_STATE = 1,
}MSG_ID;

typedef enum{
	TARGET_MAINMENU = 1,
}TARGET_ID;

#endif
