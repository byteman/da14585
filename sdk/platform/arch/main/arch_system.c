/**
 ****************************************************************************************
 *
 * @file arch_system.c
 *
 * @brief System setup.
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
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition
#include "arch.h"
#include "arch_api.h"
#include "user_callback_config.h"
#include "rwip.h"     // BLE initialization
#include "llc.h"
#include "gpio.h"
#include "rf_585.h"
#include "system_library.h"
#include "gtl_env.h"
#include "gtl_task.h"

#if (USE_TRNG)
#include "trng.h"       // True random number generator API
#endif

#include "arch_wdg.h"
#include "arch_system.h"
#include "nvds.h"
#include "user_periph_setup.h"
#include "app.h"
#include "rwble.h"

#include "uart.h"
#include "reg_uart.h"   // uart register

#include "da1458x_scatter_config.h"
#include "arch_rom.h"

#include "otp_hdr.h"
#include "hw_otpc.h"

#if (USE_CHACHA20_RAND)
#include "chacha20.h"
#endif

#if (USE_RANGE_EXT)
#include "range_ext_api.h"
#include "sky66111.h"
#endif

/*
 * DEFINES
 ****************************************************************************************
 */

#if HW_CONFIG_PRO_DK
    #define DEFAULT_XTAL16M_TRIM_VALUE (1102)
#else
    #define DEFAULT_XTAL16M_TRIM_VALUE (1186)
#endif

#if (LOG_HEAP_USAGE)
    #if (!DEVELOPMENT_DEBUG)
        #error "LOG_HEAP_USAGE must not be set when building for production (DEVELOPMENT_DEBUG is 0)"
    #else
        #warning "LOG_HEAP_USAGE is set!"
    #endif
#endif

/**
 * @addtogroup DRIVERS
 * @{
 */

bool sys_startup_flag __attribute__((section("retention_mem_area0"), zero_init));

// Delay sleep entrance at system startup
uint32_t startup_sleep_delay = STARTUP_SLEEP_DELAY_DEFAULT;

uint16_t last_temp_count         __attribute__((section("retention_mem_area0"), zero_init)); // temperature counter
uint32_t last_temp_time __attribute__((section("retention_mem_area0"), zero_init)); // time of last temperature count measurement

extern uint32_t lld_sleep_us_2_lpcycles_func(uint32_t us);
extern uint32_t lld_sleep_lpcycles_2_us_func(uint32_t lpcycles);

extern void ble_init(uint32_t base);

uint32_t lp_clk_sel __attribute__((section("retention_mem_area0"),zero_init));   //low power clock selection
uint32_t rcx_freq __attribute__((section("retention_mem_area0"),zero_init));
uint8_t rcx_cal_in_progress __attribute__((section("retention_mem_area0"),zero_init));
uint32_t rcx_period __attribute__((section("retention_mem_area0"),zero_init));
uint64_t rcx_slot_duration __attribute__((section("retention_mem_area0"), zero_init));

uint8_t force_rf_cal __attribute__((section("retention_mem_area0"),zero_init));

#if (RCX_MEASURE_ENABLED)
uint32_t rcx_freq_min __attribute__((section("retention_mem_area0"),zero_init));
uint32_t rcx_freq_max __attribute__((section("retention_mem_area0"),zero_init));
uint32_t rcx_period_last __attribute__((section("retention_mem_area0"),zero_init));
uint32_t rcx_period_diff __attribute__((section("retention_mem_area0"),zero_init));
#endif

#ifndef USE_ARCH_WKUPCT_DEB_TIME
#define USE_ARCH_WKUPCT_DEB_TIME
uint16_t arch_wkupct_deb_time    __attribute__((section("retention_mem_area0"), zero_init)); // Wakeup timer debouncing time
#endif

/// Device BD address in OTP header
struct bd_addr dev_bdaddr __attribute__((section("retention_mem_area0"), zero_init));

uint16_t iq_trim_bias_ctrl1_reg     __attribute__((section("retention_mem_area0"),zero_init));
uint16_t iq_trim_rf_mixer_ctrl1_reg __attribute__((section("retention_mem_area0"),zero_init));

// Heap logging structs
struct mem_usage_log heap_usage_env      __attribute__((section("retention_mem_area0"), zero_init));
struct mem_usage_log heap_usage_db       __attribute__((section("retention_mem_area0"), zero_init));
struct mem_usage_log heap_usage_msg      __attribute__((section("retention_mem_area0"), zero_init));
struct mem_usage_log heap_usage_nonRet   __attribute__((section("retention_mem_area0"), zero_init));

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Select low power clock.
 * @return void
 * @note function read_otp_header() handles the case where CFG_LP_CLK = LP_CLK_FROM_OTP.
 ****************************************************************************************
 */
