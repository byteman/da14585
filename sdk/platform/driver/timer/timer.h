/**
 ****************************************************************************************
 *
 * @file timer.h
 *
 * @brief Timer driver header file - common part.
 *
 * Copyright (C) 2016-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef _TIMER_H_
#define _TIMER_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "datasheet.h"

/*
 * DEFINES
 ****************************************************************************************
 */

typedef enum
{
    CLK_PER_REG_TMR_DIV_1,
    CLK_PER_REG_TMR_DIV_2,
    CLK_PER_REG_TMR_DIV_4,
    CLK_PER_REG_TMR_DIV_8
} CLK_PER_REG_TMR_DIV_t;

typedef enum
{
    CLK_PER_REG_TMR_DISABLED,
    CLK_PER_REG_TMR_ENABLED,
} CLK_PER_REG_TMR_ENABLE_t;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief  Enables/Disables TIMER0,TIMER2 clock.
 * @param[in] clk_per_reg_tmr_enable TMR_ENABLE status: CLK_PER_REG_TMR_DISABLED or CLK_PER_REG_TMR_ENABLED
 * @return void
 ****************************************************************************************
 */
__INLINE void set_tmr_enable(CLK_PER_REG_TMR_ENABLE_t clk_per_reg_tmr_enable)
{
    SetBits(CLK_PER_REG, TMR_ENABLE, clk_per_reg_tmr_enable);   // Enable/Disable TIMER0/TIMER2 clock
}

/**
 ****************************************************************************************
 * @brief  Sets TIMER0,TIMER2 clock division factor.
 * @param[in] per_tmr_div TMR_ENABLE status: CLK_PER_REG_TMR_DISABLED or CLK_PER_REG_TMR_ENABLED
 * @return void
 ****************************************************************************************
 */
__INLINE void set_tmr_div(CLK_PER_REG_TMR_DIV_t per_tmr_div)
{
    SetBits(CLK_PER_REG, TMR_DIV, per_tmr_div); // set TIMER0,TIMER2 clock division factor
}

#endif // _TIMER_H_
