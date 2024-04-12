/*******************************************************************************
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
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2013-2017 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/*******************************************************************************
* File Name   : r_rza2_ssif_lld.c
* $Rev: 6174 $
* $Date:: 2018-01-15 14:12:23 +0000#$
* Description : SSIF driver functions
******************************************************************************/

/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include <fcntl.h>
#include <unistd.h>

#include "r_typedefs.h"
#include "iodefine.h"
#include "iobitmask.h"
#include "compiler_settings.h"
#include "r_devlink_wrapper.h"
#include "r_os_abstraction_api.h"
#include "r_stb_drv_api.h"

#include "r_ssif_lld_aioif.h"
#include "r_rza2_ssif_lld.h"
#include "r_ssif_hld_cfg.h"
#include "r_ssif_lld_int.h"
#include "rza_io_regrw.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/

/*******************************************************************************
Typedef definitions
*******************************************************************************/

/*******************************************************************************
Exported global variables (to be accessed by other files)
*******************************************************************************/
ssif_info_drv_t g_ssif_info_drv;

volatile struct st_ssif* const g_ssireg[SSIF_NUM_CHANS] = SSIF_ADDRESS_LIST;

/*******************************************************************************
Private global variables and functions
*******************************************************************************/
static int_t SSIF_InitChannel(ssif_info_ch_t* const p_info_ch);
static void SSIF_UnInitChannel(ssif_info_ch_t* const p_info_ch);
static int_t SSIF_UpdateChannelConfig(ssif_info_ch_t* const p_info_ch,
                                      const ssif_channel_cfg_t* const p_ch_cfg);
static int_t SSIF_SetCtrlParams(const ssif_info_ch_t* const p_info_ch);
static int_t SSIF_CheckChannelCfg(const ssif_channel_cfg_t* const p_ch_cfg);
static int_t SSIF_CheckWordSize(const ssif_info_ch_t* const p_info_ch);
static void SSIF_Reset(const uint32_t ssif_ch);

static const e_stb_module_t gs_ssif_module[SSIF_NUM_CHANS] =
{
    MODULE_SSIF0,
    MODULE_SSIF1,
    MODULE_SSIF2,
    MODULE_SSIF3

};

static const st_drv_info_t gs_lld_info =
{
    {
        ((STDIO_SSIF_RZ_LLD_VERSION_MAJOR << 16) + STDIO_SSIF_RZ_LLD_VERSION_MINOR)
    },
    STDIO_SSIF_RZ_LLD_BUILD_NUM,
    STDIO_SSIF_RZ_LLD_DRV_NAME
};

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
* Function Name: R_SSIF_Initialise
* @brief         Initialize the SSIF driver's internal data
*
*                Description:<br>
*
* @param[in]     p_cfg_data :pointer of several parameters array per channels
* @retval        DRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
int_t R_SSIF_Initialise(const ssif_channel_cfg_t* const p_cfg_data)
{
    uint32_t        ssif_ch = 0;
    int_t           ercd = DRV_SUCCESS;
    ssif_info_ch_t* p_info_ch;

    if (NULL == p_cfg_data)
    {
        ercd = IOIF_EFAULT;
    }
    else
    {
        for (ssif_ch = 0; (ssif_ch < SSIF_NUM_CHANS) && (DRV_SUCCESS == ercd); ssif_ch++)
        {
            p_info_ch = &g_ssif_info_drv.info_ch[ssif_ch];
            p_info_ch->channel = ssif_ch;
            p_info_ch->enabled = p_cfg_data[ssif_ch].enabled;

            if (false != p_info_ch->enabled)
            {
                /* copy config data to channel info */
                ercd = SSIF_UpdateChannelConfig(p_info_ch, &p_cfg_data[ssif_ch]);

                if (DRV_SUCCESS == ercd)
                {
                    ercd = SSIF_InitChannel(p_info_ch);
                }
            }
        }

        if (DRV_SUCCESS == ercd)
        {
            for (ssif_ch = 0; ssif_ch < SSIF_NUM_CHANS; ssif_ch++)
            {
                p_info_ch = &g_ssif_info_drv.info_ch[ssif_ch];

                if (false != p_info_ch->enabled)
                {
                    SSIF_InterruptInit(ssif_ch, p_cfg_data[ssif_ch].int_level);
                }
            }
        }
    }

    return ercd;
}

/******************************************************************************
* Function Name: R_SSIF_UnInitialise
* @brief         UnInitialize the SSIF driver's internal data
*
*                Description:<br>
*
* @param         none
* @retval        DRV_SUCCESS   :Success.
******************************************************************************/
int_t R_SSIF_UnInitialise(void)
{
    uint32_t        ssif_ch;
    const int_t     ercd = DRV_SUCCESS;
    ssif_info_ch_t* p_info_ch;

    for (ssif_ch = 0; ssif_ch < SSIF_NUM_CHANS; ssif_ch++)
    {
        p_info_ch = &g_ssif_info_drv.info_ch[ssif_ch];

        if (false != p_info_ch->enabled)
        {
            SSIF_UnInitChannel(p_info_ch);
        }
    }

    return ercd;
}

