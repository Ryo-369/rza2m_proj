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
 * File Name    : r_ssid_drv_api.c
 * Version      : 1.00
 * Description  : This file contains the API for the SSIF Driver.
 *******************************************************************************
 * History      : DD.MM.YYYY Version Description
 *              : 04.02.2010 1.00    First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include <fcntl.h>

#include "r_typedefs.h"
#include "compiler_settings.h"
#include "r_devlink_wrapper.h"
#include "r_ssif_drv_api.h"
#include "r_intc_drv_api.h"
#include "FreeRTOS.h"
#include "r_ssif_drv_sc_cfg.h"
#include "r_rza2_ssif_lld_cfg.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/** priority of SSIF interrupts, used in r_ssif_drv_api.c */
    #define ISR_SSIF_IRQ_PRIORITY    INTC_PRIORITY_30

/*****************************************************************************
 Function Prototypes
 ******************************************************************************/

/* Driver Functions */
static int_t ssif_hld_open (st_stream_ptr_t stream_ptr);
static void ssif_hld_close (st_stream_ptr_t stream_ptr);
static int_t ssif_hld_read (st_stream_ptr_t stream_ptr, uint8_t *pbyBuffer, uint32_t uiCount);
static int_t ssif_hld_write (st_stream_ptr_t stream_ptr, uint8_t *pbyBuffer, uint32_t uiCount);
static int_t ssif_hld_control (st_stream_ptr_t stream_ptr, uint32_t ctlCode, void *pCtlStruct);
static int_t ssif_hld_get_version (st_stream_ptr_t stream_ptr, st_ver_info_t *pVerInfo);

/* SSIF Initialise functions */
static void *ssif_init (void * const config_data, int32_t * const p_errno);
static int_t ssif_un_init (void * const driver_instance, int32_t * const p_errno);
static int_t configure_ssif_channel (st_stream_ptr_t stream_ptr);
static int16_t ssif_get_channel (st_stream_ptr_t stream_ptr);

/*****************************************************************************
 Constant Data
 ******************************************************************************/
static const st_drv_info_t gs_hld_info =
{
{ ((R_SSIF_RZ_HLD_VERSION_MAJOR << 16) + R_SSIF_RZ_HLD_VERSION_MINOR) },

    R_SSIF_RZ_HLD_BUILD_NUM,
    R_SSIF_RZ_HLD_DRV_NAME
};

/* structure pointer for setting up DMA read */
static AIOCB *gsp_aio_r = NULL;

/* structure pointer for setting up DMA write */
static AIOCB *gsp_aio_w = NULL;

/* pointer to info for SSIF channel */
static ssif_info_ch_t *gsp_info_ch = (ssif_info_ch_t *)NULL;

/* Configured Inputs to Open */
static ssif_channel_cfg_t gs_ssif_cfg[SSIF_NUM_CHANS];

/* Define the driver function table for this */
const st_r_driver_t g_ssif_driver =
{ "SSIF Device Driver", ssif_hld_open, ssif_hld_close, ssif_hld_read, ssif_hld_write, ssif_hld_control, ssif_hld_get_version };

extern ssif_info_drv_t g_ssif_info_drv;

/******************************************************************************
 Function Name: ssif_hld_open
 Description:   Function to open the system timer device driver
 Arguments:     IN  stream_ptr - Pointer to the file stream
 Return value:  0 for success otherwise -1
 ******************************************************************************/
static int_t ssif_hld_open (st_stream_ptr_t stream_ptr)
{
    int_t ercd = DRV_SUCCESS;

    /* get the Smart Configuration index associated with the handle */
    int16_t channel = ssif_get_channel(stream_ptr);

    if (DRV_ERROR != channel)
    {
        int_t gpio_handle;
        gpio_handle = direct_open("gpio", 0);

        if (gpio_handle > 0)
        {
            /* direct_control as direct_control assumes parameter 3 is i/o, but using the parameter in input only mode is acceptable */
            direct_control(gpio_handle, CTL_GPIO_INIT_BY_TABLE, (void *) &SSIF_SC_TABLE[stream_ptr->sc_config_index].pin);
            direct_close(gpio_handle);
        }
        else
        {
            ercd = DRV_ERROR;
        }

        if (DRV_SUCCESS == ercd)
        {
            /* Configure SSIF Channel 0. 4 channels available (0-3) */
            configure_ssif_channel(stream_ptr);

            /* get access to channel to enable it */
            if (false == R_OS_SemaphoreWait( &gsp_info_ch->sem_access, R_OS_ABSTRACTION_EV_WAIT_INFINITE))
            {
                ercd = DRV_ERROR;
            }
            else
            {
                gsp_info_ch->openflag = stream_ptr->file_flag;

                /* Initialise the tx buffer element */
                gsp_info_ch->p_aio_tx_curr = NULL;

                /* Initialise the rx buffer element */
                gsp_info_ch->p_aio_rx_curr = NULL;

                ercd = R_SSIF_EnableChannel(gsp_info_ch);
                if (DRV_SUCCESS == ercd)
                {
                    gsp_info_ch->ch_stat = SSIF_CHSTS_OPEN;
                }
            }
            R_OS_SemaphoreRelease( &gsp_info_ch->sem_access);
        }

    }

    return (ercd);
}
/******************************************************************************
 End of function  ssif_hld_open
 ******************************************************************************/

