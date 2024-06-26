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
 * @headerfile     r_ssif_hld_cfg.h
 * @brief          ssif low level interface header file
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZA2M
 *****************************************************************************/
/*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef SSIF_IF_H
#define SSIF_IF_H

/*************************************************************************//**
 * @ingroup R_SW_PKG_246_SSIF_API
 * @defgroup R_SW_PKG_246_SSIF_LLD_IF SSIF Low Level Driver API
 * @brief Internal low level SSIF driver component
 *
 * @anchor R_SW_PKG_246_SSIF_LLD_IF_SUMMARY
 * @par Summary
 * @brief This interface defines the functionality that the low level driver
 * must implement for compatibility with the high level driver.
 *
 * @anchor R_SW_PKG_246_SSIF_LLD_IF_LIMITATIONS
 * @par Known Limitations
 * @ref NONE_YET
 *
 * @anchor R_SW_PKG_246_SSIF_LLD_IF_INSTANCES
 * @par Known Implementations
 * @ref RENESAS_APPLICATION_SOFTWARE_PACKAGE
 * @{
 *****************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

#include "r_typedefs.h"
#include "r_typedefs.h"
#include "r_devlink_wrapper.h"
#include "r_os_abstraction_api.h"


/******************************************************************************
 Defines
 *****************************************************************************/

/******************************************************************************
 Constant Macros
 *****************************************************************************/
#define SSIF_NUM_CHANS       (4u)    /**< Number of SSIF channels */

/* Below CLK settings depends on user environment */
#define RENESAS_RZ_A2_AUDIO_CLK   ( 11289600u)
#define RENESAS_RZ_A2_AUDIO_X1    (        0u)

/******************************************************************************
 Function Macros
 *****************************************************************************/
#define SSIF_SAMPLE_FREQENCY    44100u

/******************************************************************************
 Enumerated Types
 *****************************************************************************/

/** SSICR:CKS(Clock source for oversampling) */
typedef enum
{
    SSIF_CFG_CKS_AUDIO_CLK  = 0,  /**< select AUDIO_X1   */
    SSIF_CFG_CKS_AUDIO_X1   = 1   /**< select AUIDIO_CLK */
} ssif_chcfg_cks_t;

/** SSICR:FRM(Frame word count) */
typedef enum
{
    SSIF_CFG_FRM_00B = 0, 
    SSIF_CFG_FRM_01B = 1, 
    SSIF_CFG_FRM_10B = 2, 
    SSIF_CFG_FRM_11B = 3  
} ssif_chcfg_frame_word_t;

/** SSICR:DWL(Data word length) */
typedef enum
{
    SSIF_CFG_DATA_WORD_8  = 0,   /**< Data word length 8  */
    SSIF_CFG_DATA_WORD_16 = 1,   /**< Data word length 16 */
    SSIF_CFG_DATA_WORD_18 = 2,   /**< Data word length 18 */
    SSIF_CFG_DATA_WORD_20 = 3,   /**< Data word length 20 */
    SSIF_CFG_DATA_WORD_22 = 4,   /**< Data word length 22 */
    SSIF_CFG_DATA_WORD_24 = 5,   /**< Data word length 24 */
    SSIF_CFG_DATA_WORD_32 = 6    /**< Data word length 32 */
} ssif_chcfg_data_word_t;

/** SSICR:SWL(System word length) */
typedef enum
{
    SSIF_CFG_SYSTEM_WORD_8   = 0,    /**< System word length 8   */
    SSIF_CFG_SYSTEM_WORD_16  = 1,    /**< System word length 16  */
    SSIF_CFG_SYSTEM_WORD_24  = 2,    /**< System word length 24  */
    SSIF_CFG_SYSTEM_WORD_32  = 3,    /**< System word length 32  */
    SSIF_CFG_SYSTEM_WORD_48  = 4,    /**< System word length 48  */
    SSIF_CFG_SYSTEM_WORD_64  = 5,    /**< System word length 64  */
    SSIF_CFG_SYSTEM_WORD_128 = 6,    /**< System word length 128 */
    SSIF_CFG_SYSTEM_WORD_256 = 7     /**< System word length 256 */
} ssif_chcfg_system_word_t;

/** SSICR:MST(Master mode or slave mode) */
typedef enum
{
    SSIF_CFG_SLAVE   = 0,    /**< Slave mode  */
    SSIF_CFG_MASTER  = 1     /**< Master mode */
} ssif_chcfg_mst_slv_t;

/** SSICR:BCKP(Clock polarity) */
typedef enum
{
    SSIF_CFG_FALLING = 0,    /**< Falling edge */
    SSIF_CFG_RISING  = 1     /**< Rising edge  */
} ssif_chcfg_clock_pol_t;

/** SSICR:LRCKP(LR initial Clock */
typedef enum
{
    SSIF_CFG_HIGH_LEVEL = 0,    /**< Falling edge */
    SSIF_CFG_LOW_LEVEL  = 1     /**< Rising edge  */
} ssif_chcfg_lr_clock_pol_t;

/** SSICR:SPDP(Serial padding polarity) */
typedef enum
{
    SSIF_CFG_PADDING_LOW  = 0,   /**< Padding bits are low  */
    SSIF_CFG_PADDING_HIGH = 1    /**< Padding bits are high */
} ssif_chcfg_padding_pol_t;

