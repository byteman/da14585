/**
 ****************************************************************************************
 *
 * @file dma.h
 *
 * @brief Implementation of the DMA Low Level Driver - Header file
 *
 * Copyright (C) 2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef DMA_H_
#define DMA_H_

#include <stdint.h>
#include <stddef.h>        // for NULL
#include "stdbool.h"
#include "ll.h"
#include "datasheet.h"

/*
* ENUMERATION DEFINITIONS
*****************************************************************************************
*/

/**
 * \brief DMA channel number
 *
 */
typedef enum 
{
    DMA_CHANNEL_0 = 0,   /**< Channel number 0 */
    DMA_CHANNEL_1 = 1,   /**< Channel number 1 */
    DMA_CHANNEL_2 = 2,   /**< Channel number 2 */
    DMA_CHANNEL_3 = 3,   /**< Channel number 3 */
    DMA_CHANNEL_4 = 4,   /**< Channel number 4 */
    DMA_CHANNEL_5 = 5,   /**< Channel number 5 */
    DMA_CHANNEL_6 = 6,   /**< Channel number 6 */
    DMA_CHANNEL_7 = 7,    /**< Channel number 7 */
    DMA_CHANNEL_INVALID = 8 /**< Invalid Channel number */
} dma_channel_t;

/**
 * \brief DMA channel enable/disable
 *
 */
typedef enum
{
    DMA_STATE_DISABLED = 0x0,    /**< DMA disabled */
    DMA_STATE_ENABLED = 0x1      /**< DMA enabled */
} dma_state_t;

/**
 * \brief DMA channel bus width transfer
 *
 */
typedef enum
{
    DMA_BW_BYTE = 0x0,           /**< Byte */
    DMA_BW_HALFWORD = 0x2,       /**< Halfword */
    DMA_BW_WORD = 0x4            /**< Word */
} dma_bw_t;

/**
 * \brief DMA channel interrupt enable/disable
 *
 */
typedef enum
{
    DMA_IRQ_STATE_DISABLED = 0x0,        /**< Disable interrupt on this channel */
    DMA_IRQ_STATE_ENABLED = 0x8          /**< Enable interrupt on this channel */
} dma_irq_state_t;

/**
 * \brief DMA request input multiplexer controlled
 *
 */
typedef enum
{
    DMA_DREQ_START = 0x0,        /**< DMA channel starts immediately */
    DMA_DREQ_TRIGGERED = 0x10    /**< DMA channel must be triggered by peripheral DMA request */
} dma_dreq_t;

/**
 * \brief Increment destination address mode
 *
 */
typedef enum
{
    DMA_BINC_FALSE = 0x0,        /**< Do not increment */
    DMA_BINC_TRUE  = 0x20        /**< Increment according value of BW */
} dma_binc_t;

/**
 * \brief Increment of source address mode
 *
 */
typedef enum
{
    DMA_AINC_FALSE = 0x0,        /**< Do not increment */
    DMA_AINC_TRUE  = 0x40        /**< Increment according value of BW */
} dma_ainc_t;

/**
 * \brief Channel mode
 *
 * In normal mode the DMA transfer stops the transfer after
 * length DMAx_LEN_REG.
 * In circular mode the DMA channel repeats the transfer
 * after length DMAx_LEN_REG with the initial register values
 * DMAx_A_START_REG, DMAx_B_START_REG, DMAx_LEN_REG, DMAx_INT_REG.
 *
 * \note only works if DREQ_MODE = 1
 *
 */
typedef enum
{
    DMA_MODE_NORMAL = 0x0,       /**< Normal mode */
    DMA_MODE_CIRCULAR = 0x80     /**< Circular mode */
} dma_mode_t;

/**
 * \brief Channel priority
 *
 * Set priority level of DMA channel to determine which DMA
 * channel will be activated in case more than one DMA channel
 * requests DMA.
 *
 */
typedef enum
{
    DMA_PRIO_0 = 0x000,  /**< Lowest priority */
    DMA_PRIO_1 = 0x100,  /**< .. */
    DMA_PRIO_2 = 0x200,  /**< .. */
    DMA_PRIO_3 = 0x300,  /**< .. */
    DMA_PRIO_4 = 0x400,  /**< .. */
    DMA_PRIO_5 = 0x500,  /**< .. */
    DMA_PRIO_6 = 0x600,  /**< .. */
    DMA_PRIO_7 = 0x700   /**< Highest priority */
} dma_prio_t;