static void select_lp_clk()
{
    if (CFG_LP_CLK != LP_CLK_FROM_OTP)
    {
        lp_clk_sel = CFG_LP_CLK;
    }
}

static void read_otp_header(void)
{
    hw_otpc_init();
    hw_otpc_manual_read_on(false);

    // Read BD device address
    memcpy(&dev_bdaddr, (uint8_t *) OTP_HDR_BDADDR_ADDR, sizeof(dev_bdaddr));

    // Read IQ trim values
    {
        const uint16_t *iqtrim = (uint16_t *)OTP_HDR_IQ_TRIM_VALUE_ADDR;

        iq_trim_bias_ctrl1_reg     = *iqtrim++;
        iq_trim_rf_mixer_ctrl1_reg = *iqtrim;
    }

    // Read low power clock source
    if (CFG_LP_CLK == LP_CLK_FROM_OTP)
    {
        lp_clk_sel = *((uint32_t *)OTP_HDR_SLEEP_CLK_SOURCE_ADDR);
    }

    hw_otpc_manual_read_off();
    hw_otpc_close();
}

/**
 ****************************************************************************************
 * @brief Initialisation of ble core, pwr and clk
 *
 * The Hclk and Pclk are set
 ****************************************************************************************
 */
static __inline void init_pwr_and_clk_ble(void)
{
    SetBits16(CLK_RADIO_REG, BLE_DIV, 0);
    SetBits16(CLK_RADIO_REG, BLE_ENABLE, 1);
    SetBits16(CLK_RADIO_REG, RFCU_DIV, 1);
    SetBits16(CLK_RADIO_REG, RFCU_ENABLE, 1);

    /*
     * Power up BLE core & reset BLE Timers
    */
    SetBits16(CLK_32K_REG,  RC32K_ENABLE, 1);
    SetBits16(SYS_CTRL_REG, CLK32_SOURCE, 0);
    SetBits16(CLK_RADIO_REG, BLE_LP_RESET, 1);
    SetBits16(PMU_CTRL_REG, RADIO_SLEEP, 0);
    while (!(GetWord16(SYS_STAT_REG) & RAD_IS_UP)); // Just wait for radio to truely wake up

    select_lp_clk();

    if ( arch_clk_is_XTAL32( ) )
    {
        SetBits16(CLK_32K_REG, XTAL32K_ENABLE, 1);  // Enable XTAL32KHz

        // Disable XTAL32 amplitude regulation in BOOST mode
        if (GetBits16(ANA_STATUS_REG, BOOST_SELECTED) == 0x1)
            SetBits16(CLK_32K_REG,  XTAL32K_DISABLE_AMPREG, 1);
        else
            SetBits16(CLK_32K_REG,  XTAL32K_DISABLE_AMPREG, 0);
        SetBits16(CLK_32K_REG,  XTAL32K_CUR, 5);
        SetBits16(CLK_32K_REG,  XTAL32K_RBIAS, 3);
        SetBits16(SYS_CTRL_REG, CLK32_SOURCE, 1);  // Select XTAL32K as LP clock

     }
    else if ( arch_clk_is_RCX20( ) )
    {
        SetBits16(CLK_RCX20K_REG, RCX20K_NTC, 0xB);
        SetBits16(CLK_RCX20K_REG, RCX20K_BIAS, 0);
        SetBits16(CLK_RCX20K_REG, RCX20K_TRIM, 0);
        SetBits16(CLK_RCX20K_REG, RCX20K_LOWF, 1);

        SetBits16(CLK_RCX20K_REG, RCX20K_ENABLE, 1);

        SetBits16(CLK_RCX20K_REG, RCX20K_SELECT, 1);

        SetBits16(SYS_CTRL_REG, CLK32_SOURCE, 0);

        SetBits16(CLK_32K_REG, XTAL32K_ENABLE, 0); // Disable Xtal32KHz
    }
    else
        ASSERT_WARNING(0);

    SetBits16(CLK_32K_REG,  RC32K_ENABLE, 0);   // Disable RC32KHz

    SetBits16(CLK_RADIO_REG, BLE_LP_RESET, 0);

    if (GetBits16(ANA_STATUS_REG, BOOST_SELECTED) == 0x1)
        SetWord16(DCDC_CTRL3_REG, 0x5);

    /*
     * Just make sure that BLE core is stopped (if already running)
     */
    SetBits32(BLE_RWBLECNTL_REG, RWBLE_EN, 0);

    /*
     * Since BLE is stopped (and powered), set CLK_SEL
     */
    SetBits32(BLE_CNTL2_REG, BLE_CLK_SEL, 16);
    SetBits32(BLE_CNTL2_REG, BLE_RSSI_SEL, 1);
}

