/**
 ****************************************************************************************
 *
 * @file timer0.c
 *
 * @brief Timer0 driver source file.
 *
 * Copyright (C) 2016-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#include <stdint.h>
#include "datasheet.h"
#include "user_periph_setup.h"
#include "timer0.h"

timer0_handler_function_t* TIMER0_callback;

volatile struct __TIMER0_CTRL_REG timer0_ctrl_reg __attribute__((at(TIMER0_CTRL_REG)));

/**
 ****************************************************************************************
 * @brief  Initializes TIMER0
 *
 * @param[in] tim0_clk_sel: Timer0 uses 16MHz (fast) or 32kHz (slow) clock frequency
 * @param[in]     pwm_mode: (during high time) '1' or (fast) clock divided by 2
 * @param[in] tim0_clk_div: Division factor for TIMER0 "on" time in peripheral divider register
 *
 * @return
 ****************************************************************************************
 */
void timer0_init(TIM0_CLK_SEL_t tim0_clk_sel, PWM_MODE_t pwm_mode, TIM0_CLK_DIV_t tim0_clk_div)
{
    timer0_ctrl_reg.BITFLD_PWM_MODE = pwm_mode;
    timer0_ctrl_reg.BITFLD_TIM0_CLK_DIV = tim0_clk_div; // NOTE: divider option is used only for the "ON" counter
    timer0_ctrl_reg.BITFLD_TIM0_CLK_SEL = tim0_clk_sel;
    NVIC_SetPriority (SWTIM_IRQn, 3);                 /* set Priority for TIM0 Interrupt to be the lowest */
}


/**
 ****************************************************************************************
 * @brief  starts TIMER0
 ****************************************************************************************
 */
inline void timer0_start(void)
{
    timer0_ctrl_reg.BITFLD_TIM0_CTRL = TIM0_CTRL_RUNNING;       // Timer0 is running
}


/**
 ****************************************************************************************
 * @brief  stops TIMER0
 ****************************************************************************************
 */
inline void timer0_stop(void)
{
    timer0_ctrl_reg.BITFLD_TIM0_CTRL = TIM0_CTRL_OFF_RESET;     // Timer0 is off and in reset state
}


/**
 ****************************************************************************************
 * @brief  Releases TIMER0. Does not disable the TIM clock, as it is shared with TIMER2
 *
 * @param[in]  timer2_duty_cycle: PWM2 duty cycle
 ****************************************************************************************
 */
inline void timer0_release(void)
{
    timer0_stop();
}


/**
 ****************************************************************************************
 * @brief  configures TIMER0 ON counter reload value
 * @param[in]    pwm_on: ON COUNTER reload value
 ****************************************************************************************
 */
inline void timer0_set_pwm_on_counter(uint16_t pwm_on)
{
    SetWord16(TIMER0_ON_REG, pwm_on);
}


/**
 ****************************************************************************************
 * @brief  configures TIMER0 M counter reload value (duration of PWM0 high part)
 * @param[in]   pwm_high: M COUNTER reload value
 ****************************************************************************************
 */
inline void timer0_set_pwm_high_counter(uint16_t pwm_high)
{
    SetWord16(TIMER0_RELOAD_M_REG, pwm_high);
}


/**
 ****************************************************************************************
 * @brief  configures TIMER0 N counter reload value (duration of PWM0 low part)
 * @param[in]   pwm_low: N COUNTER reload value
 ****************************************************************************************
 */
inline void timer0_set_pwm_low_counter(uint16_t pwm_low)
{
    SetWord16(TIMER0_RELOAD_N_REG, pwm_low);
}


/**
 ****************************************************************************************
 * @brief  configures TIMER0 pwm timing parameters
 * @param[in]    pwm_on: ON COUNTER reload value
 * @param[in]  pwm_high: M COUNTER reload value
 * @param[in]   pwm_low: N COUNTER reload value
 ****************************************************************************************
 */
void timer0_set(uint16_t pwm_on, uint16_t pwm_high, uint16_t pwm_low)
{
    SetWord16(TIMER0_ON_REG, pwm_on);
    SetWord16(TIMER0_RELOAD_M_REG, pwm_high);
    SetWord16(TIMER0_RELOAD_N_REG, pwm_low);
}


/**
 ****************************************************************************************
 * @brief  enables SWTIM_IRQn
 ****************************************************************************************
 */
void timer0_enable_irq(void)
{
    NVIC_EnableIRQ(SWTIM_IRQn);
}


/**
 ****************************************************************************************
 * @brief  disables SWTIM_IRQn
 ****************************************************************************************
 */
void timer0_disable_irq(void)
{
    NVIC_DisableIRQ(SWTIM_IRQn);
}


/**
 ****************************************************************************************
 * @brief  registers a callback function to be called from within SWTIM_Handler
 *          when SWTIM_IRQn has triggered
 ****************************************************************************************
 */
inline void timer0_register_callback(timer0_handler_function_t* callback)
{
    TIMER0_callback = callback;
}


/**
 ****************************************************************************************
 * @brief SWTIM_Handler. The SWTIM_IRQn interrupt handler
 ****************************************************************************************
 */
void SWTIM_Handler(void)
{
    if (TIMER0_callback != NULL)
        TIMER0_callback();
}
