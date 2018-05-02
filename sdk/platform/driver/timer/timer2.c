/**
 ****************************************************************************************
 *
 * @file timer2.c
 *
 * @brief Timer2 driver source file.
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
#include "timer2.h"

volatile struct __TRIPLE_PWM_CTRL_REG triple_pwm_ctrl_reg __attribute__((at(TRIPLE_PWM_CTRL_REG)));

/**
 ****************************************************************************************
 * @brief  Enables/Disables TIMER2
 * @param[in]  triple_pwm_enable:  TRIPLE_PWM_DISABLED or TRIPLE_PWM_ENABLED
 ****************************************************************************************
 */
inline void timer2_enable(TRIPLE_PWM_ENABLE_t triple_pwm_enable)
{
    triple_pwm_ctrl_reg.BITFLD_TRIPLE_PWM_ENABLE = triple_pwm_enable;
}


/**
 ****************************************************************************************
 * @brief  Enables/Disables TIMER2 pause by hw
 * @param[in]  hw_pause_en:  HW_CAN_NOT_PAUSE_PWM_2_3_4 or HW_CAN_PAUSE_PWM_2_3_4
 ****************************************************************************************
 */
inline void timer2_set_hw_pause(TRIPLE_PWM_HW_PAUSE_EN_t hw_pause_en)
{
    triple_pwm_ctrl_reg.BITFLD_HW_PAUSE_EN = hw_pause_en;
}


/**
 ****************************************************************************************
 * @brief  Pauses by sw / releases sw pause for TIMER2
 * @param[in]  sw_pause_en:  PWM_2_3_4_SW_PAUSE_DISABLED or PWM_2_3_4_SW_PAUSE_ENABLED
 ****************************************************************************************
 */
inline void timer2_set_sw_pause(TRIPLE_PWM_SW_PAUSE_EN_t sw_pause_en)
{
    triple_pwm_ctrl_reg.BITFLD_SW_PAUSE_EN = sw_pause_en;
}


/**
 ****************************************************************************************
 * @brief  Sets TIMER2 frequency
 * @param[in]  triple_pwm_frequency:  TIMER2 frequency
 ****************************************************************************************
 */
inline void timer2_set_pwm_frequency(uint16_t triple_pwm_frequency)
{
    SetWord16(TRIPLE_PWM_FREQUENCY, triple_pwm_frequency);
}


/**
 ****************************************************************************************
 * @brief  Initializes TIMER2
 *
 * @param[in]           hw_pause_en: HW_CAN_NOT_PAUSE_PWM_2_3_4 or HW_CAN_PAUSE_PWM_2_3_4
 * @param[in]           sw_pause_en: PWM_2_3_4_SW_PAUSE_DISABLED or PWM_2_3_4_SW_PAUSE_ENABLED
 * @param[in]  triple_pwm_frequency: TIMER2 frequency
 ****************************************************************************************
 */
 void timer2_init(TRIPLE_PWM_HW_PAUSE_EN_t hw_pause_en, TRIPLE_PWM_SW_PAUSE_EN_t sw_pause_en, uint16_t triple_pwm_frequency)
{
    timer2_enable(TRIPLE_PWM_ENABLED);
    timer2_set_hw_pause(hw_pause_en);
    timer2_set_sw_pause(sw_pause_en);
    timer2_set_pwm_frequency(triple_pwm_frequency);
}


/**
 ****************************************************************************************
 * @brief  Stops timer2. Does not disable the TIM clock, as it is shared with TIMER0
 *
 * @param[in]  timer2_duty_cycle: PWM2 duty cycle
 ****************************************************************************************
 */
inline void timer2_stop(void)
{
    timer2_enable(TRIPLE_PWM_DISABLED);
}


/**
 ****************************************************************************************
 * @brief  Sets PWM2 duty cycle
 *
 * @param[in]  timer2_duty_cycle: PWM2 duty cycle
 ****************************************************************************************
 */
inline void timer2_set_pwm2_duty_cycle(uint16_t pwm2_duty_cycle)
{
    SetWord16(PWM2_DUTY_CYCLE, pwm2_duty_cycle);
}


/**
 ****************************************************************************************
 * @brief  Sets PWM3 duty cycle
 *
 * @param[in]  timer3_duty_cycle: PWM3 duty cycle
 ****************************************************************************************
 */
inline void timer2_set_pwm3_duty_cycle(uint16_t pwm3_duty_cycle)
{
    SetWord16(PWM3_DUTY_CYCLE, pwm3_duty_cycle);
}


/**
 ****************************************************************************************
 * @brief  Sets PWM4 duty cycle
 *
 * @param[in]  timer3_duty_cycle: PWM4 duty cycle
 ****************************************************************************************
 */
inline void timer2_set_pwm4_duty_cycle(uint16_t pwm4_duty_cycle)
{
    SetWord16(PWM4_DUTY_CYCLE, pwm4_duty_cycle);
}