/**
 * \brief DMA idle mode
 *
 * In blocking mode the DMA performs a fast back-to-back
 * copy, disabling bus access for any bus master with lower priority.
 * In interrupting mode the DMA inserts a wait cycle after each
 * store allowing the CR16 to steal cycles or cache to perform a
 * burst read.
 *
 * \note if DREQ_MODE = 1, DMA_IDLE does not have any effect
 *
 */
typedef enum
{
    DMA_IDLE_BLOCKING_MODE = 0x000,      /**< Blocking mode */
    DMA_IDLE_INTERRUPTING_MODE = 0x800   /**< Interrupting mode */
} dma_idle_t;

/**
 * \brief DMA init mode
 *
 */
typedef enum
{
    DMA_INIT_AX_BX_AY_BY = 0x0000,       /**< DMA performs copy A1 to B1, A2 to B2 */
    DMA_INIT_AX_BX_BY    = 0x1000        /**< DMA performs copy A1 to B1, B2 */
} dma_init_t;

/**
 * \brief Channel request trigger
 *
 */
typedef enum
{
    DMA_TRIG_SPI_RXTX = 0x0,
    DMA_TRIG_UART_RXTX = 0x2,
    DMA_TRIG_UART2_RXTX = 0x3,
    DMA_TRIG_I2C_RXTX = 0x4,
    DMA_TRIG_I2S_LEFTRIGHT = 0x8,
    DMA_TRIG_PDM_LEFTRIGHT = 0x9,
    DMA_TRIG_NONE = 0xF
} dma_trig_t;

/**
 * \brief Channel Request Sense
 *
 */
typedef enum
{
    DMA_SENSE_LEVEL_SENSITIVE = 0x0,
    DMA_SENSE_POSITIVE_EDGE_SENSITIVE = (1<<13),
} dma_sense_t;
/**
 ****************************************************************************************
 * \brief   DMA channel transfer callback
 *
 * \details This function is called by the DMA driver when the
 *          interrupt is fired.
 *
 * \param   [in]  user_data transfered data
 * \param   [in]  len length of transfered data
 *
 * \return  None 
 *
 ****************************************************************************************
 */
typedef void (*hw_dma_transfer_cb)(void *user_data, uint16_t len);

/**
 * \brief DMA parameters structure
 *
 */
typedef struct
{
    dma_channel_t       channel_number;         /**< DMA Channel Number to be used */
    dma_bw_t            bus_width;              /**< Transfer Bus width: 8, 16 or 32 bits */
    dma_irq_state_t     irq_enable;             /**< Enable or disable IRQ generation */
    uint16_t            irq_nr_of_trans;        /**< Number of transfers before IRQ generation
                                                 set to 0 to fire IRQ after transfer ends */
    dma_dreq_t          dreq_mode;              /**< Start DMA immediately or triggered by peripheral */
    dma_ainc_t          a_inc;                  /**< Increment of source address */
    dma_binc_t         	b_inc;                  /**< Increment of destination address */
    dma_mode_t          circular;               /**< Select normal or circular operation */
    dma_prio_t          dma_prio;               /**< Channel priority from 0 to 7 */
    dma_idle_t          dma_idle;               /**< Idle mode: blocking or interrupting */
    dma_init_t          dma_init;               /**< Copy mode: block copy or mem init */
    dma_trig_t          dma_req_mux;            /**< DMA trigger */
    dma_sense_t         dma_sense;              /**< DMA sensitivity level or edge */
    uint32_t            src_address;            /**< Source address */
    uint32_t            dest_address;           /**< Destination address */
    uint16_t            length;                 /**< Number of DMA transfers */
    hw_dma_transfer_cb  callback;               /**< Function to call after irq_nr_of_trans transfers */
    void                *user_data;             /**< Data to pass to Callback */
} DMA_setup;

/*
* API FUNCTIONS DEFINITIONS
*****************************************************************************************
*/

/**
 ****************************************************************************************
 * \brief   Initialize DMA Channel
 *
 * \param   [in] channel_setup pointer to struct of type DMA_Setup
 *
 * \return  None
 *
 ****************************************************************************************
 */
void dma_channel_initialization(DMA_setup *channel_setup);

/**
 ****************************************************************************************
 * \brief   Update DMA source address and length
 *
 * \details When DMA is configured for some peripheral, it could be enough to setup only source 
 *          address and data length. Other parameters most likely do not change for same type 
 *          of transmission for values that ware specified in \p dma_channel_initialization().
 *          This function should speed up DMA start time when only address and size changes 
 *          from previous transmission.
 *
 * \param   [in] channel DMA channel number to modify
 * \param   [in] addr new source address
 * \param   [in] length new data transfer length
 * \param   [in] cb function to call after transmission finishes
 *
 * \return  None
 * 
 ****************************************************************************************
 */
