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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* System Name : [RZ/A2M] SSIF Sample
* File Name   : readme-e.txt
*******************************************************************************/
/*******************************************************************************
*
* History     : Sep. 30,2020 Rev.1.00.00    First edition issued
*             : Jan. 20,2022 Rev.2.00.00    Updated to e2 studio 2022-01
*                                           Updated to complier 9-2020-q2-update
*******************************************************************************/

1. Before Use

  This sample code has been tested using the GR-MANGO Rev.B board
  with the RZ/A2M group R7S921058. 
  Use this readme file, the application note, and the sample code as a reference 
  for your software development.

  ****************************** CAUTION ******************************
   This sample code are all reference, and no one to guarantee the 
   operation. Please use this sample code for the technical 
   reference when customers develop software.
  ****************************** CAUTION ******************************


2. System Verification

  This sample code was developed and tested with the following components:

    CPU                                 : RZ/A2M
    Board                               : GR-MANGO Rev.B board
    Compiler                            : GNU ARM Embedded 9-2020-q2-update
    Integrated development environment  : e2 studio Version 2022-01.


3. About Sample Code

  This sample code operates the following processing.

  (1) Application program [rza2m_ssif_sample_freertos_gcc]
    This application note describes a sample application that transmits and receives data using the Serial Sound 
    Interface with FIFO (SSIF-2) with the Direct Memory Access Controller (DMAC).
    For further information refer to application note. 

    The application program is executed from the loader program after the Octa memory controller register 
    setting processing is performed by the loader program.
    The loader program is stored in the following folder.
      [rza2m_blinky_sample_osless_gcc\generate\gr_mango_boot\mbed_sf_boot.c]


4. Operation Confirmation Conditions

  (1) Boot mode
    - Boot mode 6
      Boot from OctaFlash(SPI mode) connected to OctaFlash space.
      * The program can not be operated if the boot mode except the above is specified.

  (2) Operating frequency
      The RZ/A2M clock pulse oscillator module is set to ensure that the RZ/A2M clocks on the CPU board
      board have the following frequencies:
      (The frequencies indicate the values in the state that the clock with 24MHz
      is input to the EXTAL pin in RZ/A2M clock mode 1.)
      - CPU clock (I clock)                 : 528MHz
      - Image processing clock (G clock)    : 264MHz
      - Internal bus clock (B clock)        : 132MHz
      - Peripheral clock1 (P1 clock)        :  66MHz
      - Peripheral clock0 (P0 clock)        :  33MHz
      - OM_CK/OM_CK#                        : 132MHz
      - CKIO                                : 132MHz

  (3) Setting for asynchronous communication
      - Bit rate        : 115200 bps
      - Data bit        : 8 bits
      - Parity bit      : none
      - Stop bit        : 1 bit

  (4) OctaFlash (connected to OctaFlash space) used
    - Manufacturer  : Macronix Inc.
    - Product No.   : MX25UW12845GXDIO0

  (5) Setting for cache
      Initial setting for the L1 and L2 caches is executed by the MMU. 
      Refer to the "RZ/A2M group Example of Initialization" application note about "Setting for MMU" for 
      the valid/invalid area of L1 and L2 caches.


5. Operational Procedure

  Use the following procedure to execute this sample code.

  (1) Activating integrated development environment
    Activate the e2 studio integrated development environment.

  (2) Building application program ([rza2m_ssif_sample_freertos_gcc] project)
    After importing [rza2m_ssif_sample_freertos_gcc] project using the e2 studio menu, build the project 
    and generate the binary format file named as rza2m_ssif_sample_freertos_gcc.bin.

  (3) Downloading sample code
    Connect GR-MANGO CN1 and PC via USB cable.
    PC detects the GR-MANGO as MBED drive.
    Drag-and-drop the binary file to the MBED drive.

  (4) Executing sample code
    Press reset button on the GR-MANGO to run the code.

  (5) Enter commands at the command prompt
    Enter the play or record command.
    See the application note for command details.

/* End of File */
