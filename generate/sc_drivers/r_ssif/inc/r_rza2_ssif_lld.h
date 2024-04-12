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
 * and to discontinue the availability of this software. By using this
 * software, you agree to the additional terms and conditions found by
 * accessing the following link:
 * http://www.renesas.com/disclaimer
 *******************************************************************************
 * Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/******************************************************************************
 * @headerfile     r_rza2_ssif_lld.h
 * @brief          ssif low level header file
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZA2M
 *****************************************************************************/
/*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef SSIF_H
#define SSIF_H

/*************************************************************************//**
 * @ingroup R_SW_PKG_246_SSIF_API
 * @defgroup R_SW_PKG_246_SSIF_LLD SSIF Low Level Driver API
 * @brief Internal low level SSIF driver component
 *
 * @anchor R_SW_PKG_246_SSIF_LLD_SUMMARY
 * @par Summary
 * @brief This interface defines the functionality that the low level driver
 * must implement for compatibility with the high level driver.
 *
 * @anchor R_SW_PKG_246_SSIF_LLD_LIMITATIONS
 * @par Known Limitations
 * @ref NONE_YET
 *
 * @anchor R_SW_PKG_246_SSIF_LLD_INSTANCES
 * @par Known Implementations
 * @ref RENESAS_APPLICATION_SOFTWARE_PACKAGE
 * @{
 *****************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_typedefs.h"
#include "r_ssif_hld_cfg.h"
#include "r_ssif_lld_aioif.h"

/******************************************************************************
Macro definitions
******************************************************************************/
/* Version Number of API */
/** Driver Name */
#define STDIO_SSIF_RZ_LLD_DRV_NAME ("LLD EBK_RZA2M SSIF")

/** Major Version Number of API */
#define STDIO_SSIF_RZ_LLD_VERSION_MAJOR      (1)
/** Minor Version Number of API */
#define STDIO_SSIF_RZ_LLD_VERSION_MINOR      (0)
/** Minor Version Number of API */
#define STDIO_SSIF_RZ_LLD_BUILD_NUM          (0)
/** Unique ID */
#define STDIO_SSIF_RZ_LLD_UID                (244)


#define SSIF_CHNUM_0    (0u)
#define SSIF_CHNUM_1    (1u)
#define SSIF_CHNUM_2    (2u)
#define SSIF_CHNUM_3    (3u)

#define SSIF_CHSTR_0 "\\0"
#define SSIF_CHSTR_1 "\\1"
#define SSIF_CHSTR_2 "\\2"
#define SSIF_CHSTR_3 "\\3"

#define SSIF_CR_INT_ERR_MASK    (SSIF_SSICR_TUIEN | SSIF_SSICR_TOIEN | SSIF_SSICR_RUIEN | SSIF_SSICR_ROIEN)
#define SSIF_SR_INT_ERR_MASK    (SSIF_SSISR_TUIRQ | SSIF_SSISR_TOIRQ | SSIF_SSISR_RUIRQ | SSIF_SSISR_ROIRQ)

/* noise canceled bit */
#define GPIO_SNCR_BIT_SSI5NCE  (1u << 5)
#define GPIO_SNCR_BIT_SSI4NCE  (1u << 4)
#define GPIO_SNCR_BIT_SSI3NCE  (1u << 3)
#define GPIO_SNCR_BIT_SSI2NCE  (1u << 2)
#define GPIO_SNCR_BIT_SSI1NCE  (1u << 1)
#define GPIO_SNCR_BIT_SSI0NCE  (1u << 0)

#define SSIF_ADDRESS_LIST \
{   /* ->MISRA 11.3 */ /* ->SEC R2.7.1 */ \
    &SSIF0, &SSIF1, &SSIF2, &SSIF3 \
}   /* <-MISRA 11.3 */ /* <-SEC R2.7.1 */ /* { } is for MISRA 19.4 */

#define	SSIF_CPG_STBCR    (CPG.STBCR7.BYTE)

#define SSIF_CPG_STBCR_CHNUM_0  (0x08u) /* SSIF0 */
#define SSIF_CPG_STBCR_CHNUM_1  (0x04u) /* SSIF1 */
#define SSIF_CPG_STBCR_CHNUM_2  (0x02u) /* SSIF2 */
#define SSIF_CPG_STBCR_CHNUM_3  (0x01u) /* SSIF3 */

#define SSIF_CPG_STBCR_CHNUM_MASK	(SSIF_CPG_STBCR_CHNUM_0 | SSIF_CPG_STBCR_CHNUM_1 | SSIF_CPG_STBCR_CHNUM_2 | SSIF_CPG_STBCR_CHNUM_3)