/******************************************************************************
 Function Name: ssif_hld_close
 Description:   Function to close the standard IN driver. All open devices
 connected to the tmr are closed.
 Arguments:     IN  stream_ptr - Pointer to the file stream
 Return value:  none
 ******************************************************************************/
static void ssif_hld_close (st_stream_ptr_t stream_ptr)
{
    /* Unused parameter*/
    (void) stream_ptr;

    int32_t ercd = DRV_SUCCESS;

    ercd = ssif_un_init( &g_ssif_info_drv, &ercd);

    /* Ensure that there is a drive to close */
    if ((NULL == gsp_info_ch) || (DRV_SUCCESS != ercd))
    {
        ercd = DRV_ERROR;
    }
    else
    {
        /* Get semaphore to access the channel data */
        if (false == R_OS_SemaphoreWait( &gsp_info_ch->sem_access, R_OS_ABSTRACTION_EV_WAIT_INFINITE))
        {
            ercd = DRV_ERROR;
        }
        else
        {
            if (SSIF_CHSTS_UNINIT != gsp_info_ch->ch_stat)
            {
                ercd = DRV_ERROR;
            }
            else
            {
                /* No p_errno not required */
                R_SSIF_PostAsyncCancel(gsp_info_ch, NULL);

                ercd = R_SSIF_DisableChannel(gsp_info_ch);

                if (DRV_SUCCESS == ercd)
                {
                    gsp_info_ch->ch_stat = SSIF_CHSTS_INIT;
                }
            }

            /* Release semaphore */
            R_OS_SemaphoreRelease( &gsp_info_ch->sem_access);
        }
        /* delete the private semaphore */
        R_OS_SemaphoreDelete(&gsp_info_ch->sem_access);
    }

}
/******************************************************************************
 End of function  ssif_hld_close
 ******************************************************************************/

/******************************************************************************
 * Function Name: ssif_hld_read
 * @brief         Enqueue asynchronous read request
 *
 *                Description:<br>
 *
 * @param[in,out] p_fd       :channel handle which was returned by R_SSIF_Open
 * @param[in]     p_aio      :aio control block of read request
 * @param[in,out] p_errno    :pointer of error code
 * @retval        DRV_SUCCESS   :Success.
 * @retval        DRV_ERROR     :Failure.
 ******************************************************************************/
static int_t ssif_hld_read (st_stream_ptr_t stream_ptr, uint8_t *pbyBuffer, uint32_t uiCount)
{
    int_t ercd = DRV_SUCCESS;

    /* Ensure reading is allowed */
    if (O_WRONLY != stream_ptr->file_flag)
    {

        /* Ensure that the driver is configured */
        if ((NULL == gsp_info_ch) || (NULL == gsp_aio_r))
        {
            ercd = DRV_ERROR;
        }
        else
        {
            /* update file descriptor field with pointer to channel configuration */
            gsp_aio_r->aio_fildes = (int) gsp_info_ch;

            /* Enable callback on message */
            gsp_aio_r->aio_sigevent.sigev_notify = SIGEV_THREAD;

            /* set operation type */
            gsp_aio_r->aio_return = SSIF_ASYNC_R;

            /* number of bytes */
            gsp_aio_r->aio_nbytes = uiCount;

            /* pointer to buffer */
            gsp_aio_r->aio_buf = (void *) pbyBuffer;

            /* pass message to config structure */
            gsp_info_ch->p_aio_rx_next = gsp_aio_r;

            /* Go! */
            R_SSIF_PostAsyncIo(gsp_info_ch, gsp_aio_r);
        }
    }

    return ercd;
}
/*******************************************************************************
 End of function ssif_hld_read
 ******************************************************************************/

