/******************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only
 * intended for use with Renesas products. No other uses are authorized. This
 * software is owned by Renesas Electronics Corporation and is protected under
 * all applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
 * LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
 * ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software
 * and to discontinue the availability of this software. By using this
 * software, you agree to the additional terms and conditions found by
 * accessing the following link:
 * http://www.renesas.com/disclaimer
 * Copyright (C) 2013-2017 Renesas Electronics Corporation.
 * All rights reserved.
 *****************************************************************************/

/******************************************************************************
 * File Name   : ssif_dma.c
 * Description : SSIF driver DMA functions
 *****************************************************************************/

/******************************************************************************
 Includes <System Includes>, "Project Includes"
 *****************************************************************************/
#include "iodefine.h"
#include "iobitmask.h"
#include "r_rza2_ssif_lld.h"
#include "r_ssif_hld_cfg.h"
#include "r_ssif_lld_int.h"
#include "fcntl.h"
#include "r_dmac_drv_api.h"
#include "r_dmac_lld_cfg_rza2m.h"
#include "rza_io_regrw.h"

/******************************************************************************
 Macro definitions
 *****************************************************************************/
#define SSIF_DUMMY_DMA_BUF_SIZE_PRV_        (4096u)

/*
 * The common multiple of 2, 4, 6, 8, 12, 24 and 32
 * which are all sampling sizes
 */
#define SSIF_DUMMY_DMA_TRN_SIZE_PRV_        (4032u)

/******************************************************************************
 Typedef definitions
 *****************************************************************************/

/******************************************************************************
 Exported global variables (to be accessed by other files)
 *****************************************************************************/

/******************************************************************************
 Private global variables and functions
 *****************************************************************************/

static int_t enable_ssif_transfer(ssif_info_ch_t * const p_info_ch);
static int_t open_dma_driver(ssif_info_ch_t * const p_info_ch);
static void SSIF_DMA_TxEndCallback(void);
static void SSIF_DMA_RxEndCallback(void);

static const e_r_drv_dmac_xfer_resource_t
        s_ssif_dma_tx_resource[SSIF_NUM_CHANS] =
{
    DMA_RS_INT_SSIF_DMA_TX_0,
    DMA_RS_INT_SSIF_DMA_TX_1,
    DMA_RS_INT_SSIF_DMA_TX_2,
    DMA_RS_INT_SSIF_DMA_TX_3
};

static const e_r_drv_dmac_xfer_resource_t
        s_ssif_dma_rx_resource[SSIF_NUM_CHANS] =
{
    DMA_RS_INT_SSIF_DMA_RX_0,
    DMA_RS_INT_SSIF_DMA_RX_1,
    DMA_RS_INT_SSIF_DMA_RX_2,
    DMA_RS_INT_SSIF_DMA_RX_3
};

static AIOCB s_ssif_dma_tx_end_aiocb[SSIF_NUM_CHANS];
static AIOCB s_ssif_dma_rx_end_aiocb[SSIF_NUM_CHANS];

static st_r_drv_dmac_next_transfer_t s_ssif_txdma_dummy_trparam[SSIF_NUM_CHANS];
static st_r_drv_dmac_next_transfer_t s_ssif_rxdma_dummy_trparam[SSIF_NUM_CHANS];

static uint32_t s_ssif_tx_dummy_buf[SSIF_DUMMY_DMA_BUF_SIZE_PRV_ / sizeof(uint32_t)];
static uint32_t s_ssif_rx_dummy_buf[SSIF_DUMMY_DMA_BUF_SIZE_PRV_ / sizeof(uint32_t)];

static int_t s_dma_driver_write_handle = (-1);
static int_t s_dma_driver_read_handle = (-1);

static ssif_info_ch_t * sp_info_ch;

/******************************************************************************
 * Function Name: R_SSIF_InitDMA
 * @brief         Allocate and Setup DMA_CH for specified SSIF channel
 * @param[in,out] p_info_ch: channel object
 * @retval        IOIF_ESUCCESS: Success
 * @retval        error code: Failure
 *****************************************************************************/
