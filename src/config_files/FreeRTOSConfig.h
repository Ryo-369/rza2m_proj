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
*
* Copyright (C) 2012 - 2021 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/* Standard includes. */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "r_task_priority.h"
//#include "serial_term_uart.h"    //190325 modified for RZ/A2M. It is for RX.

/* Unity includes. */
#if defined(AMAZON_FREERTOS_ENABLE_UNIT_TESTS)
#include "unity_internals.h"
#elif defined(ENABLE_UNIT_TESTS)
#include "unity.h"
#endif

/*-----------------------------------------------------------
* Application specific definitions.
*
* These definitions should be adjusted for your particular hardware and
* application requirements.
*
* THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
* FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
* http://www.freertos.org/a00110.html
*
* The bottom of this file contains some constants specific to running the UDP
* stack in this demo.  Constants specific to FreeRTOS+TCP itself (rather than
* the demo) are contained in FreeRTOSIPConfig.h.
*----------------------------------------------------------*/

//190325 modified for RZ/A2M. It is change of the description position -->
/*
 * The FreeRTOS Cortex-A port implements a full interrupt nesting model.
 *
 * Interrupts that are assigned a priority at or below
 * configMAX_API_CALL_INTERRUPT_PRIORITY (which counter-intuitively in the ARM
 * generic interrupt controller [GIC] means a priority that has a numerical
 * value above configMAX_API_CALL_INTERRUPT_PRIORITY) can call FreeRTOS safe API
 * functions and will nest.
 *
 * Interrupts that are assigned a priority above
 * configMAX_API_CALL_INTERRUPT_PRIORITY (which in the GIC means a numerical
 * value below configMAX_API_CALL_INTERRUPT_PRIORITY) cannot call any FreeRTOS
 * API functions, will nest, and will not be masked by FreeRTOS critical
 * sections (although it is necessary for interrupts to be globally disabled
 * extremely briefly as the interrupt mask is updated in the GIC).
 *
 * FreeRTOS functions that can be called from an interrupt are those that end in
 * "FromISR".  FreeRTOS maintains a separate interrupt safe API to enable
 * interrupt entry to be shorter, faster, simpler and smaller.
 *
 * The Renesas RZ implements 32 unique interrupt priorities.  For the purpose of
 * setting configMAX_API_CALL_INTERRUPT_PRIORITY 31 represents the lowest
 * priority.
 */
#define configMAX_API_CALL_INTERRUPT_PRIORITY   (25)

#define configAPPLICATION_ALLOCATED_HEAP        (1)
#define configUSE_TICKLESS_IDLE                 (0)

#define configSMALL_STACK_SIZE                  ( ( unsigned short ) 2048 )
#define configDEFAULT_STACK_SIZE                ( ( unsigned short ) 4096 )

#define configMEMORY_TYPE_FOR_ALLOCATOR         ( 0 )

/* Prevent C code being included in assembly files when the IAR compiler is
used. */
#ifndef __IASMARM__
    void vInitialiseRunTimeStats( void );
    #define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() vInitialiseRunTimeStats()

    /* Normal assert() semantics without relying on the provision of an assert.h
    header file. */
//190325 modified for RZ/A2M. move to below.
//  void R_OS_AssertCalled( const char * pcFile,  unsigned long ulLine );
//  #define configASSERT( x ) if( ( x ) == 0 ) R_OS_AssertCalled( __FILE__, __LINE__ );

    /****** Hardware specific settings. *******************************************/
    /*
     * The application must provide a function that configures a peripheral to
     * create the FreeRTOS tick interrupt, then define configSETUP_TICK_INTERRUPT()
     * in FreeRTOSConfig.h to call the function.  This file contains a function
     * that is suitable for use on the Renesas RZ MPU.  FreeRTOS_Tick_Handler() must
     * be installed as the peripheral's interrupt handler.
     */
    void vConfigureTickInterrupt( void );
    #define configSETUP_TICK_INTERRUPT() vConfigureTickInterrupt()
#endif /* __IASMARM__ */

