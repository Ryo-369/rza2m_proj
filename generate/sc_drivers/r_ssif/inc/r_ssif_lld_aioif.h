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
 * and to discontinue the availability of this software. By using this
 * software, you agree to the additional terms and conditions found by
 * accessing the following link:
 * http://www.renesas.com/disclaimer
 *******************************************************************************
 * Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/******************************************************************************
 * @headerfile     r_ssif_lld_aioif.h
 * @brief          ssif low level header file
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZA2M
 *****************************************************************************/
/*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef AIOIF_H
    #define AIOIF_H

/*************************************************************************//**
 * @ingroup R_SW_PKG_93_SSIF_API
 * @defgroup R_SW_PKG_93_SSIF_LLD_AIOIF SSIF Low Level Driver API
 * @brief Internal low level SSIF driver component
 *
 * @anchor R_SW_PKG_93_SSIF_LLD_AIOIF_SUMMARY
 * @par Summary
 * @brief This interface defines the functionality that the low level driver
 * must implement for compatibility with the high level driver.
 *
 * @anchor R_SW_PKG_93_SSIF_LLD_AIOIF_LIMITATIONS
 * @par Known Limitations
 * @ref NONE_YET
 *
 * @anchor R_SW_PKG_93_SSIF_LLD_AIOIF_INSTANCES
 * @par Known Implementations
 * @ref RENESAS_APPLICATION_SOFTWARE_PACKAGE
 * @{
 *****************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
    #include "compiler_settings.h"
    #include "r_os_abstraction_api.h"

/*******************************************************************************
 Macro definitions
 *******************************************************************************/
    #define AHF_CREATESEM   0x1 /* create a semaphore for locking */ /* Source Merge 08-82 */
    #define AHF_LOCKSEM     0x2 /* use a semaphore for locking */    /* Source Merge 08-82 */
    #define AHF_LOCKINT     0x4 /* use interrupts for locking */

    #define IOIF_ESUCCESS    (0)
    #define IOIF_EERROR      (-1)

    #define IOIF_EBADF       (-17) /* Bad file descriptor */
    #define IOIF_EINVAL      (-28) /* Invalid argument */
    #define IOIF_ENOMEM      (-33) /* No memory available */
    #define IOIF_ENOLCK      (-34) /* No lock available */
    #define IOIF_ENOENT      (-42) /* No such file or directory */
    #define IOIF_ETIMEDOUT   (-50) /* Timed out */
    #define IOIF_EACCES      (-64) /* Access denied */
    #define IOIF_EAGAIN      (-65) /* Resource unavailable. Try again */
    #define IOIF_EBUSY       (-67) /* Device or resource busy */
    #define IOIF_ECANCELED   (-68) /* Operation was cancelled */
    #define IOIF_EDOM        (-69) /* Maths argument out of domain */
    #define IOIF_EEXIST      (-70) /* File, directory or mount exists */
    #define IOIF_EFBIG       (-71) /* File too big */
    #define IOIF_EINPROGRESS (-72) /* Operation in progress */
    #define IOIF_EIO         (-74) /* Input or Output error (possibly recoverable)*/
    #define IOIF_EISDIR      (-75) /* Is a directory */
    #define IOIF_EMFILE      (-76) /* Too many open files */
    #define IOIF_ENAMETOOLONG (-77) /* File name too long */
    #define IOIF_ENFILE      (-78) /* Too many files are open in the system */
    #define IOIF_ENOBUFS     (-79) /* No buffers available */
    #define IOIF_ENODEV      (-80) /* Not a valid device */
    #define IOIF_ENOSPC      (-84) /* No space available on device */
    #define IOIF_ENOTDIR     (-85) /* Not a directory */
    #define IOIF_ENOTEMPTY   (-86) /* Directory not empty */
    #define IOIF_ENOTSUP     (-87) /* Not supported */
    #define IOIF_ENOTTY      (-88) /* No TTY available */
    #define IOIF_ENXIO       (-89) /* Not valid IO address or device (unrecoverable)*/
    #define IOIF_EPERM       (-90) /* Operation not permitted */
    #define IOIF_EROFS       (-91) /* Read only file system */
    #define IOIF_ESPIPE      (-92) /* Invalid seek on pipe */
    #define IOIF_EXDEV       (-93) /* Device cross linked */
    #define IOIF_ENOTSOCK    (-94) /* Not a socket */
    #define IOIF_EMSGSIZE    (-95) /* Message too large */
    #define IOIF_EINTR       (-96) /* Function interrupted */
    #define IOIF_EFAULT      (-97) /* Section fault, bad address */
    #define IOIF_EPIPE       (-98) /* Broken pipe */
    #define IOIF_EALREADY    (-99) /* Connection already started */
    #define IOIF_EDESTADDRREQ (-100) /* Destination address requred */
    #define IOIF_EPROTOTYPE  (-101) /* Wrong protocol or codec type */
    #define IOIF_ENOPROTOOPT (-102) /* Protocol or codec not available */
    #define IOIF_EPROTONOSUPPORT (-103) /* Protocol or codec not supported */
    #define IOIF_EOPNOTSUPP  (-104) /* Operation not supported */
    #define IOIF_EAFNOSUPPORT (-105) /* Address family not supported */
    #define IOIF_EADDRINUSE  (-106) /* Address already in use */
    #define IOIF_EADDRNOTAVAIL (-107) /* Address not available */
    #define IOIF_ECONNRESET  (-108) /* Connection reset */
    #define IOIF_EISCONN     (-109) /* Connected */
    #define IOIF_ENOTCONN    (-110) /* Not connected */
    #define IOIF_ECONNREFUSED (-112) /* Connection refused */
    #define IOIF_EPROCLIM    (-113) /* Too many processes */
    #define IOIF_EUSERS      (-114) /* Too many users */
    #define IOIF_EDEADLK     (-115) /* Resource deadlock would occur */
    #define IOIF_EPROTO      (-116) /* Protocol or codec error */
    #define IOIF_EOVERFLOW   (-117) /* Value too large to store in data type */
    #define IOIF_ESRCH       (-118) /* No such task */
    #define IOIF_EILSEQ      (-119) /* Illegal byte sequence format */
    #define IOIF_EBADMSG     (-120) /* Bad message */
    #define IOIF_ENOSYS      (-121) /* Function not supported */
    #define IOIF_ECRC        (-122) /* CRC failure */
    #define IOIF_EEOF        (-123) /* Position is End Of File */
    #define IOIF_ESEEKPASTEND (-124) /* Seek past EOF */
    #define IOIF_EMEDIUMTYPE (-127) /* Wrong medium type */
    #define IOIF_ENOMEDIUM   (-128) /* No medium present */
    #define IOIF_ESEEKRANGE  (-129) /* seek setting error */  /* Source Merge 11-1 */

