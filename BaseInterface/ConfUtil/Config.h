/**
 * @defgroup ConfUtil ConfUtil API
 * @ingroup BaseInterface API
 * @{
 *
 * 本模块提供对配置文件进行操作的API接口.
 *
 * @file Config.h
 * @brief API file.
 *
 *******************************************************************/

#ifndef __CONFIG_H
#define __CONFIG_H

#include "./../DataSet.h"
#include "file.h"
#ifdef __cplusplus
extern "C" {
#endif

#define  SUCCESS				0x00 ////*成功*/
#define  FAILURE				0x01 ////**失败*/

#define  FILENAME_NOTEXIST		0x02 ////*配置文件名不存在*/
#define  SECTIONNAME_NOTEXIST	0x03 ////*节名不存在*/
#define  KEYNAME_NOTEXIST		0x04 ////*键名不存在*/
#define  STRING_LENNOTEQUAL		0x05 ////*两个字符串长度不同*/
#define  STRING_NOTEQUAL		0x06/// /*两个字符串内容不相同*/
#define  STRING_EQUAL			0x00 ////*两个字符串内容相同*/

 /**
 * @brief 获取键值（整形）.
 *
 * 在配置文件中，查找set名为pInSectionName,Item名为pInKeyName的值，然后以pOutKeyValue返回结果
 *
 * @param[in] pInFileName      配置文件路径.
 * @param[in] pInSectionName   段名.
 * @param[in] pInKeyName       键名.
 * @param[out] pOutKeyValue    键值.
 *
 * @return  成功获取键值返回SUCCESS，键值不存在返回KEYNAME_NOTEXIST.
 */
int GetConfigIntValue(char *pInFileName,char *pInSectionName,char *pInKeyName,int *pOutKeyValue);
 
/**
 * @brief 获取键值（字符）.
 *
 * 在配置文件中，查找set名为pInSectionName,Item名为pInKeyName的值，然后以pOutKeyValue返回结果
 *
 * @param[in] pInFileName      配置文件路径.
 * @param[in] pInSectionName   段名.
 * @param[in] pInKeyName       键名.
 * @param[out] pOutKeyValue    键值.
 *
 * @return  成功获取键值返回SUCCESS，键值不存在返回KEYNAME_NOTEXIST.
 */
int GetConfigStringValue(char *pInFileName,char *pInSectionName,char *pInKeyName,char *pOutKeyValue);

/**
 * @brief 获取键值（字符）.
 *
 * 在配置文件中，设置set名为pInSectionName,Item名为pInKeyName的值，设置为NewKeyValue值
 *
 * @param[in] pInFileName		配置文件路径.
 * @param[in] pInSectionName 	段名.
 * @param[in] pInKeyName   		键名.
 * @param[out] NewKeyValue		键值.
 *
 * @return  设置配置文件的结果状态，0：表示执行成功，非0表示失败
 */
int SetConfigIntValue(char *pInFileName,char *pInSectionName,char *pInKeyName,int NewKeyValue);

/**
 * @brief 获取键值（字符）.
 *
 * 在配置文件中，设置set名为pInSectionName,Item名为pInKeyName的值，设置为NewKeyValue值
 *
 * @param[in] pInFileName		配置文件路径.
 * @param[in] pInSectionName 	段名.
 * @param[in] pInKeyName   		键名.
 * @param[out] NewKeyValue		键值.
 *
 * @return  设置配置文件的结果状态，0：表示执行成功，非0表示失败
 */
int SetConfigStringValue(char *pInFileName,char *pInSectionName,char *pInKeyName,char *pNewKeyValue);

#ifdef __cplusplus
}
#endif

#endif

/**
 * @}
 */
