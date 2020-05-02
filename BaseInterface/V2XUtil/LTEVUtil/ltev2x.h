/***************************************************************//**
 * @addtogroup DTVL3000_GROUP1 Radio Access Network Layer API
 * @{
 *
 *  This module provides APIs to send/receive LTEV2X packets.
 * 
 *  
 *
 *******************************************************************/


/***************************************************************//**
 * COPYRIGHT NOTICE
 * Copyright (c) 2017 Datang Telecom Group All rights reserved
 * @file ltev2x.h
 * @brief API file.
 * 
 *    
 *
 * @date 2017.3.15
 *******************************************************************/
#ifndef _LTEV2X_H_
#define _LTEV2X_H_


/** 
 * @brief error code.
 */ 
extern int errcode;

/** 
 * @brief LTEV2X handle.
 */ 
typedef int                         LTEV2X_HANDLE;

/** 
 * @brief LTEV2X options.
 */ 
typedef struct {
    unsigned int   ue_id;       /**< UE ID, 24bits valid, default 0. */
    unsigned int   ps_id;       /**< PS ID, 24bits valid, default 0. */
    signed   char  tx_power;    /**< Transmission power, -30..30, unit dBm, default 0. */
    unsigned char  priority;    /**< Packet priority, 0..7, 0 is the highest, default 7. */
    unsigned char  period;      /**< SPS period, 0..10, 0 means oneshot and (1..10) means (100ms..1000ms), default 1. */
    unsigned char  proc_id;     /**< SPS process id, 0..1, default 0. */
    unsigned short data_len;    /**< Max payload data length in bytes. */
    unsigned char  test_mode;   /**< Test mode, 0..6, only valid when lower layer test mode enabled, default 0. */
    unsigned char  test_sfn;    /**< Test subframe number, 0..100, only valid when lower layer test mode enabled, used for fixed sfn mode, default 0. */
} LTEV2X_Option;

/** 
 * @brief Open a relevant LTEV2X interface and store the handle.
 * @return  A handle on success or -1 on error.
 */
extern LTEV2X_HANDLE LTEV2X_Open();

/** 
 * @brief Set options of the LTEV2X interface, must be called after LTEV2X_Open, otherwise, run with default options.
 * @return  0 on success or -1 on error.
 */
extern int LTEV2X_SetOption(LTEV2X_HANDLE handle, LTEV2X_Option* option);

/** 
 * @brief Close the underlying LTEV2X interface and clear the handle.
 * @return  0 on success or -1 on error.
 */
extern int LTEV2X_Close(LTEV2X_HANDLE handle);

/** 
 * @brief Attempts to receive size bytes from a LTEV2X interface via handle into the buffer starting at buf.
 *        If no data available, the receive would block.
 * @param[in] handle  LTEV2X handle.
 * @param[in] buf  Point to the receiving buffer.
 * @param[in] size  Expected receiving data length in byte.
 * @param[out] rx_power Receiving power in dBm.
 * @return  The number of bytes received on success or -1 on error.
 */
extern int LTEV2X_Recv(LTEV2X_HANDLE handle, void* buf, int size, int* rx_power);

/** 
 * @brief Attempts to send size bytes to a LTEV2X interface via handle into the buffer starting at buf.
 * @param[in] handle  LTEV2X handle.
 * @param[in] buf  Point to the sending buffer.
 * @param[in] size  Expected sending data length in byte.
 * @return  The number of bytes sent on success or -1 on error.
 */
extern int LTEV2X_Send(LTEV2X_HANDLE handle, void* buf, int size);


#endif

/**
 * @}
 */
