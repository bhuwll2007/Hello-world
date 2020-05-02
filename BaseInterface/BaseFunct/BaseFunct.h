/**
 * @defgroup BaseInterface BaseInterface API
 * @{
 *
 * 本模块提供基本操作接口层的API接口.
 *
 * @}
 *
 */

/**
 * @defgroup BaseFunct BaseFunct API
 * @ingroup BaseInterface API
 * @{
 *
 * 本模块提供基本操作接口层中，基本操作功能的API接口.
 *
 * @file BaseFunct.h
 * @brief API file.
 *
 */

#ifndef _BASEFUNCT_H_
#define _BASEFUNCT_H_
#include "./../DataSet.h"

/**
 * @brief pkt_code 数据转义
 *
 * @param[in] 	src_buf			--	转义前数据
 * @param[out] 	target_buf		--	转义后数据
 * @param[in] 	len				--	转义前数据长度
 *
 * @return 转义后数据长度
 */
uint32_t pkt_code(uint8_t src_buf[], uint8_t target_buf[], uint32_t len);

/**
 * @brief Fun_Xor 计算指定数组的异或校验值
 *
 * @param[in] 	ppSendBuf		--	待计算校验值的数组
 * @param[in] 	pnSendBuflen	--	待计算校验值的数组的长度
 *
 * @return void 无
 */
void Fun_Xor(uint8_t* ppSendBuf, uint32_t pnSendBuflen);

/**
 * @brief Pkt_Decode 进行数据包的解码工作，去头去尾，通过校验码判断数据包数据是否正确.
 * @param[in] 	src_buf：	--	待解码的数据存储的buf
 * @param[out] 	target_buf: --	解码后数据存储的buf 仅包括帧内容，不包括帧序号和校验位
 * @param[in] 	len: 		--	待解码的数据长度
 * @return  	<0 解码失败
 * @return  	>0 解码成功，此时代表解码后的数据存储buf
 */
uint32_t Pkt_Decode(uint8_t *src_buf, uint8_t *target_buf, uint32_t len);
uint32_t pkt_code_DF(uint8_t src_buf[], uint8_t target_buf[], uint32_t len);
#endif /*#ifndef _BASEFUNCT_H_*/
/**
 * @}
 */