/******************************************************************************
 Typedef definitions
 ******************************************************************************/
/// Mutex Definition structure contains setup information for a mutex.
typedef struct os_mutex_def  {
  void                      *mutex;    ///< pointer to internal data
} osMutexDef_t;

/** event handle object */
typedef void *event_t;

typedef struct AHF_S
{
        struct aiocb *head;
        struct aiocb *tail;
        uint32_t     flags;
        void *      semid; /* Source Merge 08-81 */
        osMutexDef_t *p_cmtx;
        uint32_t     saved_int_mask;
} AHF_S;

#define SIGEV_NONE      (1) /**< No notification */
#define SIGEV_THREAD    (2) /**< A notification function is called in the driver's context */
#define SIGEV_EVENT     (3) /**< An event flag is set */
#define SIGEV_CALLBACK  (4) /**< An inter-process callback is made */

union sigval
{
    int             sival_int;          /**< Integer interpretation */
    void *          sival_ptr;          /**< Pointer interpretation */
};

struct sigevent
{
    int             sigev_notify;       /**< SIGEV_NONE, SIGEV_THREAD, SIGEV_EVENT */
    void *          sigev_signo;
    union sigval    sigev_value;        /**< user defined value to pass */
    void            (*sigev_notify_function) (union sigval);
                                        /**< notification function */
};