/** SSICR:SDTA(Serial data alignment) */
typedef enum
{
    SSIF_CFG_DATA_FIRST    = 0,  /**< Data first         */
    SSIF_CFG_PADDING_FIRST = 1   /**< Padding bits first */
} ssif_chcfg_serial_alignment_t;

/** SSICR:PDTA(Parallel data alignment) */
typedef enum
{
    SSIF_CFG_LEFT  = 0,  /**< Left aligned  */
    SSIF_CFG_RIGHT = 1   /**< Right aligned */
} ssif_chcfg_parallel_alignment_t;

/** SSICR:DEL(Serial data delay) */
typedef enum
{
    SSIF_CFG_DELAY    = 0,   /**< 1 clock delay */
    SSIF_CFG_NO_DELAY = 1    /**< No delay      */
} ssif_chcfg_ws_delay_t;

/** SSICR:MUEN(Mute on/off) */
typedef enum
{
    SSIF_CFG_MUTE_OFF  = 0,
    SSIF_CFG_MUTE_ON   = 1
} ssif_chcfg_mute_ena_t;

/** SSICR:CKDV(Serial oversampling clock division ratio) */
typedef enum
{
    SSIF_CFG_CKDV_BITS_1   = 0,
    SSIF_CFG_CKDV_BITS_2   = 1,
    SSIF_CFG_CKDV_BITS_4   = 2,
    SSIF_CFG_CKDV_BITS_8   = 3,
    SSIF_CFG_CKDV_BITS_16  = 4,
    SSIF_CFG_CKDV_BITS_32  = 5,
    SSIF_CFG_CKDV_BITS_64  = 6,
    SSIF_CFG_CKDV_BITS_128 = 7,
    SSIF_CFG_CKDV_BITS_6   = 8,
    SSIF_CFG_CKDV_BITS_12  = 9,
    SSIF_CFG_CKDV_BITS_24  = 10,
    SSIF_CFG_CKDV_BITS_48  = 11,
    SSIF_CFG_CKDV_BITS_96  = 12
} ssif_chcfg_ckdv_t;


/** SNCR:SSIxNL(Serial sound interface channel x noise canceler enable) */
typedef enum
{
    SSIF_CFG_DISABLE_NOISE_CANCEL = 0, /**< Not use noise cancel function */
    SSIF_CFG_ENABLE_NOISE_CANCEL  = 1  /**< Use noise cancel function     */
} ssif_chcfg_noise_cancel_t;


/** SSITDMR:TDM(TDM mode) */
typedef enum
{
    SSIF_CFG_DISABLE_TDM = 0,   /**< not TDM mode */
    SSIF_CFG_ENABLE_TDM  = 1    /**< set TDM mode */
} ssif_chcfg_tdm_t;

/******************************************************************************
 Structures
 *****************************************************************************/

/**< This structure contains the configuration settings */
typedef struct
{
    bool_t                          enabled;             /* The enable flag for the channel       */
    uint8_t                         int_level;           /* Interrupt priority for the channel    */
    bool_t                          slave_mode;          /* Mode of operation                     */
    uint32_t                        sample_freq;         /* Audio Sampling frequency(Hz)          */
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
    ssif_chcfg_noise_cancel_t       noise_cancel;        /* GPIO-SNCR : Noise cancel              */
    ssif_chcfg_tdm_t                tdm_mode;            /* SSITDMR-TDM: TDM mode                 */
} ssif_channel_cfg_t;

/******************************************************************************
 External Data
 *****************************************************************************/


/******************************************************************************
 Function Prototypes
 *****************************************************************************/

/**
* @brief         Convert SSICR:SWL bits to system word length
* @param[in]     ssicr_swl  :SSICR register SWL field value(0 to 7)
* @retval        8 to 256   :system word length(byte)
*/
extern int_t R_SSIF_SWLtoLen(const ssif_chcfg_system_word_t ssicr_swl);

/**
* @brief         This function initialise pin multiplex settings.
* @param[in]     ssif_ch      :channel number.
* @retval        IOIF_ESUCCESS: Success
*                error code   : Failure.
*/
extern int_t R_SSIF_Userdef_InitPinMux(const uint32_t ssif_ch);

/**
* @brief         Calculates value for divided audio clock.
* @param[in]     p_ch_cfg     :pointer of channel configuration parameter.
* @param[in,out] p_clk_div    :pointer of SSICR register CKDV value
* @retval        IOIF_ESUCCESS:Success.
* @retval        error code   :Failure.
*/
extern int_t R_SSIF_Userdef_SetClockDiv(const ssif_channel_cfg_t* const p_ch_cfg, ssif_chcfg_ckdv_t* const p_clk_div);

/**
* @brief         Get Driver version
* @param[out] pinfo - pointer to version information structure
* @retval        IOIF_ESUCCESS:Success.
*/
extern int32_t R_SSIF_GetVersion(st_ver_info_t *pinfo);

/**
* @brief         This function initialise pin multiplex settings.
* @param[in]     ssif_ch       :channel number.
* @retval        IOIF_ESUCCESS :Success.
* @retval        error code    :Failure.
*/
extern int_t SSIF_PortSetting(const uint32_t ssif_ch);

#ifdef __cplusplus
}
#endif

#endif /* SSIF_IF_H */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
/*EOF*/

