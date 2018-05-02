/**
 ****************************************************************************************
 *
 * @file arch_main.c
 *
 * @brief Main loop of the application.
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
 * INCLUDES
 ****************************************************************************************
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>     // standard definitions
#include <stdbool.h>    // boolean definition
#include "arch.h"
#include "arch_api.h"
#include "boot.h"       // boot definition
#include "rwip.h"       // BLE initialization
#include "syscntl.h"    // System control initialization
#include "em_map_ble.h"
#include "ke_mem.h"
#include "ke_event.h"
#include "user_periph_setup.h"

#include "uart.h"   // UART initialization
#include "nvds.h"   // NVDS initialization
#include "rf.h"     // RF initialization
#include "app.h"    // application functions
#include "dbg.h"    // For dbg_warning function

#include "datasheet.h"

#include "em_map_ble.h"

#include "lld_sleep.h"
#include "rwble.h"
#include "rf_585.h"
#include "gpio.h"

#include "lld_evt.h"
#include "arch_console.h"

#include "arch_system.h"

#include "system_library.h"

#include "arch_wdg.h"

#include "user_callback_config.h"

#include "ea.h"

#include "arch_ram.h"

#if (USE_RANGE_EXT)
#include "range_ext_api.h"
#endif

/**
 * @addtogroup DRIVERS
 * @{
 */

/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * STRUCTURE DEFINITIONS
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

extern struct arch_sleep_env_tag sleep_env;

extern last_ble_evt arch_rwble_last_event;

uint32_t code_size                      __attribute__((section("retention_mem_area0"), zero_init));
uint8_t ret_mode_for_non_ret_heap       __attribute__((section("retention_mem_area0"), zero_init));
uint8_t ret_mode                        __attribute__((section("retention_mem_area0"), zero_init));
uint8_t ret_mode_for_ret_data           __attribute__((section("retention_mem_area0"), zero_init));
uint8_t reinit_non_ret_heap             __attribute__((section("retention_mem_area0"), zero_init));

/*
 * LOCAL FUNCTION DECLARATIONS
 ****************************************************************************************
 */
static inline void otp_prepare(uint32_t code_size);
static inline bool ble_is_powered(void);
static inline void ble_turn_radio_off(void);
static inline void schedule_while_ble_on(void);
static inline sleep_mode_t ble_validate_sleep_mode(sleep_mode_t current_sleep_mode);
static inline void arch_turn_peripherals_off(sleep_mode_t current_sleep_mode);
static inline void arch_goto_sleep(sleep_mode_t current_sleep_mode);
static inline void arch_switch_clock_goto_sleep(sleep_mode_t current_sleep_mode);
static inline void arch_resume_from_sleep(void);
static inline sleep_mode_t rwip_power_down(void);
static inline arch_main_loop_callback_ret_t app_asynch_trm(void);
static inline arch_main_loop_callback_ret_t app_asynch_proc(void);
static inline void app_asynch_sleep_proc(void);
static inline void app_sleep_prepare_proc(sleep_mode_t *sleep_mode);
static inline void app_sleep_exit_proc(void);
static inline void app_sleep_entry_proc(sleep_mode_t sleep_mode);
static inline void init_retention_mode(void);

#if !defined (CFG_CUSTOM_SCATTER_FILE)
static inline uint8_t get_retention_mode(void);
static inline uint8_t get_retention_mode_data(void);
#endif
static inline uint8_t get_retention_mode_non_ret_heap(void);


/*
 * MAIN FUNCTION
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief BLE main function.
 *        This function is called right after the booting process has completed.
 *        It contains the main function loop.
 ****************************************************************************************
 */
int main(void) __attribute__((noreturn));

