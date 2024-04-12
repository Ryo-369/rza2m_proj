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
* File Name   : r_ssif_lld_int.c
* $Rev: 8404 $
* $Date:: 2018-06-13 14:09:40 +0100#$
* Description : SSIF driver interrupt functions
******************************************************************************/

/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include "r_rza2_ssif_lld.h"
#include "r_ssif_hld_cfg.h"
#include "r_intc_lld_rza2m.h"
#include "iodefine.h"
#include "iobitmask.h"
#include "rza_io_regrw.h"

/*******************************************************************************
Typedef definitions
*******************************************************************************/


/*******************************************************************************
Macro definitions
*******************************************************************************/


/*******************************************************************************
Private global variables and functions
*******************************************************************************/

/******************************************************************************
 Function prototypes
 *****************************************************************************/

static void SSIF_ERI0_Handler(uint32_t int_sense);
static void SSIF_ERI1_Handler(uint32_t int_sense);
static void SSIF_ERI2_Handler(uint32_t int_sense);
static void SSIF_ERI3_Handler(uint32_t int_sense);

static void SSIF_ERI_Handler(const uint32_t ssif_ch);

/******************************************************************************
* Function Name: SSIF_InterruptInit
* @brief         Initialize interrupt setting for SSIF channel.
*
*                Description:<br>
*                Enable interrupt and register interrupt handler.
* @param[in]     ssif_ch    :SSIF channel
* @param[in]     int_level  :GIC priority level of interrupt
* @retval        none
******************************************************************************/
void SSIF_InterruptInit(const uint32_t ssif_ch, const uint8_t int_level)
{
    uint32_t ret;

    switch (ssif_ch)
    {
        case SSIF_CHNUM_0: 
            ret = R_INTC_RegistIntFunc(INTC_ID_SSIF_INT_SSIF_INT_REQ_0, &SSIF_ERI0_Handler);
            if (INTC_SUCCESS != ret)
            {
                /* NON_NOTICE_ASSERT: Illegal IRQ number */
            }
            R_INTC_SetPriority(INTC_ID_SSIF_INT_SSIF_INT_REQ_0, int_level);
            R_INTC_Enable(INTC_ID_SSIF_INT_SSIF_INT_REQ_0);
            break;
        case SSIF_CHNUM_1:
            ret = R_INTC_RegistIntFunc(INTC_ID_SSIF_INT_SSIF_INT_REQ_1, &SSIF_ERI1_Handler);
            if (INTC_SUCCESS != ret)
            {
                /* NON_NOTICE_ASSERT: Illegal IRQ number */
            }
            R_INTC_SetPriority(INTC_ID_SSIF_INT_SSIF_INT_REQ_1, int_level);
            R_INTC_Enable(INTC_ID_SSIF_INT_SSIF_INT_REQ_1);
            break;
        case SSIF_CHNUM_2:
            ret = R_INTC_RegistIntFunc(INTC_ID_SSIF_INT_SSIF_INT_REQ_2, &SSIF_ERI2_Handler);
            if (INTC_SUCCESS != ret)
            {
                /* NON_NOTICE_ASSERT: Illegal IRQ number */
            }
            R_INTC_SetPriority(INTC_ID_SSIF_INT_SSIF_INT_REQ_2, int_level);
            R_INTC_Enable(INTC_ID_SSIF_INT_SSIF_INT_REQ_2);
            break;
        case SSIF_CHNUM_3:
            ret = R_INTC_RegistIntFunc(INTC_ID_SSIF_INT_SSIF_INT_REQ_3, &SSIF_ERI3_Handler);
            if (INTC_SUCCESS != ret)
            {
                /* NON_NOTICE_ASSERT: Illegal IRQ number */
            }
            R_INTC_SetPriority(INTC_ID_SSIF_INT_SSIF_INT_REQ_3, int_level);
            R_INTC_Enable(INTC_ID_SSIF_INT_SSIF_INT_REQ_3);
            break;
        default:
            /* NON_NOTICE_ASSERT: Illegal channel number */
            break;
    }

    return;
}
/*******************************************************************************
 End of function SSIF_InterruptInit
 ******************************************************************************/

/******************************************************************************
* Function Name: SSIF_InterruptShutdown
* @brief         Uninitialize interrupt setting for SSIF channel.
*
*                Description:<br>
*                Disable interrupt and unregister interrupt handler
* @param[in]     ssif_ch    :SSIF channel
* @retval        none
******************************************************************************/
void SSIF_InterruptShutdown(const uint32_t ssif_ch)
{
    switch (ssif_ch)
    {
        case SSIF_CHNUM_0:
            R_INTC_Disable(INTC_ID_SSIF_INT_SSIF_INT_REQ_0);
            break;
        case SSIF_CHNUM_1:
            R_INTC_Disable(INTC_ID_SSIF_INT_SSIF_INT_REQ_1);
            break;
        case SSIF_CHNUM_2:
            R_INTC_Disable(INTC_ID_SSIF_INT_SSIF_INT_REQ_2);
            break;
        case SSIF_CHNUM_3:
            R_INTC_Disable(INTC_ID_SSIF_INT_SSIF_INT_REQ_3);
            break;
        /* ->IPA R3.5.2 : There is nothing to do when unusual conditons. */
        default:
            /* NON_NOTICE_ASSERT: Illegal channel number */
            break;
        /* <-IPA R3.5.2 */
    }
    return;
}
/*******************************************************************************
 End of function SSIF_InterruptShutdown
 ******************************************************************************/