/**
 ****************************************************************************************
 * @brief Creates sw cursor in power profiler tool. Used for Development/ Debugging
 *
 * @return void
 ****************************************************************************************
 */
void arch_set_pxact_gpio(void)
{
    if (DEVELOPMENT_DEBUG)
    {
        uint32_t i;

        SetWord16(P13_MODE_REG, PID_GPIO|OUTPUT);
        SetWord16(P1_SET_DATA_REG, 0x8);
        for (i = 0; i < 150; i++); //20 is almost 7.6usec of time.
        SetWord16(P1_RESET_DATA_REG, 0x8);
    }

    return;
}

void calibrate_rcx20(uint16_t cal_time)
{
    if ((CFG_LP_CLK == LP_CLK_FROM_OTP) || (CFG_LP_CLK == LP_CLK_RCX20))
    {
        SetWord16(CLK_REF_CNT_REG, cal_time);
        SetBits16(CLK_REF_SEL_REG, REF_CLK_SEL, 0x3); //RCX select
        SetBits16(CLK_REF_SEL_REG, REF_CAL_START, 0x1); //Start Calibration
        rcx_cal_in_progress = 1;
    }
}

void read_rcx_freq(uint16_t cal_time)
{
    if ( (rcx_cal_in_progress) && ((CFG_LP_CLK == LP_CLK_FROM_OTP) || (CFG_LP_CLK == LP_CLK_RCX20)) )
    {
        while(GetBits16(CLK_REF_SEL_REG, REF_CAL_START) == 1);
        uint32_t high = GetWord16(CLK_REF_VAL_H_REG);
        uint32_t low = GetWord16(CLK_REF_VAL_L_REG);
        uint32_t value = ( high << 16 ) + low;
        uint32_t f = (16000000 * cal_time) / value;

        rcx_cal_in_progress = 0;

        rcx_freq = f;
        rcx_period = (64 * value) / cal_time;

        rcx_slot_duration = 625 * cal_time * 16;
        rcx_slot_duration = rcx_slot_duration << 20;
        rcx_slot_duration = rcx_slot_duration / value;

#if (RCX_MEASURE_ENABLED)
        if (rcx_period_last)
        {
            int diff = rcx_period_last - rcx_period;
            if (abs(diff) > rcx_period_diff)
                rcx_period_diff = abs(diff);
        }
        rcx_period_last = rcx_period;

        if (rcx_freq_min == 0)
        {
            rcx_freq_min = rcx_freq;
            rcx_freq_max = rcx_freq;
        }
        if (rcx_freq < rcx_freq_min)
            rcx_freq_min = rcx_freq;
        else if (rcx_freq > rcx_freq_max)
            rcx_freq_max = rcx_freq;
#endif
    }
}

void set_sleep_delay(void)
{
    int16_t delay;

    if (!USE_POWER_OPTIMIZATIONS)
    {
        uint32_t sleep_delay;

        delay = 0;

        if ( arch_clk_is_RCX20() )
        {
            if (rcx_period > (RCX_PERIOD_MAX << 10) )
                ASSERT_ERROR(0);

            sleep_delay = SLP_PROC_TIME + SLEEP_PROC_TIME + (4 * RCX_PERIOD_MAX); // 400
        }
        else
        {
            sleep_delay = /*SLP_PROC_TIME + SLEEP_PROC_TIME + */(4 * XTAL32_PERIOD_MAX); // ~200
        }

        // Actual "delay" is application specific and is the execution time of the BLE_WAKEUP_LP_Handler(), which depends on XTAL trimming delay.
        // In case of OTP copy, this is done while the XTAL is settling. Time unit of delay is usec.
        delay += XTAL_TRIMMING_TIME_USEC;
        // Increase time taking into account the time from the setting of DEEP_SLEEP_ON until the assertion of DEEP_SLEEP_STAT.
        delay += sleep_delay;
        // Add any application specific delay
        delay += APP_SLEEP_DELAY_OFFSET;
    }
    else // USE_POWER_OPTIMIZATIONS
    {
        delay = MINIMUM_SLEEP_DURATION;

        // if XTAL_TRIMMING_TIME_USEC changes (i.e. gets larger) then this
        // will make sure that the user gets notified to increase "delay" by 1 or more
        // slots so there's enough time for XTAL to settle
        # if ( (3125 + (MINIMUM_SLEEP_DURATION + 625)) < (LP_ISR_TIME_USEC + 1100)) //1.1ms max power-up time
        # error "Minimum sleep duration is too small for the 16MHz crystal used..."
        # endif
    }

    rwip_wakeup_delay_set(delay);
}