/******************************************************************************
* Function Name: R_SSIF_EnableChannel
* @brief         Enable the SSIF channel
*
*                Description:<br>
*
* @param[in,out] p_info_ch  :channel object
* @retval        DRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
int_t R_SSIF_EnableChannel(ssif_info_ch_t* const p_info_ch)
{
    int_t ercd = DRV_SUCCESS;
    uint32_t was_masked;
    uint32_t ssif_ch;
    volatile uint8_t dummy_buf;

    if (NULL == p_info_ch)
    {
        ercd = IOIF_EFAULT;
    }
    else
    {
        ssif_ch = p_info_ch->channel;

        if (ssif_ch >= SSIF_NUM_CHANS)
        {
            ercd = IOIF_EFAULT;
        }
        else
        {
            /* check channel open flag(duplex) */
            if ((O_RDWR == p_info_ch->openflag)
                && (false == p_info_ch->is_full_duplex))
            {
                ercd = IOIF_EINVAL;
            }

            /* enable the SSIF clock */
            if (DRV_SUCCESS == ercd)
            {
                /* software reset */
                SSIF_Reset(ssif_ch);

#if defined (__ICCARM__)
                was_masked = __disable_irq_iar();
#else
                was_masked = __disable_irq();
#endif

                R_STB_StartModule(gs_ssif_module[ssif_ch]);

                if (0 == was_masked)
                {
                    __enable_irq();
                }
            }

            /* configure channel hardware */
            if (DRV_SUCCESS == ercd)
            {
                /* Set control parameters */
                ercd = SSIF_SetCtrlParams(p_info_ch);
            }

            /* allocate and setup/start DMA transfer */
            if (DRV_SUCCESS == ercd)
            {
                ercd = R_SSIF_InitDMA(p_info_ch);
            }
        }
    }

    return ercd;
}

/******************************************************************************
* Function Name: R_SSIF_DisableChannel
* @brief         Disable the SSIF channel
*
*                Description:<br>
*
* @param[in,out] p_info_ch  :channel object
* @retval        DRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
int_t R_SSIF_DisableChannel(ssif_info_ch_t* const p_info_ch)
{
    uint32_t dummy_read;
    uint32_t   was_masked;
    int_t   ret = DRV_SUCCESS;
    uint32_t ssif_ch;

    if (NULL == p_info_ch)
    {
        ret = IOIF_EFAULT;
    }
    else
    {
        ssif_ch = p_info_ch->channel;

        if (ssif_ch >= SSIF_NUM_CHANS)
        {
            ret = IOIF_EFAULT;
        }
        else
        {
            /* disable ssif clock */
#if defined (__ICCARM__)
            was_masked = __disable_irq_iar();
#else
            was_masked = __disable_irq();
