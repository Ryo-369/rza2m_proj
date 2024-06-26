/***********************************************************************************************************************
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
 * Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_dmac_drv_link.h
 * Description  : Link high-low level driver include file
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 27.06.2018 1.00     First Release
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup Interface_Library
 * @defgroup RZA1H_RSK_GPIO_LINK RSK+RZA1H GPIO driver link configuration
 * @brief This interface manages the link between the high and low layer drivers (HLD & LLD).
 *
 * @section RZA1_RSK_GPIO_LINK_SUMMARY Summary
 * @brief The INTC driver for the RSK+RZA1H is comprised of the following components ...
 *
 * @section RZA1H_RSK_GPIO_LINK_API_LIMITATIONS Known Limitations
 * NONE_YET
 *
 * @section RZA1H_RSK_GPIO_LINK_API_INSTANCES Known Implementations
 * @ref NONE_YET
 *
 * @section RZA1H_RSK_GPIO_LINK_API_RELATED Related modules
 * @ingroup RZA1H_RSK_GPIO_DRIVER
 * See also:
 * @ref RZA1H_RSK_GPIO_DRIVER
 * @ref RZA1H_RSK_GPIO
 * @{
 **********************************************************************************************************************/

#include "r_dmac_lld_rza2m.h"       /* low layer to high layer API */
#include "r_intc_lld_rza2m.h"       /* INTC low layer driver used in HLD */

#ifndef R_DMAC_INC_R_DMAC_DRV_LINK_H_
#define R_DMAC_INC_R_DMAC_DRV_LINK_H_

/***********************************************************************************************************************
 Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

#endif /* R_DMAC_INC_R_DMAC_DRV_LINK_H_ */

/******************************************************************************
 End  Of File
 ******************************************************************************/

/*******************************************************************************************************************//**
 * @} (end addtogroup RZA1H_RSK_DMAC_DRV_LINK)
 ***********************************************************************************************************************/
