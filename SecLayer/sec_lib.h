#ifndef __SEC_LIB_H__
#define __SEC_LIB_H__

#include <stdint.h>

#define   HASH_ERR        (-2)
#define   GEN_KEY_ERR     (-3)
#define   SIGN_ERR        (-4)
#define   VERIFY_ERR      (-5)
#define   RANDOM_ERR      (-6)

/*
  生成随机数函数，需要用户实现，　内部API会调用

  输入参数：
    len             ：           需要生成的随机数长度
  输出参数
    random　　　　　 ：           生成随机数的缓冲区
  返回值
    0               ：          随机数生产成功
    非0             ：          随机数生成失败
*/
int GenRandom(unsigned char *random, int len);


/**
 * 产生公、私钥对
 *
 * @sk: 输入，私钥数据的缓冲区起始地址
 * @pk: 输出，公钥数据的缓冲区起始地址
 *
 * @return: 0   密钥对生成成功
 *			< 0 密钥生成失败
 */
int GenSM2KeyPair(unsigned char *sk,  unsigned char *pk);

/**
 * 产生公、私钥对
 *
 * @index: 公、私钥对的索引值。[0,50)
 *
 * @return: 0   密钥对生成成功
 *			< 0 密钥生成失败
 */

int GenSM2KeyPair_By_Index(unsigned int index);

/**
 * 获取公钥
 *
 * @index: 公、私钥对的索引值。[0, 50)
 * @pk:		公钥缓存区, 密钥长度64字节
 *
 * @return:
 *			0	获取公钥成功
 *			< 0 获取公钥失败
 */
int getSM2Pk(unsigned int index, unsigned char *pk);


/**
 * 使用私钥数据对消息做签名
 *
 * @message:	输入，待签名的消息
 * @message_len: 输入，待签名的消息长度
 * @sk:			输入，签名使用的私钥(由GenSM2KeyPair函数生成)
 * @pk:			输入，签名使用的公钥钥(由GenSM2KeyPair函数生成)
 * @sign:		输出，签名值
 * @sign_len:	输出，签名长度(2048bit)
 *
 * @return:
 *		0:		签名成功
 *		非0 :　	签名失败
 */
int SM2SignMessage(unsigned char *message, int message_len, unsigned char *sk, unsigned char *sign, int *sign_len);
//int SM2SignMsg(unsigned char *msg, int mlen, unsigned char *sk, unsigned char *pk, unsigned char *sign, int *slen);

/**
 * 使用私钥索引对消息做签名
 *
 * @message:	输入，待签名的消息
 * @message_len: 输入，待签名的消息长度
 * @index:		输入，签名使用的私钥索引值(由GenSM2KeyPair函数生成)
 * @pk:			输入，签名使用的公钥钥(由GenSM2KeyPair函数生成)
 * @sign:		输出，签名值
 * @sign_len:	输出，签名长度(2048bit)
 *
 * @return:
 *		0:		签名成功
 *		非0:　	签名失败
 */
//int SM2SignMessage_by_index(unsigned char *message, int message_len, unsigned int index, unsigned char *sign, int *sign_len);
int SM2SignMsg_by_index(unsigned char *msg, int mlen, unsigned int index, unsigned char *pk, unsigned char *sign, int *slen);


/**
 * 对消息的签名验签
 *
 * @message:	输入， 待验签的消息
 * @message_len: 输入， 待验签的消息长度
 * @pk:         输入，验签使用的公钥(由GenSM2KeyPair函数生成，并且需要和验签使用的私钥匹配)
 * @sign:       输出，签名值
 * @sign_len:   输出，签名长度(2048bit)
 * @return:
 *		0   :   验签成功
 *		非0 : 　验签失败
 */
int SM2VerifyMessage(unsigned char *message, int message_len, unsigned char *pk, unsigned char *sign, int sign_len);