struct aiocb
{
    /* Implementation specific stuff */
    struct aiocb *  pNext;              /**< next aio request */
    struct aiocb *  pPrev;              /**< previous aio request */
    ssize_t         aio_return;         /**< return value of operation */
    int             aio_complete;       /**< non-zero when op is completed */

    /* User parameters */
    int             aio_fildes;         /**< FD for asynchronous op */
    volatile void*  aio_buf;            /**< Buffer to read from/write to */
    size_t          aio_nbytes;         /**< Number of bytes to read/write */
    struct sigevent aio_sigevent;       /**< Signal to deliver */

    /* Private implementation specific function pointer used by IOIF. Application should not set. */
    ssize_t (*aio_iofn)(void *,void *,size_t,int);  /* synchronous I/O function */
};
typedef struct aiocb AIOCB;

/* AHF (Asynchronous Helper Functions */
/**
 * @brief Creates an empty aio control queue pointer. Creates a mutex if AHF_CREATESEM bit flag is set.
 * @param[in] ahf  - aio queue structure pointer.
 * @param[in] f    - flag indicating that at semaphore is to be created.
 * @return    0 on success.   negative error code on error.
 **/
int32_t ahf_create (AHF_S * const ahf, const uint32_t f); /* Source Merge 08-83 */

/**
 * @brief Delete aio control block mutex (if it exists). Note: Does not delete the aio control block queue.
 * @param[in] ahf  - aio queue structure pointer.
 **/
void ahf_destroy (AHF_S const * const ahf);

/**
 * @brief Add an aio control block to the queue.
 * @param[in] ahf  - aio queue structure pointer.
 * @param[in] aio  - pointer to queue structure.
 **/
void ahf_addtail (AHF_S * const ahf, struct aiocb * const aio); /* Source Merge 08-04 */

/**
 * @brief Remove an aio control block from the queue.
 * @param[in] ahf  - aio queue structure pointer.
 * @param[in] aio  - pointer to queue structure.
 * @return    aio control block, or NULL if queue is empty.
 **/
struct aiocb *ahf_removehead (AHF_S * const ahf);

int32_t ahf_remove (AHF_S *ahf, struct aiocb *aio);

/**
 * @brief Get an aio control block from the queue, but do not remove from the queue.
 * @param[in] ahf  - aio queue structure pointer.
 * @return    aio control block, or NULL if queue is empty.
 **/
struct aiocb *ahf_peekhead (AHF_S * const ahf);

/**
 * @brief Empty an aio control block queue.
 * @param[in] ahf  - aio queue structure pointer.
 **/
void ahf_cancelall (AHF_S * const ahf);

/**
 * @brief Flag that the operation is complete.
 If required by the sigev_notify variable contents, the calling task is notified.
 If a notify was setup, it is signalled.
 * @param[in] ahf  - aio queue structure pointer.
 * @param[in] aio  - pointer to queue structure.
 * @return    0 on success.   negative error code on error.
 **/
void ahf_complete (AHF_S *ahf, struct aiocb * const aio);

/**
 * @brief Remove an aio control block from the queue.
 Signal completion to the calling task and wake it up.
 If aio is NULL, cancel all.
 * @param[in] ahf  - aio queue structure pointer.
 * @param[in] aio  - pointer to queue structure.
 * @return    0 on success.   negative error code on error.
 **/
int32_t ahf_cancel (AHF_S * const ahf, struct aiocb * const aio);

#endif /* AIOIF_H */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