/******************************************************************************
Private global variables and functions
******************************************************************************/

/*************************************************************************
 Enumerated Types
*************************************************************************/
/** Driver initialisation status */
typedef enum {
    SSIF_DRVSTS_UNINIT = 0,
    SSIF_DRVSTS_INIT
} ssif_drv_stat_t;

/** Channel initialisation status */
typedef enum
{
    SSIF_CHSTS_UNINIT = 0,
    SSIF_CHSTS_INIT,
    SSIF_CHSTS_OPEN
} ssif_ch_stat_t;

/* Read or write mode selection */
typedef enum
{
    SSIF_ASYNC_W = 0,
    SSIF_ASYNC_R
} ssif_rw_mode_t;

/** Serial bit clock direction */
typedef enum
{
    SSIF_CFG_CLOCK_IN  = 0,  /**< Clock IN - Slave mode */
    SSIF_CFG_CLOCK_OUT = 1   /**< Clock OUT - Master mode */
} ssif_chcfg_clock_dir_t;


/** Serial word select direction */
typedef enum
{
    SSIF_CFG_WS_IN  = 0,     /**< Word select IN - Slave mode */
    SSIF_CFG_WS_OUT = 1      /**< Word select OUT - Master mode */
} ssif_chcfg_ws_dir_t;

/*************************************************************************
 Structures
*************************************************************************/
/** Channel information structure */
typedef struct ssif_info_ch
{
    uint32_t    channel;
    bool_t      enabled;
    uint8_t     int_level;           /* Interrupt priority for the channel    */
    bool_t      slave_mode;
    uint32_t    sample_freq;
    ssif_ch_stat_t ch_stat;
    uint32_t    sem_access;
    AHF_S       tx_que;
    AHF_S       rx_que;
    int_t       dma_rx_ch;
    int_t       dma_tx_ch;
    bool_t      is_full_duplex;         /* full/half duplex */
    int_t       openflag;
    AIOCB*      p_aio_tx_curr;
    AIOCB*      p_aio_tx_next;
    AIOCB*      p_aio_rx_curr;
    AIOCB*      p_aio_rx_next;
    ssif_chcfg_clock_dir_t          clock_direction;
    ssif_chcfg_ws_dir_t             ws_direction;
    ssif_chcfg_cks_t                clk_select;          /* SSICR-CKS : Audio clock select        */
    ssif_chcfg_frame_word_t         frame_word;          /* SSICR-FRM: Audio Frame word           */
    ssif_chcfg_data_word_t          data_word;           /* SSICR-DWL : Data word length          */
    ssif_chcfg_system_word_t        system_word;         /* SSICR-SWL : System word length        */
    ssif_chcfg_mst_slv_t            master_slave;        /* SSICR-MST : master mode or slave mode */
    ssif_chcfg_clock_pol_t          bclk_pol;            /* SSICR-BCKP: Bit Clock polarity        */
    ssif_chcfg_lr_clock_pol_t       lr_clock;            /* SSICR-LRCKP: LR Clock                 */
    ssif_chcfg_padding_pol_t        padding_pol;         /* SSICR-SPDP: Padding polarity          */
    ssif_chcfg_serial_alignment_t   serial_alignment;    /* SSICR-SDTA: Serial data alignment     */
    ssif_chcfg_parallel_alignment_t parallel_alignment;  /* SSICR-PDTA: Parallel data alignment   */
    ssif_chcfg_ws_delay_t           ws_delay;            /* SSICR-DEL : Serial clock delay        */
    ssif_chcfg_mute_ena_t           mute_onoff;          /* SSICR-MUEN : Mute on/off              */
    ssif_chcfg_noise_cancel_t       noise_cancel;
    ssif_chcfg_tdm_t                tdm_mode;
    ssif_chcfg_ckdv_t               clk_div;
} ssif_info_ch_t;

/** driver information structure */
typedef struct ssif_info_drv
{
    ssif_drv_stat_t drv_stat;
    ssif_info_ch_t  info_ch[SSIF_NUM_CHANS];
} ssif_info_drv_t;

/** SSIF peripheral register access */
extern volatile struct st_ssif* const g_ssireg[SSIF_NUM_CHANS];

/******************************************************************************
 Function Prototypes
 *****************************************************************************/

/**
 * @brief Initialise the SSIF driver's internal data
 * @param[in] p_cfg_data :pointer of several parameters array per channels
 * @return    DEVDRV_SUCCESS   :Success.
 *            error code :Failure.
 **/