/* The following constants describe the hardware, and are correct for the
Renesas RZ MPU. */
#define configINTERRUPT_CONTROLLER_BASE_ADDRESS 0xE8221000
#define configINTERRUPT_CONTROLLER_CPU_INTERFACE_OFFSET 0x1000
#define configUNIQUE_INTERRUPT_PRIORITIES       32

/* Map the FreeRTOS IRQ and SVC/SWI handlers to the names used in the C startup
code (which is where the vector table is defined). */
#define FreeRTOS_IRQ_Handler IRQ_Handler
#define FreeRTOS_SWI_Handler SWI_Handler
//190325 modified for RZ/A2M. It is change of the description position <--

#define configENABLE_BACKWARD_COMPATIBILITY        1
#define configUSE_PREEMPTION                       1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION    0
#define configMAX_PRIORITIES                       ( TASK_NUMBER_OF_PRIORITIES )
#define configTICK_RATE_HZ                         ( ( portTickType ) 1000 )
#define configMINIMAL_STACK_SIZE                   ( ( unsigned short ) 160 )
#define configTOTAL_HEAP_SIZE                      ( ( size_t ) ( 512 * 1024U ) )
#define configMAX_TASK_NAME_LEN                    ( 24 )
#define configUSE_TRACE_FACILITY                   1
#define configUSE_16_BIT_TICKS                     0
#define configIDLE_SHOULD_YIELD                    1
#define configUSE_CO_ROUTINES                      0
#define configUSE_MUTEXES                          1
#define configUSE_RECURSIVE_MUTEXES                1
#define configQUEUE_REGISTRY_SIZE                  8
#define configUSE_APPLICATION_TASK_TAG             0
#define configUSE_COUNTING_SEMAPHORES              1
#define configUSE_ALTERNATIVE_API                  0
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS    3      /* FreeRTOS+FAT requires 2 pointers if a CWD is supported. */
#define configRECORD_STACK_HIGH_ADDRESS            1

#define configUSE_DAEMON_TASK_STARTUP_HOOK 0

#define configCPU_CLOCK_HZ              ( 528000000UL )
#define configPERIPHERAL_CLOCK_HZ       ( 66660000UL )
#define configUSE_QUEUE_SETS            0

/* Hook function related definitions. */
#define configUSE_TICK_HOOK                        0
#define configUSE_IDLE_HOOK                        0
#define configUSE_MALLOC_FAILED_HOOK               0
#define configCHECK_FOR_STACK_OVERFLOW             2      /* Not applicable to the Win32 port. */

/* Software timer related definitions. */
#define configUSE_TIMERS                           1
#define configTIMER_TASK_PRIORITY                  ( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH                   5
#define configTIMER_TASK_STACK_DEPTH               ( configMINIMAL_STACK_SIZE * 2 )

/* The interrupt priority used by the kernel itself for the tick interrupt and
the pended interrupt.  This would normally be the lowest priority. */
#define configKERNEL_INTERRUPT_PRIORITY         1

/* The maximum interrupt priority from which FreeRTOS API calls can be made.
Interrupts that use a priority above this will not be effected by anything the
kernel is doing. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    4

/* The peripheral used to generate the tick interrupt is configured as part of
the application code.  This constant should be set to the vector number of the
peripheral chosen.  As supplied this is CMT0. */
//#define configTICK_VECTOR                     _CMT0_CMI0      //CMT0    //190325 modified for RZ/A2M. It is for RX.
//#define configTICK_VECTOR                     _CMT1_CMI1      //CMT1
//#define configTICK_VECTOR                     _CMT2_CMI2      //CMT2
//#define configTICK_VECTOR                     _CMT3_CMI3      //CMT3

/* Event group related definitions. */
#define configUSE_EVENT_GROUPS                     1

/* Run time stats gathering definitions. */
unsigned long ulGetRunTimeCounterValue( void );
void vConfigureTimerForRunTimeStats( void );
#define configGENERATE_RUN_TIME_STATS    1
//#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()    vConfigureTimerForRunTimeStats()
#define portGET_RUN_TIME_COUNTER_VALUE()            ulGetRunTimeCounterValue()

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES                   0    //190325 multiple definition. (line 137)
#define configMAX_CO_ROUTINE_PRIORITIES         ( 2 )