#endif
            /* TEN and REN are disable */
            RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) 0, SSIF_SSICR_TEN_SHIFT, 
            SSIF_SSICR_TEN);
            RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) 0, SSIF_SSICR_REN_SHIFT, 
            SSIF_SSICR_REN);

            SSIF_DisableErrorInterrupt(ssif_ch);

            /* free DMA resources */
            R_SSIF_UnInitDMA(p_info_ch);

            /* disable DMA end interrupt */
            RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSIFCR.LONG, (uint32_t) 0, SSIF_SSIFCR_TIE_SHIFT, 
            SSIF_SSIFCR_TIE);
            RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSIFCR.LONG, (uint32_t) 0, SSIF_SSIFCR_RIE_SHIFT, 
            SSIF_SSIFCR_RIE);

            /* Master Mode? */
            if (p_info_ch->master_slave == SSIF_CFG_MASTER)
            {
                RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSIFCR.LONG, (uint32_t) 0, SSIF_SSIFCR_AUCKE_SHIFT, 
                SSIF_SSIFCR_AUCKE);
            }

            R_STB_StopModule(gs_ssif_module[ssif_ch]);

            if (0 == was_masked)
            {
                __enable_irq();
            }

            /* cancel event to ongoing request */
            if (NULL != p_info_ch->p_aio_tx_curr)
            {
                p_info_ch->p_aio_tx_curr->aio_return = IOIF_ECANCELED;
                ahf_complete(&p_info_ch->tx_que, p_info_ch->p_aio_tx_curr);
                p_info_ch->p_aio_tx_curr = NULL;
            }
            if (NULL != p_info_ch->p_aio_tx_next)
            {
                p_info_ch->p_aio_tx_next->aio_return = IOIF_ECANCELED;
                ahf_complete(&p_info_ch->tx_que, p_info_ch->p_aio_tx_next);
                p_info_ch->p_aio_tx_next = NULL;
            }
            if (NULL != p_info_ch->p_aio_rx_curr)
            {
                p_info_ch->p_aio_rx_curr->aio_return = IOIF_ECANCELED;
                ahf_complete(&p_info_ch->rx_que, p_info_ch->p_aio_rx_curr);
                p_info_ch->p_aio_rx_curr = NULL;
            }
            if (NULL != p_info_ch->p_aio_rx_next)
            {
                p_info_ch->p_aio_rx_next->aio_return = IOIF_ECANCELED;
                ahf_complete(&p_info_ch->rx_que, p_info_ch->p_aio_rx_next);
                p_info_ch->p_aio_rx_next = NULL;
            }
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: R_SSIF_ErrorRecovery
* @brief         Restart the SSIF channel
*
*                Description:<br>
*                When normal mode<br>
*                  Stop and restart DMA transfer.<br>
*                Note: This function execute in interrupt context.
* @param[in,out] p_info_ch  :channel object
* @retval        none
******************************************************************************/
void R_SSIF_ErrorRecovery(ssif_info_ch_t* const p_info_ch)
{
    uint32_t dummy_read;
    int_t   ercd = DRV_SUCCESS;
    uint32_t ssif_ch;

    if (NULL == p_info_ch)
    {
        ercd = IOIF_EFAULT;
    }
    else
    {
        ssif_ch = p_info_ch->channel;

        if (ssif_ch >= SSIF_NUM_CHANS)
        {
            ercd = IOIF_EFAULT;
        }
        else
        {
            /* disable DMA end interrupt */
            RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSIFCR.LONG, (uint32_t) 0, SSIF_SSIFCR_TIE_SHIFT, 
            SSIF_SSIFCR_TIE);
            RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSIFCR.LONG, (uint32_t) 0, SSIF_SSIFCR_RIE_SHIFT, 
            SSIF_SSIFCR_RIE);

            SSIF_DisableErrorInterrupt(ssif_ch);

            /* TEN and REN are disable */
            RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) 0, SSIF_SSICR_TEN_SHIFT,
            SSIF_SSICR_TEN);
            RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) 0, SSIF_SSICR_REN_SHIFT,
            SSIF_SSICR_REN);

            /* Reset FIFO */
            RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSIFCR.LONG, (uint32_t) 1, SSIF_SSIFCR_TFRST_SHIFT, 
            SSIF_SSIFCR_TFRST);
            RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSIFCR.LONG, (uint32_t) 1, SSIF_SSIFCR_RFRST_SHIFT, 
            SSIF_SSIFCR_RFRST);

            dummy_read = RZA_IO_RegRead_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSIFCR.LONG, IOREG_NONSHIFT_ACCESS,
            IOREG_NONMASK_ACCESS);

            (void)(dummy_read);
            RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSIFCR.LONG, (uint32_t) 0, SSIF_SSIFCR_TFRST_SHIFT, 
            SSIF_SSIFCR_TFRST);
            RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSIFCR.LONG, (uint32_t) 0, SSIF_SSIFCR_RFRST_SHIFT, 
            SSIF_SSIFCR_RFRST);

            /* pause DMA transfer */
            R_SSIF_CancelDMA(p_info_ch);

            /* clear status reg */
            RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSISR.LONG, (uint32_t) 0, IOREG_NONSHIFT_ACCESS,
            IOREG_NONMASK_ACCESS);

            /* cancel event to ongoing request */
            if (NULL != p_info_ch->p_aio_tx_curr)
            {
                p_info_ch->p_aio_tx_curr->aio_return = IOIF_EIO;
                ahf_complete(&p_info_ch->tx_que, p_info_ch->p_aio_tx_curr);
                p_info_ch->p_aio_tx_curr = NULL;
            }
            if (NULL != p_info_ch->p_aio_tx_next)
            {
                p_info_ch->p_aio_tx_next->aio_return = IOIF_EIO;
                ahf_complete(&p_info_ch->tx_que, p_info_ch->p_aio_tx_next);
                p_info_ch->p_aio_tx_next = NULL;
            }
            if (NULL != p_info_ch->p_aio_rx_curr)
            {
                p_info_ch->p_aio_rx_curr->aio_return = IOIF_EIO;
                ahf_complete(&p_info_ch->rx_que, p_info_ch->p_aio_rx_curr);
                p_info_ch->p_aio_rx_curr = NULL;
            }
            if (NULL != p_info_ch->p_aio_rx_next)
            {
                p_info_ch->p_aio_rx_next->aio_return = IOIF_EIO;
                ahf_complete(&p_info_ch->rx_que, p_info_ch->p_aio_rx_next);
                p_info_ch->p_aio_rx_next = NULL;
            }
        }

        /* configure channel hardware */
        if (DRV_SUCCESS == ercd)
        {
            /* software reset */
            SSIF_Reset(ssif_ch);

            /* Set control parameters */
            ercd = SSIF_SetCtrlParams(p_info_ch);
        }

        if (DRV_SUCCESS == ercd)
        {
            /* setup/restart DMA transfer */
            ercd = R_SSIF_RestartDMA(p_info_ch);
        }
    }

    if (DRV_SUCCESS != ercd)
    {
        /* NON_NOTICE_ASSERT: cannot restart channel */
    }

    return;
}

/******************************************************************************
* Function Name: R_SSIF_PostAsyncIo
* @brief         Enqueue asynchronous read/write request
*
*                Description:<br>
*
* @param[in,out] p_info_ch  :channel object
* @param[in,out] p_aio      :aio control block of read/write request
* @retval        none
******************************************************************************/
void R_SSIF_PostAsyncIo(ssif_info_ch_t* const p_info_ch, AIOCB* const p_aio)
{
    if ((NULL == p_info_ch) || (NULL == p_aio))
    {
        /* NON_NOTICE_ASSERT: illegal pointer */
    }
    else
    {
        if (SSIF_ASYNC_W == p_aio->aio_return)
        {
            ahf_addtail(&p_info_ch->tx_que, p_aio);
        }
        else if (SSIF_ASYNC_R == p_aio->aio_return)
        {
            ahf_addtail(&p_info_ch->rx_que, p_aio);
        }
        else
        {
            /* NON_NOTICE_ASSERT: illegal request type */
        }
    }

    return;
}

/******************************************************************************
* Function Name: R_SSIF_PostAsyncCancel
* @brief         Cancel read or write request(s)
*
*                Description:<br>
*
* @param[in,out] p_info_ch  :channel object
* @param[in,out] p_aio      :aio control block to cancel or NULL to cancel all.
* @retval        none
******************************************************************************/
void R_SSIF_PostAsyncCancel(ssif_info_ch_t* const p_info_ch, AIOCB* const p_aio)
{
    int32_t ioif_ret;

    if (NULL == p_info_ch)
    {
        /* NON_NOTICE_ASSERT: illegal pointer */
    }
    else
    {
        if (NULL == p_aio)
        {
            ahf_cancelall(&p_info_ch->tx_que);
            ahf_cancelall(&p_info_ch->rx_que);
        }
        else
        {
            ioif_ret = ahf_cancel(&p_info_ch->tx_que, p_aio);
            if (DRV_SUCCESS != ioif_ret)
            {
                /* NON_NOTICE_ASSERT: unexpected aioif error */
            }

            ioif_ret = ahf_cancel(&p_info_ch->rx_que, p_aio);
            if (DRV_SUCCESS != ioif_ret)
            {
                /* NON_NOTICE_ASSERT: unexpected aioif error */
            }
        }
    }

    return;
}

