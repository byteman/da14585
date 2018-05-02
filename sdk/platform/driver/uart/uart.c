/**
 ****************************************************************************************
 *
 * @file uart.c
 *
 * @brief UART driver
 *
 * Copyright (C) RivieraWaves 2009-2012
 * Copyright (C) 2017 Modified by Dialog Semiconductor
 *
 * $Rev: 6880 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup UART
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stddef.h>     // standard definition
#include <stdint.h>
#include "uart.h"       // uart definition
#include "reg_uart.h"   // uart register
#include "hcic_eif.h"   // hci uart definition
#include "rwble_config.h"
/*
 * DEFINES
 *****************************************************************************************
 */

/*
 * ENUMERATION DEFINITIONS
 *****************************************************************************************
 */

enum UART_ID
{
    MODEM_STAT         = 0,
    NO_INT_PEND        = 1,
    THR_EMPTY          = 2,
    RECEIVED_AVAILABLE = 4,
    UART_TIMEOUT       = 12
};

/// RX_LVL values
enum UART_RXLVL
{
    UART_RXLVL_1,
    UART_RXLVL_4,
    UART_RXLVL_8,
    UART_RXLVL_14
};

/// WORD_LEN values
enum UART_WORDLEN
{
    UART_WORDLEN_5,
    UART_WORDLEN_6,
    UART_WORDLEN_7,
    UART_WORDLEN_8
};

/// FIFO_SZ values
enum UART_FIFOSIZE
{
    UART_FIFOSIZE_16,
    UART_FIFOSIZE_32,
    UART_FIFOSIZE_64,
    UART_FIFOSIZE_128
};

/*
 * STRUCT DEFINITIONS
 *****************************************************************************************
 */

/* TX and RX channel class holding data used for asynchronous read and write data
 * transactions
 */
/// UART TX RX Channel
struct uart_txrxchannel
{
    /// size
    uint32_t  size;
    /// buffer pointer
    uint8_t  *bufptr;
    /// call back function pointer
    void (*callback) (uint8_t);
};

/// UART environment structure
struct uart_env_tag
{
    /// tx channel
    struct uart_txrxchannel tx;
    /// rx channel
    struct uart_txrxchannel rx;
    /// error detect
    uint8_t errordetect;
    //flow off retries
    uint32_t flow_off_retries;
    //flow off retries_limit
    uint32_t flow_off_retries_limit;
};

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// uart environment structure
struct uart_env_tag uart_env __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY


/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Serves the receive data available interrupt requests. It clears the requests and
 *        executes the callback function.
 *
 ****************************************************************************************
 */
static void uart_rec_data_avail_isr(void)
{
    void (*callback) (uint8_t) = NULL;

    while (uart_data_rdy_getf())
    {
        // Read the received in the FIFO
        *uart_env.rx.bufptr = uart_rxdata_getf();

        // Update RX parameters
        uart_env.rx.size--;
        uart_env.rx.bufptr++;

        // Check if all expected data have been received
        if (uart_env.rx.size == 0)
        {
            // Reset RX parameters
            uart_env.rx.bufptr = NULL;

            // Disable Rx interrupt
            uart_rec_data_avail_setf(0);

            // Retrieve callback pointer
            callback = uart_env.rx.callback;

            if(callback != NULL)
            {
                // Clear callback pointer
                uart_env.rx.callback = NULL;

                // Call handler
                callback(UART_STATUS_OK);
            }
            else
            {
                ASSERT_ERR(0);
            }

            // Exit loop
            break;
        }
    }
}


/**
 ****************************************************************************************
 * @brief Serves the receive data error interrupt requests. It clears the requests and
 *        executes the callback function.
 *
 ****************************************************************************************
 */