/**
 * 非对称加密接口
 *
 * @plain:	输入，明文数据
 * @plain_len: 输入，明文长度
 * @random: 加密随机向量
 * @random_len: 输入，随机向量长度
 * @pk: 输入，加密使用的公钥
 * @pk_len: 输入，公钥长度
 * @cipher: 输出，加密后的密文
 * @cipher_len: 输出，密文长度
 *
 * @return:
 *
 *
 */
int SM2_Encrypt(const uint8_t *plain, uint32_t plain_len, const uint8_t *random, uint32_t random_len, const uint8_t *pk, uint32_t pk_len,
						uint8_t *cipher, uint32_t *cipher_len);
/**
 * 通过私钥数据对密文进行非对称解密接口
 *
 * @cipher: 输入，密文数据
 * @cipher_len: 输入，密文长度
 * @sk:		输入，解密使用的私钥
 * @sk_len:	输入，私钥长度
 * @plain:	输出，解密后的明文
 * @plain_len: 输出，明文长度
 *
 * @return:
 *
 */
int SM2_Decrypt(const uint8_t *cipher, uint32_t cipher_len, const uint8_t *sk, uint32_t sk_len, uint8_t *plain, uint32_t *plain_len);

/**
 * 通过私钥索引对密文进行非对称解密接口
 *
 * @cipher: 输入，密文数据
 * @cipher_len: 输入，密文长度
 * @index:		输入，解密使用的私钥的索引
 * @plain:	输出，解密后的明文
 * @plain_len: 输出，明文长度
 *
 * @return:
 *
 */
int SM2_Decrypt_by_index(const uint8_t *cipher, uint32_t cipher_len, const uint8_t *sk, uint8_t *plain, uint32_t *plain_len);


/**
 * 公钥压缩算法
 *
 * @xy: 输入, 公钥数据，64字节
 * @x:	输出，压缩后公钥数据x值，32字节
 * @y_smbol: 输出，公钥数据y值标识符，1字节
 */
void SM2PkCompress(unsigned char *x, unsigned char *y_smbol, unsigned char *xy);


/**
 * 公钥解压缩算法
 *
 * @x: 输入, 公钥数据x值，32字节
 * @y: 输入，公钥数据y值标识符，1字节
 * @r: 输出，解压缩后的公钥数据，64字节
 */
void SM2PkDecompress(unsigned char *r, unsigned char *x, unsigned char y);

/**
 * 消息摘要算法
 *
 * @msg: 输入，消息数据
 * @msg_len: 输入，消息长度
 * @hash: 输出，消息摘要值
 * @hash_len:
 *
 * @return:
 *
 */
int SM3Hash(unsigned char *msg, unsigned int msg_len, unsigned char *hash, unsigned int hash_len);

/**
 * SM4 CBC模式加密接口
 *
 * @key:	输入，密钥首地址
 * @key_len:输入，密钥长度,默认为16
 * @pt:		输入，待加密数据首地址
 * @pt_len:	输入，待加密数据长度，为16的倍数
 * @ct:		输出，加密后密文的首地址（输出长度与密文长度相同）
 * @iv:		输入，加密的随机变量地址
 * @flag:	输入，0:一次调用，１:多次调用
 *
 * @return:
 *		0:			运算成功
 *		-1:			输入非法
 */
int SM4_EncCBC(unsigned char *key, unsigned int key_len,unsigned char *pt,
				 unsigned int pt_len,unsigned char *ct,unsigned char *iv,unsigned int flag);

/**
 * SM4 CBC模式加密接口
 *
 * @key:	输入，密钥首地址
 * @key_len:输入，密钥长度,默认为16
 * @ct:		输入，待解密密文数据首地址
 * @ct_len:	输入，待解密密文数据长度，为16的倍数
 * @pt:		输出，解密后的明文首地址（输出长度与密文长度相同）
 * @iv:		输入，解密使用的随机向量
 * @flag:	输入，0:一次调用，１:多次调用
 *
 * @return:
 *		0:			运算成功
 *		-1:			输入非法
 */
int SM4_DecCBC(unsigned char *key, unsigned int key_len,unsigned char *ct,
				 unsigned int ct_len,unsigned char *pt,unsigned char *iv,unsigned int flag);


#endif
