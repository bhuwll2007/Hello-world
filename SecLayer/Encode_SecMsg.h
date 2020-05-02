
/**
 * @defgroup SecLayer SecLayer API
 * @{
 *
 * 本模块提供提供安全层的API接口.
 *
 * @}
 *
 */

/**
 * @ingroup SecLayeer API
 * @{
 *
 * 本模块提供提供安全层的API接口-安全数据编码接口.
 *
 * @file Encode_SecMsg.h
 * @brief API file.
 *
 */

#ifndef SECLAYER_ENCODE_SECMSG_H_
#define SECLAYER_ENCODE_SECMSG_H_

#include "FunCommun.h"
/**
 * @brief 安全消息编码.
 * @param[in] nAid  DSM数据AID.
 * @param[in] signingData  DSM数据实体部分.
 * @param[in] signingDatalen  DSM数据实体字节长度.
 * @param[in] pbSecuredMessage 安全数据.
 * @param[in] nSecuredMessageLen 安全数据长度.
 * @return 编码信息
 */
int Encode_SecuredMessage(int SubjectType, int CAOrgan, int nAid,
		uint8_t *signingData, int signingDatalen,
		unsigned char *pbSecuredMessage, int *nSecuredMessageLen);

#endif /* APP_SENDSECMSG_H_ */

/**
 * @}
 */
