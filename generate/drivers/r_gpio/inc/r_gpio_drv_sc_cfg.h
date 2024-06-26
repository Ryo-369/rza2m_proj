/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_gpio_drv_sc_cfg.h
* Version      : 1.0.0
* Device(s)    : R7S921053
* Description  : Pin Configuration.
* Creation Date: 2023-03-28
***********************************************************************************************************************/

#ifndef DRIVERS_R_GPIO_INC_R_GPIO_DRV_SC_CFG_H_
#define DRIVERS_R_GPIO_INC_R_GPIO_DRV_SC_CFG_H_

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_gpio_drv_api.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Variable Externs
******************************************************************************/

/* This code is auto-generated. Do not edit manually */
static const st_r_drv_gpio_sc_config_t GPIO_SC_TABLE_INIT[] =
{
    /* {<pin>, {<function>, <tint>, <current>}}, */
    {GPIO_PORT_6_PIN_6, {GPIO_FUNC_OUT_HIGH, GPIO_TINT_DISABLE, GPIO_CURRENT_4mA}},
    {GPIO_PORT_6_PIN_7, {GPIO_FUNC_IN, GPIO_TINT_DISABLE, GPIO_CURRENT_RESERVED}},
    {GPIO_PORT_8_PIN_2, {GPIO_FUNC_IN, GPIO_TINT_DISABLE, GPIO_CURRENT_RESERVED}},
    {GPIO_PORT_D_PIN_6, {GPIO_FUNC_IN, GPIO_TINT_DISABLE, GPIO_CURRENT_RESERVED}},
    {GPIO_PORT_D_PIN_7, {GPIO_FUNC_IN, GPIO_TINT_DISABLE, GPIO_CURRENT_RESERVED}},
};
static const st_r_drv_gpio_sc_config_t GPIO_SC_TABLE_MANUAL[] =
{
    /* {<pin>, {<function>, <tint>, <current>}}, */
};
static const st_r_drv_gpio_sc_config_t GPIO_SC_TABLE_riic1[] =
{
    /* {<pin>, {<function>, <tint>, <current>}}, */
    {GPIO_PORT_D_PIN_2, {GPIO_FUNC_PERIPHERAL1, GPIO_TINT_DISABLE, GPIO_CURRENT_RESERVED}},
    {GPIO_PORT_D_PIN_3, {GPIO_FUNC_PERIPHERAL1, GPIO_TINT_DISABLE, GPIO_CURRENT_RESERVED}},
};
static const st_r_drv_gpio_sc_config_t GPIO_SC_TABLE_scifa4[] =
{
    /* {<pin>, {<function>, <tint>, <current>}}, */
    {GPIO_PORT_9_PIN_0, {GPIO_FUNC_PERIPHERAL4, GPIO_TINT_DISABLE, GPIO_CURRENT_4mA}},
    {GPIO_PORT_9_PIN_1, {GPIO_FUNC_PERIPHERAL4, GPIO_TINT_DISABLE, GPIO_CURRENT_RESERVED}},
};
static const st_r_drv_gpio_sc_config_t GPIO_SC_TABLE_ssif3[] =
{
    /* {<pin>, {<function>, <tint>, <current>}}, */
    {GPIO_PORT_J_PIN_0, {GPIO_FUNC_PERIPHERAL5, GPIO_TINT_DISABLE, GPIO_CURRENT_RESERVED}},
    {GPIO_PORT_J_PIN_2, {GPIO_FUNC_PERIPHERAL5, GPIO_TINT_DISABLE, GPIO_CURRENT_8mA}},
    {GPIO_PORT_J_PIN_3, {GPIO_FUNC_PERIPHERAL5, GPIO_TINT_DISABLE, GPIO_CURRENT_8mA}},
    {GPIO_PORT_J_PIN_4, {GPIO_FUNC_PERIPHERAL5, GPIO_TINT_DISABLE, GPIO_CURRENT_8mA}},
};
/* End of modification */

/******************************************************************************
Functions Prototypes
******************************************************************************/

#endif  /* DRIVERS_R_GPIO_INC_R_GPIO_DRV_SC_CFG_H_ */

/* End of File */