uint16_t get_rc16m_count(void)
{
    uint16_t count;

    uint16_t trim = GetBits16(CLK_16M_REG, RC16M_TRIM); // save trim value

    SetBits16(CLK_16M_REG, RC16M_ENABLE, 1);
    SetBits16(CLK_16M_REG, RC16M_TRIM, 0x0);

    SetWord16(CLK_REF_CNT_REG, RC16_CAL_REF_CYCLES);//0x0400); // Preset reference cycles to 1024
    SetWord16(CLK_REF_SEL_REG, 0x0005); // Select RX16M and start counting
    //SetWord16(P1_SET_DATA_REG, 0x8);
    while (GetWord16(CLK_REF_SEL_REG) & 0x0004);
    //SetWord16(P1_RESET_DATA_REG, 0x8);
    count = GetWord16(CLK_REF_VAL_L_REG);  // Read back number of 16 MHz XTALO cycles

    SetBits16(CLK_16M_REG, RC16M_TRIM, 0xA);
    SetBits16(CLK_16M_REG, RC16M_ENABLE, 0);

    SetBits16(CLK_16M_REG, RC16M_TRIM, trim); // restore trim value

    return count;
}

void conditionally_run_radio_cals(void)
{
    uint16_t count, count_diff;

    bool rf_cal_stat;

    uint32_t current_time = lld_evt_time_get();

    if (current_time < last_temp_time)
    {
        last_temp_time = 0;
    }

    if (force_rf_cal)
    {
        force_rf_cal = 0;

        last_temp_time = current_time;
        last_temp_count = get_rc16m_count();
        #if (LUT_PATCH_ENABLED)
        pll_vcocal_LUT_InitUpdate(LUT_UPDATE);    //Update pll look up table
        #endif

        #if (LUT_PATCH_ENABLED)
        rf_cal_stat = rf_calibration_580();
        #else
        rf_cal_stat = rf_calibration_585();
        #endif

        if ( rf_cal_stat == false)
            force_rf_cal = 1;

        return;
    }

    if ( (current_time - last_temp_time) >= 3200) //2 sec
    {
        last_temp_time = current_time;
        count = get_rc16m_count();                  // Estimate the RC16M frequency

        if (count > last_temp_count)
            count_diff = count - last_temp_count;
        else
            count_diff = last_temp_count - count ;

        if (count_diff >= 24)// If corresponds to 5 C degrees difference
        {

            // Update the value of last_count
            last_temp_count = count;
            #if (LUT_PATCH_ENABLED)
            pll_vcocal_LUT_InitUpdate(LUT_UPDATE);    //Update pll look up table
            #endif

            #if (LUT_PATCH_ENABLED)
            rf_cal_stat = rf_calibration_580();
            #else
            rf_cal_stat = rf_calibration_585();
            #endif

            if ( rf_cal_stat == false)
                force_rf_cal = 1;         // Perform the radio calibrations
        }
    }
}

/**
 ****************************************************************************************
 * @brief       Converts us to low power cycles for RCX20 clock.
 *
 * @param[in]   us. microseconds
 *
 * @return      uint32_t. Low power cycles
 ****************************************************************************************
 */
uint32_t lld_sleep_us_2_lpcycles_rcx_func(uint32_t us)
{
    uint32_t lpcycles = 0;

    lpcycles = (us * rcx_freq) / 1000000;

    return(lpcycles);
}

/**
 ****************************************************************************************
 * @brief       Converts low power cycles to us for RCX20 clock.
 *
 * @param[in]   lpcycles. Low power cycles
 *
 * @return      uint32_t. microseconds
 ****************************************************************************************
 */