int main(void)
{
    sleep_mode_t sleep_mode;
    
    // initialize retention mode
    init_retention_mode();

    //global initialise
    system_init();

    /*
     ************************************************************************************
     * Platform initialization
     ************************************************************************************
     */

    while(1)
    {
        do {
            // schedule all pending events
            schedule_while_ble_on();
        }
        while (app_asynch_proc() != GOTO_SLEEP);    //grant control to the application, try to go to power down
                                                    //if the application returns GOTO_SLEEP

        //wait for interrupt and go to sleep if this is allowed
        if (((!BLE_APP_PRESENT) && (check_gtl_state())) || (BLE_APP_PRESENT))
        {
            //Disable the interrupts
            GLOBAL_INT_STOP();

            app_asynch_sleep_proc();

            // get the allowed sleep mode
            // time from rwip_power_down() to WFI() must be kept as short as possible!!
            sleep_mode = rwip_power_down();

            if ((sleep_mode == mode_ext_sleep) || (sleep_mode == mode_ext_sleep_otp_copy))
            {
                //power down the radio and whatever is allowed
                arch_goto_sleep(sleep_mode);

                // In extended sleep mode the watchdog timer is disabled
                // (power domain PD_SYS is automatically OFF). Although, if the debugger
                // is attached the watchdog timer remains enabled and must be explicitly
                // disabled.
                if ((GetWord16(SYS_STAT_REG) & DBG_IS_UP) == DBG_IS_UP)
                {
                    wdg_freeze();    // Stop watchdog timer
                }
                
                //wait for an interrupt to resume operation
                WFI();

                //resume operation
                arch_resume_from_sleep();
            }
            else if (sleep_mode == mode_idle)
            {
                if (((!BLE_APP_PRESENT) && check_gtl_state()) || (BLE_APP_PRESENT))
                {   
                    //wait for an interrupt to resume operation
                    WFI();
                }
            }
            // restore interrupts
            GLOBAL_INT_START();
        }
        wdg_reload(WATCHDOG_DEFAULT_PERIOD);
    }
}

/**
 ****************************************************************************************
 * @brief Power down the BLE Radio and whatever is allowed according to the sleep mode and
 *        the state of the system and application
 * @param[in] current_sleep_mode The current sleep mode proposed by the application.
 * @return void
 ****************************************************************************************
 */
static inline void arch_goto_sleep (sleep_mode_t current_sleep_mode)
{
#if (USE_RANGE_EXT)
    // Disable range extender
    range_ext.disable(NULL);
#endif
    
    sleep_mode_t sleep_mode = current_sleep_mode;

    ble_turn_radio_off ( );
    //turn the radio off and check if we can go into deep sleep
    sleep_mode = ble_validate_sleep_mode(sleep_mode);

    // grant access to the application to check if we can go to sleep
    app_sleep_prepare_proc(&sleep_mode);    //SDK Improvements for uniformity this one should be changed?

    //turn the peripherals off according to the current sleep mode
    arch_turn_peripherals_off(sleep_mode);

    // hook for app specific tasks just before sleeping
    app_sleep_entry_proc(sleep_mode);

    #if ((EXTERNAL_WAKEUP) && (!BLE_APP_PRESENT)) // external wake up, only in external processor designs
        ext_wakeup_enable(EXTERNAL_WAKEUP_GPIO_PORT, EXTERNAL_WAKEUP_GPIO_PIN, EXTERNAL_WAKEUP_GPIO_POLARITY);
    #endif

    // do the last house keeping of the clocks and go to sleep
    arch_switch_clock_goto_sleep (sleep_mode);
}

/**
 ****************************************************************************************
 * @brief Manage the clocks and go to sleep.
 * @param[in] current_sleep_mode The current sleep mode proposed by the system so far
 * @return void
 ****************************************************************************************
 */
static inline void arch_switch_clock_goto_sleep(sleep_mode_t current_sleep_mode)
{
    if ( (current_sleep_mode == mode_ext_sleep) || (current_sleep_mode == mode_ext_sleep_otp_copy) )
    {
        SetBits16(CLK_16M_REG, XTAL16_BIAS_SH_ENABLE, 0);      // Set BIAS to '0' if sleep has been decided

        if (USE_POWER_OPTIMIZATIONS)
        {
            clk_freq_trim_reg_value = GetWord16(CLK_FREQ_TRIM_REG); // store used trim value

            SetBits16(CLK_16M_REG, RC16M_ENABLE, 1);                // Enable RC16

            for (volatile int i = 0; i < 20; i++);

            SetBits16(CLK_CTRL_REG, SYS_CLK_SEL, 1);                // Switch to RC16

            while( (GetWord16(CLK_CTRL_REG) & RUNNING_AT_RC16M) == 0 );

            // Do not disable XTAL16M! It will be disabled when we sleep...
            SetWord16(CLK_FREQ_TRIM_REG, 0x0000);                   // Set zero value to CLK_FREQ_TRIM_REG
        }
    }
}