int_t R_SSIF_InitDMA(ssif_info_ch_t * const p_info_ch)
{
    int_t ercd = IOIF_ESUCCESS;
    int_t dma_ret = 0;
    uint32_t ssif_ch;
    st_r_drv_dmac_config_t dma_config;
    st_r_drv_dmac_config_t *ptdma_config;
    int_t result;
    st_r_drv_dmac_next_transfer_t next_transfer;

    if (NULL == p_info_ch)
    {
        return (IOIF_EFAULT);
    }

    if (open_dma_driver(p_info_ch) != IOIF_ESUCCESS)
    {
        return (IOIF_EFAULT);
    }

    sp_info_ch = p_info_ch;
    ssif_ch = p_info_ch->channel;

    /* setup DMA channel for write (if necessary) */
    if (O_RDONLY != p_info_ch->openflag)
    {
        AIOCB * const p_tx_aio = &s_ssif_dma_tx_end_aiocb[ssif_ch];

        p_tx_aio->aio_sigevent.sigev_notify = 0;
        p_tx_aio->aio_sigevent.sigev_notify = SIGEV_THREAD;
        p_tx_aio->aio_sigevent.sigev_value.sival_ptr = (void *) p_info_ch;
    }

    /* setup DMA channel for read (if necessary) */
    if (O_WRONLY != p_info_ch->openflag)
    {
        AIOCB * const p_rx_aio = &s_ssif_dma_rx_end_aiocb[ssif_ch];

        p_rx_aio->aio_sigevent.sigev_notify = SIGEV_THREAD;
        p_rx_aio->aio_sigevent.sigev_value.sival_ptr = (void *) p_info_ch;
    }

    /* start DMA dummy transfer for write (if necessary) */
    if (O_RDONLY != p_info_ch->openflag)
    {
        /* setup short dummy transfer */
        s_ssif_txdma_dummy_trparam[ssif_ch].source_address = (void *) &s_ssif_tx_dummy_buf[0];
        s_ssif_txdma_dummy_trparam[ssif_ch].destination_address = (uint32_t *) &g_ssireg[ssif_ch]->SSIFTDR.LONG;
        s_ssif_txdma_dummy_trparam[ssif_ch].count = SSIF_DUMMY_DMA_TRN_SIZE_PRV_;
        dma_config.config.resource = s_ssif_dma_tx_resource[ssif_ch];                                    /* DMA transfer resource */
        dma_config.config.source_width = DMA_DATA_SIZE_2;                                                /* DMA transfer unit size (source) - 32 bits */
        dma_config.config.destination_width = DMA_DATA_SIZE_2;                                           /* DMA transfer unit size (destination) - 32 bits */
        dma_config.config.source_address_type = DMA_ADDRESS_INCREMENT;                                   /* DMA address type (source) */
        dma_config.config.destination_address_type = DMA_ADDRESS_FIX;                                    /* DMA address type (destination) */
        dma_config.config.direction = DMA_REQUEST_DESTINATION;                                           /* DMA transfer direction will be set by the driver */
        dma_config.config.source_address = s_ssif_txdma_dummy_trparam[ssif_ch].source_address;           /* Source Address */
        dma_config.config.destination_address = s_ssif_txdma_dummy_trparam[ssif_ch].destination_address; /* Destination Address */
        dma_config.config.count = s_ssif_txdma_dummy_trparam[ssif_ch].count;                             /* length */
        dma_config.config.p_dmaComplete = SSIF_DMA_TxEndCallback;                                           /* set callback function (DMA end interrupt) */
        result = control(s_dma_driver_write_handle, CTL_DMAC_SET_CONFIGURATION, (void *) &dma_config);
        if (DRV_SUCCESS == result)
        {
            result = control(s_dma_driver_write_handle, CTL_DMAC_ENABLE, NULL);
        }

        if (DRV_SUCCESS == result)
        {
            next_transfer.source_address      = (void *) &s_ssif_tx_dummy_buf[0];
            next_transfer.destination_address = (uint32_t *) &g_ssireg[ssif_ch]->SSIFTDR.LONG;
            next_transfer.count               = SSIF_DUMMY_DMA_TRN_SIZE_PRV_;
            result = control(s_dma_driver_write_handle, CTL_DMAC_NEXT_TRANSFER, &next_transfer);
        }

        dma_ret = (DRV_SUCCESS == result) ? IOIF_ESUCCESS : IOIF_EERROR;

        if (IOIF_EERROR == dma_ret)
        {
            ercd = IOIF_EFAULT;
        }
    }

    /* start DMA dummy transfer for read (if necessary) */
    if (IOIF_ESUCCESS == ercd)
    {
        if (O_WRONLY != p_info_ch->openflag)
        {
            /* setup short dummy transfer */
            s_ssif_rxdma_dummy_trparam[ssif_ch].source_address = &g_ssireg[ssif_ch]->SSIFRDR.LONG;
            s_ssif_rxdma_dummy_trparam[ssif_ch].destination_address = (void *) &s_ssif_rx_dummy_buf[0];
            s_ssif_rxdma_dummy_trparam[ssif_ch].count = SSIF_DUMMY_DMA_TRN_SIZE_PRV_;

            st_r_drv_dmac_next_transfer_t next_data;

            next_data.source_address      = s_ssif_rxdma_dummy_trparam[ssif_ch].source_address;
            next_data.destination_address = s_ssif_rxdma_dummy_trparam[ssif_ch].destination_address;
            next_data.count               = s_ssif_rxdma_dummy_trparam[ssif_ch].count;

            result = control(s_dma_driver_read_handle, CTL_DMAC_NEXT_TRANSFER, &next_data);

            dma_ret = (DRV_SUCCESS == result) ? IOIF_ESUCCESS : IOIF_EERROR;

            if (IOIF_EERROR == dma_ret)
            {
                ercd = IOIF_EFAULT;
            }
            else
            {
                dma_config.config.resource = s_ssif_dma_rx_resource[ssif_ch];                                    /* DMA transfer resource */
                dma_config.config.source_width = DMA_DATA_SIZE_2;                                                /* DMA transfer unit size (source) - 32 bits */
                dma_config.config.destination_width = DMA_DATA_SIZE_2;                                           /* DMA transfer unit size (destination) - 32 bits */
                dma_config.config.source_address_type = DMA_ADDRESS_FIX;                                         /* DMA address type (source) */
                dma_config.config.destination_address_type = DMA_ADDRESS_INCREMENT;                              /* DMA address type (destination) */
                dma_config.config.direction = DMA_REQUEST_SOURCE;                                                /* DMA transfer direction will be set by the driver */
                dma_config.config.source_address = s_ssif_rxdma_dummy_trparam[ssif_ch].source_address;           /* Source Address */
                dma_config.config.destination_address = s_ssif_rxdma_dummy_trparam[ssif_ch].destination_address; /* Destination Address */
                dma_config.config.count = s_ssif_rxdma_dummy_trparam[ssif_ch].count;                             /* length */
                dma_config.config.p_dmaComplete = SSIF_DMA_RxEndCallback;                                           /* set callback function (DMA end interrupt) */

                result = control(s_dma_driver_read_handle, CTL_DMAC_SET_CONFIGURATION, (void *) &dma_config);

                if (DRV_SUCCESS == result)
                {
                    result = control(s_dma_driver_read_handle, CTL_DMAC_ENABLE, NULL);
                }

                if (DRV_SUCCESS == result)
                {
                    next_transfer.source_address = (uint32_t *) &g_ssireg[ssif_ch]->SSIFRDR.LONG;
                    next_transfer.destination_address = (void *) &s_ssif_rx_dummy_buf[0];
                    next_transfer.count               = SSIF_DUMMY_DMA_TRN_SIZE_PRV_;

                    result = control(s_dma_driver_read_handle, CTL_DMAC_NEXT_TRANSFER, &next_transfer);
                }

                dma_ret = (DRV_SUCCESS == result) ? IOIF_ESUCCESS : IOIF_EERROR;

                if (IOIF_EERROR == dma_ret)
                {
                    ercd = IOIF_EFAULT;
                }
            }
        }
    }

    /* enable SSIF transfer */
    if (IOIF_ESUCCESS == ercd)
    {
        ercd = enable_ssif_transfer(p_info_ch);
    }

    /* cleanup DMA resources when error occurred */
    if (IOIF_ESUCCESS != ercd)
    {
        R_SSIF_CancelDMA(p_info_ch);
    }

    return (ercd);
}
/******************************************************************************
 End of function R_SSIF_InitDMA
 *****************************************************************************/