/******************************************************************************
 * Function Name: ssif_hld_write
 * @brief         Enqueue asynchronous write request
 *
 *                Description:<br>
 *
 * @param[in,out] p_fd       :channel handle which was returned by R_SSIF_Open
 * @param[in]     p_aio      :aio control block of write request
 * @param[in,out] p_errno    :pointer of error code
 * @retval        DRV_SUCCESS   :Success.
 * @retval        DRV_ERROR     :Failure.
 ******************************************************************************/
static int_t ssif_hld_write (st_stream_ptr_t stream_ptr, uint8_t *pbyBuffer, uint32_t uiCount)
{
    int        lp_cnt;

    int_t ercd = DRV_SUCCESS;

    /* Ensure writing is allowed */
    if (O_RDONLY != stream_ptr->file_flag)
    {

        /* Ensure that the ssif is configured correctly */
        if ((NULL == gsp_info_ch) || (NULL == gsp_aio_w))
        {
            ercd = DRV_ERROR;
        }
        else
        {
            /* Validate parameters */
            if ((0u == uiCount) || (NULL == pbyBuffer))
            {
                ercd = DRV_ERROR;
            }
            else
            {
                /* update file descriptor field with pointer to channel configuration */
                gsp_aio_w->aio_fildes = (int) gsp_info_ch;

                /* Enable callback on message */
                gsp_aio_w->aio_sigevent.sigev_notify = SIGEV_THREAD;

                /* set operation type */
                gsp_aio_w->aio_return = SSIF_ASYNC_W;

                /* number of bytes */
                gsp_aio_w->aio_nbytes = uiCount;

                /* pointer to buffer */
                gsp_aio_w->aio_buf = (void *) pbyBuffer;

                /* pass message to config structure */
                gsp_info_ch->p_aio_tx_next = gsp_aio_w;

                /* Go! */
                R_SSIF_PostAsyncIo(gsp_info_ch, gsp_aio_w);
            }
        }
    }

    return ercd;
}
/*******************************************************************************
 End of function ssif_hld_write
 ******************************************************************************/

/******************************************************************************
 Function Name: ssif_hld_control
 Description:   Function to handle custom controls for the standard IN device
 Arguments:     IN  stream_ptr - Pointer to the file stream
 IN  ctlCode - The custom control code
 IN  pCtlStruct - Pointer to the custom control structure
 Return value:  0 for success -1 on error
 ******************************************************************************/
static int_t ssif_hld_control (st_stream_ptr_t stream_ptr, uint32_t ctlCode, void *pCtlStruct)
{
    int_t result = DRV_ERROR;

    /* Avoid unused parameter compiler warning */
    (void) stream_ptr;
    if (pCtlStruct)
    {
        switch (ctlCode)
        {
            case CTL_SSIF_SET_CONFIGURE:
            {
                ssif_channel_cfg_t *p_channel_cfg = pCtlStruct;

                /* Comparison with NULL */
                if (NULL == p_channel_cfg)
                {
                    result = DRV_ERROR;
                }
                else
                {
                    result = R_SSIF_IOCTL_ConfigChannel(gsp_info_ch, p_channel_cfg);
                    if (DRV_SUCCESS != result)
                    {
                        result = DRV_ERROR;
                    }
                }
                break;
            }

            case CTL_SSIF_GET_STATUS:
            {
                uint32_t *p_status = pCtlStruct;

                /* Comparison with NULL */
                if (NULL == p_status)
                {
                    result = DRV_ERROR;
                }
                else
                {
                    result = R_SSIF_IOCTL_GetStatus(gsp_info_ch, p_status);
                    if (DRV_SUCCESS != result)
                    {
                        result = DRV_ERROR;
                    }
                }
                break;
            }
            case CTL_SSIF_CANCEL:
            {
                /* not implement */
                result = DRV_ERROR;
            }
            break;

            case CTL_SSIF_READ:
            {
                /* point to read setup */
                gsp_aio_r = (AIOCB *) pCtlStruct;
                result = DRV_SUCCESS;
            }
            break;

            case CTL_SSIF_WRITE:
            {
                /* point to write setup */
                gsp_aio_w = (AIOCB *) pCtlStruct;
                result = DRV_SUCCESS;
            }
            break;

            default:
            {
                result = DRV_ERROR;
                break;
            }
        }
    }
    return result;
}
/******************************************************************************
 End of function  ssif_hld_control
 ******************************************************************************/