/**
 ****************************************************************************************
 * @brief  An interrupt came, resume from sleep.
 * @return void
 ****************************************************************************************
 */
static inline void arch_resume_from_sleep(void)
{
    // Check if non retained heap should be re-initialized
    if (reinit_non_ret_heap)
    {
        ke_mem_init(KE_MEM_NON_RETENTION, (uint8_t*)(rom_cfg_table[rwip_heap_non_ret_pos]), rom_cfg_table[rwip_heap_non_ret_size]);
    }

    // hook for app specific tasks just after waking up
    app_sleep_exit_proc( );

#if ((EXTERNAL_WAKEUP) && (!BLE_APP_PRESENT)) // external wake up, only in external processor designs
    // Disable external wakeup interrupt
    ext_wakeup_disable();
#endif

    // restore ARM Sleep mode
    // reset SCR[2]=SLEEPDEEP bit else the mode=idle WFI will cause a deep sleep
    // instead of a processor halt
    SCB->SCR &= ~(1<<2);
}

/**
 ****************************************************************************************
 * @brief Check if the BLE module is powered on.
 * @return void
 ****************************************************************************************
 */
static inline bool ble_is_powered()
{
    return ((GetBits16(CLK_RADIO_REG, BLE_ENABLE) == 1) &&
            (GetBits32(BLE_DEEPSLCNTL_REG, DEEP_SLEEP_STAT) == 0) &&
            !(rwip_prevent_sleep_get() & RW_WAKE_UP_ONGOING));
}

/**
 ****************************************************************************************
 * @brief Call the scheduler if the BLE module is powered on.
 * @return void
 ****************************************************************************************
 */
static inline void schedule_while_ble_on(void)
{
    // BLE clock is enabled
    while (ble_is_powered())
    {
        //execute messages and events
        rwip_schedule();

#if (!FPGA)
        if (arch_rwble_last_event == BLE_EVT_END)
        {
           uint32_t sleep_duration = 0;
           rcx20_read_freq();

            //if you have enough time run a temperature calibration of the radio
            if (ea_sleep_check(&sleep_duration, 4)) //6 slots -> 3.750 ms
                // check time and temperature to run radio calibrations.
                conditionally_run_radio_cals();
        }
#endif // (!FPGA)

        //grant control to the application, try to go to sleep
        //if the application returns GOTO_SLEEP
        if (app_asynch_trm() != GOTO_SLEEP)
        {
            continue; // so that rwip_schedule() is called again
        }
        else
        {
            arch_printf_process();
            break;
        }
    }
}

/**
 ****************************************************************************************
 * @brief Power down the ble ip if possible.
 * @return sleep_mode_t return the current sleep mode
 ****************************************************************************************
 */
static inline sleep_mode_t rwip_power_down(void)
{
    sleep_mode_t sleep_mode;
    // if app has turned sleep off, rwip_sleep() will act accordingly
    // time from rwip_sleep() to WFI() must be kept as short as possible!
    sleep_mode = rwip_sleep();

    // BLE is sleeping ==> app defines the mode
    if (sleep_mode == mode_sleeping) {
        if (sleep_env.slp_state == ARCH_EXT_SLEEP_ON) {
            sleep_mode = mode_ext_sleep;
        } else {
            sleep_mode = mode_ext_sleep_otp_copy;
        }
    }
    return (sleep_mode);
}

/**
 ****************************************************************************************
 * @brief Turn the radio off.
 * @return void
 ****************************************************************************************
 */
static inline void ble_turn_radio_off(void)
{
    SetBits16(PMU_CTRL_REG, RADIO_SLEEP, 1); // turn off radio
}

/**
 ****************************************************************************************
 * @brief Validate that we can use the proposed sleep mode.
 * @param[in] current_sleep_mode The current sleep mode proposed by the system so far
 * @return sleep_mode_t return the allowable sleep mode
 ****************************************************************************************
 */
static inline sleep_mode_t ble_validate_sleep_mode(sleep_mode_t current_sleep_mode)
{
    return current_sleep_mode;
}

/**
 ****************************************************************************************
 * @brief Initialize retention mode.
 * @return void
 ****************************************************************************************
 */