/******************************************************************************
 * Function Name: R_SSIF_UnInitDMA
 * @brief         Free DMA_CH for specified SSIF channel
 * @param[in,out] p_info_ch: channel object
 * @retval        none
 *****************************************************************************/
void R_SSIF_UnInitDMA(ssif_info_ch_t * const p_info_ch)
{
    uint32_t remain;

    if (NULL == p_info_ch)
    {
        return;
    }

    if (s_dma_driver_write_handle >= 0)
    {
        control(s_dma_driver_write_handle, CTL_DMAC_DISABLE, &remain);
    }

    if (s_dma_driver_read_handle >= 0)
    {
        control(s_dma_driver_read_handle, CTL_DMAC_DISABLE, &remain);
    }
}
/******************************************************************************
 End of function R_SSIF_UnInitDMA
 *****************************************************************************/

/******************************************************************************
 * Function Name: R_SSIF_RestartDMA
 * @brief         Setup DMA_CH for specified SSIF channel (without allocate)
 * @param[in,out] p_info_ch: channel object
 * @retval        IOIF_ESUCCESS: Success
 * @retval        error code: Failure
 *****************************************************************************/
int_t R_SSIF_RestartDMA(ssif_info_ch_t* const p_info_ch)
{
    int_t ercd = IOIF_ESUCCESS;
    int_t dma_ret;
    uint32_t ssif_ch;
    st_r_drv_dmac_config_t dma_config;
    int_t result;
    st_r_drv_dmac_next_transfer_t next_transfer;

    if (NULL == p_info_ch)
    {
        return (IOIF_EFAULT);
    }

    ssif_ch = p_info_ch->channel;

    /* setup DMA channel for write (if necessary) */
    if (O_RDONLY != p_info_ch->openflag)
    {
        AIOCB * const p_tx_aio = &s_ssif_dma_tx_end_aiocb[ssif_ch];
        p_tx_aio->aio_sigevent.sigev_notify = SIGEV_THREAD;
        p_tx_aio->aio_sigevent.sigev_value.sival_ptr = (void *) p_info_ch;
    }

    /* setup DMA channel for read (if necessary) */
    if (O_WRONLY != p_info_ch->openflag)
    {
        AIOCB * const p_rx_aio = &s_ssif_dma_rx_end_aiocb[ssif_ch];
        p_rx_aio->aio_sigevent.sigev_notify = SIGEV_THREAD;
        p_rx_aio->aio_sigevent.sigev_value.sival_ptr = (void *) p_info_ch;
    }

    /* start DMA dummy transfer for write (if necessary) */
    if (O_RDONLY != p_info_ch->openflag)
    {
        /* setup short dummy transfer */
        s_ssif_txdma_dummy_trparam[ssif_ch].source_address = (void *) &s_ssif_tx_dummy_buf[0];
        s_ssif_txdma_dummy_trparam[ssif_ch].destination_address = (uint32_t *) &g_ssireg[ssif_ch]->SSIFTDR.LONG;
        s_ssif_txdma_dummy_trparam[ssif_ch].count = SSIF_DUMMY_DMA_TRN_SIZE_PRV_;

        dma_config.config.resource = s_ssif_dma_tx_resource[ssif_ch];                                    /* DMA transfer resource */
        dma_config.config.source_width = DMA_DATA_SIZE_2;                                                /* DMA transfer unit size (source) - 32 bits */
        dma_config.config.destination_width = DMA_DATA_SIZE_2;                                           /* DMA transfer unit size (destination) - 32 bits */
        dma_config.config.source_address_type = DMA_ADDRESS_INCREMENT;                                   /* DMA address type (source) */
        dma_config.config.destination_address_type = DMA_ADDRESS_FIX;                                    /* DMA address type (destination) */
        dma_config.config.direction = DMA_REQUEST_DESTINATION;                                           /* DMA transfer direction will be set by the driver */
        dma_config.config.source_address = s_ssif_txdma_dummy_trparam[ssif_ch].source_address;           /* Source Address */
        dma_config.config.destination_address = s_ssif_txdma_dummy_trparam[ssif_ch].destination_address; /* Destination Address */
        dma_config.config.count = s_ssif_txdma_dummy_trparam[ssif_ch].count;                             /* length */
        dma_config.config.p_dmaComplete = SSIF_DMA_TxEndCallback;                                           /* set callback function (DMA end interrupt) */

        result = control(s_dma_driver_write_handle, CTL_DMAC_SET_CONFIGURATION, (void *) &dma_config);

        if (DRV_SUCCESS == result)
        {
            result = control(s_dma_driver_write_handle, CTL_DMAC_ENABLE, NULL);
        }

        if (DRV_SUCCESS == result)
        {
            next_transfer.source_address      = (void *) &s_ssif_tx_dummy_buf[0];
            next_transfer.destination_address = (uint32_t *) &g_ssireg[ssif_ch]->SSIFTDR.LONG;
            next_transfer.count               = SSIF_DUMMY_DMA_TRN_SIZE_PRV_;

            result = control(s_dma_driver_write_handle, CTL_DMAC_NEXT_TRANSFER, &next_transfer);
        }

        dma_ret = (DRV_SUCCESS == result) ? IOIF_ESUCCESS : IOIF_EERROR;

        if (IOIF_EERROR == dma_ret)
        {
            ercd = IOIF_EFAULT;
        }
    }

    /* start DMA dummy transfer for read (if necessary) */
    if (IOIF_ESUCCESS == ercd)
    {
        if (O_WRONLY != p_info_ch->openflag)
        {
            /* setup short dummy transfer */
            s_ssif_rxdma_dummy_trparam[ssif_ch].source_address = (uint32_t *) &g_ssireg[ssif_ch]->SSIFRDR.LONG;
            s_ssif_rxdma_dummy_trparam[ssif_ch].destination_address = (void *) &s_ssif_rx_dummy_buf[0];
            s_ssif_rxdma_dummy_trparam[ssif_ch].count = SSIF_DUMMY_DMA_TRN_SIZE_PRV_;

            dma_config.config.resource = s_ssif_dma_rx_resource[ssif_ch];                                    /* DMA transfer resource */
            dma_config.config.source_width = DMA_DATA_SIZE_2;                                                /* DMA transfer unit size (source) - 32 bits */
            dma_config.config.destination_width = DMA_DATA_SIZE_2;                                           /* DMA transfer unit size (destination) - 32 bits */
            dma_config.config.source_address_type = DMA_ADDRESS_FIX;                                         /* DMA address type (source) */
            dma_config.config.destination_address_type = DMA_ADDRESS_INCREMENT;                              /* DMA address type (destination) */
            dma_config.config.direction = DMA_REQUEST_SOURCE;                                                /* DMA transfer direction will be set by the driver */
            dma_config.config.source_address = s_ssif_rxdma_dummy_trparam[ssif_ch].source_address;           /* Source Address */
            dma_config.config.destination_address = s_ssif_rxdma_dummy_trparam[ssif_ch].destination_address; /* Destination Address */
            dma_config.config.count = s_ssif_rxdma_dummy_trparam[ssif_ch].count;                             /* length */
            dma_config.config.p_dmaComplete = SSIF_DMA_RxEndCallback;                                           /* set callback function (DMA end interrupt) */

            result = control(s_dma_driver_read_handle, CTL_DMAC_SET_CONFIGURATION, (void *) &dma_config);

            if (DRV_SUCCESS == result)
            {
                result = control(s_dma_driver_read_handle, CTL_DMAC_ENABLE, NULL);
            }

            if (DRV_SUCCESS == result)
            {
                next_transfer.source_address      = (uint32_t *) &g_ssireg[ssif_ch]->SSIFRDR.LONG;
                next_transfer.destination_address = (void *) &s_ssif_rx_dummy_buf[0];
                next_transfer.count               = SSIF_DUMMY_DMA_TRN_SIZE_PRV_;

                result = control(s_dma_driver_read_handle, CTL_DMAC_NEXT_TRANSFER, &next_transfer);
            }

            dma_ret = (DRV_SUCCESS == result) ? IOIF_ESUCCESS : IOIF_EERROR;

            if (IOIF_EERROR == dma_ret)
            {
                ercd = IOIF_EFAULT;
            }
        }
    }

    /* enable SSIF transfer */
    if (IOIF_ESUCCESS == ercd)
    {
        ercd = enable_ssif_transfer(p_info_ch);
    }

    return (ercd);
}
/******************************************************************************
 End of function R_SSIF_RestartDMA
 *****************************************************************************/

