/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_ssif_drv_sc_cfg.h
 * Description  : SSIF driver configuration header for Smart Configurator
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History      : DD.MM.YYYY Version Description
 *              : 29.06.2020 1.00    Release
 *********************************************************************************************************************/

#include "r_typedefs.h"
#include "r_ssif_drv_api.h"
#include "r_gpio_drv_api.h"
#include "r_gpio_drv_sc_cfg.h"

#ifndef SRC_RENESAS_CONFIG_R_SSIF_DRV_SC_CFG_H_
#define SRC_RENESAS_CONFIG_R_SSIF_DRV_SC_CFG_H_

typedef struct
{
    uint8_t                         int_level;           /* Interrupt priority for the channel    */
    ssif_chcfg_mst_slv_t            master_slave;        /* SSICR-MST : master mode or slave mode */
    uint32_t                        sample_freq;         /* Audio Sampling frequency(Hz)          */
    ssif_chcfg_cks_t                clk_select;          /* SSICR-CKS : Audio clock select        */
    ssif_chcfg_frame_word_t         frame_word;          /* SSICR-FRM: Audio Frame word           */
    ssif_chcfg_data_word_t          data_word;           /* SSICR-DWL : Data word length          */
    ssif_chcfg_system_word_t        system_word;         /* SSICR-SWL : System word length        */
    ssif_chcfg_clock_pol_t          bclk_pol;            /* SSICR-BCKP: Bit Clock polarity        */
    ssif_chcfg_lr_clock_pol_t       lr_clock;            /* SSICR-LRCKP: LR Clock                 */
    ssif_chcfg_padding_pol_t        padding_pol;         /* SSICR-SPDP: Padding polarity          */
    ssif_chcfg_serial_alignment_t   serial_alignment;    /* SSICR-SDTA: Serial data alignment     */
    ssif_chcfg_parallel_alignment_t parallel_alignment;  /* SSICR-PDTA: Parallel data alignment   */
    ssif_chcfg_ws_delay_t           ws_delay;            /* SSICR-DEL : Serial clock delay        */
    ssif_chcfg_mute_ena_t           mute_onoff;          /* SSICR-MUEN : Mute on/off              */
    ssif_chcfg_noise_cancel_t       noise_cancel;        /* GPIO-SNCR : Noise cancel              */
    ssif_chcfg_tdm_t                tdm_mode;            /* SSITDMR-TDM: TDM mode                 */
} st_ssif_config_t;

/**
 * @typedef st_r_drv_ssif_sc_config_t Smart Configurator Interface
 */
typedef struct
{
    r_channel_t                     channel;
    st_ssif_config_t                config;
    st_r_drv_gpio_pin_init_table_t  pin;
} st_r_drv_ssif_sc_config_t;

/**
 * @section SSIF Smart Configurator settings table.
 */
static const st_r_drv_ssif_sc_config_t SSIF_SC_TABLE[] =
{
    /* This code is auto-generated. Do not edit manually */
    { 3, 
        {
            30, 
            SSIF_CFG_SLAVE, 
            44100, 
            SSIF_CFG_CKS_AUDIO_X1, 
            SSIF_CFG_FRM_00B, 
            SSIF_CFG_DATA_WORD_16, 
            SSIF_CFG_SYSTEM_WORD_24, 
            SSIF_CFG_FALLING, 
            SSIF_CFG_HIGH_LEVEL, 
            SSIF_CFG_PADDING_LOW, 
            SSIF_CFG_DATA_FIRST, 
            SSIF_CFG_LEFT, 
            SSIF_CFG_DELAY, 
            SSIF_CFG_MUTE_OFF, 
            SSIF_CFG_ENABLE_NOISE_CANCEL, 
            SSIF_CFG_DISABLE_TDM, 
        }, 
        {
            &GPIO_SC_TABLE_ssif3[0], 
            sizeof(GPIO_SC_TABLE_ssif3)/sizeof(st_r_drv_gpio_sc_config_t), 
        }
    },
    /* End of modification */
};


#endif /* SRC_RENESAS_CONFIG_R_SSIF_DRV_SC_CFG_H_ */

/*******************************************************************************************************************//**
 End  Of File
 ***********************************************************************************************************************/
