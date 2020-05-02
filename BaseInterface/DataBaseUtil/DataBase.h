/**
 * @defgroup DataBaseUtil DataBaseUtil API
 * @ingroup BaseInterface API
 * @{
 *
 * 本模块提供对收发数据库进行操作的API接口.
 *
 * @file Config.h
 * @brief API file.
 *
 */

#ifndef __SQLINTER_H_
#define __SQLINTER_H_

#include "./../DataSet.h"

#include <sqlite3.h>

/**
 * @brief 存储当前数据库中表.
 */
struct Tbllist
{
	int nNbr;
	int cNbr;
	char **TblStr;

};

struct TxRxTblData
{
	int KeyID;
	int msgCnt;
	int MsgType;
	uint64_t DataTime;
	int DataLen;
	unsigned char *DataBuf;

};

//YuTong RSUInfo
typedef struct _SPAT_INFO
{
	int nRecvbytes;	//接收数据的字节数
	int nRecvPags;	//接收数据的包数
	int nSendPags;	//发送SPAT包数
	int nSendBytes;	//发送SPAT字节数
	int nAddRecvbytes;	//增加的接收数据的字节数
	int nAddRecvPags;	//增加的接收数据的包数
	int nAddSendPags;	//增加的发送SPAT包数
	int nAddSendBytes;	//增加的发送SPAT字节数
	int nModemStatus;	//模组状态
	char acLmi40IpAddr[20];	//IP地址
	unsigned long long ullLmi40RxBytes;	//lmi40接收字节数
	unsigned long long ullLmi40TxBytes;	//lmi40发送字节数
} tSPAT_INFO;

/**
 * @brief DataBase_OpenDB 创建或者打开数据库.
 *
 * @param[in] p_pdbName     数据库文件名称 例如：a.db.
 * @param[in] nCreateMode   数据库打开模式，0：打开时不做任何操作，1：打开时清空删除数据库当前所有表.
 *
 * @return >0 代表创建成功，数据库的操作句柄. <=0 数据库创建失败
 */
sqlite3 *DataBase_OpenDB(char *p_pdbName, int nCreateMode);

/**
 * @brief DataBase_CloseDB 关闭数据库.
 *
 * @param[in] p_sqldb  数据库的操作句柄.
 *
 * @return  数据库关闭成败状态，0表示成功，其他表示失败.
 */
int DataBase_CloseDB(sqlite3 *p_sqldb);

/**
 * @brief 在句柄db的数据库中创建名为p_ptbName的表.
 *
 * @param[in] p_ptbName		数据表表名.
 * @param[in] p_sqldb		数据库操作句柄.
 *
 * @return  数据库中，对表进行创建操作的执行状态，0表示成功，其他表示失败.
 */
int DataBase_CreatTab(char *p_ptbName, sqlite3 *p_sqldb);

/**
 * @brief 在句柄db的数据库中删除表名为p_ptbName的表.
 *
 * @param[in] p_ptbName		数据表表名.
 * @param[in] p_sqldb		数据库操作句柄.
 *
 * @return  数据库中，对表进行删除操作的执行状态，0表示成功，其他表示失败.
 */
int DataBase_DeleteTab(char *p_ptbName, sqlite3 *p_sqldb);

/**
 * @brief 在句柄db的数据库中清空表名为p_ptbName的表.
 *
 * @param[in] p_ptbName		数据表表名.
 * @param[in] p_sqldb		数据库操作句柄.
 *
 * @return  数据库中，对表进行清空操作的执行状态，0表示成功，其他表示失败.
 */

int DataBase_ClearTab(char *p_ptbName, sqlite3 *p_sqldb);

/**
 * @brief 向数据库的指定表中批量添加数据
 *
 * 向数据库的指定表中批量添加数据，主要用于存储发送数据和接收数据功能
 *
 * @param[in] p_ptbName  数据库中表名.
 * @param[in] p_sqldb    数据库中数据库操作句柄.
 * @param[in] Data       数据结构体.
 * @param[in] NbrData    要添加的数据个数.
 *
 * @return  数据库中，对表进行添加数据操作的执行状态，0表示成功，其他表示失败.
 */
int DataBase_Tab_Insert(char *p_ptbName, sqlite3 *p_sqldb,
		struct TxRxTblData Data[], int NbrData);

/**
 * @brief 在数据库的指定表中，删除字段名为p_pkeyName的字段中，字段值为p_pkeyValue的信息.
 *
 * @param[in] p_ptbName    数据库中表名.
 * @param[in] p_sqldb      数据库中数据库操作句柄.
 * @param[in] p_pkeyName   字段名称.
 * @param[in] p_pkeyValue  字段值.
 *
 * @return  数据库中，对表中的数据进行删除操作的执行状态，0表示成功，其他表示失败.
 */
int DataBase_Tab_Delete(char *p_ptbName, sqlite3 *p_sqldb, char *p_pkeyName,
		char *p_pkeyValue);

/**
 * @brief 数据库查询
 *
 * 在数据库的指定表中查找字段名为p_pkeyName的字段中，字段值为p_pkeyValue的信息（信息集）.
 *
 * @param[in] p_ptbName    数据库中表名.
 * @param[in] p_sqldb      数据库中数据库操作句柄.
 * @param[in] p_pkeyName   字段名称.
 * @param[in] p_pkeyValue  字段值.
 * @param[in] limitNbr     限制查询的数据个数（-1表示不限制）.
 * @param[out] p_pData     查询到的数据指针.
 * @param[out] NbrData     查询到的数据个数.
 *
 * @return  数据库中，对表中的数据进行查询操作的执行状态，0表示成功，其他表示失败.
 */
int DataBase_Tab_Select(char *p_ptbName, sqlite3 *p_sqldb, char *p_pkeyName,
		char *p_pkeyValue, struct TxRxTblData p_pData[], int *NbrData,
		int limitNbr);

/**
 * @brief 数据表更新功能
 *
 * 在数据库的指定表中将字段名为p_pkeyName的字段中，旧值p_pkeyOldValue更新为新值p_pkeyNewValue.
 *
 * @param[in] p_ptbName       数据库中表名.
 * @param[in] p_sqldb         数据库中数据库操作句柄.
 * @param[in] p_pkeyName      字段名称.
 * @param[in] p_pkeyOldValue  字段旧值.
 * @param[in] p_pkeyNewValue  字段新值.
 *
 * @return  插入数据的成功状态，0：表示插入成功，非0表示失败.
 */
int DataBase_Tab_Update(char *p_ptbName, sqlite3 *p_sqldb, char *p_pkeyName,
		char *p_pkeyOldValue, char *p_pkeyNewValue);


int DataBase_Insert_YuTong(sqlite3 *db, tSPAT_INFO SpatInfo);
#endif

/**
 * @}
 */