/******************************************************************************
 * Function Name: R_SSIF_CancelDMA
 * @brief         Pause DMA transfer for specified SSIF channel
 * @param[in,out] p_info_ch: channel object
 * @retval        none
 *****************************************************************************/
void R_SSIF_CancelDMA(const ssif_info_ch_t * const p_info_ch)
{
    uint32_t remain;

    if (NULL == p_info_ch)
    {
        return;
    }

    if (s_dma_driver_write_handle >= 0)
    {
        control(s_dma_driver_write_handle, CTL_DMAC_DISABLE, &remain);
    }

    if (s_dma_driver_read_handle >= 0)
    {
        control(s_dma_driver_read_handle, CTL_DMAC_DISABLE, &remain);
    }
}
/******************************************************************************
 End of function R_SSIF_CancelDMA
 *****************************************************************************/

/******************************************************************************
 Private functions
 *****************************************************************************/

/******************************************************************************
 * Function Name: enable_ssif_transfer
 * @brief         Start the SSIF DMA transfer for read and / or write
 * @param[in,out] p_info_ch: channel object
 * @retval        IOIF_ESUCCESS: Success
 * @retval        error code: Failure
 *****************************************************************************/
static int_t enable_ssif_transfer(ssif_info_ch_t * const p_info_ch)
{
    /* clear status and enable error interrupt */
    SSIF_EnableErrorInterrupt(p_info_ch->channel);

    /* enable end interrupt */
    RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[p_info_ch->channel]->SSIFCR.LONG, (uint32_t) 1, SSIF_SSIFCR_TIE_SHIFT,
    SSIF_SSIFCR_TIE);
    RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[p_info_ch->channel]->SSIFCR.LONG, (uint32_t) 1, SSIF_SSIFCR_RIE_SHIFT,
    SSIF_SSIFCR_RIE);

    if (O_RDWR == p_info_ch->openflag)
    {
        /* start write and read DMA at the same time */
        RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[p_info_ch->channel]->SSICR.LONG,
        (uint32_t) (1 <<SSIF_SSICR_TEN_SHIFT) | (1 <<SSIF_SSICR_REN_SHIFT),
        SSIF_SSICR_REN_SHIFT,
        (uint32_t) (1 <<SSIF_SSICR_TEN_SHIFT) | (1 <<SSIF_SSICR_REN_SHIFT));
    }
    else if (O_WRONLY == p_info_ch->openflag)
    {
        /* start write DMA only */
        RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[p_info_ch->channel]->SSICR.LONG, (uint32_t) 1, SSIF_SSICR_TEN_SHIFT,
        SSIF_SSICR_TEN);
    }
    else if (O_RDONLY == p_info_ch->openflag)
    {
        /* start read DMA only */
        RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[p_info_ch->channel]->SSICR.LONG, (uint32_t) 1, SSIF_SSICR_REN_SHIFT,
        SSIF_SSICR_REN);
    }
    else
    {
        return (IOIF_EINVAL);
    }

    return (IOIF_ESUCCESS);
}
/******************************************************************************
 End of function enable_ssif_transfer
 *****************************************************************************/