/******************************************************************************
* Function Name: R_SSIF_IOCTL_ConfigChannel
* @brief         Save configuration to the SSIF driver.
*
*                Description:<br>
*                Update channel object.
* @param[in,out] p_info_ch  :channel object
* @param[in]     p_ch_cfg   :SSIF channel configuration parameter
* @retval        DRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
int_t R_SSIF_IOCTL_ConfigChannel(ssif_info_ch_t* const p_info_ch,
                                const ssif_channel_cfg_t* const p_ch_cfg)
{
    int_t    ercd;

    if ((NULL == p_info_ch) || (NULL == p_ch_cfg))
    {
        ercd = IOIF_EFAULT;
    }
    else
    {
        /* stop DMA transfer */
        ercd = R_SSIF_DisableChannel(p_info_ch);

        if (DRV_SUCCESS == ercd)
        {
            /* copy config data to channel info */
            ercd = SSIF_UpdateChannelConfig(p_info_ch, p_ch_cfg);
        }

        if (DRV_SUCCESS == ercd)
        {
            /* restart DMA transfer */
            ercd = R_SSIF_EnableChannel(p_info_ch);
        }
    }

    return ercd;
}

/******************************************************************************
* Function Name: R_SSIF_IOCTL_GetStatus
* @brief         Get a value of SSISR register.
*
*                Description:<br>
*
* @param[in]     p_info_ch  :channel object
* @param[in,out] p_status   :pointer of status value
* @retval        DRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
int_t R_SSIF_IOCTL_GetStatus(const ssif_info_ch_t* const p_info_ch, uint32_t* const p_status)
{
    int_t ret = DRV_SUCCESS;

    if ((NULL == p_info_ch) || (NULL == p_status))
    {
        ret = IOIF_EFAULT;
    }
    else
    {
        *p_status = RZA_IO_RegRead_32((volatile uint32_t *) &g_ssireg[p_info_ch->channel]->SSISR.LONG, IOREG_NONSHIFT_ACCESS,
        IOREG_NONMASK_ACCESS);
    }

    return ret;
}

/******************************************************************************
* Function Name: SSIF_SWLtoLen
* @brief         Convert SSICR:SWL bits to system word length
*
*                Description:<br>
*
* @param[in]     ssicr_swl  :SSICR register SWL field value(0 to 7)
* @retval        8 to 256   :system word length(bits)
******************************************************************************/
int_t R_SSIF_SWLtoLen(const ssif_chcfg_system_word_t ssicr_swl)
{
    /* -> IPA M1.10.1 : This is conversion table that can't be macro-coding. */
    static const int_t decode_enum_swl[SSIF_CFG_SYSTEM_WORD_256+1] = {
        8,      /* SSIF_CFG_SYSTEM_WORD_8   */
        16,     /* SSIF_CFG_SYSTEM_WORD_16  */
        24,     /* SSIF_CFG_SYSTEM_WORD_24  */
        32,     /* SSIF_CFG_SYSTEM_WORD_32  */
        48,     /* SSIF_CFG_SYSTEM_WORD_48  */
        64,     /* SSIF_CFG_SYSTEM_WORD_64  */
        128,    /* SSIF_CFG_SYSTEM_WORD_128 */
        256     /* SSIF_CFG_SYSTEM_WORD_256 */
    };
    /* <- IPA M1.10.1 */

    return decode_enum_swl[ssicr_swl];
}

/******************************************************************************
* Function Name: R_SSIF_DWLtoLen
* @brief         Convert SSICR:DWL bits to data word length
*
*                Description:<br>
*
* @param[in]     ssicr_dwl  :SSICR register DWL field value(0 to 6)
* @retval        8 to 32    :data word length(bits)
******************************************************************************/
int_t R_SSIF_DWLtoLen(const ssif_chcfg_data_word_t ssicr_dwl)
{
    /* -> IPA M1.10.1 : This is conversion table that can't be macro-coding. */
    static const int_t decode_enum_dwl[SSIF_CFG_DATA_WORD_32+1] = {
        8,   /* SSIF_CFG_DATA_WORD_8  */
        16,  /* SSIF_CFG_DATA_WORD_16 */
        18,  /* SSIF_CFG_DATA_WORD_18 */
        20,  /* SSIF_CFG_DATA_WORD_20 */
        22,  /* SSIF_CFG_DATA_WORD_22 */
        24,  /* SSIF_CFG_DATA_WORD_24 */
        32   /* SSIF_CFG_DATA_WORD_32 */
    };
    /* <- IPA M1.10.1 */

    return decode_enum_dwl[ssicr_dwl];
}

/******************************************************************************
Private functions
******************************************************************************/

