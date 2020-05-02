/***************************************************************//**
 * @defgroup RsmConfig RsiConfig API
 * @ingroup FaciLayer
 * @{
 *
 *  This module provides APIs to do the operate about RsmConfig.
 *
 * @file RsiConfig.h
 * @brief API file.
 *  Created on: 20200316
 *      Author: chaizhijun
 *******************************************************************/

#ifndef __RSICONFIG_H_
#define __RSICONFIG_H_
#include "./../Algorithm/Type.h"
#include "./../Include/BaseInterface_Include.h"
//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------
/****************************************************************************
* 与平台Rsi消息相关接口定义
****************************************************************************/
typedef struct rectRsiPlatformToV2X
{
	uint8_t   m_ucRsiNum;
	uint32_t  m_unYearTime;
	uint8_t   m_ucRsuNum[8];
	uint32_t  m_unRsiID;
	uint16_t  m_usRsiType;
	uint8_t   m_ucRSIDescrib[32];
	nint32_t  m_unRsiLat;
	nint32_t  m_unRsiLong;
	uint16_t  m_usRsiAlt;
	nint32_t  m_unRsiPoint1lat;
	nint32_t  m_unRsiPoint1long;
	nint32_t  m_unRsiPoint2lat;
	nint32_t  m_unRsiPoint2long;
}rectRsiPlatformToV2X_t;

extern void FG_RsiRecvFromPlatformFree(void);

extern int FG_RsiRecvFromPlatformMsgGet(rectRsiPlatformToV2X_t *p_prectRsiPlatformToV2Xt);

extern int FG_RsiRecvFrompPlatformMsgSet(rectRsiPlatformToV2X_t *p_prectRsiPlatformToV2Xt);

#endif /*__LASERDATA_H_*/

/**
 * @}
 */