/*******************************************************************************
 Function Name: ssif_hld_get_version
 Description  : Provides build information even if driver fails to open
 version information is updated by developer
 Arguments    : none
 Return Value : 0 (never fails)
 ******************************************************************************/
static int_t ssif_hld_get_version (st_stream_ptr_t stream_ptr, st_ver_info_t *pVerInfo)
{
    /* Unused parameter */
    (void) stream_ptr;

    pVerInfo->hld.version.sub.major = gs_hld_info.version.sub.major;
    pVerInfo->hld.version.sub.minor = gs_hld_info.version.sub.minor;
    pVerInfo->hld.build = gs_hld_info.build;
    pVerInfo->hld.p_szdriver_name = gs_hld_info.p_szdriver_name;

    /* Obtain version information from Low layer Driver */
    R_SSIF_GetVersion(pVerInfo);

    return 0;
}
/*******************************************************************************
 End of function ssif_hld_get_version
 ******************************************************************************/

/******************************************************************************
 * Function Name: ssif_init
 * @brief         Initialises the SSIF driver.
 *
 *                Description: Initialises the SSIF Driver.
 *
 * @param[in,out] driver_instance :driver instance which was returned by<br>
 ssif_init
 * @param[in,out] p_errno         :pointer of error code
 * @retval        DRV_SUCCESS        :Success.
 * @retval        DRV_ERROR          :Failure.
 ******************************************************************************/
static void *ssif_init (void * const p_config_data, int32_t * const p_errno)
{
    /* Unused parameter */
    (void) p_errno;

    int_t ercd = DRV_ERROR;

    /* cast to void */
    void *p_ret = (void *)NULL;

    /* driver instance must not be NULL */
    if (NULL == p_config_data)
    {
        ercd = DRV_ERROR;
    }
    else if (SSIF_DRVSTS_UNINIT != g_ssif_info_drv.drv_stat)
    {
        ercd = DRV_ERROR;
    }
    else
    {
        g_ssif_info_drv.drv_stat = SSIF_DRVSTS_INIT;

        /* cast to ssif_channel_cfg_t pointer */
        ercd = R_SSIF_Initialise((ssif_channel_cfg_t *) p_config_data);

        if (DRV_SUCCESS == ercd)
        {
            /* cast to void */
            p_ret = (void *) &g_ssif_info_drv;
        }
        else
        {
            g_ssif_info_drv.drv_stat = SSIF_DRVSTS_UNINIT;
        }
    }

    if (DRV_ERROR == ercd)
    {
        /* Set p_ret to NULL */
        p_ret = NULL;
    }

    return p_ret;
}
/*******************************************************************************
 End of function ssif_init
 ******************************************************************************/

/******************************************************************************
 * Function Name: ssif_un_init
 * @brief         Uninitialise the SSIF driver.
 *
 *                Description: Unitailises the SSIF Driver.
 *
 * @param[in,out] driver_instance :driver instance which was returned by<br>
 ssif_init
 * @param[in,out] p_errno         :pointer of error code
 * @retval        DRV_SUCCESS        :Success.
 * @retval        DRV_ERROR          :Failure.
 ******************************************************************************/
static int_t ssif_un_init (void * const driver_instance, int32_t * const p_errno)
{
    /* Unused parameter */
    (void) p_errno;

    int_t ercd;
    int_t ret = DRV_SUCCESS;
    ssif_info_drv_t * const p_info_drv = driver_instance;

    /* Null comparison */
    if (NULL == p_info_drv)
    {
        ercd = DRV_ERROR;
    }
    else
    {
        if (SSIF_DRVSTS_INIT != p_info_drv->drv_stat)
        {
            ercd = DRV_ERROR;
        }
        else
        {
            ercd = R_SSIF_UnInitialise();
            p_info_drv->drv_stat = SSIF_DRVSTS_UNINIT;
        }
    }

    if (DRV_SUCCESS != ercd)
    {
        ret = DRV_ERROR;
    }

    return ret;
}
/*******************************************************************************
 End of function ssif_un_init
 ******************************************************************************/

/*******************************************************************************
 * Function Name: configure_ssif_channel
 * Description  : Configures the appropriate SSIF channel. The limit to which is
 *                   defined in the driver. Currently the function is limited to a
 *                   hard coded gs_ssif_cfg structure.
 * Arguments    : int_t channel - configures the appropriate channel
 ******************************************************************************/