/******************************************************************************
 * Function Name: open_dma_driver
 * @brief         Open the DMA driver for read and / or write
 * @param[in,out] p_info_ch: channel object
 * @retval        IOIF_ESUCCESS: Success
 * @retval        error code: Failure
 *****************************************************************************/
static int_t open_dma_driver(ssif_info_ch_t * const p_info_ch)
{
    /* open DMA driver for write (if necessary) */
    if (O_RDONLY == p_info_ch->openflag)
    {
        p_info_ch->dma_tx_ch = -1;
    }
    else
    {
        /* close the driver handle if already open */
        if ((-1) != s_dma_driver_write_handle)
        {
            close(s_dma_driver_write_handle);
        }

        s_dma_driver_write_handle = open(DEVICE_INDENTIFIER "dma_ssif_wr", O_WRONLY);

        /* we failed to open the DMA driver */
        if (s_dma_driver_write_handle < 0)
        {
            return (IOIF_EFAULT);
        }
    }

    /* open DMA driver for read (if necessary) */
    if (O_WRONLY == p_info_ch->openflag)
    {
        p_info_ch->dma_rx_ch = -1;
    }
    else
    {
        /* close the driver handle if already open */
        if ((-1) != s_dma_driver_read_handle)
        {
            close(s_dma_driver_read_handle);
        }

        s_dma_driver_read_handle = open(DEVICE_INDENTIFIER "dma_ssif_rd", O_RDONLY);

        /* we failed to open the DMA driver */
        if (s_dma_driver_read_handle < 0)
        {
            return (IOIF_EFAULT);
        }
    }
    return (IOIF_ESUCCESS);
}
/******************************************************************************
 End of function open_dma_driver
 *****************************************************************************/