int_t R_SSIF_Initialise(const ssif_channel_cfg_t* const p_cfg_data);

/**
 * @brief UnInitialize the SSIF driver's internal data
 * @return DEVDRV_SUCCESS   :Success.
 **/
int_t R_SSIF_UnInitialise(void);

/**
 * @brief Enable the SSIF channel
 * @param[in,out] p_info_ch  :channel object
 * @return    DEVDRV_SUCCESS   :Success.
 *            error code :Failure.
 **/
int_t R_SSIF_EnableChannel(ssif_info_ch_t* const p_info_ch);

/**
 * @brief Disable the SSIF channel
 * @param[in,out] p_info_ch  :channel object
 * @return    DEVDRV_SUCCESS   :Success.
 *            error code :Failure.
 **/
int_t R_SSIF_DisableChannel(ssif_info_ch_t* const p_info_ch);

/**
 * @brief Restart the SSIF channel

*         When normal mode<br>
*         Stop and restart DMA transfer.<br>
*         Note: This function execute in interrupt context.
* @param[in,out] p_info_ch  :channel object
 **/
void  R_SSIF_ErrorRecovery(ssif_info_ch_t* const p_info_ch);

/**
 * @brief Enqueue asynchronous read/write request
 * @param[in,out] p_info_ch  :channel object
 * @param[in,out] p_aio      :aio control block of read/write request
 **/
void R_SSIF_PostAsyncIo(ssif_info_ch_t* const p_info_ch, AIOCB* const p_aio);

/**
 * @brief Cancel read or write request(s)
 * @param[in,out] p_info_ch  :channel object
 * @param[in,out] p_aio      :aio control block to cancel or NULL to cancel all.
 **/
void R_SSIF_PostAsyncCancel(ssif_info_ch_t* const p_info_ch, AIOCB* const p_aio);

/**
 * @brief Save configuration to the SSIF driver. Update channel object.
 * @param[in,out] p_info_ch  :channel object
 * @param[in]     p_ch_cfg   :SSIF channel configuration parameter
 * @return        DEVDRV_SUCCESS   :Success.
 *                error code       :Failure.
 **/
int_t R_SSIF_IOCTL_ConfigChannel(ssif_info_ch_t* const p_info_ch,
                const ssif_channel_cfg_t* const p_ch_cfg);

/**
 * @brief Get a value of SSISR register.
 * @param[in]     p_info_ch  :channel object
 * @param[in,out] p_status   :pointer of status value
 * @return        DEVDRV_SUCCESS   :Success.
 *                error code       :Failure.
 **/
int_t R_SSIF_IOCTL_GetStatus(const ssif_info_ch_t* const p_info_ch, uint32_t* const p_status);

/**
 * @brief Allocate and Setup DMA_CH for specified SSIF channel.
 * @param[in,out] p_info_ch  :channel object.
 * @return        DEVDRV_SUCCESS   :Success.
 *                error code       :Failure.
 **/
int_t R_SSIF_InitDMA(ssif_info_ch_t* const p_info_ch);

/**
 * @brief Free DMA_CH for specified SSIF channel.
 * @param[in,out] p_info_ch  :channel object.
 **/
void R_SSIF_UnInitDMA(ssif_info_ch_t* const p_info_ch);

/**
 * @brief Pause DMA transfer for specified SSIF channel.
 * @param[in,out] p_info_ch  :channel object.
 **/
void R_SSIF_CancelDMA(const ssif_info_ch_t* const p_info_ch);

/**
 * @brief Setup DMA_CH for specified SSIF channel(without allocate)
 * @param[in,out] p_info_ch  :channel object.
 * @return        DEVDRV_SUCCESS   :Success.
 *                error code       :Failure.
 **/
int_t R_SSIF_RestartDMA(ssif_info_ch_t* const p_info_ch);

/**
 * @brief Convert SSICR:SWL bits to system word length
 * @param[in]     ssicr_swl  :SSICR register SWL field value(0 to 7)
 * @return        8 to 256   :system word length(bits)
 **/
int_t R_SSIF_SWLtoLen(const ssif_chcfg_system_word_t ssicr_swl);

/**
 * @brief Convert SSICR:DWL bits to data word length
 * @param[in]     ssicr_dwl  :SSICR register DWL field value(0 to 6)
 * @return        8 to 32    :data word length(bits)
 **/
int_t R_SSIF_DWLtoLen(const ssif_chcfg_data_word_t ssicr_dwl);

/** ssif driver main data structure */
extern ssif_info_drv_t g_ssif_info_drv;

#endif /* SSIF_H */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