uint32_t lld_sleep_lpcycles_2_us_rcx_func(uint32_t lpcycles)
{
    uint32_t us = 0;

    // Sanity check: The number of lp cycles should not be too high to avoid overflow
    ASSERT_ERR(lpcycles < 1000000);

    if (lpcycles >= 0x8000) {
        // lpcycles MUST be very large!
        uint64_t res = (uint64_t)lpcycles * rcx_period;
        us = res >> 10;
    }
    else {
        us = (lpcycles * rcx_period) >> 10;
    }

    return(us);
}

uint32_t lld_sleep_us_2_lpcycles_sel_func(uint32_t us)
{
    uint32_t lpcycles = 0;

    if ( arch_clk_is_XTAL32( ) )
        lpcycles = lld_sleep_us_2_lpcycles_func(us);
    else if ( arch_clk_is_RCX20( ) )
        lpcycles = lld_sleep_us_2_lpcycles_rcx_func(us);

    return(lpcycles);
}

uint32_t lld_sleep_lpcycles_2_us_sel_func(uint32_t lpcycles)
{
    uint32_t us = 0;

    if ( arch_clk_is_XTAL32( ) )
        us = lld_sleep_lpcycles_2_us_func(lpcycles);
    else  if ( arch_clk_is_RCX20() )
        us = lld_sleep_lpcycles_2_us_rcx_func(lpcycles);

    return(us);
}


#if (!BLE_APP_PRESENT)

bool check_gtl_state(void)
{
    bool ret = true;

    if (use_h4tl & 0x01)
    {
        if ((h4tl_env.tx_state != H4TL_STATE_TX_IDLE)||
            ((h4tl_env.rx_state != H4TL_STATE_RX_START) &&
            (h4tl_env.rx_state != H4TL_STATE_RX_OUT_OF_SYNC)) )
            ret = false;
    }
    else
    {
        if ((ke_state_get(TASK_GTL) != GTL_TX_IDLE) ||
            ((gtl_env.rx_state != GTL_STATE_RX_START) &&
            (gtl_env.rx_state != GTL_STATE_RX_OUT_OF_SYNC)) )
            ret = false;
    }

    return ret;
}
#endif

#if ((EXTERNAL_WAKEUP) && (!BLE_APP_PRESENT)) // only in full embedded designs
void ext_wakeup_enable(uint32_t port, uint32_t pin, uint8_t polarity)
{
    rwip_env.ext_wakeup_enable = 2;
    if (DEVELOPMENT_DEBUG)
        RESERVE_GPIO(EXT_WAKEUP_GPIO, (GPIO_PORT) port, (GPIO_PIN) pin, PID_GPIO);

    if ( polarity == 0 ) // active low
        GPIO_ConfigurePin((GPIO_PORT) port, (GPIO_PIN) pin, INPUT_PULLUP, PID_GPIO, false ); // active low. Set default to input high.
    else // active high
        GPIO_ConfigurePin((GPIO_PORT) port, (GPIO_PIN) pin, INPUT_PULLDOWN, PID_GPIO, false); // active high. Set default to input low.

    wkupct_register_callback(ext_wakeup_cb);

    wkupct_enable_irq(1 << pin, (uint32_t)(polarity == 0) ? (1 << pin) : (0), 1, 0); // pin, active low/high, 1 event, debouncing time = 0ms
}

void ext_wakeup_disable(void)
{
    SetWord16(WKUP_RESET_IRQ_REG, 1); // Acknowledge it
    SetBits16(WKUP_CTRL_REG, WKUP_ENABLE_IRQ, 0); // No more interrupts of this kind
}

void ext_wakeup_cb(void)
{
    if (GetBits16(SYS_STAT_REG, PER_IS_DOWN))
    {
        // Return GPIO functionality from external wakeup GPIO
        if (DEVELOPMENT_DEBUG)
            GPIO_reservations();
        set_pad_functions();
    }
    SetBits32(GP_CONTROL_REG, BLE_WAKEUP_REQ, 1);
}
#endif // ((EXTERNAL_WAKEUP) && (!BLE_APP_PRESENT))

#if USE_TRNG
void init_rand_seed_from_trng(void)
{
    uint8_t trng_bits[16];

    trng_acquire(trng_bits);

#if (USE_CHACHA20_RAND)
    csprng_seed(trng_bits);
#else
    uint32_t seed = *( (uint32_t *) trng_bits );
    co_random_init(seed);
#endif
}
#endif // USE_TRNG

uint8_t check_sys_startup_period(void)
{
    uint8_t ret_value = 0;

    if (sys_startup_flag)
    {
        uint32_t current_time;

        current_time = lld_evt_time_get();

        // startup_sleep_delay after system startup to allow system to sleep
        if (current_time < startup_sleep_delay)
        {
            ret_value = 1;
        }
        else // After 2 seconds system can sleep
        {
            sys_startup_flag = false;
        }
    }
    return ret_value;
}

