/**
 ****************************************************************************************
 *
 * @file    arch_sleep.c
 *
 * @brief   Sleep control functions.
 *
 * Copyright (C) 2012-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdbool.h>
#include "arch.h"
#include "arch_api.h"
#include "app.h"
#include "rwip.h"

/// Application Environment Structure
extern struct arch_sleep_env_tag    sleep_env;
static uint8_t sleep_md             __attribute__((section("retention_mem_area0"), zero_init));
static uint8_t sleep_pend           __attribute__((section("retention_mem_area0"), zero_init));
static uint8_t sleep_cnt            __attribute__((section("retention_mem_area0"), zero_init));
static bool sleep_ext_force         __attribute__((section("retention_mem_area0"), zero_init));

extern last_ble_evt arch_rwble_last_event;

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void arch_disable_sleep(void)
{
    if (sleep_md == 0)
    {
        sleep_env.slp_state = ARCH_SLEEP_OFF;

        rwip_env.sleep_enable = false;

    }
    else
        sleep_pend = 0x80 | ARCH_SLEEP_OFF;
}

void arch_set_extended_sleep(bool otp_copy)
{
    if (sleep_md == 0)
    {
        sleep_env.slp_state = otp_copy ? ARCH_EXT_SLEEP_OTP_COPY_ON : ARCH_EXT_SLEEP_ON;

        rwip_env.sleep_enable = true;
    }
    else
    {
        sleep_pend = 0x80 | (otp_copy ? ARCH_EXT_SLEEP_OTP_COPY_ON : ARCH_EXT_SLEEP_ON);
    }
}

void arch_set_deep_sleep(bool ext_wakeup)
{
    wdg_freeze();                                               // Stop watchdog timer

    GLOBAL_INT_STOP()                                           // disable interrupts

    SetBits16(PMU_CTRL_REG, RADIO_SLEEP, 1);                    // disable radio

    SetBits16(CLK_RADIO_REG, BLE_ENABLE, 0);                    // disable the BLE core clocks

    SetBits16(CLK_RADIO_REG, BLE_LP_RESET, 1);                  // apply HW reset to BLE_Timers

    SCB->SCR |= 1<<2;                                           // enable deep sleep

    SetBits16(SYS_CTRL_REG, PAD_LATCH_EN, 0);                   // activate PAD latches

    SetBits16(PMU_CTRL_REG, PERIPH_SLEEP, 1);                   // disable peripherals

    SetBits16(PMU_CTRL_REG, RETENTION_MODE, 0x00);              // disable all RAM blocks

    SetBits16(CLK_16M_REG, XTAL16_BIAS_SH_ENABLE, 0);           // set BIAS to '0' if sleep has been decided

    if (ext_wakeup)
    {
        SetBits16(PMU_CTRL_REG, RESET_ON_WAKEUP, 1);            // enable boot after wake-up
    }
    else
    {
        SetBits16(PMU_CTRL_REG, RESET_ON_WAKEUP, 0);
        SetWord16(WKUP_CTRL_REG, 0x00);                         // disable wake-up interrupt
    }
    
    *((volatile unsigned long *)(0xE000E280)) = 0xFFFFFFFF;     // clear all interrupts pending status
   
    __NOP();
    __NOP();
    __NOP();
    
    WFI();                                                      // enter deep sleep mode
}

void arch_set_sleep_mode(sleep_state_t sleep_state)
{
    switch (sleep_state)
    {
        case ARCH_SLEEP_OFF:
            arch_disable_sleep();
            break;
        case ARCH_EXT_SLEEP_ON:
            arch_set_extended_sleep(false);
            break;
        case ARCH_EXT_SLEEP_OTP_COPY_ON:
            arch_set_extended_sleep(true);
            break;
        default:
            break;
    }
}

uint8_t arch_get_sleep_mode(void)
{
    uint8_t ret = 0;

    switch(sleep_env.slp_state)
    {
        case ARCH_SLEEP_OFF: 
            ret = 0; break;
        case ARCH_EXT_SLEEP_ON:
            ret = 1; break;
        case ARCH_EXT_SLEEP_OTP_COPY_ON: 
            ret = 2; break;
    }

    return ret;
}

void arch_restore_sleep_mode(void)
{
    uint8_t cur_mode;
    
    if (sleep_cnt > 0)
        sleep_cnt--;
    
    if (sleep_cnt > 0)
        return;     // cannot restore it yet. someone else has requested active mode and we'll wait him to release it.
        
    if (sleep_pend != 0)
    {
        sleep_md = sleep_pend & 0x3;
        sleep_pend = 0;
    }
    else if (sleep_md)
        sleep_md--;
    
    cur_mode = sleep_md;
    sleep_md = 0;
    
    switch(cur_mode) 
    {
       case 0:  break;
       case 1:  arch_set_extended_sleep(false); break;
       case 2:  arch_set_extended_sleep(true); break;
       default: break;
    }
}

void arch_force_active_mode(void)
{
    uint8_t cur_mode;
    
    sleep_cnt++;
    
    if (sleep_md == 0)  // add this check for safety! If it's called again before restore happens then sleep_md won't get overwritten
    {
        cur_mode = arch_get_sleep_mode();
        cur_mode++;     // 1: disabled, 2: extended sleep without OTP copy, 3: extended sleep with OTP copy (!=0 ==> sleep is in forced state)
        arch_disable_sleep();
        sleep_md = cur_mode;
    }
}

void arch_ble_ext_wakeup_on(void)
{
    sleep_ext_force = true;
}

void arch_ble_ext_wakeup_off(void)
{
    sleep_ext_force = false;
}

bool arch_ble_ext_wakeup_get(void)
{
    return sleep_ext_force;
}

bool arch_ble_force_wakeup(void)
{
    bool retval = false;
    
    // If BLE is sleeping, wake it up!
    GLOBAL_INT_DISABLE();
    if (GetBits16(CLK_RADIO_REG, BLE_ENABLE) == 0) { // BLE clock is off
        SetBits16(GP_CONTROL_REG, BLE_WAKEUP_REQ, 1);
        retval = true;
    }
    GLOBAL_INT_RESTORE();
    
    return retval;
}

last_ble_evt arch_last_rwble_evt_get(void)
{
    return arch_rwble_last_event;
}