static inline void init_retention_mode(void)
{
#if defined (CFG_CUSTOM_SCATTER_FILE)
    // User selects which RAM blocks to retain (code and retention data or only retention data)
    ret_mode = ret_mode_for_ret_data = (RETAIN_RAM_1_BLOCK |
                                        RETAIN_RAM_2_BLOCK |
                                        RETAIN_RAM_3_BLOCK |
                                        RETAIN_RAM_4_BLOCK);
#else
    // RAM retention mode for code and retention data
    ret_mode = get_retention_mode();
    
    // RAM retention mode for retention data only
    ret_mode_for_ret_data = get_retention_mode_data();
#endif
    // RAM retention mode for the non retainable heap data only
    ret_mode_for_non_ret_heap = get_retention_mode_non_ret_heap();
}

/**
 ****************************************************************************************
 * @brief Define which RAM blocks will be retained based on the base address of the 
 *        retention data which touch the aforementioned RAM blocks.
 *        The RAM_4 block is always retained.
 * @return the retention mode (the RAM blocks to be retained)
 * @note This function is strongly related to the default SDK scatter file structure.
 ****************************************************************************************
 */
#if !defined (CFG_CUSTOM_SCATTER_FILE)
static inline uint8_t get_retention_mode_data(void)
{
    uint32_t ret_mem_base_addr = (uint32_t) &Image$$RET_DATA_UNINIT$$Base;
    
    // Check the base address of the retention data
    if (ret_mem_base_addr >= RAM_4_BASE_ADDR)
    {
        // Retain RAM_4 block
        return (RETAIN_RAM_4_BLOCK);
    }
    else if(ret_mem_base_addr >= RAM_3_BASE_ADDR)
    {
        // Retain RAM_3 and RAM_4 block
        return (RETAIN_RAM_3_BLOCK |
                RETAIN_RAM_4_BLOCK);
    }
    else if (ret_mem_base_addr >= RAM_2_BASE_ADDR)
    {
        // Retain RAM_2, RAM_3 and RAM_4 block
        return (RETAIN_RAM_2_BLOCK |
                RETAIN_RAM_3_BLOCK |
                RETAIN_RAM_4_BLOCK);
    }
    else
    {
        // Retain all RAM blocks
        return (RETAIN_RAM_1_BLOCK |
                RETAIN_RAM_2_BLOCK |
                RETAIN_RAM_3_BLOCK |
                RETAIN_RAM_4_BLOCK);
    }
}

/**
 ****************************************************************************************
 * @brief Define which RAM blocks will be retained based on the code and the retention
 *        data size.
 *        The RAM_4 block is always retained.
 * @return the retention mode (the RAM blocks to be retained)
 * @note This function is strongly related to the default SDK scatter file.
 ****************************************************************************************
 */
static inline uint8_t get_retention_mode(void)
{
    // The following equation calculates the code size and is strongly related to the 
    // default SDK scatter file structure
    code_size = ((uint32_t) &Image$$ER_IROM3$$Base - RAM_1_BASE_ADDR) + (uint32_t) &Image$$ER_IROM3$$Length;
    
    // Check the code size
    if (code_size <= (RAM_2_BASE_ADDR - RAM_1_BASE_ADDR))
    {
        // Code fits in RAM_1 block
        return (RETAIN_RAM_1_BLOCK |
                get_retention_mode_data());
    }
    else if ((code_size > (RAM_2_BASE_ADDR - RAM_1_BASE_ADDR)) && code_size <= (RAM_3_BASE_ADDR - RAM_1_BASE_ADDR))
    {
        // Code fits in RAM_1 block and RAM_2 block
        return (RETAIN_RAM_1_BLOCK |
                RETAIN_RAM_2_BLOCK |
                get_retention_mode_data());
    }
    else
    {
        // Retain all RAM blocks
        return (RETAIN_RAM_1_BLOCK |
                RETAIN_RAM_2_BLOCK |
                RETAIN_RAM_3_BLOCK |
                RETAIN_RAM_4_BLOCK);
    }
}
#endif // CFG_CUSTOM_SCATTER_FILE

/**
 ****************************************************************************************
 * @brief Calculate the RAM blocks which will be retained depending on the non retained
 *        heap base address and size.
 * @return the retention mode (the RAM blocks to be retained)
 ****************************************************************************************
 */