__weak bool app_use_lower_clocks_check(void)
{
    return false;
}

void set_xtal16m_trim_value(uint16_t trim_value)
{
    SetBits16(CLK_16M_REG, RC16M_ENABLE, 1);                        // enable RC 16MHz
    for (volatile int i = 0; i < 20; i++);

    SetBits16(CLK_CTRL_REG, SYS_CLK_SEL, 1);                        // switch to  RC16
    while( (GetWord16(CLK_CTRL_REG) & RUNNING_AT_RC16M) == 0 );     // wait for actual switch

    SetWord16(CLK_FREQ_TRIM_REG, trim_value);                       // set default trim value
    for (volatile int i = 0; i < 1600; i++);                        // wait for 1 ms

    SetBits16(CLK_CTRL_REG , SYS_CLK_SEL ,0);                       // switch to  XTAL16
    while( (GetWord16(CLK_CTRL_REG) & RUNNING_AT_XTAL16M) == 0 );   // wait for actual switch
}

/**
 ****************************************************************************************
 * @brief Check that XTAL 16 MHz clock is calibrated.
 * @return void
 * About: Check if XTAL 16 MHz clock is calibrated and if not make corrective actions.
 ****************************************************************************************
 */
static __inline void xtal16_calibration_check(void)
{
    if(DEFAULT_XTAL16M_TRIM_VALUE_USED)
    {
    // Apply the default XTAL16 trim value if a trim value has not been programmed in OTP
        if (0 == GetWord16(CLK_FREQ_TRIM_REG))
        {
            set_xtal16m_trim_value(DEFAULT_XTAL16M_TRIM_VALUE);
        }
    }
}

/*
 ********************* System initialisation related functions *******************
*/


void set_system_clocks(void);

void lld_sleep_init_func(void)
{
    /// ROM struct definition
    struct lld_sleep_env_tag
    {
        uint32_t irq_mask;
    };
    extern struct lld_sleep_env_tag lld_sleep_env;


    // Clear the environment
    memset(&lld_sleep_env, 0, sizeof(lld_sleep_env));

    // Set wakeup_delay
    set_sleep_delay();

    // Enable external wake-up by default
    ble_extwkupdsb_setf(0);
}

#if defined(VERIFY_OTP_CALIBRATION_VALUES)
/**
 * @brief Check CRC of calibration values in OTP header
 *
 * If the CRC check is successfull then the calibration values are
 * also applied to the appropriate registers.
 * If the CRC check fails then:
 *   - a breakpoint is issued in case a debugger is attached
 *   - a watchdog reset is triggered otherwise
 */
static void verify_otp_calibration_values(void)
{
    bool ret = otp_verify_crc();

    if (!ret)
    {
        if ((GetWord16(SYS_STAT_REG) & DBG_IS_UP) == DBG_IS_UP)
            __asm("BKPT #0\n");
        else
        {
            SetWord16(SET_FREEZE_REG, FRZ_WDOG);
            SetWord16(WATCHDOG_CTRL_REG, 1);
            SetWord16(WATCHDOG_REG, 1);
            SetWord16(RESET_FREEZE_REG, FRZ_WDOG);
        }
    }
}
#endif // defined(VERIFY_OTP_CALIBRATION_VALUES)

#ifdef __DA14586__

#include "spi_flash.h"

// DA14586 internal SPI flash pins
#define DA14586_FLASH_PORT  GPIO_PORT_2

#define DA14586_FLASH_EN_PIN     GPIO_PIN_3
#define DA14586_FLASH_CLK_PIN    GPIO_PIN_0
#define DA14586_FLASH_DO_PIN     GPIO_PIN_9
#define DA14586_FLASH_DI_PIN     GPIO_PIN_4

#define DA14586_FLASH_SIZE            MX25R2035F_TOTAL_FLASH_SIZE
#define DA14586_FLASH_PAGE_SIZE       MX25R2035F_PAGE_SIZE

