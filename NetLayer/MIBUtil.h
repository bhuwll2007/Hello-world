
/**
 * @ingroup NetLayer API
 * @{
 *
 * 本模块V2X网络层的API接口.
 *
 * @file MIBUtil.h
 * @brief API file.
 *
 */
#ifndef _NETLAYER_MIBUTIL_H_
#define _NETLAYER_MIBUTIL_H_
#include "DataSet.h"
#include "list.h"

//*******************************应用配置信息***************************************//
/**
 * @brief 应用配置信息.
 */
typedef struct UstEntry
{
	uint16_t ApplicatonIdentifier;
}__attribute__ ((packed)) tUserServiceInfo;

//应用配置信息
/**
 * @brief 应用配置信息.
 */
typedef struct ApplicationMIB
{
	tUserServiceInfo UserServiceInfo;
	struct list_head list;
}__attribute__ ((packed)) tApplicationMIB_list;

//*******************************系统状态信息***************************************//
/**
 * @brief 系统状态信息.
 */
typedef struct LocalInfo
{
	uint16_t DSMMaxLength;     //最大消息长度
}__attribute__ ((packed)) tLocalInfo;

//系统状态信息
/**
 * @brief 系统状态信息.
 */
typedef struct SystemMIB
{
	tLocalInfo LocalInfo;			//本地默认参数信息
}__attribute__ ((packed)) tSystemMIB;

//**********************************************************************//

//网络层管理信息实体MIB
/**
 * @brief 网络层管理信息实体MIB.
 */
typedef struct DSMMIB
{
	tSystemMIB SystemMIB;
	tApplicationMIB_list *pApplicationMIB_lsit;
}__attribute__ ((packed)) tDSMMIB;

int DSM_MIB_Setup(uint16_t aid[], int nAidNum);
int DSM_AID_Check(uint16_t Aid);

#endif /* _NETLAYER_MIBUTIL_H_ */
/**
 * @}
 */
