/**
 ****************************************************************************************
 *
 * @file dma.h
 *
 * @brief Implementation of the DMA Low Level Driver
 *
 * Copyright (C) 2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */
 
#define HW_CONFIG_DMA_CHANNELS_COUNT 4

#include "dma.h"

static struct hw_dma_callback_data {
        hw_dma_transfer_cb callback;
        void *user_data;
} dma_callbacks_user_data[HW_CONFIG_DMA_CHANNELS_COUNT];

#define DMA_CHN_REG(reg, chan) ((volatile uint16_t *)(reg) + ((chan) * 8))

void dma_channel_initialization(DMA_setup *channel_setup)
{
    volatile uint16_t *dma_x_ctrl_reg;
    volatile uint16_t *dma_x_a_start_low_reg;
    volatile uint16_t *dma_x_a_start_high_reg;
    volatile uint16_t *dma_x_b_start_low_reg;
    volatile uint16_t *dma_x_b_start_high_reg;
    volatile uint16_t *dma_x_len_reg;
    volatile uint16_t *dma_x_int_reg;
    uint32_t src_address;
    uint32_t dest_address;

    /* Make sure the DMA channel length is not zero */
    ASSERT_WARNING(channel_setup->length > 0);

    // Look up DMAx_CTRL_REG address
    dma_x_ctrl_reg = DMA_CHN_REG(DMA0_CTRL_REG, channel_setup->channel_number);

    // Look up DMAx_A_STARTL_REG address
    dma_x_a_start_low_reg = DMA_CHN_REG(DMA0_A_STARTL_REG, channel_setup->channel_number);

    // Look up DMAx_A_STARTH_REG address
    dma_x_a_start_high_reg = DMA_CHN_REG(DMA0_A_STARTH_REG, channel_setup->channel_number);

    // Look up DMAx_B_STARTL_REG address
    dma_x_b_start_low_reg = DMA_CHN_REG(DMA0_B_STARTL_REG, channel_setup->channel_number);

    // Look up DMAx_B_STARTH_REG address
    dma_x_b_start_high_reg = DMA_CHN_REG(DMA0_B_STARTH_REG, channel_setup->channel_number);

    // Look up DMAx_LEN_REG address
    dma_x_len_reg = DMA_CHN_REG(DMA0_LEN_REG, channel_setup->channel_number);

    // Look up DMAx_INT
    dma_x_int_reg = DMA_CHN_REG(DMA0_INT_REG, channel_setup->channel_number);

    // Make sure DMA channel is disabled first
    SetBits16(dma_x_ctrl_reg, DMA_ON, DMA_STATE_DISABLED);

    // Set DMAx_CTRL_REG width provided settings, but do not start the channel.
    // Start the channel with the "dma_channel_enable" function separately.
    *dma_x_ctrl_reg =
        channel_setup->bus_width |
        channel_setup->irq_enable |
        channel_setup->dreq_mode |
        channel_setup->b_inc |
        channel_setup->a_inc |
        channel_setup->circular |
        channel_setup->dma_prio |
        channel_setup->dma_idle |
        channel_setup->dma_init |
        channel_setup->dma_sense;
    
    // Set DMA_REQ_MUX_REG for the requested channel / trigger combination
    if(channel_setup->dma_req_mux != DMA_TRIG_NONE)
    {
        switch (channel_setup->channel_number)
        {
            case DMA_CHANNEL_0:
            case DMA_CHANNEL_1:
                    GLOBAL_INT_DISABLE();
                    SetBits16(DMA_REQ_MUX_REG, DMA01_SEL, channel_setup->dma_req_mux);
                    GLOBAL_INT_RESTORE();
                    break;
            case DMA_CHANNEL_2:
            case DMA_CHANNEL_3:
                    GLOBAL_INT_DISABLE();
                    SetBits16(DMA_REQ_MUX_REG, DMA23_SEL, channel_setup->dma_req_mux);
                    GLOBAL_INT_RESTORE();
                    break;
            default:
                    break;
        }
        /*
         * When different DMA channels are used for same device it is important
         * that only one trigger is set for specific device at a time.
         * Having same trigger for different channels can cause unpredictable results.
         * Following code also should help when SPI1 is assigned to non 0 channel.
         */
        GLOBAL_INT_DISABLE();
        switch (channel_setup->channel_number)
        {
            case DMA_CHANNEL_2:
            case DMA_CHANNEL_3:            
                if (GetBits16(DMA_REQ_MUX_REG, DMA01_SEL) == channel_setup->dma_req_mux) {
                        SetBits16(DMA_REQ_MUX_REG, DMA01_SEL, DMA_TRIG_NONE);
                }
                break;
            case DMA_CHANNEL_0:
            case DMA_CHANNEL_1:
            default:
                break;
        }
        GLOBAL_INT_RESTORE();
    }
    src_address = channel_setup->src_address;
    dest_address = channel_setup->dest_address;

    // Set source address registers
    *dma_x_a_start_low_reg = (src_address & 0xffff);
    *dma_x_a_start_high_reg = (src_address >> 16);

    // Set destination address registers
    *dma_x_b_start_low_reg = (dest_address & 0xffff);
    *dma_x_b_start_high_reg = (dest_address >> 16);

    // Set IRQ number of transfers
    if (channel_setup->irq_nr_of_trans > 0)
    {
        // If user explicitly set this number use it
        *dma_x_int_reg = channel_setup->irq_nr_of_trans - 1;
    } else
    {
        // If user passed 0, use transfer length to fire interrupt after transfer ends
        *dma_x_int_reg = channel_setup->length - 1;
    }

    // Set the transfer length
    *dma_x_len_reg = (channel_setup->length) - 1;

    if (channel_setup->irq_enable)
        dma_callbacks_user_data[channel_setup->channel_number].callback = channel_setup->callback;
    else
        dma_callbacks_user_data[channel_setup->channel_number].callback = NULL;
    dma_callbacks_user_data[channel_setup->channel_number].user_data = channel_setup->user_data;
}