static void uart_rec_error_isr(void)
{
    void (*callback) (uint8_t) = NULL;
    // Reset RX parameters
    uart_env.rx.size = 0;
    uart_env.rx.bufptr = NULL;

    // Disable RX interrupt
    SetBits16(UART_IER_DLH_REG, ERBFI_dlh0, 0); // uart_rec_data_avail_setf(0);

    // Retrieve callback pointer
    callback = uart_env.rx.callback;

    if(callback != NULL)
    {
        // Clear callback pointer
        uart_env.rx.callback = NULL;

        // Call handler
        callback(UART_STATUS_ERROR);
    }
    else
    {
        ASSERT_ERR(0);
    }
}


/**
 ****************************************************************************************
 * @brief Serves the transmit data fill interrupt requests. It clears the requests and
 *        executes the callback function.
 *
 * The callback function is called as soon as the last byte of the provided data is
 * put into the FIFO. The interrupt is disabled at the same time.
 ****************************************************************************************
 */
static void uart_thr_empty_isr(void)
{
    void (*callback) (uint8_t) = NULL;
    // Fill TX FIFO until there is no more room inside it
    while (uart_txfifo_full_getf())
    {
        // Put a byte in the FIFO
        uart_txdata_setf(*uart_env.tx.bufptr);

        // Update TX parameters
        uart_env.tx.size--;
        uart_env.tx.bufptr++;

        if (uart_env.tx.size == 0)
        {
            // Reset TX parameters
            uart_env.tx.bufptr = NULL;

            // Disable TX interrupt
            uart_thr_empty_setf(0);

            // Retrieve callback pointer
            callback = uart_env.tx.callback;

            if(callback != NULL)
            {
                // Clear callback pointer
                uart_env.tx.callback = NULL;

                // Call handler
                callback(UART_STATUS_OK);
            }
            else
            {
                ASSERT_ERR(0);
            }

            // Exit loop
            break;
        }
    }
}


/**
 ****************************************************************************************
 * @brief Check if RX FIFO is empty.
 *
 * @return FIFO empty      false: not empty  / true: empty
 *****************************************************************************************
 */
static bool uart_is_rx_fifo_empty(void)
{
    return (uart_data_rdy_getf() == 0);
}