static void power_down_da14586_flash(void)
{
    // Power up peripherals' power domain
    SetBits16(PMU_CTRL_REG, PERIPH_SLEEP, 0);
    while (!(GetWord16(SYS_STAT_REG) & PER_IS_UP))
        ;

    // Enable the pads
    SetBits16(SYS_CTRL_REG, PAD_LATCH_EN, 1);

    // Configure internal flash GPIOs
    GPIO_ConfigurePin( DA14586_FLASH_PORT,  DA14586_FLASH_EN_PIN,   OUTPUT, PID_SPI_EN, true );
    GPIO_ConfigurePin( DA14586_FLASH_PORT,  DA14586_FLASH_CLK_PIN,  OUTPUT, PID_SPI_CLK, false );
    GPIO_ConfigurePin( DA14586_FLASH_PORT,  DA14586_FLASH_DO_PIN,   OUTPUT, PID_SPI_DO, false );
    GPIO_ConfigurePin( DA14586_FLASH_PORT,  DA14586_FLASH_DI_PIN,   INPUT,  PID_SPI_DI, false );

    // Initialize SPI driver
    SPI_Pad_t cs_pin = { .port = DA14586_FLASH_PORT, .pin = DA14586_FLASH_EN_PIN };
    spi_init(&cs_pin, SPI_MODE_8BIT, SPI_ROLE_MASTER, SPI_CLK_IDLE_POL_LOW, SPI_PHA_MODE_0, SPI_MINT_DISABLE, SPI_XTAL_DIV_8);
    // Initialize SPI flash driver
    spi_flash_init(DA14586_FLASH_SIZE, DA14586_FLASH_PAGE_SIZE);
    // Power up flash
    spi_flash_release_from_power_down();

    // Power down flash
    spi_flash_power_down();
    // Release SPI controller
    spi_release();

    // Release internal flash GPIOs
    GPIO_ConfigurePin( DA14586_FLASH_PORT,  DA14586_FLASH_EN_PIN,   OUTPUT,         PID_GPIO, true );
    GPIO_ConfigurePin( DA14586_FLASH_PORT,  DA14586_FLASH_CLK_PIN,  INPUT_PULLDOWN, PID_GPIO, false );
    GPIO_ConfigurePin( DA14586_FLASH_PORT,  DA14586_FLASH_DO_PIN,   INPUT_PULLDOWN, PID_GPIO, false );
    GPIO_ConfigurePin( DA14586_FLASH_PORT,  DA14586_FLASH_DI_PIN,   INPUT_PULLDOWN, PID_GPIO, false );
}
#endif

/**
 ****************************************************************************************
 * @brief  Global System Init
 * @return void
 ****************************************************************************************
 */
void system_init(void)
{
    sys_startup_flag = true;
    /*
     ************************************************************************************
     * Platform initialization
     ************************************************************************************
     */
    //initialise the Watchdog unit
    wdg_init(0);

    //confirm XTAL 16 MHz calibration
    xtal16_calibration_check ();

    //set the system clocks
    set_system_clocks();

#if defined(VERIFY_OTP_CALIBRATION_VALUES)
    // Check CRC of calibration values in OTP header
    verify_otp_calibration_values();
#endif

    // Initialize ROM variables
    arch_rom_init();
    
#if (USE_RANGE_EXT)
    // Register range extender driver API
    range_ext_init(&range_ext);
#endif

    //Initiliaze the GPIOs
    GPIO_init();

    // Read OTP header fields
    read_otp_header();

#ifdef __DA14586__
    power_down_da14586_flash();
#endif

    //Peripherals initilization
    periph_init();

    // Initialize random process
    co_random_init(1);

    /*
     ************************************************************************************
     * BLE initialization
     ************************************************************************************
     */
    init_pwr_and_clk_ble();

    ble_init(_ble_base);
    
#if (USE_RANGE_EXT)
    // Enable range extender
    range_ext.enable(MAX_POWER, NULL);
#endif

    // Disable sleep and external wake up. We have to do this because rwip_init() sets them both to true.
    rwip_env.sleep_enable = false;
    rwip_env.ext_wakeup_enable = false;

    //Enable the BLE core
    SetBits32(BLE_RWBLECNTL_REG, RWBLE_EN, 1);

    // Initialise random number generator seed using random bits acquired from TRNG
    if (USE_TRNG)
        init_rand_seed_from_trng();

#ifdef DG_DBG_DIAG_PORT
    dbg_init_diag_port();
#endif

    rcx20_calibrate ();

    arch_disable_sleep();

    /*
     ************************************************************************************
     * Application initializations
     ************************************************************************************
     */
    // Initialise APP

#if (BLE_APP_PRESENT)
        app_init();         // Initialize APP
#endif

     if (user_app_main_loop_callbacks.app_on_init !=NULL)
          user_app_main_loop_callbacks.app_on_init();

    //Initialise lld_sleep
    lld_sleep_init_func();

    /*
     ************************************************************************************
     * XTAL16M trimming settings
     ************************************************************************************
     */
    //set trim and bias of xtal16
    xtal16__trim_init();

#if (CFG_RF_DIAG_INT)
     // Enable the TX_EN/RX_EN interrupts, depending on the RF mode of operation (PLL-LUT/MGC_KMODALPHA and VCO coarse calibration combinations)
    enable_rf_diag_irq(RF_DIAG_IRQ_MODE_RXTX);
#endif // CFG_RF_DIAG_INT

    /*
     ************************************************************************************
     * Watchdog
     ************************************************************************************
     */
    if(USE_WDOG)
    {
        wdg_reload(WATCHDOG_DEFAULT_PERIOD);
        wdg_resume ();
    }
}