/* Currently the TCP/IP stack is using dynamic allocation, and the MQTT task is
 * using static allocation. */
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configSUPPORT_STATIC_ALLOCATION         0

/* Set the following definitions to 1 to include the API function, or zero
 * to exclude the API function. */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskCleanUpResources           1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_uxTaskGetStackHighWaterMark     1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_xTimerGetTimerTaskHandle        0
#define INCLUDE_xTaskGetIdleTaskHandle          0
#define INCLUDE_xQueueGetMutexHolder            1
#define INCLUDE_eTaskGetState                   1
#define INCLUDE_xEventGroupSetBitsFromISR       1
#define INCLUDE_xTimerPendFunctionCall          1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_xTaskAbortDelay                 1

/* This demo makes use of one or more example stats formatting functions.  These
 * format the raw data provided by the uxTaskGetSystemState() function in to human
 * readable ASCII form.  See the notes in the implementation of vTaskList() within
 * FreeRTOS/Source/tasks.c for limitations.  configUSE_STATS_FORMATTING_FUNCTIONS
 * is set to 2 so the formatting functions are included without the stdio.h being
 * included in tasks.c.  That is because this project defines its own sprintf()
 * functions. */
#define configUSE_STATS_FORMATTING_FUNCTIONS    1

#if defined(ENABLE_UNIT_TESTS) || defined(AMAZON_FREERTOS_ENABLE_UNIT_TESTS)
/* unity testing */
#define configASSERT( x ) do { if( ( x ) == 0 ) TEST_ABORT(); } while( 0 )
#elif defined(CONFIG_FREERTOS_ASSERT_DISABLE) || defined(NDEBUG)
/* Disable Assert call for release builds. */
#define configASSERT( x ) ( ( void ) 0 )
#else /* CONFIG_FREERTOS_ASSERT_FAIL_ABORT or nothing */
/* Assert call defined for debug builds. */
//extern void vAssertCalled( void );
//#define configASSERT( x ) do { if( ( x ) == 0 ) vAssertCalled(); } while( 0 )
void R_OS_AssertCalled( const char * pcFile,  unsigned long ulLine );
#define configASSERT( x ) if( ( x ) == 0 ) R_OS_AssertCalled( __FILE__, __LINE__ );
#endif

/* The function that implements FreeRTOS printf style output, and the macro
 * that maps the configPRINTF() macros to that function. */
extern void vLoggingPrintf( const char * pcFormat, ... );
#define configPRINTF( X )    vLoggingPrintf X

/* Non-format version thread-safe print */
extern void vLoggingPrint( const char * pcMessage );
#define configPRINT( X )     vLoggingPrint( X )

/* Map the logging task's printf to the board specific output function. */
//#define configPRINT_STRING( x )    uart_string_printf( x )
#define configPRINT_STRING( x )  printf (x)    //190325 modified for RZ/A2M. It is for RZ/A2M.

/* Sets the length of the buffers into which logging messages are written - so
 * also defines the maximum length of each log message. */
#define configLOGGING_MAX_MESSAGE_LENGTH            192

/* Set to 1 to prepend each log message with a message number, the task name,
 * and a time stamp. */
#define configLOGGING_INCLUDE_TIME_AND_TASK_NAME    1

/* Application specific definitions follow. **********************************/

/* If configINCLUDE_DEMO_DEBUG_STATS is set to one, then a few basic IP trace
 * macros are defined to gather some UDP stack statistics that can then be viewed
 * through the CLI interface. */
#define configINCLUDE_DEMO_DEBUG_STATS       1

/* The size of the global output buffer that is available for use when there
 * are multiple command interpreters running at once (for example, one on a UART
 * and one on TCP/IP).  This is done to prevent an output buffer being defined by
 * each implementation - which would waste RAM.  In this case, there is only one
 * command interpreter running, and it has its own local output buffer, so the
 * global buffer is just set to be one byte long as it is not used and should not
 * take up unnecessary RAM. */
#define configCOMMAND_INT_MAX_OUTPUT_SIZE    2096