/*
 * EXPORTED FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void uart_init_func(uint16_t baudr, uint8_t dlf_value, uint8_t mode)
{
    // Initialize UART component
    SetBits16(CLK_PER_REG, UART1_ENABLE, 1);  // enable clock - always @16MHz


    //ENABLE FIFO, REGISTER FCR IF UART_LCR_REG.DLAB=0
    // XMIT FIFO RESET, RCVR FIFO RESET, FIFO ENABLED
    SetBits16(UART_LCR_REG, UART_DLAB, 0);
    SetWord16(UART_IIR_FCR_REG,7);

    //DISABLE INTERRUPTS, REGISTER IER IF UART_LCR_REG.DLAB=0
    SetWord16(UART_IER_DLH_REG, 0);

    // ACCESS DIVISORLATCH REGISTER FOR BAUDRATE, REGISTER UART_DLH/DLL_REG IF UART_LCR_REG.DLAB=1
    SetBits16(UART_LCR_REG, UART_DLAB, 1);
    SetWord16(UART_IER_DLH_REG, (baudr >> 8) & 0xFF);
    SetWord16(UART_RBR_THR_DLL_REG, baudr & 0xFF);

    // NO PARITY, 1 STOP BIT, 8 DATA LENGTH AND
    SetWord16(UART_LCR_REG,mode);
    SetWord16(UART_DLF_REG,dlf_value);
    //ENABLE TX INTERRUPTS, REGISTER IER IF UART_LCR_REG.DLAB=0
    SetBits16(UART_LCR_REG, UART_DLAB, 0);
    //SetBits16(UART_IER_DLH_REG, ETBEI_dlh1, 1);

    NVIC_DisableIRQ(UART_IRQn);
    NVIC_SetPriority(UART_IRQn,1);
    NVIC_EnableIRQ(UART_IRQn);
    NVIC_ClearPendingIRQ(UART_IRQn);

    // Configure UART environment
#if 0
    uart_env.errordetect = UART_ERROR_DETECT_DISABLED;
    uart_env.rx.bufptr = NULL;
    uart_env.rx.size = 0;
    uart_env.tx.bufptr = NULL;
    uart_env.tx.size = 0;
#endif
}


void uart_flow_on_func(void)
{
    // Configure modem (HW flow control enable)
    SetWord32(UART_MCR_REG, UART_AFCE|UART_RTS);
}


bool uart_flow_off_func(void)
{
    volatile unsigned i;
    bool flow_off = true;

    do
    {
        // First check if no transmission is ongoing
        if ((uart_temt_getf() == 0) || (uart_thre_getf() == 0))
        {
            if(uart_env.flow_off_retries_limit == 0 || (++uart_env.flow_off_retries < uart_env.flow_off_retries_limit))
            {
                flow_off = false;
                break;
            }
        }

        uart_env.flow_off_retries = 0;

        // Configure modem (HW flow control disable, 'RTS flow off')
#if 0
        uart_mcr_pack(UART_ENABLE,     // extfunc
                      UART_DISABLE,    // autorts
                      UART_ENABLE,     // autocts
                      UART_DISABLE);   // rts
#endif
        SetWord32(UART_MCR_REG, 0);

        // Wait for 1 character duration to ensure host has not started a transmission at the
        // same time
        for (i=0;i<350;i++);

        #ifndef __GNUC__
//        timer_wait(UART_CHAR_DURATION);
        #endif //__GNUC__

        // Check if data has been received during wait time
        if(!uart_is_rx_fifo_empty())
        {
            // Re-enable UART flow
            uart_flow_on();

            // We failed stopping the flow
            flow_off = false;
        }
    } while(false);

    return (flow_off);
}


void uart_finish_transfers_func(void)
{
    // Configure modem (HW flow control disable, 'RTS flow off')
    uart_mcr_pack(UART_ENABLE,     // extfunc
                  UART_DISABLE,    // autorts
                  UART_ENABLE,     // autocts
                  UART_DISABLE);   // rts

    // Wait TX FIFO empty
    while(!uart_thre_getf() || !uart_temt_getf());
}


void uart_read_func(uint8_t *bufptr, uint32_t size,void (*callback) (uint8_t))
{
    // Sanity check
    ASSERT_ERR(bufptr != NULL);
    ASSERT_ERR(size != 0);
    ASSERT_ERR(uart_env.rx.bufptr == NULL);

    // Prepare RX parameters
    uart_env.rx.size = size;
    uart_env.rx.bufptr = bufptr;
    uart_env.rx.callback = callback;



    // Start data transaction
    uart_rec_data_avail_setf(1); //=SetBits16(UART_IER_DLH_REG, ETBEI_dlh0, 1);
}


void uart_write_func(uint8_t *bufptr, uint32_t size, void (*callback) (uint8_t))
{
    // Sanity check
    ASSERT_ERR(bufptr != NULL);
    ASSERT_ERR(size != 0);
    ASSERT_ERR(uart_env.tx.bufptr == NULL);

    // Prepare TX parameters
    uart_env.tx.size = size;
    uart_env.tx.bufptr = bufptr;
    uart_env.tx.callback = callback;


    /* start data transaction
     * first isr execution is done without interrupt generation to reduce
     * interrupt load
     */
    uart_thr_empty_isr();
    if (uart_env.tx.bufptr != NULL)
    {
        uart_thr_empty_setf(1);
    }
}


void UART_Handler_func(void)
{
    uint32_t idd;
    idd = 0x0F & GetWord32(UART_IIR_FCR_REG);
    if(idd!=NO_INT_PEND)
    {
        switch(idd)
        {
          case UART_TIMEOUT:
            if ((uart_env.errordetect == UART_ERROR_DETECT_ENABLED) && uart_fifo_err_getf())
            {
                uart_rec_error_isr();
            }
            break;
          case RECEIVED_AVAILABLE:
            uart_rec_data_avail_isr();
            break;

          case THR_EMPTY:
            uart_thr_empty_isr();
            break;

          default:
            break;
        }
    }
}


void uart_set_flow_off_retries_limit(uint32_t limit)
{
    uart_env.flow_off_retries_limit = limit;
}
/// @} UART
