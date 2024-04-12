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
* Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name   : riic_max9867.c
* Description  : This module is sample of Audio codec control program.
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "iodefine.h"
#include "r_riic_drv_api.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define MAX9867_S_ADDR_WRITE (0x30) /* I2C Slave Address for all write commands */


/* ==== Register Address ==== */
/* STATUS */
#define MAX9867_REGADR_STATUS       (0x00u)
#define MAX9867_REGADR_JACKSENSE    (0x01u)
#define MAX9867_REGADR_AUX_HIGH     (0x02u)
#define MAX9867_REGADR_AUX_LOW      (0x03u)
#define MAX9867_REGADR_INT_ENABLE   (0x04u)
/* CLOCK CONTROL */
#define MAX9867_REGADR_SYS_CLOCK    (0x05u)
#define MAX9867_REGADR_AUD_CTL_HIGH (0x06u)
#define MAX9867_REGADR_AUD_CTL_LOW  (0x07u)
/* DIGITAL AUDIO INTERFACE */
#define MAX9867_REGADR_INT_MODE1    (0x08u)
#define MAX9867_REGADR_INT_MODE2    (0x09u)
/* DIGITAL FILTERING */
#define MAX9867_REGADR_FILTER       (0x0Au)
/* LEVEL CONTROL */
#define MAX9867_REGADR_SIDETONE     (0x0Bu)
#define MAX9867_REGADR_DAC_LV       (0x0Cu)
#define MAX9867_REGADR_ADC_LV       (0x0Du)
#define MAX9867_REGADR_LINE_IN_LV_L (0x0Eu)
#define MAX9867_REGADR_LINE_IN_LV_R (0x0Fu)
#define MAX9867_REGADR_VOL_L        (0x10u)
#define MAX9867_REGADR_VOL_R        (0x11u)
#define MAX9867_REGADR_MIC_GAIN_L   (0x12u)
#define MAX9867_REGADR_MIC_GAIN_R   (0x13u)
/* CONFIGURATION */
#define MAX9867_REGADR_ADC_INPUT    (0x14u)
#define MAX9867_REGADR_MIC          (0x15u)
#define MAX9867_REGADR_MODE         (0x16u)
/* POWER MANAGEMENT */
#define MAX9867_REGADR_SYS_DOWN     (0x17u)
#define MAX9867_REGADR_REVISION     (0xFFu)

/* ==== Register Setting Value ==== */
/* RESET */
#define MAX9867_RESET_INI_VALUE      (0x80u)  /* SHDN=1,LNLEN=0,LNREN=0,DALEN=0,DAREN=0,ADLEN=0,ADREN=0 */
#define MAX9867_PW_ON_VALUE          (0x0Eu)  /* SHDN=0,LNLEN=0,LNREN=0,DALEN=1,DAREN=1,ADLEN=1,ADREN=0 */
/* Frequency */
#define MAX9867_FREQ_48000_HIGH      (0x624Eu)
#define MAX9867_FREQ_44100_HIGH      (0x5A51u)
#define MAX9867_FREQ_32000_HIGH      (0x4189u)
#define MAX9867_FREQ_24000_HIGH      (0x3127u)
#define MAX9867_FREQ_16000_HIGH      (0x20C5u)
#define MAX9867_FREQ_8000_HIGH       (0x1062u)

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/