void dma_channel_update_source(dma_channel_t channel, void* addr, uint16_t length,
                                                                        hw_dma_transfer_cb cb);

/**
 ****************************************************************************************
 * \brief   Update DMA destination address and length
 *
 * \details When DMA is configured for some peripheral, it could be enough to setup
 *          only destination address and data length. Other parameters most likely
 *          do not change for same type of transmission for values that ware specified
 *          in \p dma_channel_initialization(). This function should speed up DMA
 *          start time when only address and size changes from previous transmission.
 *
 * \param   [in]  channel DMA channel number to modify
 * \param   [in]  addr new source address
 * \param   [in]  length new data transfer length
 * \param   [in]  cb function to call after transmission finishes
 *
 * \return  None
 *
 ****************************************************************************************
 */
void dma_channel_update_destination(dma_channel_t channel, void *addr, uint16_t length,
                                                                        hw_dma_transfer_cb cb);

/**
 ****************************************************************************************
 * \brief   Update DMA interrupt trigger index
 *
 * \details DMA channel can trigger an interrupt after arbitrary transfer has finished.
 *          Usually interrupt is triggered after transmission finishes but for cyclic mode,
 *          where DMA never stops, it is convenient trigger interrupt at other times.
 *          This function allows to specify the number of transfers after which the interrupt is triggered.
 *
 * \param   [in]  channel DMA channel number to modify
 * \param   [in]  int_ix Number of transfers until the interrupt is triggered
 *
 * \return  None
 *
 ****************************************************************************************
 */
void dma_channel_update_int_ix(dma_channel_t channel, uint16_t int_ix);


/**
 ****************************************************************************************
 * \brief   Enables or disables a DMA channel
 *
 * \param   [in]  channel_number DMA channel number to start/stop
 * \param   [in]  dma_on enable/disable DMA channel  
 *
 * \return  None
 *
 ****************************************************************************************
 */ 
void dma_channel_enable(dma_channel_t channel_number, dma_state_t dma_on);

/**
 ****************************************************************************************
 * \brief   Stop DMA channel if operation is in progress
 *
 * \details If no transfer is in progress nothing happens.
 *          If there is outstanding DMA transfer it will be stopped and
 *          callback will be called with count of data already transfered
 *
 * \param   [in] channel_number DMA channel number to stop
 *
 ****************************************************************************************
 */
void dma_channel_stop(dma_channel_t channel_number);

/**
 ****************************************************************************************
 * \brief   Read number of transmitted bytes so far
 *
 * \details Use this function to see how many bytes were transfered
 *          via DMA channel so far. This number can changed very soon.
 *
 * \param   [in] channel_number DMA channel number
 *
 * \return  uint16_t
 * 
 * \retval  number of bytes already transfered (when transfer is in progress),
 *          0 - if transfer is already finished,
 *          undefined if called or not started channel
 *
 ****************************************************************************************
 */
 uint16_t dma_channel_transfered_bytes(dma_channel_t channel_number);

/**
 ****************************************************************************************
 * \brief   Freezes the DMA 
 *
 * \param   None
 *
 * \return  None
 *
 ****************************************************************************************
 */
static inline void dma_freeze(void)
{
    SetBits16(SET_FREEZE_REG, FRZ_DMA, 1);
}


/**
 ****************************************************************************************
 * \brief   Unfreezes the DMA 
 *
 * \param   None
 *
 * \return  None
 *
 ****************************************************************************************
 */
static inline void dma_unfreeze(void)
{
    SetBits16(RESET_FREEZE_REG, FRZ_DMA, 1);
}

/**
 ****************************************************************************************
 * \brief   Checks whether a DMA channel is enabled (active) or not
 *
 * \param   None  
 *
 * \return  bool
 *
 * \retval  Channel enabled/disabled
 *          <ul>
 *              <li> true if the DMA channel is enabled
 *              <li> false if the DMA channel is disabled
 *          </ul>
 ****************************************************************************************
 */
bool dma_channel_active(void);

/**
 ****************************************************************************************
 * \brief   DMA initialization minimal
 *
 * \param   channel_setup The setup of the channel
 *
 * \return  None
 *
 ****************************************************************************************
 */
void dma_channel_initialization_minimal(DMA_setup *channel_setup);

#endif /* HW_DMA_H_ */