/******************************************************************************
* Function Name: SSIF_EnableErrorInterrupt
* @brief         Enable the SSIF channel error interrupt
*
*                Description:<br>
*
* @param[in]     ssif_ch    :SSIF channel
* @retval        none
******************************************************************************/
void SSIF_EnableErrorInterrupt(const uint32_t ssif_ch)
{
    /* clear error status */
    RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSISR.LONG, (uint32_t) 0, IOREG_NONSHIFT_ACCESS,
    IOREG_NONMASK_ACCESS);

    /* enable error interrupt */
    RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) 1, SSIF_SSICR_TUIEN_SHIFT,
    SSIF_SSICR_TUIEN);
    RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) 1, SSIF_SSICR_TOIEN_SHIFT,
    SSIF_SSICR_TOIEN);
    RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) 1, SSIF_SSICR_RUIEN_SHIFT,
    SSIF_SSICR_RUIEN);
    RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) 1, SSIF_SSICR_ROIEN_SHIFT,
    SSIF_SSICR_ROIEN);
    return;
}
/*******************************************************************************
 End of function SSIF_EnableErrorInterrupt
 ******************************************************************************/

/******************************************************************************
* Function Name: SSIF_DisableErrorInterrupt
* @brief         Disable the SSIF channel error interrupt
*
*                Description:<br>
*
* @param[in]     ssif_ch    :SSIF channel
* @retval        none
******************************************************************************/
void SSIF_DisableErrorInterrupt(const uint32_t ssif_ch)
{
    /* disable error interrupt */
    RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) 0, SSIF_SSICR_TUIEN_SHIFT,
    SSIF_SSICR_TUIEN);
    RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) 0, SSIF_SSICR_TOIEN_SHIFT,
    SSIF_SSICR_TOIEN);
    RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) 0, SSIF_SSICR_RUIEN_SHIFT,
    SSIF_SSICR_RUIEN);
    RZA_IO_RegWrite_32((volatile uint32_t *) &g_ssireg[ssif_ch]->SSICR.LONG, (uint32_t) 0, SSIF_SSICR_ROIEN_SHIFT,
    SSIF_SSICR_ROIEN);
    return;
}
/*******************************************************************************
 End of function SSIF_DisableErrorInterrupt
 ******************************************************************************/

/******************************************************************************
* Function Name: SSIF_ERI<n>_Handler
* @brief         SSIF error interrupt handler per channel
*
*                Description:<br>
*
* @param         none
* @retval        none
******************************************************************************/

static void SSIF_ERI0_Handler(uint32_t int_sense)
{
    UNUSED_PARAM(int_sense);
    SSIF_ERI_Handler(SSIF_CHNUM_0);
    return;
}

static void SSIF_ERI1_Handler(uint32_t int_sense)
{
    UNUSED_PARAM(int_sense);
    SSIF_ERI_Handler(SSIF_CHNUM_1);
    return;
}

static void SSIF_ERI2_Handler(uint32_t int_sense)
{
    UNUSED_PARAM(int_sense);
    SSIF_ERI_Handler(SSIF_CHNUM_2);
    return;
}

static void SSIF_ERI3_Handler(uint32_t int_sense)
{
    UNUSED_PARAM(int_sense);
    SSIF_ERI_Handler(SSIF_CHNUM_3);
    return;
}
/*******************************************************************************
 End of function SSIF_ERI0_Handler
 ******************************************************************************/

/******************************************************************************
* Function Name: SSIF_ERI_Handler
* @brief         SSIF error interrupt handler common function
*
*                Description:<br>
*
* @param[in]     ssif_ch    :SSIF channel
* @retval        none
******************************************************************************/
static void SSIF_ERI_Handler(const uint32_t ssif_ch)
{
	uint32_t	ssisr_sts;

    ssif_info_ch_t* const p_info_ch = &g_ssif_info_drv.info_ch[ssif_ch];

    ssisr_sts = RZA_IO_RegRead_32((volatile uint32_t *) &g_ssireg[p_info_ch->channel]->SSISR.LONG, IOREG_NONSHIFT_ACCESS,
    IOREG_NONMASK_ACCESS);

    if (0u != (ssisr_sts & SSIF_SR_INT_ERR_MASK))
    {
        /* Restart or Callback */
        R_SSIF_ErrorRecovery(p_info_ch);
    }
    return;
}
/*******************************************************************************
 End of function SSIF_ERI_Handler
 ******************************************************************************/