void arch_wkupct_tweak_deb_time(bool tweak)
{
    if (arch_clk_is_RCX20())
    {
        if (tweak)
        {
            // Recalculate debouncing time to correct silicon bug
            uint32_t temp = (rcx_freq * arch_wkupct_deb_time) / 32768;
            SetBits16(WKUP_CTRL_REG, WKUP_DEB_VALUE, temp & 0x3F);
        }
        else
        {
            SetBits16(WKUP_CTRL_REG, WKUP_DEB_VALUE, arch_wkupct_deb_time & 0x3F);
        }
    }
}

const struct gap_cfg_user_struct gap_cfg_user_var_struct =
{
    .GAP_TMR_LECB_DISCONN_TIMEOUT_VAR=0x0BB8,
    .GAP_APPEARANCE_VAR=0x0000,
    .GAP_PPCP_CONN_INTV_MAX_VAR=0x0064,
    .GAP_PPCP_CONN_INTV_MIN_VAR=0x00c8,
    .GAP_PPCP_SLAVE_LATENCY_VAR=0x0000,
    .GAP_PPCP_STO_MULT_VAR=0x07d0,
    .GAP_TMR_LIM_ADV_TIMEOUT_VAR=0x4650,
    .GAP_TMR_GEN_DISC_SCAN_VAR=0x0300,
    .GAP_TMR_LIM_DISC_SCAN_VAR=0x0300,
    .GAP_TMR_PRIV_ADDR_INT_VAR=0x3A98,
    .GAP_TMR_CONN_PAUSE_CT_VAR=0x0064,
    .GAP_TMR_CONN_PAUSE_PH_VAR=0x01F4,
    .GAP_TMR_CONN_PARAM_TIMEOUT_VAR=0x0BB8,
    .GAP_TMR_LECB_CONN_TIMEOUT_VAR=0x0BB8,
    .GAP_TMR_SCAN_FAST_PERIOD_VAR=0x0C00,
    .GAP_TMR_ADV_FAST_PERIOD_VAR=0x0BB8,
    .GAP_LIM_DISC_SCAN_INT_VAR=0x0012,
    .GAP_SCAN_FAST_INTV_VAR=0x0030,
    .GAP_SCAN_FAST_WIND_VAR=0x0030,
    .GAP_SCAN_SLOW_INTV1_VAR=0x00CD,
    .GAP_SCAN_SLOW_INTV2_VAR=0x019A,
    .GAP_SCAN_SLOW_WIND1_VAR=0x0012,
    .GAP_SCAN_SLOW_WIND2_VAR=0x0024,
    .GAP_ADV_FAST_INTV1_VAR=0x0030,
    .GAP_ADV_FAST_INTV2_VAR=0x0064,
    .GAP_ADV_SLOW_INTV_VAR=0x00B0,
    .GAP_INIT_CONN_MIN_INTV_VAR=0x0018,
    .GAP_INIT_CONN_MAX_INTV_VAR=0x0028,
    .GAP_INQ_SCAN_INTV_VAR=0x0012,
    .GAP_INQ_SCAN_WIND_VAR=0x0012,
    .GAP_CONN_SUPERV_TIMEOUT_VAR=0x07D0,
    .GAP_CONN_MIN_CE_VAR=0x0000,
    .GAP_CONN_MAX_CE_VAR=0xFFFF,
    .GAP_CONN_LATENCY_VAR=0x0000,
    //.GAP_LE_MASK_VAR=0x1F,
    .GAP_MAX_LE_MTU_VAR=512,
#ifdef __DA14586__
    .GAP_DEV_NAME_VAR="DA14586",
#else
    .GAP_DEV_NAME_VAR="DA14585",
#endif
};

/// @}