static inline uint8_t get_retention_mode_non_ret_heap()
{
    uint32_t non_ret_heap_base = (uint32_t) &heap_mem_area_not_ret$$Base;
    uint32_t non_ret_heap_length = (uint32_t) &heap_mem_area_not_ret$$Length;
    
    if (non_ret_heap_base >= RAM_4_BASE_ADDR)
    {
        return (RETAIN_RAM_4_BLOCK);
    }
    else if (non_ret_heap_base >= RAM_3_BASE_ADDR)
    {
        if (non_ret_heap_base + non_ret_heap_length < RAM_4_BASE_ADDR)
        {
            return (RETAIN_RAM_3_BLOCK);
        }
        else
        {
            return (RETAIN_RAM_3_BLOCK |
                    RETAIN_RAM_4_BLOCK);
        }
    }
    else if (non_ret_heap_base >= RAM_2_BASE_ADDR)
    {
        if (non_ret_heap_base + non_ret_heap_length < RAM_3_BASE_ADDR)
        {
            return (RETAIN_RAM_2_BLOCK);
        }
        else if (non_ret_heap_base + non_ret_heap_length < RAM_4_BASE_ADDR)
        {
            return (RETAIN_RAM_2_BLOCK |
                    RETAIN_RAM_3_BLOCK);
        }
        else
        {
            return (RETAIN_RAM_2_BLOCK |
                    RETAIN_RAM_3_BLOCK |
                    RETAIN_RAM_4_BLOCK);
        }
    }
    else
    {
        if (non_ret_heap_base + non_ret_heap_length < RAM_2_BASE_ADDR)
        {
            return (RETAIN_RAM_1_BLOCK);
            
        }
        else if (non_ret_heap_base + non_ret_heap_length < RAM_3_BASE_ADDR)
        {
            return (RETAIN_RAM_1_BLOCK |
                    RETAIN_RAM_2_BLOCK);
            
        }
        else if (non_ret_heap_base + non_ret_heap_length < RAM_4_BASE_ADDR)
        {
            return (RETAIN_RAM_1_BLOCK |
                    RETAIN_RAM_2_BLOCK |
                    RETAIN_RAM_3_BLOCK);
        }
        else
        {
            return (RETAIN_RAM_1_BLOCK |
                    RETAIN_RAM_2_BLOCK |
                    RETAIN_RAM_3_BLOCK |
                    RETAIN_RAM_4_BLOCK);
        }
    }
}

/**
 ****************************************************************************************
 * @brief  Turn the peripherals off according to the current sleep mode.
 * @param[in] current_sleep_mode The current sleep mode proposed by the system so far
 * @return void
 ****************************************************************************************
 */
static inline void arch_turn_peripherals_off (sleep_mode_t current_sleep_mode)
{
    if (current_sleep_mode == mode_ext_sleep || current_sleep_mode == mode_ext_sleep_otp_copy)
    {
        SCB->SCR |= 1<<2; // enable deep sleep  mode bit in System Control Register (SCR[2]=SLEEPDEEP)

        SetBits16(SYS_CTRL_REG, PAD_LATCH_EN, 0);          // activate PAD latches
        SetBits16(PMU_CTRL_REG, PERIPH_SLEEP, 1);          // turn off peripheral power domain
        if (current_sleep_mode == mode_ext_sleep)
        {
            /*
             * Sleep mode: EXTENDED - image kept at external resource
             * 
             * The RAM blocks that hold the code and the retention data
             * must be retained.
             */
            
            // OTP copy and OTP copy emulation will be disabled
            SetBits16(SYS_CTRL_REG, OTP_COPY, 0);
            
            // dynamically select the retained RAM blocks
            SetBits16(PMU_CTRL_REG, RETENTION_MODE, ret_mode);
        }
        else
        {
            /*
             * Sleep mode: EXTENDED - image kept at OTP (OTP mirroring takes place on wake-up)
             * 
             * The RAM blocks that hold the retention data must be retained.
             */
            
#if DEVELOPMENT_DEBUG

            // enable OTP copy emulation
            SetBits16(SYS_CTRL_REG, OTP_COPY, 1);
            SetBits16(SYS_CTRL_REG, DEV_PHASE, 1);
            
            // dynamically select the retained RAM blocks
            SetBits16(PMU_CTRL_REG, RETENTION_MODE, ret_mode);
#else
            // enable OTP copy
            SetBits16(SYS_CTRL_REG, DEV_PHASE, 0);
            SetBits16(PMU_CTRL_REG, RETENTION_MODE, ret_mode_for_ret_data);
#endif
            otp_prepare((code_size + 3) >> 2);       // convert function argument from bytes to 32-bit words
        }

        // manage the non retained heap
        if (!ke_mem_is_empty(KE_MEM_NON_RETENTION))
        {
            reinit_non_ret_heap = 0;
            SetBits16(PMU_CTRL_REG, RETENTION_MODE, ret_mode_for_non_ret_heap | GetBits16(PMU_CTRL_REG, RETENTION_MODE));
        }
        else
        {
            reinit_non_ret_heap = 1;
        }
    }
}

