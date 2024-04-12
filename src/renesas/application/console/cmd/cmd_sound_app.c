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
 * and to discontinue the availability of this software. By using this software,
 * you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *******************************************************************************
 * Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************
 * File Name    : cmd_sound_app.c
 * Version      : 1.0
 * Device(s)    : Renesas
 * Tool-Chain   : N/A
 * OS           : N/A
 * H/W Platform : RSK+
 * Description  : The Sound application specific commands
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 14.06.2018 1.00 First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

/******************************************************************************
 System Includes
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

/******************************************************************************
 User Includes
 ******************************************************************************/

#include "r_typedefs.h"
#include "compiler_settings.h"
#include "console.h"
#include "r_task_priority.h"
#include "main.h"
#include "r_os_abstraction_api.h"
#include "r_sound_app.h"
#include "r_sound.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
 External functions
 ******************************************************************************/

 /*****************************************************************************
 External Variables
 ******************************************************************************/

/******************************************************************************
 Private Functions
 ******************************************************************************/

static int16_t cmd_play_sound_app(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom);
static int16_t cmd_record_sound_app(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom);

/*****************************************************************************
 Global Variables
 ******************************************************************************/

/* Table that associates command letters, function pointer and a little
 description of what the command does */
static st_cmdfnass_t s_cmd_sound_app[] =
{
     {
      "play",
      cmd_play_sound_app,
      "<CR> - run play sound application ",
     },
     {
      "record",
      cmd_record_sound_app,
      "<CR> - run record sound application",
     }
};

/* Table that points to the above table and contains the number of entries */
const st_command_table_t g_cmd_tbl_sound_app =
{
    "Sound Application Commands",
    s_cmd_sound_app,
    ((sizeof(s_cmd_sound_app)) / sizeof(st_cmdfnass_t))
};

/******************************************************************************
 Function Name : cmd_play_sound_app
 Description   : Run the adc application demo,
 Arguments     : IN  iArgCount - The number of arguments in the argument list
                 IN  ppszArgument - The argument list
                 IN  pCom - Pointer to the command object
 Return value  : CMD_OK for success
 ******************************************************************************/
static int16_t cmd_play_sound_app(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{
    /* Not used */
    (void) ppszArgument;

    /* Not used */
    (void) iArgCount;

    R_SOUND_PlaySample(pCom->p_in, pCom->p_out);

    return CMD_OK;
}
/******************************************************************************
 End of function  cmd_play_sound_app
 ******************************************************************************/


/******************************************************************************
 Function Name : cmd_record_sound_app
 Description   : Run the adc application demo,
 Arguments     : IN  iArgCount - The number of arguments in the argument list
                 IN  ppszArgument - The argument list
                 IN  pCom - Pointer to the command object
 Return value  : CMD_OK for success
 ******************************************************************************/
static int16_t cmd_record_sound_app(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{
    /* Not used */
    (void) ppszArgument;

    /* Not used */
    (void) iArgCount;

    R_SOUND_RecordSample(pCom->p_in, pCom->p_out);

    return CMD_OK;
}
/******************************************************************************
 End of function  cmd_record_sound_app
 ******************************************************************************/



/******************************************************************************
 End  Of File
 ******************************************************************************/