/******************************************************************************
* Function Name: SSIF_InitChannel
* @brief         Initialize for the SSIF channel
*
*                Description:<br>
*                Create semaphore and queue for channel.<br>
*                And setup SSIF pin.
* @param[in,out] p_info_ch  :channel object
* @retval        DRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
static int_t SSIF_InitChannel(ssif_info_ch_t* const p_info_ch)
{
    int32_t os_ret;
    uint32_t ssif_ch;
    int_t ercd = DRV_SUCCESS;
    bool_t sem_created = false;

    static const bool_t is_duplex_ch[SSIF_NUM_CHANS] =
    {
        true,   /* SSIF0 is full duplex channel */
        true,   /* SSIF1 is full duplex channel */
        false,  /* SSIF2 is half duplex channel */
        true    /* SSIF3 is full duplex channel */
    };

    if (NULL == p_info_ch)
    {
        ercd = IOIF_EFAULT;
    }
    else
    {
        ssif_ch = p_info_ch->channel;

        p_info_ch->is_full_duplex = is_duplex_ch[ssif_ch];

        /* Create sem_access semaphore */
        sem_created = R_OS_SemaphoreCreate(&p_info_ch->sem_access, 1u);

        if (!sem_created)
        {
            ercd = DRV_ERROR;
        }

        if (DRV_SUCCESS == ercd)
        {
            ercd = ahf_create(&p_info_ch->tx_que, AHF_LOCKINT);

            if (DRV_SUCCESS != ercd)
            {
                ercd = DRV_ERROR;
            }
        }

        if (DRV_SUCCESS == ercd)
        {
            ercd = ahf_create(&p_info_ch->rx_que, AHF_LOCKINT);
            if (DRV_SUCCESS != ercd)
            {
                ercd = DRV_ERROR;
            }
        }
        if (DRV_SUCCESS == ercd)
        {
            /* set channel initialize */
            p_info_ch->openflag = 0;

            p_info_ch->p_aio_tx_curr = NULL;       /* tx request pointer */
            p_info_ch->p_aio_tx_next = NULL;       /* tx request pointer */
            p_info_ch->p_aio_rx_curr = NULL;       /* rx request pointer */
            p_info_ch->p_aio_rx_next = NULL;       /* rx request pointer */
        }

        if (DRV_SUCCESS == ercd)
        {
            ercd = R_SSIF_Userdef_InitPinMux(ssif_ch);
        }

        if (DRV_SUCCESS == ercd)
        {
            p_info_ch->ch_stat = SSIF_CHSTS_INIT;
        }
        else
        {
            if (0 != p_info_ch->sem_access)
            {
                R_OS_SemaphoreDelete(&p_info_ch->sem_access);
            }
        }
    }

    return ercd;
}

/******************************************************************************
* Function Name: SSIF_UnInitChannel
* @brief         Uninitialise the SSIF channel.
*
*                Description:<br>
*
* @param[in,out] p_info_ch  :channel object
* @retval        none
******************************************************************************/
static void SSIF_UnInitChannel(ssif_info_ch_t* const p_info_ch)
{
    uint32_t was_masked;
    uint32_t ssif_ch;

    if (NULL == p_info_ch)
    {
        /* NON_NOTICE_ASSERT: illegal pointer */
    }
    else
    {
        ssif_ch = p_info_ch->channel;

        if (SSIF_CHSTS_INIT != p_info_ch->ch_stat)
        {
            /* NON_NOTICE_ASSERT: unexpected channel status */
        }

        p_info_ch->ch_stat = SSIF_CHSTS_UNINIT;

        SSIF_DisableErrorInterrupt(ssif_ch);

#if defined (__ICCARM__)
        was_masked = __disable_irq_iar();
#else
        was_masked = __disable_irq();
#endif

        /* delete the tx queue */
        ahf_cancelall(&p_info_ch->tx_que);
        ahf_destroy(&p_info_ch->tx_que);

        /* delete the rx queue */
        ahf_cancelall(&p_info_ch->rx_que);
        ahf_destroy(&p_info_ch->rx_que);

        SSIF_InterruptShutdown(ssif_ch);

        if (0 == was_masked)
        {
            __enable_irq();
        }
    }

    return;
}

/******************************************************************************
* Function Name: SSIF_UpdateChannelConfig
* @brief         Save configuration to the SSIF driver.
*
*                Description:<br>
*                Update channel object.
* @param[in,out] p_info_ch  :channel object
* @param[in]     p_ch_cfg   :SSIF channel configuration parameter
* @retval        DRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
static int_t SSIF_UpdateChannelConfig(ssif_info_ch_t* const p_info_ch,
                                      const ssif_channel_cfg_t* const p_ch_cfg)
{
    int_t ercd;

    if ((NULL == p_info_ch) || (NULL == p_ch_cfg))
    {
        ercd = IOIF_EFAULT;
    }
    else
    {
        ercd = SSIF_CheckChannelCfg(p_ch_cfg);

        if (DRV_SUCCESS == ercd)
        {
            p_info_ch->slave_mode = p_ch_cfg->slave_mode;

            if (false != p_info_ch->slave_mode)
            {
                /* slave mode */
                p_info_ch->clock_direction = SSIF_CFG_CLOCK_IN;
                p_info_ch->ws_direction = SSIF_CFG_WS_IN;
            }
            else
            {
                /* master mode */
                p_info_ch->clock_direction = SSIF_CFG_CLOCK_OUT;
                p_info_ch->ws_direction = SSIF_CFG_WS_OUT;

                /* when master mode, always disable noise cancel */
                p_info_ch->noise_cancel = SSIF_CFG_DISABLE_NOISE_CANCEL;
            }

            p_info_ch->sample_freq        = p_ch_cfg->sample_freq;

            p_info_ch->clk_select         = p_ch_cfg->clk_select;
            p_info_ch->data_word          = p_ch_cfg->data_word;
            p_info_ch->system_word        = p_ch_cfg->system_word;
            p_info_ch->bclk_pol           = p_ch_cfg->bclk_pol;
            p_info_ch->padding_pol        = p_ch_cfg->padding_pol;
            p_info_ch->serial_alignment   = p_ch_cfg->serial_alignment;
            p_info_ch->parallel_alignment = p_ch_cfg->parallel_alignment;
            p_info_ch->ws_delay           = p_ch_cfg->ws_delay;
            p_info_ch->tdm_mode           = p_ch_cfg->tdm_mode;
            p_info_ch->frame_word         = p_ch_cfg->frame_word;
            p_info_ch->master_slave       = p_ch_cfg->master_slave;
            p_info_ch->lr_clock           = p_ch_cfg->lr_clock;
            p_info_ch->mute_onoff         = p_ch_cfg->mute_onoff;
        }

        if (DRV_SUCCESS == ercd)
        {
            ercd = SSIF_CheckWordSize(p_info_ch);
        }

        if (DRV_SUCCESS == ercd)
        {
            if (false == p_info_ch->slave_mode)
            {
                /* Master: call user own clock setting function */
                ercd = R_SSIF_Userdef_SetClockDiv(p_ch_cfg, &p_info_ch->clk_div);
            }
            else
            {
                /* Slave: set dummy value for clear */
                p_info_ch->clk_div = SSIF_CFG_CKDV_BITS_1;
            }
        }
    }

    return ercd;
}

