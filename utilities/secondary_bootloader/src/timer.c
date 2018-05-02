/**
 ****************************************************************************************
 *
 * @file timer.c
 *
 * @brief timer handling functions
 *
 * Copyright (C) 2012-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#include <stdint.h>
#include "datasheet.h"
#include <core_cm0.h>
#include "timer.h"
#include "gpio.h"
#include "user_periph_setup.h"

extern volatile uint32_t timeout_multi;
extern volatile uint32_t timeout_occur;

/////////////////////////////////////////////////////////////
// Set SW timer to count
/////////////////////////////////////////////////////////////

/**
 ****************************************************************************************
 * @brief Set SW timer to count
 *
 * @param[in] times_x_fourms:  timeout 
 ****************************************************************************************
 */
void start_timer(uint32_t times_x_fourms)
{
    SetBits16(CLK_PER_REG, TMR_ENABLE ,1);    // enable  clock for  TMR DIV=0 
    SetWord16(RESET_FREEZE_REG,FRZ_SWTIM);   
    timeout_occur=0;                          // set timeout global variable to false
    SetWord16(TIMER0_CTRL_REG,0x0);           // stop timer
    SetWord16(TIMER0_RELOAD_M_REG,0x7D00);    // set to 0x7D00/16MHz time out    2ms
    SetWord16(TIMER0_RELOAD_N_REG,0x7D00);    // set to 0x7D00/16MHz time out    2ms  
    timeout_multi=times_x_fourms;             // set how many 4ms
    SetWord16(TIMER0_CTRL_REG,0x07);          // set timer with 16MHz source clock no div 10
    NVIC_EnableIRQ(SWTIM_IRQn);               // enable software timer interrupt
}

/**
 ****************************************************************************************
 * @brief Close SW timer 
 *
 ****************************************************************************************
 */
void stop_timer()
{
    SetWord16(TIMER0_CTRL_REG,0x6);      // stop timer

    NVIC_ClearPendingIRQ(SWTIM_IRQn);
    NVIC_DisableIRQ(SWTIM_IRQn);         // disable software timer interrupt
}

void SWTIM_Handler(void) 
{
    if(  timeout_multi==0) {                // when pass  timeout time
        timeout_occur=1;                    // set timeout flag
        SetWord16(TIMER0_CTRL_REG,0x6);     // stop timer
        // disable software timer interrupt
    } else
        timeout_multi--;              //pass 4ms and increase the 4ms counter
}