void dma_channel_update_source(dma_channel_t channel, void *addr, uint16_t length,
                                                                        hw_dma_transfer_cb cb)
{
    uint32_t phy_addr = (uint32_t) addr;

    dma_callbacks_user_data[channel].callback = cb;

    // Look up DMAx_A_STARTL_REG address
    volatile uint16_t *dma_x_a_start_low_reg = DMA_CHN_REG(DMA0_A_STARTL_REG, channel);

    // Look up DMAx_A_STARTH_REG address
    volatile uint16_t *dma_x_a_start_high_reg = DMA_CHN_REG(DMA0_A_STARTH_REG, channel);

    // Look up DMAX_LEN_REG address
    volatile uint16_t *dma_x_len_reg = DMA_CHN_REG(DMA0_LEN_REG, channel);


    volatile uint16_t *dma_x_int_reg = DMA_CHN_REG(DMA0_INT_REG, channel);

    // Set source address registers
    *dma_x_a_start_low_reg = (phy_addr & 0xffff);
    *dma_x_a_start_high_reg = (phy_addr >> 16);

    *dma_x_int_reg = length - 1;

    // Set the transfer length
    *dma_x_len_reg = length - 1;
}

void dma_channel_initialization_minimal(DMA_setup *channel_setup)
{
    volatile uint16_t *dma_x_ctrl_reg;
    volatile uint16_t *dma_x_b_start_low_reg;
    volatile uint16_t *dma_x_b_start_high_reg;

    uint32_t dest_address;

    // Look up DMAx_CTRL_REG address
    dma_x_ctrl_reg = DMA_CHN_REG(DMA0_CTRL_REG, channel_setup->channel_number);

    // Look up DMAx_B_STARTL_REG address
    dma_x_b_start_low_reg = DMA_CHN_REG(DMA0_B_STARTL_REG, channel_setup->channel_number);

    // Look up DMAx_B_STARTH_REG address
    dma_x_b_start_high_reg = DMA_CHN_REG(DMA0_B_STARTH_REG, channel_setup->channel_number);

    // Make sure DMA channel is disabled first
    SetBits16(dma_x_ctrl_reg, DMA_ON, DMA_STATE_DISABLED);

    dest_address = channel_setup->dest_address;

    // Set destination address registers
    *dma_x_b_start_low_reg = (dest_address & 0xffff);
    *dma_x_b_start_high_reg = (dest_address >> 16);

    if (channel_setup->irq_enable)
        dma_callbacks_user_data[channel_setup->channel_number].callback = channel_setup->callback;
    else
        dma_callbacks_user_data[channel_setup->channel_number].callback = NULL;
    dma_callbacks_user_data[channel_setup->channel_number].user_data = channel_setup->user_data;
}

void dma_channel_update_destination(dma_channel_t channel, void *addr, uint16_t length,
                                                                        hw_dma_transfer_cb cb)
{
    uint32_t phy_addr = (uint32_t) addr;

    dma_callbacks_user_data[channel].callback = cb;

    // Look up DMAx_B_STARTL_REG address
    volatile uint16_t *dma_x_b_start_low_reg = DMA_CHN_REG(DMA0_B_STARTL_REG, channel);

    // Look up DMAx_B_STARTH_REG address
    volatile uint16_t *dma_x_b_start_high_reg = DMA_CHN_REG(DMA0_B_STARTH_REG, channel);

    // Look up DMAX_LEN_REG address
    volatile uint16_t *dma_x_len_reg = DMA_CHN_REG(DMA0_LEN_REG, channel);
    volatile uint16_t *dma_x_int_reg = DMA_CHN_REG(DMA0_INT_REG, channel);

    // Set destination address registers
    *dma_x_b_start_low_reg = (phy_addr & 0xffff);
    *dma_x_b_start_high_reg = (phy_addr >> 16);

    *dma_x_int_reg = length - 1;

    // Set the transfer length
    *dma_x_len_reg = length - 1;
}

void dma_channel_update_int_ix(dma_channel_t channel, uint16_t int_ix)
{
    volatile uint16_t *dma_x_int_reg = DMA_CHN_REG(DMA0_INT_REG, channel);

    *dma_x_int_reg = int_ix;
}