/******************************************************************************
* Function Name: SSIF_SetCtrlParams
* @brief         Set SSIF configuration to hardware.
*
*                Description:<br>
*                Update SSICR register.
* @param[in]     p_info_ch  :channel object
* @retval        DRV_SUCCESS   :Success.
* @retval        error code :Failure.
******************************************************************************/
static int_t SSIF_SetCtrlParams(const ssif_info_ch_t* const p_info_ch)
{
    int_t ret = DRV_SUCCESS;
    uint32_t was_masked;
    uint32_t ssif_ch;
    uint32_t dummy_read;

    if (NULL == p_info_ch)
    {
        ret = IOIF_EFAULT;
    }
    else
    {
        ssif_ch = p_info_ch->channel;

#if defined (__ICCARM__)
        was_masked = __disable_irq_iar();
#else
        was_masked = __disable_irq();
#endif
        /* ALL CLEAR */
        RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, 0, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);
        RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSISR.LONG, 0, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

        /* AUDIO MCK clock Rewrite when AUDIO MCK supply is stopped(MST) */
        RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) (p_info_ch->master_slave), SSIF_SSICR_MST_SHIFT,
        SSIF_SSICR_MST);
        /* write when LR clock is stopped(LRCKP) */
        RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) (p_info_ch->lr_clock), SSIF_SSICR_LRCKP_SHIFT,
        SSIF_SSICR_LRCKP);
        RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) (p_info_ch->frame_word), SSIF_SSICR_FRM_SHIFT,
        SSIF_SSICR_FRM);
        /* write when LR clock is stopped(DWL/SWL) */
        RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) (p_info_ch->data_word), SSIF_SSICR_DWL_SHIFT,
        SSIF_SSICR_DWL);
        RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) (p_info_ch->system_word), SSIF_SSICR_SWL_SHIFT,
        SSIF_SSICR_SWL);
        RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) (p_info_ch->padding_pol), SSIF_SSICR_SPDP_SHIFT,
        SSIF_SSICR_SPDP);
        RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) (p_info_ch->serial_alignment), SSIF_SSICR_SDTA_SHIFT,
        SSIF_SSICR_SDTA);
        RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) (p_info_ch->parallel_alignment), SSIF_SSICR_PDTA_SHIFT,
        SSIF_SSICR_PDTA);
        RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) (p_info_ch->ws_delay), SSIF_SSICR_DEL_SHIFT,
        SSIF_SSICR_DEL);
        RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) (p_info_ch->mute_onoff), SSIF_SSICR_MUEN_SHIFT,
        SSIF_SSICR_MUEN);
        /* TDM formatt? */
        if (p_info_ch->tdm_mode == SSIF_CFG_ENABLE_TDM)
        {
            /* Set 0 for TDM format */
            RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, 0, SSIF_SSICR_DEL_SHIFT, SSIF_SSICR_DEL);
        }

        /* Master Mode? */
        if (p_info_ch->master_slave == SSIF_CFG_MASTER)
        {
            /* AUDIO MCK clock Rewrite when AUDIO MCK supply is stopped(CKS/BCKP/CKDV) */
            RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) (p_info_ch->clk_select), SSIF_SSICR_CKS_SHIFT,
            SSIF_SSICR_CKS);
            RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) (p_info_ch->bclk_pol), SSIF_SSICR_BCKP_SHIFT,
            SSIF_SSICR_BCKP);
            RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) (p_info_ch->clk_div), SSIF_SSICR_CKDV_SHIFT,
            SSIF_SSICR_CKDV);
            RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSIFCR.LONG, (uint32_t) 1, SSIF_SSIFCR_AUCKE_SHIFT, 
            SSIF_SSIFCR_AUCKE);
        }
        /* Slave Mode */
        else
        {
            if (SSIF_CFG_ENABLE_NOISE_CANCEL == p_info_ch->noise_cancel)
            {
                /* ENABLE_NOISE_CANCEL && slave mode */
                RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[p_info_ch->channel]->SSIFCR.LONG, (uint32_t) 1, SSIF_SSIFCR_BCKNCE_SHIFT,
                SSIF_SSIFCR_BCKNCE);
                RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[p_info_ch->channel]->SSIFCR.LONG, (uint32_t) 1, SSIF_SSIFCR_LRCKNCE_SHIFT,
                SSIF_SSIFCR_LRCKNCE);
                RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[p_info_ch->channel]->SSIFCR.LONG, (uint32_t) 1, SSIF_SSIFCR_RXDNCE_SHIFT,
                SSIF_SSIFCR_RXDNCE);
            }
            else
            {
                /* DISABLE_NOISE_CANCEL || master mode */
                RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[p_info_ch->channel]->SSIFCR.LONG, (uint32_t) 0, SSIF_SSIFCR_BCKNCE_SHIFT,
                SSIF_SSIFCR_BCKNCE);
                RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[p_info_ch->channel]->SSIFCR.LONG, (uint32_t) 0, SSIF_SSIFCR_LRCKNCE_SHIFT,
                SSIF_SSIFCR_LRCKNCE);
                RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[p_info_ch->channel]->SSIFCR.LONG, (uint32_t) 0, SSIF_SSIFCR_RXDNCE_SHIFT,
                SSIF_SSIFCR_RXDNCE);
            }
        }

        /* FIFO initialization */
        RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSIFCR.LONG, (uint32_t) 1, SSIF_SSIFCR_TFRST_SHIFT, 
        SSIF_SSIFCR_TFRST);
        RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSIFCR.LONG, (uint32_t) 1, SSIF_SSIFCR_RFRST_SHIFT, 
        SSIF_SSIFCR_RFRST);

        dummy_read = RZA_IO_RegRead_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSIFCR.LONG, IOREG_NONSHIFT_ACCESS,
        IOREG_NONMASK_ACCESS);

        (void)(dummy_read);
        RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSIFCR.LONG, (uint32_t) 0, SSIF_SSIFCR_TFRST_SHIFT, 
        SSIF_SSIFCR_TFRST);
        RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSIFCR.LONG, (uint32_t) 0, SSIF_SSIFCR_RFRST_SHIFT, 
        SSIF_SSIFCR_RFRST);

        dummy_read = RZA_IO_RegRead_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSIFCR.LONG, IOREG_NONSHIFT_ACCESS,
        IOREG_NONMASK_ACCESS);
        

        /* change SNCR register: exit exclusive */
        if (0 == was_masked)
        {
            __enable_irq();
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: SSIF_CheckChannelCfg
* @brief         Check channel configuration parameters are valid or not.
*
*                Description:<br>
*
* @param[in]     p_ch_cfg     :channel configuration
* @retval        DRV_SUCCESS:Success.
* @retval        error code   :Failure.
******************************************************************************/
static int_t SSIF_CheckChannelCfg(const ssif_channel_cfg_t* const p_ch_cfg)
{
    int_t ret = DRV_SUCCESS;

    if (NULL == p_ch_cfg)
    {
        ret = IOIF_EFAULT;
    }
    else
    {
        switch (p_ch_cfg->clk_select)
        {
        case SSIF_CFG_CKS_AUDIO_X1:
            /* fall through */
        case SSIF_CFG_CKS_AUDIO_CLK:
            /* do nothing */
            break;
        default:
            ret = IOIF_EINVAL;
            break;
        }

        if (DRV_SUCCESS == ret)
        {
            switch (p_ch_cfg->frame_word)
            {
            case SSIF_CFG_FRM_00B:
                /* fall through */
            case SSIF_CFG_FRM_01B:
                /* fall through */
            case SSIF_CFG_FRM_10B:
                /* fall through */
            case SSIF_CFG_FRM_11B:
                /* do nothing */
                break;
            default:
                ret = IOIF_EINVAL;
                break;
            }
        }

        if (DRV_SUCCESS == ret)
        {
            switch (p_ch_cfg->data_word)
            {
            case SSIF_CFG_DATA_WORD_8:
                /* fall through */
            case SSIF_CFG_DATA_WORD_16:
                /* fall through */
            case SSIF_CFG_DATA_WORD_18:
                /* fall through */
            case SSIF_CFG_DATA_WORD_20:
                /* fall through */
            case SSIF_CFG_DATA_WORD_22:
                /* fall through */
            case SSIF_CFG_DATA_WORD_24:
                /* fall through */
            case SSIF_CFG_DATA_WORD_32:
                /* do nothing */
                break;
            default:
                ret = IOIF_EINVAL;
                break;
            }
        }

        if (DRV_SUCCESS == ret)
        {
            switch (p_ch_cfg->system_word)
            {
            case SSIF_CFG_SYSTEM_WORD_8:
                /* fall through */
            case SSIF_CFG_SYSTEM_WORD_16:
                /* fall through */
            case SSIF_CFG_SYSTEM_WORD_24:
                /* fall through */
            case SSIF_CFG_SYSTEM_WORD_32:
                /* fall through */
            case SSIF_CFG_SYSTEM_WORD_48:
                /* fall through */
            case SSIF_CFG_SYSTEM_WORD_64:
                /* fall through */
            case SSIF_CFG_SYSTEM_WORD_128:
                /* fall through */
            case SSIF_CFG_SYSTEM_WORD_256:
                /* do nothing */
                break;
            default:
                ret = IOIF_EINVAL;
                break;
            }
        }

        if (DRV_SUCCESS == ret)
        {
            switch (p_ch_cfg->bclk_pol)
            {
            case SSIF_CFG_FALLING:
                /* fall through */
            case SSIF_CFG_RISING:
                /* do nothing */
                break;
            default:
                ret = IOIF_EINVAL;
                break;
            }
        }

        if (DRV_SUCCESS == ret)
        {
            switch (p_ch_cfg->lr_clock)
            {
            case SSIF_CFG_HIGH_LEVEL:
                /* fall through */
            case SSIF_CFG_LOW_LEVEL:
                /* do nothing */
                break;
            default:
                ret = IOIF_EINVAL;
                break;
            }
        }

        if (DRV_SUCCESS == ret)
        {
            switch (p_ch_cfg->padding_pol)
            {
            case SSIF_CFG_PADDING_LOW:
                /* fall through */
            case SSIF_CFG_PADDING_HIGH:
                /* do nothing */
                break;
            default:
                ret = IOIF_EINVAL;
                break;
            }
        }

        if (DRV_SUCCESS == ret)
        {
            switch (p_ch_cfg->serial_alignment)
            {
            case SSIF_CFG_DATA_FIRST:
                /* fall through */
            case SSIF_CFG_PADDING_FIRST:
                /* do nothing */
                break;
            default:
                ret = IOIF_EINVAL;
                break;
            }
        }

        if (DRV_SUCCESS == ret)
        {
            switch (p_ch_cfg->parallel_alignment)
            {
            case SSIF_CFG_LEFT:
                /* fall through */
            case SSIF_CFG_RIGHT:
                /* do nothing */
                break;
            default:
                ret = IOIF_EINVAL;
                break;
            }
        }

        if (DRV_SUCCESS == ret)
        {
            switch (p_ch_cfg->ws_delay)
            {
            case SSIF_CFG_DELAY:
                /* fall through */
            case SSIF_CFG_NO_DELAY:
                /* do nothing */
                break;
            default:
                ret = IOIF_EINVAL;
                break;
            }
        }

        if (DRV_SUCCESS == ret)
        {
            switch (p_ch_cfg->noise_cancel)
            {
            case SSIF_CFG_DISABLE_NOISE_CANCEL:
                /* fall through */
            case SSIF_CFG_ENABLE_NOISE_CANCEL:
                /* do nothing */
                break;
            default:
                ret = IOIF_EINVAL;
                break;
            }
        }

        if (DRV_SUCCESS == ret)
        {
            switch (p_ch_cfg->tdm_mode)
            {
            case SSIF_CFG_DISABLE_TDM:
                /* fall through */
            case SSIF_CFG_ENABLE_TDM:
                /* do nothing */
                break;
            default:
                ret = IOIF_EINVAL;
                break;
            }
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: SSIF_CheckWordSize
* @brief         Check system word size whether that is valid or not.
*
*                Description:<br>
*                if system_words couldn't involve specified number of<br>
*                data_words then error.
* @param[in]     p_info_ch    :channel object
* @retval        DRV_SUCCESS:Success.
* @retval        error code   :Failure.
******************************************************************************/
static int_t SSIF_CheckWordSize(const ssif_info_ch_t* const p_info_ch)
{
    uint32_t ssicr_chnl;
    uint32_t dw_per_sw;
    uint32_t datawd_len;
    uint32_t syswd_len;
    int_t ret = DRV_SUCCESS;

    if (NULL == p_info_ch)
    {
        ret = IOIF_EFAULT;
    }
    else
    {
        ssicr_chnl = p_info_ch->frame_word;
        /* ->MISRA 13.7 : This is verbose error check by way of precaution */
        if (SSIF_CFG_FRM_11B < ssicr_chnl)
        /* <-MISRA 13.7 */
        {
            ret = IOIF_EINVAL;
        }
        else
        {
            /* data_words number per system_words */
            if (SSIF_CFG_ENABLE_TDM == p_info_ch->tdm_mode)
            {
                /* When TDM Mode data_word number per system_words fixed to 1 */
                dw_per_sw = 1u;
            }
            else
            {
                /* When not TDM data_word number per system_words depends CHNL */
                dw_per_sw = ssicr_chnl + 1u;
            }

            /* size of data_words */
            datawd_len = (uint32_t)R_SSIF_DWLtoLen(p_info_ch->data_word);

            if (0u == datawd_len)
            {
                ret = IOIF_EINVAL;
            }
            else
            {
                /* size of system_words */
                syswd_len = (uint32_t)R_SSIF_SWLtoLen(p_info_ch->system_word);

                if (syswd_len < (datawd_len * dw_per_sw))
                {
                    ret = IOIF_EINVAL;
                }
            }
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: SSIF_Reset
* @brief         SSIF software reset
*
*                Description:<br>
*
* @param[in]     ssif_ch       :SSIF channel
* @retval        none
******************************************************************************/
static void SSIF_Reset(const uint32_t ssif_ch)
{
    uint32_t was_masked;
    uint32_t dummy_read;

    /* change register: enter exclusive */
#if defined (__ICCARM__)
    was_masked = __disable_irq_iar();
#else
    was_masked = __disable_irq();
#endif

    /* SW Reset ON */
    RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSIFCR.LONG, (uint32_t) 1, SSIF_SSIFCR_SSIRST_SHIFT, 
    SSIF_SSIFCR_SSIRST);
    dummy_read = RZA_IO_RegRead_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSIFCR.LONG, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);
    (void)(dummy_read);

    /* SW Reset OFF */
    RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSIFCR.LONG, (uint32_t) 0, SSIF_SSIFCR_SSIRST_SHIFT, 
    SSIF_SSIFCR_SSIRST);
    do {
        dummy_read = RZA_IO_RegRead_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSIFCR.LONG, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);
        /* Wait for SW Reset OFF */
    } while ((dummy_read & SSIF_SSIFCR_SSIRST) != 0);

    /* change register: exit exclusive */
    if (0 == was_masked)
    {
        __enable_irq();
    }

    return;
}


/*******************************************************************************
 * Function Name: R_SSIF_GetVersion
 * Description  :
 * Arguments    : *pinfo - pointer to version information structure
 * Return Value : 0
  ******************************************************************************/
int32_t R_SSIF_GetVersion(st_ver_info_t *pinfo)
{
    pinfo->lld.version.sub.major = gs_lld_info.version.sub.major;
    pinfo->lld.version.sub.minor = gs_lld_info.version.sub.minor;
    pinfo->lld.build = gs_lld_info.build;
    pinfo->lld.p_szdriver_name = gs_lld_info.p_szdriver_name;

    return (0);
}
/*******************************************************************************
 End of function R_WDT_GetVersion
 ******************************************************************************/