/******************************************************************************
 * Function Name: SSIF_DMA_TxEndCallback
 * @brief         DMA callback function
 * @retval        none
 *****************************************************************************/
static void SSIF_DMA_TxEndCallback(void)
{
    ssif_info_ch_t * const p_info_ch = sp_info_ch;
    uint32_t ssif_ch;
    st_r_drv_dmac_next_transfer_t next_transfer;


    if (NULL == p_info_ch)
    {
        return;
    }

    ssif_ch = p_info_ch->channel;

    if (NULL != p_info_ch->p_aio_tx_curr)
    {
        /* now complete user request transfer, Signal to application */

        /* return aio complete */
        p_info_ch->p_aio_tx_curr->aio_return = (ssize_t) p_info_ch->p_aio_tx_curr->aio_nbytes;
        ahf_complete(&p_info_ch->tx_que, p_info_ch->p_aio_tx_curr);
    }

    /* copy next to curr (even if it's NULL) */
    p_info_ch->p_aio_tx_curr = p_info_ch->p_aio_tx_next;

    /* get next request (it could be NULL) */
    p_info_ch->p_aio_tx_next = ahf_removehead(&p_info_ch->tx_que);

    if (NULL != p_info_ch->p_aio_tx_next)
    {
        next_transfer.source_address      = (void *) p_info_ch->p_aio_tx_next->aio_buf;
        next_transfer.destination_address = (uint32_t *) &g_ssireg[ssif_ch]->SSIFTDR.LONG;
        next_transfer.count               = (uint32_t) p_info_ch->p_aio_tx_next->aio_nbytes;
    }
    else
    {
        next_transfer.source_address      = (void *) s_ssif_txdma_dummy_trparam[ssif_ch].source_address;
        next_transfer.destination_address = (void *) s_ssif_txdma_dummy_trparam[ssif_ch].destination_address;
        next_transfer.count               = (uint32_t) s_ssif_txdma_dummy_trparam[ssif_ch].count;
    }

    control(s_dma_driver_write_handle, CTL_DMAC_NEXT_TRANSFER, &next_transfer);
}
/******************************************************************************
 End of function SSIF_DMA_TxEndCallback
 *****************************************************************************/