void dma_channel_enable(dma_channel_t channel_number, dma_state_t dma_on)
{
    volatile uint16_t *dma_x_ctrl_reg;

    // Look up DMAx_CTRL_REG address
    dma_x_ctrl_reg = DMA_CHN_REG(DMA0_CTRL_REG, channel_number);

    if (dma_on == DMA_STATE_ENABLED)
    {
        uint16_t dma_ctrl = *dma_x_ctrl_reg;
        SetBits16(&dma_ctrl, DMA_ON, 1);
        if (dma_callbacks_user_data[channel_number].callback)
        {
            SetBits16(&dma_ctrl, IRQ_ENABLE, 1);
        }
        // Start the chosen DMA channel
        *dma_x_ctrl_reg = dma_ctrl;
        NVIC_EnableIRQ(DMA_IRQn);
    } 
    else
    {
        // Stop the chosen DMA channel
        SetBits16(dma_x_ctrl_reg, DMA_ON, 0);
        SetBits16(dma_x_ctrl_reg, IRQ_ENABLE, 0);
    }
}


/**
 ****************************************************************************************
 * \brief   DMA helper function
 *
 * \param   [in]  
 *
 * \return  None
 *
 ****************************************************************************************
 */
static inline void dma_helper(dma_channel_t channel_number, uint16_t len, bool stop_dma)
{
    hw_dma_transfer_cb cb;

    NVIC_DisableIRQ(DMA_IRQn);
    cb = dma_callbacks_user_data[channel_number].callback;
    if (stop_dma)
    {
        dma_callbacks_user_data[channel_number].callback = NULL;
        dma_channel_enable(channel_number, DMA_STATE_DISABLED);
    }
    if (cb)
    {
        cb(dma_callbacks_user_data[channel_number].user_data, len);
    }
    NVIC_EnableIRQ(DMA_IRQn);
}

bool dma_channel_active(void)
{
    int dma_on;

    dma_on =  GetBits16(DMA0_CTRL_REG, DMA_ON);
    dma_on |= GetBits16(DMA1_CTRL_REG, DMA_ON);
    dma_on |= GetBits16(DMA2_CTRL_REG, DMA_ON);
    dma_on |= GetBits16(DMA3_CTRL_REG, DMA_ON);
#if (HW_CONFIG_DMA_CHANNELS_COUNT > 4)
    dma_on |= GetBits16(DMA4_CTRL_REG, DMA_ON);
    dma_on |= GetBits16(DMA5_CTRL_REG, DMA_ON);
#endif        
#if (HW_CONFIG_DMA_CHANNELS_COUNT > 6)    
    dma_on |= GetBits16(DMA6_CTRL_REG, DMA_ON);         
    dma_on |= GetBits16(DMA7_CTRL_REG, DMA_ON);
#endif        
    return (dma_on == 1);
}

/**
 ****************************************************************************************
 * \brief   Capture DMA Interrupt Handler 
 *
 * \details Calls the user interrupt handler
 *
 * \param   None 
 *
 * \return  None
 *
 ****************************************************************************************
 */
void DMA_Handler(void)
{
    uint16_t risen;
    volatile uint16_t *dma_x_len_reg;
    volatile uint16_t *dma_x_int_reg;
    volatile uint16_t *dma_x_ctrl_reg;

    risen = GetWord16(DMA_INT_STATUS_REG);

    for (dma_channel_t i = DMA_CHANNEL_0; risen != 0 && i < DMA_CHANNEL_INVALID; ++i, risen >>= 1)
    {
        if (risen & 1)
        {
            bool stop;

            /*
             * DMAx_INT_REG shows after how many transfers the interrupt
             * is generated
             */
            dma_x_int_reg = DMA_CHN_REG(DMA0_INT_REG, i);

            /*
             * DMAx_LEN_REG shows the length of the DMA transfer
             */
            dma_x_len_reg = DMA_CHN_REG(DMA0_LEN_REG, i);

            dma_x_ctrl_reg = DMA_CHN_REG(DMA0_CTRL_REG, i);

            /*
             * Stop DMA if:
             *  - transfer is completed
             *  - mode is not circular
             */
        
            stop = (*dma_x_int_reg == *dma_x_len_reg)
                    && (!GetBits16(dma_x_ctrl_reg, CIRCULAR));
            SetBits16(DMA_CLEAR_INT_REG, (1<<i), 1);
            dma_helper(i, *dma_x_int_reg + 1, stop);
        }
    }
}

void dma_channel_stop(dma_channel_t channel_number)
{
    // Stopping DMA will clear DMAx_IDX_REG so read it before
    volatile uint16_t *dma_x_idx_reg = DMA_CHN_REG(DMA0_IDX_REG, channel_number);
    dma_helper(channel_number, *dma_x_idx_reg, true);
}

uint16_t dma_channel_transfered_bytes(dma_channel_t channel_number)
{
    volatile uint16_t *dma_x_int_reg = DMA_CHN_REG(DMA0_IDX_REG, channel_number);

    return *dma_x_int_reg;
}
