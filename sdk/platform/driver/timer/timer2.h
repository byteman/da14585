/**
 ****************************************************************************************
 *
 * @file timer2.h
 *
 * @brief Timer2 driver header file.
 *
 * Copyright (C) 2016-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef _TIMER2_H_
#define _TIMER2_H_

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
    HW_CAN_NOT_PAUSE_PWM_2_3_4,
    HW_CAN_PAUSE_PWM_2_3_4
} TRIPLE_PWM_HW_PAUSE_EN_t;

typedef enum
{
    PWM_2_3_4_SW_PAUSE_DISABLED,
    PWM_2_3_4_SW_PAUSE_ENABLED
} TRIPLE_PWM_SW_PAUSE_EN_t;

typedef enum
{
    TRIPLE_PWM_DISABLED,
    TRIPLE_PWM_ENABLED
} TRIPLE_PWM_ENABLE_t;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Enables/Disables TIMER2.
 * @param[in] triple_pwm_enable TRIPLE_PWM_DISABLED or TRIPLE_PWM_ENABLED
 * @return void
 ****************************************************************************************
 */
void timer2_enable(TRIPLE_PWM_ENABLE_t triple_pwm_enable);

/**
 ****************************************************************************************
 * @brief Enables/Disables TIMER2 pause by hw.
 * @param[in] hw_pause_en HW_CAN_NOT_PAUSE_PWM_2_3_4 or HW_CAN_PAUSE_PWM_2_3_4
 * @return void
 ****************************************************************************************
 */
void timer2_set_hw_pause(TRIPLE_PWM_HW_PAUSE_EN_t hw_pause_en);

/**
 ****************************************************************************************
 * @brief Pauses by sw / releases sw pause for TIMER2.
 * @param[in] sw_pause_en PWM_2_3_4_SW_PAUSE_DISABLED or PWM_2_3_4_SW_PAUSE_ENABLED
 * @return void
 ****************************************************************************************
 */
void timer2_set_sw_pause(TRIPLE_PWM_SW_PAUSE_EN_t sw_pause_en);

/**
 ****************************************************************************************
 * @brief Sets TIMER2 frequency.
 * @param[in] triple_pwm_frequency TIMER2 frequency
 * @return void
 ****************************************************************************************
 */
 void timer2_set_pwm_frequency(uint16_t triple_pwm_frequency);

/**
 ****************************************************************************************
 * @brief Initializes TIMER2.
 * @param[in] hw_pause_en          HW_CAN_NOT_PAUSE_PWM_2_3_4 or HW_CAN_PAUSE_PWM_2_3_4
 * @param[in] sw_pause_en          PWM_2_3_4_SW_PAUSE_DISABLED or PWM_2_3_4_SW_PAUSE_ENABLED
 * @param[in] triple_pwm_frequency TIMER2 frequency
 * @return void
 ****************************************************************************************
 */
void timer2_init(TRIPLE_PWM_HW_PAUSE_EN_t hw_pause_en, TRIPLE_PWM_SW_PAUSE_EN_t sw_pause_en, uint16_t triple_pwm_frequency);

/**
 ****************************************************************************************
 * @brief Stops timer2. Does not disable the TIM clock, as it is shared with TIMER0.
 * @param[in] timer2_duty_cycle PWM2 duty cycle
 * @return void
 ****************************************************************************************
 */
void timer2_stop(void);

/**
 ****************************************************************************************
 * @brief Sets PWM2 duty cycle.
 * @param[in] timer2_duty_cycle PWM2 duty cycle
 * @return void
 ****************************************************************************************
 */
void timer2_set_pwm2_duty_cycle(uint16_t pwm2_duty_cycle);

/**
 ****************************************************************************************
 * @brief Sets PWM3 duty cycle.
 * @param[in] timer3_duty_cycle PWM3 duty cycle.
 * @return void
 ****************************************************************************************
 */
void timer2_set_pwm3_duty_cycle(uint16_t pwm3_duty_cycle);

/**
 ****************************************************************************************
 * @brief Sets PWM4 duty cycle.
 * @param[in] timer3_duty_cycle PWM4 duty cycle.
 * @return void
 ****************************************************************************************
 */
void timer2_set_pwm4_duty_cycle(uint16_t pwm4_duty_cycle);

#endif // _TIMER2_H_
