/**********************************************************************************************************************
 * File Name    : main.c
 * Device(s)    : RZ/A2M
 * Tool-Chain   : e2Studio Ver 7.6.0
 *              : GNU Arm Embedded Toolchain 6-2017-q2-update
 * OS           : None
 * H/W Platform : RZ/A2M Evaluation Board
 * Description  : RZ/A2M Sample Program - Main
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "r_typedefs.h"
#include "iodefine.h"
#include "r_cpg_drv_api.h"
#include "r_scifa_drv_api.h"
#include "r_gpio_drv_api.h"
#include "r_startup_config.h"
#include "compiler_settings.h"
#include "main.h"
#include "r_os_abstraction_api.h"
#include "r_task_priority.h"
#include "command.h"

#include "iodefine.h"

/**********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#define MAIN_PRV_LED_ON     (1)
#define MAIN_PRV_LED_OFF    (0)

/**********************************************************************************************************************
 Imported global variables and functions (from other files)
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables and functions (to be accessed by other files)
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private global variables and functions
 *********************************************************************************************************************/
static uint32_t s_main_led_flg;      /* LED lighting/turning off */
static int_t s_my_gpio_handle;
static st_r_drv_gpio_pin_rw_t s_p82_hi =
{
    GPIO_PORT_8_PIN_2,
    GPIO_LEVEL_HIGH,
    GPIO_SUCCESS
};
static st_r_drv_gpio_pin_rw_t s_p82_lo =
{
    GPIO_PORT_8_PIN_2,
    GPIO_LEVEL_LOW,
    GPIO_SUCCESS
};
static const r_gpio_port_pin_t s_led_pin_list[] =
{
    GPIO_PORT_8_PIN_2,
};

/* Terminal window escape sequences */
static const char_t * const sp_clear_screen = "\x1b[2J";
static const char_t * const sp_cursor_home  = "\x1b[H";

/**********************************************************************************************************************
 * Function Name: os_console_task_t
 * Description  : console task
 * Arguments    : none
 * Return Value : 0
 *********************************************************************************************************************/
int_t os_console_task_t(void)
{
    /* never exits */
    while (1)
    {
        /* ==== Receive command, activate sample software ==== */
        cmd_console(stdin, stdout, "REE>");

        R_OS_TaskSleep(500);
    }

    return (0);
}
/**********************************************************************************************************************
 End of function os_console_task_t
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: os_main_task_t
 * Description  : FreeRTOS main task called by R_OS_KernelInit()
 *              : FreeRTOS is now configured and R_OS_Abstraction calls
 *              : can be used.
 *              : From this point forward no longer use direct_xxx calls.
 *              : For example
 *              : in place of   direct_open("ostm2", O_RDWR);
 *              : use           open(DEVICE_INDENTIFIER "ostm2", O_RDWR);
 *              :
 * Arguments    : none
 * Return Value : 0
 *********************************************************************************************************************/
int_t os_main_task_t(void)
{
    int_t err;
    st_r_drv_gpio_pin_list_t pin_led;
    char_t data;

    /* For information only
     * Use stdio calls to open drivers once  the kernel is initialised
     *
     * i.e.
     * int_t ostm3_handle;
     * ostm3_handle = open (DEVICE_INDENTIFIER "ostm2", O_RDWR);
     * close (ostm3_handle);
     */

    s_my_gpio_handle = open(DEVICE_INDENTIFIER "gpio", O_RDWR);

    /* On error */
    if (s_my_gpio_handle < 0)
    {
        /* stop execute */
        while (1)
        {
            R_COMPILER_Nop();
        }
    }

    /**************************************************
     * Initialise P6_0 pin parameterised in GPIO_SC_TABLE_MANUAL
     **************************************************/
    pin_led.p_pin_list = s_led_pin_list;
    pin_led.count = (sizeof(s_led_pin_list)) / (sizeof(s_led_pin_list[0]));
    err = direct_control(s_my_gpio_handle, CTL_GPIO_INIT_BY_PIN_LIST, &pin_led);

    /* On error */
    if (err < 0)
    {
        /* stop execute */
        while (1)
        {
            R_COMPILER_Nop();
        }
    }

    /* ==== Output banner message ==== */
    printf("%s%s", sp_clear_screen, sp_cursor_home);
    show_welcome_msg(stdout, true);

    /* Create a task to run the console */
    R_OS_TaskCreate("Console", (os_task_code_t) os_console_task_t, NULL,
                        R_OS_ABSTRACTION_DEFAULT_STACK_SIZE,
                        TASK_CONSOLE_TASK_PRI);

    while (1)
    {
        R_OS_TaskSleep(500);
    }

    return (err);
}
/**********************************************************************************************************************
 * End of function os_main_task_t
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: main
 * Description  : C Entry point
 *              : opens and configures cpg driver
 *              : starts the freertos kernel
 * Arguments    : none
 * Return Value : 0
 *********************************************************************************************************************/
int_t main(void)
{
    int_t cpg_handle;

    /* configure any drivers that are required before the Kernel initialises */

    /* Initialize the devlink layer */
    R_DEVLINK_Init();

    /* Initialize CPG */
    cpg_handle = direct_open("cpg", 0);

    if (cpg_handle < 0)
    {
        /* stop execute */
        while (1)
        {
            R_COMPILER_Nop();
        }
    }

    /* Can close handle if no need to change clock after here */
    direct_close(cpg_handle);

    /* Start FreeRTOS */
    /* R_OS_InitKernel should never return */
    R_OS_KernelInit();

    return (0);
}
/**********************************************************************************************************************
 * End of function main
 *********************************************************************************************************************/

/* End of File */