/* Only used when running in the FreeRTOS Windows simulator.  Defines the
 * priority of the task used to simulate Ethernet interrupts. */
#define configMAC_ISR_SIMULATOR_PRIORITY     ( configMAX_PRIORITIES - 1 )

/* This demo creates a virtual network connection by accessing the raw Ethernet
 * or WiFi data to and from a real network connection.  Many computers have more
 * than one real network port, and configNETWORK_INTERFACE_TO_USE is used to tell
 * the demo which real port should be used to create the virtual port.  The ports
 * available are displayed on the console when the application is executed.  For
 * example, on my development laptop setting configNETWORK_INTERFACE_TO_USE to 4
 * results in the wired network being used, while setting
 * configNETWORK_INTERFACE_TO_USE to 2 results in the wireless network being
 * used. */
#define configNETWORK_INTERFACE_TO_USE       2L

/* The address of an echo server that will be used by the two demo echo client
 * tasks:
 * http://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/TCP_Echo_Clients.html,
 * http://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/UDP_Echo_Clients.html. */
#define configECHO_SERVER_ADDR0              192
#define configECHO_SERVER_ADDR1              168
#define configECHO_SERVER_ADDR2              1
#define configECHO_SERVER_ADDR3              200
#define configTCP_ECHO_CLIENT_PORT           9999

/* Default MAC address configuration.  The demo creates a virtual network
 * connection that uses this MAC address by accessing the raw Ethernet/WiFi data
 * to and from a real network connection on the host PC.  See the
 * configNETWORK_INTERFACE_TO_USE definition above for information on how to
 * configure the real network connection to use. */
#define configMAC_ADDR0                      0x74
#define configMAC_ADDR1                      0x90
#define configMAC_ADDR2                      0x50
#define configMAC_ADDR3                      0x00
#define configMAC_ADDR4                      0x79
#define configMAC_ADDR5                      0x03

/* Default IP address configuration.  Used in ipconfigUSE_DHCP is set to 0, or
 * ipconfigUSE_DHCP is set to 1 but a DNS server cannot be contacted. */
#define configIP_ADDR0                       192
#define configIP_ADDR1                       168
#define configIP_ADDR2                       1
#define configIP_ADDR3                       100

/* Default gateway IP address configuration.  Used in ipconfigUSE_DHCP is set to
 * 0, or ipconfigUSE_DHCP is set to 1 but a DNS server cannot be contacted. */
#define configGATEWAY_ADDR0                  192
#define configGATEWAY_ADDR1                  168
#define configGATEWAY_ADDR2                  1
#define configGATEWAY_ADDR3                  1

/* Default DNS server configuration.  OpenDNS addresses are 208.67.222.222 and
 * 208.67.220.220.  Used in ipconfigUSE_DHCP is set to 0, or ipconfigUSE_DHCP is
 * set to 1 but a DNS server cannot be contacted.*/

#define configDNS_SERVER_ADDR0               172
#define configDNS_SERVER_ADDR1               29
#define configDNS_SERVER_ADDR2               39
#define configDNS_SERVER_ADDR3               1

/* Default netmask configuration.  Used in ipconfigUSE_DHCP is set to 0, or
 * ipconfigUSE_DHCP is set to 1 but a DNS server cannot be contacted. */
#define configNET_MASK0                      255
#define configNET_MASK1                      255
#define configNET_MASK2                      255
#define configNET_MASK3                      0

/* The UDP port to which print messages are sent. */
#define configPRINT_PORT                     ( 15000 )

#define configPROFILING                      ( 0 )

/* Pseudo random number generater used by some demo tasks. */
uint32_t ulRand(void);
#define configRAND32()    ulRand()

/* The platform FreeRTOS is running on. */
#define configPLATFORM_NAME    "RenesasRZA2M"

/* Header required for the tracealyzer recorder library. */
//#include "trcRecorder.h"

/* When the FIT configurator or the Smart Configurator is used, platform.h has to be used. */
#define configINCLUDE_PLATFORM_H_INSTEAD_OF_IODEFINE_H  1

#endif /* FREERTOS_CONFIG_H */