/******************************************************************************
 * Function Name: SSIF_DMA_RxEndCallback
 * @brief         DMA callback function
 * @retval        none
 *****************************************************************************/
static void SSIF_DMA_RxEndCallback(void)
{
    ssif_info_ch_t * const p_info_ch = sp_info_ch;
    uint32_t ssif_ch;
    st_r_drv_dmac_next_transfer_t next_transfer;

    if (NULL == p_info_ch)
    {
        return;
    }

    ssif_ch = p_info_ch->channel;

    if (NULL != p_info_ch->p_aio_rx_curr)
    {
        /* now complete user request transfer, Signal to application */

        /* return aio complete */
        p_info_ch->p_aio_rx_curr->aio_return = (ssize_t) p_info_ch->p_aio_rx_curr->aio_nbytes;
        ahf_complete(&p_info_ch->rx_que, p_info_ch->p_aio_rx_curr);
    }

    /* copy next to curr (even if it's NULL) */
    p_info_ch->p_aio_rx_curr = p_info_ch->p_aio_rx_next;

    /* get next request (it could be NULL) */
    p_info_ch->p_aio_rx_next = ahf_removehead(&p_info_ch->rx_que);

    if (NULL != p_info_ch->p_aio_rx_next)
    {
        next_transfer.source_address      = (uint32_t *) &g_ssireg[ssif_ch]->SSIFRDR.LONG;
        next_transfer.destination_address = (void *) p_info_ch->p_aio_rx_next->aio_buf;
        next_transfer.count               = (uint32_t) p_info_ch->p_aio_rx_next->aio_nbytes;
    }
    else
    {
        next_transfer.source_address      = (void *) s_ssif_rxdma_dummy_trparam[ssif_ch].source_address;
        next_transfer.destination_address = (void *) s_ssif_rxdma_dummy_trparam[ssif_ch].destination_address;
        next_transfer.count               = (uint32_t) s_ssif_rxdma_dummy_trparam[ssif_ch].count;
    }

    control(s_dma_driver_read_handle, CTL_DMAC_NEXT_TRANSFER, &next_transfer);
}
/******************************************************************************
 End of function SSIF_DMA_RxEndCallback
 *****************************************************************************/