/******************************************************************************
* Function Name: R_CODEC_MAX9867Init
* Description  : MAX9867 initialization
* Arguments    : none
* Return Value : none
******************************************************************************/
void R_CODEC_MAX9867Init(void)
{
    uint8_t        reg_addr;
    uint8_t        reg_data;
    int_t          handle;
    uint8_t        cmd[5] = {0};
    st_r_drv_riic_transfer_t i2c_write;
    uint16_t       lrclk;
    int_t          ret;
    
    handle = open(DEVICE_INDENTIFIER "riic1", O_RDWR);

    /* ==== Soft Reset ==== */
    cmd[0] = 0x80;    /* SHDN=1,LNLEN=0,LNREN=0,DALEN=0,DAREN=0,ADLEN=0,ADREN=0 */
    
    i2c_write.device_address = MAX9867_S_ADDR_WRITE;
    i2c_write.sub_address_type = RIIC_SUB_ADDR_WIDTH_8_BITS;
    i2c_write.sub_address = 0x17;
    i2c_write.number_of_bytes = 1;
    i2c_write.p_data_buffer = cmd;
    ret = control(handle, CTL_RIIC_WRITE, &i2c_write);

    /* ==== Frequency Setting ==== */
    lrclk = 0x5A51;   /* 44.1kHz */
    cmd[0] = 0x10;    /* PSCLK = 00  FREQ = 0x0 */
    cmd[1] = (uint8_t)(lrclk >> 8);
    cmd[2] = (uint8_t)(lrclk);

    i2c_write.device_address = MAX9867_S_ADDR_WRITE;
    i2c_write.sub_address_type = RIIC_SUB_ADDR_WIDTH_8_BITS;
    i2c_write.sub_address = 0x05;
    i2c_write.number_of_bytes = 3;
    i2c_write.p_data_buffer = cmd;
    ret = control(handle, CTL_RIIC_WRITE, &i2c_write);
    
    /* ==== Digital Interface Setting ==== */
    /* Interrupt Enable */
    cmd[0] = 0x00;    /*ICLE=0,ISLD=0,IULK=0,SDODLY=0,IJDET=0*/

    i2c_write.device_address = MAX9867_S_ADDR_WRITE;
    i2c_write.sub_address_type = RIIC_SUB_ADDR_WIDTH_8_BITS;
    i2c_write.sub_address = 0x04;
    i2c_write.number_of_bytes = 1;
    i2c_write.p_data_buffer = cmd;
    ret = control(handle, CTL_RIIC_WRITE, &i2c_write);
    
    /* Interface Mode */
    cmd[0] = 0x90;    /* MAS=1,WCI=0,BCI=0,DLY=1,HIZOFF=0,TDM=0 */
    cmd[1] = 0x02;    /* LVOLFIX=0,DMONO=0,BSEL=010 */

    i2c_write.device_address = MAX9867_S_ADDR_WRITE;
    i2c_write.sub_address_type = RIIC_SUB_ADDR_WIDTH_8_BITS;
    i2c_write.sub_address = 0x08;
    i2c_write.number_of_bytes = 2;
    i2c_write.p_data_buffer = cmd;
    ret = control(handle, CTL_RIIC_WRITE, &i2c_write);

    /* Digital Gain Control */
    cmd[0] = 0x00;    /* DSTS=0,DVST=0 */

    i2c_write.device_address = MAX9867_S_ADDR_WRITE;
    i2c_write.sub_address_type = RIIC_SUB_ADDR_WIDTH_8_BITS;
    i2c_write.sub_address = 0x0B;
    i2c_write.number_of_bytes = 1;
    i2c_write.p_data_buffer = cmd;
    ret = control(handle, CTL_RIIC_WRITE, &i2c_write);

    /* Line Input Registers */
    cmd[0] = 0x40;    /* LILM=1,LIGL=0x00 */
    cmd[1] = 0x40;    /* LIRM=1,LIGR=0x00 */

    i2c_write.device_address = MAX9867_S_ADDR_WRITE;
    i2c_write.sub_address_type = RIIC_SUB_ADDR_WIDTH_8_BITS;
    i2c_write.sub_address = 0x0E;
    i2c_write.number_of_bytes = 2;
    i2c_write.p_data_buffer = cmd;
    ret = control(handle, CTL_RIIC_WRITE, &i2c_write);

    /* Microphone Input Registers */
    cmd[0] = 0x45;    /* +35dB: PALEN=10(+20dB),PGAML=0x05(+15dB) */
    cmd[1] = 0x00;    /* Right Microphone Gain PAREN=00,PGAMR=0x00 */

    i2c_write.device_address = MAX9867_S_ADDR_WRITE;
    i2c_write.sub_address_type = RIIC_SUB_ADDR_WIDTH_8_BITS;
    i2c_write.sub_address = 0x12;
    i2c_write.number_of_bytes = 2;
    i2c_write.p_data_buffer = cmd;
    ret = control(handle, CTL_RIIC_WRITE, &i2c_write);

    /* CONFIGURATION */
    cmd[0] = 0x40;    /* MXINL=01,MXINR=00,AUXCAP=0,AUXGAIN=0,AUXCAL=0,AUXEN= */
    cmd[1] = 0x00;    /* MICCLK=0,DIGMICL=0,DIGMICR=0 */
    cmd[2] = 0x02;    /* DSLEW=0,VSEN=0,ZDEN=0,JDETEN=0,HPMODE=000 */

    i2c_write.device_address = MAX9867_S_ADDR_WRITE;
    i2c_write.sub_address_type = RIIC_SUB_ADDR_WIDTH_8_BITS;
    i2c_write.sub_address = 0x14;
    i2c_write.number_of_bytes = 3;
    i2c_write.p_data_buffer = cmd;
    ret = control(handle, CTL_RIIC_WRITE, &i2c_write);

    /* ==== Power ON ==== */
    cmd[0] = 0x8E;    /* SHDN=1,LNLEN=0,LNREN=0,DALEN=1,DAREN=1,ADLEN=1,ADREN=0 */
    
    i2c_write.device_address = MAX9867_S_ADDR_WRITE;
    i2c_write.sub_address_type = RIIC_SUB_ADDR_WIDTH_8_BITS;
    i2c_write.sub_address = 0x17;
    i2c_write.number_of_bytes = 1;
    i2c_write.p_data_buffer = cmd;
    ret = control(handle, CTL_RIIC_WRITE, &i2c_write);

    close(handle);
}

/* End of File */
