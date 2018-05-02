/**
 ****************************************************************************************
 *
 * @file arch_ram.h
 *
 * @brief System RAM definitions.
 *
 * Copyright (C) 2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef _ARCH_RAM_H_
#define _ARCH_RAM_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdint.h>

/*
 * DEFINES
 ****************************************************************************************
 */

// The base addresses of the 4 RAM blocks
#define RAM_1_BASE_ADDR                 (0x07FC0000)
#define RAM_2_BASE_ADDR                 (0x07FC8000)
#define RAM_3_BASE_ADDR                 (0x07FCC000)
#define RAM_4_BASE_ADDR                 (0x07FD0000)

#if defined (CFG_CUSTOM_SCATTER_FILE)

    #if defined (CFG_RETAIN_RAM_1_BLOCK)
        #define RETAIN_RAM_1_BLOCK      (0x01)
    #else
        #define RETAIN_RAM_1_BLOCK      (0)
    #endif

    #if defined (CFG_RETAIN_RAM_2_BLOCK)
        #define RETAIN_RAM_2_BLOCK      (0x02)
    #else
        #define RETAIN_RAM_2_BLOCK      (0)
    #endif

    #if defined (CFG_RETAIN_RAM_3_BLOCK)
        #define RETAIN_RAM_3_BLOCK      (0x04)
    #else
        #define RETAIN_RAM_3_BLOCK      (0)
    #endif

#else

    #define RETAIN_RAM_1_BLOCK          (0x01)
    #define RETAIN_RAM_2_BLOCK          (0x02)
    #define RETAIN_RAM_3_BLOCK          (0x04)
    
    // Keil ARM linker generated symbols
    extern uint32_t Image$$ER_IROM3$$Base;
    extern uint32_t Image$$ER_IROM3$$Length;
    extern uint32_t Image$$RET_DATA_UNINIT$$Base;

#endif // CFG_CUSTOM_SCATTER_FILE

extern uint32_t heap_mem_area_not_ret$$Base;
extern uint32_t heap_mem_area_not_ret$$Length;

#define RETAIN_RAM_4_BLOCK              (0x08)

#endif // _ARCH_RAM_H_