/**
 ****************************************************************************************
 * @brief Prepare OTP Controller in order to be able to reload SysRAM at the next power-up.
 ****************************************************************************************
 */
static inline void otp_prepare(uint32_t code_size)
{
    // Enable OPTC clock in order to have access
    SetBits16 (CLK_AMBA_REG, OTP_ENABLE, 1);

    // Wait a little bit to start the OTP clock...
    for(uint8_t i = 0 ; i<10 ; i++); //change this later to a defined time

    SetBits16(SYS_CTRL_REG, OTP_COPY, 1);

    // Copy the size of software from the first word of the retention mem.
    SetWord32 (OTPC_NWORDS_REG, code_size - 1);

    // And close the OPTC clock to save power
    SetBits16 (CLK_AMBA_REG, OTP_ENABLE, 0);
}

/**
 ****************************************************************************************
 * @brief Used for sending messages to kernel tasks generated from
 *        asynchronous events that have been processed in app_asynch_proc.
 * @return KEEP_POWERED to force calling of schedule_while_ble_on(), else GOTO_SLEEP
 ****************************************************************************************
 */
static inline arch_main_loop_callback_ret_t app_asynch_trm(void)
{
    if (user_app_main_loop_callbacks.app_on_ble_powered != NULL)
    {
        return user_app_main_loop_callbacks.app_on_ble_powered();
    }
    else
    {
        return GOTO_SLEEP;
    }
}

/**
 ****************************************************************************************
 * @brief Used for processing of asynchronous events at ?user? level. The
 *        corresponding ISRs should be kept as short as possible and the
 *        remaining processing should be done at this point.
 * @return KEEP_POWERED to force calling of schedule_while_ble_on(), else GOTO_SLEEP
 ****************************************************************************************
 */
static inline arch_main_loop_callback_ret_t app_asynch_proc(void)
{
    if (user_app_main_loop_callbacks.app_on_system_powered != NULL)
    {
        return user_app_main_loop_callbacks.app_on_system_powered();
    }
    else
    {
        return GOTO_SLEEP;
    }
}

/**
 ****************************************************************************************
 * @brief Used for updating the state of the application just before sleep checking starts.
 * @return void
 ****************************************************************************************
 */
static inline void app_asynch_sleep_proc(void)
{
    if (user_app_main_loop_callbacks.app_before_sleep != NULL)
        user_app_main_loop_callbacks.app_before_sleep();
}

/**
 ****************************************************************************************
 * @brief Used to override the slected extended sleep mode , based on the current 
 *        application state.
 *        BLE and Radio are still powered off.
 * @param[in] sleep_mode     Sleep Mode
 * @return void
 ****************************************************************************************
 */
static inline void app_sleep_prepare_proc(sleep_mode_t *sleep_mode)
{
    if (user_app_main_loop_callbacks.app_validate_sleep != NULL)
        (*sleep_mode) = user_app_main_loop_callbacks.app_validate_sleep(*sleep_mode);
}

/**
 ****************************************************************************************
 * @brief Used for application specific tasks just before entering the low power mode.
 * @param[in] sleep_mode     Sleep Mode
 * @return void
 ****************************************************************************************
 */
static inline void app_sleep_entry_proc(sleep_mode_t sleep_mode)
{
    if (user_app_main_loop_callbacks.app_going_to_sleep != NULL)
        user_app_main_loop_callbacks.app_going_to_sleep(sleep_mode);
}

/**
 ****************************************************************************************
 * @brief Used for application specific tasks immediately after exiting the low power mode.
 * @param[in] sleep_mode     Sleep Mode
 * @return void
 ****************************************************************************************
 */
static inline void app_sleep_exit_proc(void)
{
    if (user_app_main_loop_callbacks.app_resume_from_sleep != NULL)
        user_app_main_loop_callbacks.app_resume_from_sleep();
}

/// @} DRIVERS