static int_t configure_ssif_channel (st_stream_ptr_t stream_ptr)
{
    ssif_info_drv_t *p_info_drv;
    int_t ecrd = DRV_SUCCESS;
    int16_t channel = ssif_get_channel(stream_ptr);

    gs_ssif_cfg[channel].enabled = true;
    gs_ssif_cfg[channel].int_level = SSIF_SC_TABLE[stream_ptr->sc_config_index].config.int_level;
    gs_ssif_cfg[channel].master_slave = SSIF_SC_TABLE[stream_ptr->sc_config_index].config.master_slave;
    gs_ssif_cfg[channel].sample_freq = SSIF_SC_TABLE[stream_ptr->sc_config_index].config.sample_freq;
    gs_ssif_cfg[channel].clk_select = SSIF_SC_TABLE[stream_ptr->sc_config_index].config.clk_select;
    gs_ssif_cfg[channel].frame_word = SSIF_SC_TABLE[stream_ptr->sc_config_index].config.frame_word;
    gs_ssif_cfg[channel].data_word = SSIF_SC_TABLE[stream_ptr->sc_config_index].config.data_word;
    gs_ssif_cfg[channel].system_word = SSIF_SC_TABLE[stream_ptr->sc_config_index].config.system_word;
    gs_ssif_cfg[channel].bclk_pol = SSIF_SC_TABLE[stream_ptr->sc_config_index].config.bclk_pol;
    gs_ssif_cfg[channel].lr_clock = SSIF_SC_TABLE[stream_ptr->sc_config_index].config.lr_clock;
    gs_ssif_cfg[channel].padding_pol = SSIF_SC_TABLE[stream_ptr->sc_config_index].config.padding_pol;
    gs_ssif_cfg[channel].serial_alignment = SSIF_SC_TABLE[stream_ptr->sc_config_index].config.serial_alignment;
    gs_ssif_cfg[channel].parallel_alignment = SSIF_SC_TABLE[stream_ptr->sc_config_index].config.parallel_alignment;
    gs_ssif_cfg[channel].ws_delay = SSIF_SC_TABLE[stream_ptr->sc_config_index].config.ws_delay;
    gs_ssif_cfg[channel].mute_onoff = SSIF_SC_TABLE[stream_ptr->sc_config_index].config.mute_onoff;
    gs_ssif_cfg[channel].noise_cancel = SSIF_SC_TABLE[stream_ptr->sc_config_index].config.noise_cancel;
    gs_ssif_cfg[channel].tdm_mode = SSIF_SC_TABLE[stream_ptr->sc_config_index].config.tdm_mode;

    if (gs_ssif_cfg[channel].master_slave == SSIF_CFG_MASTER)
    {
        gs_ssif_cfg[channel].slave_mode = false;
    }
    else
    {
        gs_ssif_cfg[channel].slave_mode = true;
    }

    /* initialise driver data structures */
    p_info_drv = ssif_init( &gs_ssif_cfg, NULL);

    /* Casting NULL */
    if (NULL == p_info_drv)
    {
        /* Error - SSIF not initialised correctly */
        ecrd = DRV_ERROR;
    }
    else
    {
        gsp_info_ch = &p_info_drv->info_ch[channel];
    }
    return ecrd;
}
/*******************************************************************************
 End of function configure_ssif_channel
 ******************************************************************************/

/*******************************************************************************
 * Function Name: ssif_get_channel
 * Description  : Provides channel number written in stream_ptr
 * Arguments    : IN stream_ptr - Pointer to the file stream
 * Return value:  0 to 3 for channel number -1 on error
 ******************************************************************************/
static int16_t ssif_get_channel (st_stream_ptr_t stream_ptr)
{
    int16_t ret_value = DRV_ERROR;
    int16_t channel_id;

    if (NULL != stream_ptr)
    {
        /* cast channel id to an int_t */
        channel_id = (int_t) (SSIF_SC_TABLE[stream_ptr->sc_config_index].channel);

        /* verify that the channel is supported */
        /* look for channel in the map of available channels from low level driver */
        /* (e_channel_id_t). This is a bit field where the power of 2 is the available channel */
        if (((1 << channel_id) <= R_CH15))
        {
            /* set channel as bit field where value is 2 << channel */
            uint16_t channel_bit_value = (uint16_t) (1 << channel_id);

            /* get bit field of supported channels */
            uint16_t supported_channels = SSIF_CFG_LLD_SUPPORTED_CHANNELS;

            if (0 != (channel_bit_value & supported_channels))
            {
                ret_value = (int_t) channel_id;
            }
        }
    }

    /* return status as error or channel number  */
    return (ret_value);
}
/*******************************************************************************
 End of function ssif_get_channel
 ******************************************************************************/

