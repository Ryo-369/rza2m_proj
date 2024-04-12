/**********************************************************************************************************************
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
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * @headerfile     r_rza2_ssif_lld_cfg.h
 * @brief          SSIF Low Layer Driver cfg header
 * H/W Platform    RZ/A2M
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History       : DD.MM.YYYY Version Description
 *               : 05.08.2020 1.00    Release
 *********************************************************************************************************************/

#ifndef R_SSIF_INC_R_RZA2_SSIF_LLD_CFG_H_
#define R_SSIF_INC_R_RZA2_SSIF_LLD_CFG_H_

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Macro Definitions
 *********************************************************************************************************************/

/* This define specifies what channels are supported by this Low Layer Driver */
/* It is used to inform the High Layer Driver the supported channels */
#define SSIF_CFG_LLD_SUPPORTED_CHANNELS     (R_CH0 | R_CH1 | R_CH2 | R_CH3)

/* The define specifies the total number of channels supported by this Low Layer Driver */
#define SSIF_CFG_LLD_NUM_CHANNELS           (4)

/* The define specifies Final available channel  */
#define R_PRV_CFG_SSIF_LAST_CHANNEL         (4)

/**********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/

#endif /* R_SSIF_INC_R_RZA2_SSIF_LLD_CFG_H_ */
