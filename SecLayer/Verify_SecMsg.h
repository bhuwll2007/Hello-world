
/**
 * @ingroup SecLayeer API
 * @{
 *
 * 本模块提供V2X安全层相关的API接口.
 *
 * @file Verify_SecMsg.h
 * @brief API file.
 *
 */
#ifndef SECLAYER_VERIFY_SECMSG_H_
#define SECLAYER_VERIFY_SECMSG_H_

#include "FunCommun.h"
/**
 * @brief 验证证书.
 * @param[in] PCAName  文件路径.
 * @param[in] POBU_Cert  指向Certificate_t指针
 * @return 校验结果
 */
int Certificate_Verify(char PCAName[], Certificate_t *POBU_Cert);
/**
 * @brief 验证签名消息.
 * @param[in] pSecuredMessage  指向SecuredMessage_t的指针.
 * @return 验证结果.
 */
int SecuredMessage_Verify(SecuredMessage_t *pSecuredMessage);
/**
 * @brief 验证安全消息.
 * @param[in] SecBuf  安全消息.
 * @param[in] SecBufLen  安全消息长度.
 * @param[in] nTxAid  Aid.
 * @return 验证结果.
 */
int SecurityVerify_SecBuf(uint8_t SecBuf[], int SecBufLen, int nTxAid);
#endif /* SECLAYER_VERIFY_SECMSG_H_ */
