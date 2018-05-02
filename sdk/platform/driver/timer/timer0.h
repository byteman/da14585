/**
 ****************************************************************************************
 *
 * @file timer0.h
 *
 * @brief Timer0 driver header file.
 *
 * Copyright (C) 2016-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef _TIMER0_H_
#define _TIMER0_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdint.h>
#include "timer.h"

/*
 * DEFINES
 ****************************************************************************************
 */

typedef enum
{
    PWM_MODE_ONE,
    PWM_MODE_CLOCK_DIV_BY_TWO
} PWM_MODE_t;

typedef enum
{
    TIM0_CLK_DIV_BY_10,
    TIM0_CLK_NO_DIV
} TIM0_CLK_DIV_t;

typedef enum
{
    TIM0_CLK_32K,
    TIM0_CLK_FAST
} TIM0_CLK_SEL_t;

typedef enum
{
    TIM0_CTRL_OFF_RESET,
    TIM0_CTRL_RUNNING
} TIM0_CTRL_t;

typedef void (timer0_handler_function_t)(void);

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief  Initializes TIMER0.
 * @param[in] tim0_clk_sel  Timer0 uses 16MHz (fast) or 32kHz (slow) clock frequency
 * @param[in] pwm_mode      (during high time) '1' or (fast) clock divided by 2
 * @param[in] tim0_clk_div  Division factor for TIMER0 "on" time in peripheral divider register
 * @return void
 ****************************************************************************************
 */
void timer0_init(TIM0_CLK_SEL_t tim0_clk_sel, PWM_MODE_t pwm_mode, TIM0_CLK_DIV_t tim0_clk_div);

/**
 ****************************************************************************************
 * @brief Starts TIMER0
 * @return void
 ****************************************************************************************
 */
void timer0_start(void);

/**
 ****************************************************************************************
 * @brief Stops TIMER0.
 * @return void
 ****************************************************************************************
 */
void timer0_stop(void);

/**
 ****************************************************************************************
 * @brief  Releases TIMER0. Does not disable the TIM clock, as it is shared with TIMER2.
 * @return void
 ****************************************************************************************
 */
void timer0_release(void);

/**
 ****************************************************************************************
 * @brief Configures TIMER0 ON counter reload value.
 * @param[in] pwm_on ON COUNTER reload value
 * @return void
 ****************************************************************************************
 */
void timer0_set_pwm_on_counter(uint16_t pwm_on);

/**
 ****************************************************************************************
 * @brief Configures TIMER0 M counter reload value (duration of PWM0 high part).
 * @param[in] pwm_high M COUNTER reload value
 * @return void
 ****************************************************************************************
 */
void timer0_set_pwm_high_counter(uint16_t pwm_high);

/**
 ****************************************************************************************
 * @brief Configures TIMER0 N counter reload value (duration of PWM0 low part).
 * @param[in] pwm_low N COUNTER reload value
 * @return void
 ****************************************************************************************
 */
void timer0_set_pwm_low_counter(uint16_t pwm_low);

/**
 ****************************************************************************************
 * @brief Configures TIMER0 pwm timing parameters.
 * @param[in] pwm_on   ON COUNTER reload value
 * @param[in] pwm_high M COUNTER reload value
 * @param[in] pwm_low  N COUNTER reload value
 * @return void
 ****************************************************************************************
 */
void timer0_set(uint16_t pwm_on, uint16_t pwm_high, uint16_t pwm_low);

/**
 ****************************************************************************************
 * @brief Enables SWTIM_IRQn.
 * @return void
 ****************************************************************************************
 */
void timer0_enable_irq(void);

/**
 ****************************************************************************************
 * @brief Disables SWTIM_IRQn.
 * @return void
 ****************************************************************************************
 */
void timer0_disable_irq(void);

/**
 ****************************************************************************************
 * @brief Registers a callback function to be called from within SWTIM_Handler
 *        when SWTIM_IRQn has triggered.
 * @param[in] callback Callback function
 * @return void
 ****************************************************************************************
 */
void timer0_register_callback(timer0_handler_function_t* callback);

#endif // _TIMER0_H_
