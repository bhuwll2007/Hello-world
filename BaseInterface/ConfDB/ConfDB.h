/**
 * @defgroup ConfDB ConfDB API
 * @ingroup BaseInterface API
 * @{
 *
 * 本模块提供对参数数据库进行操作的API接口.
 *
 * @file ConfDB.h
 * @brief API file.
 *
 */

#ifndef __CONF_DB_H_
#define __CONF_DB_H_

#include <sqlite3.h>
#include "./../DataSet.h"
/**
 * @brief 对数据库元素进行ID检索时的结果结构体.
 */
typedef struct _CommParmItem {
	char keyName[30];
	char keyValue[30];
} ITEM;

/**
 * @brief 创建或者打开数据库.
 *
 * 创建或者打开数据库
 *
 * @param[in] p_pdbName    数据库文件名称 例如：a.db.
 * @param[in] nCreateMode  数据库打开模式，0：打开时不做任何操作，1：打开时清空删除数据库当前所有表.
 *
 * @return  数据库的操作句柄.
 */
sqlite3 *ConfDB_OpenDB(char* p_pdbName, int nCreateMode);

/**
 * @brief 关闭数据库.
 *
 * 关闭数据库
 *
 * @param[in] p_sqldb  数据库的操作句柄.
 *
 * @return  数据库关闭成败状态，0 执行成功，非0 执行失败.
 */
int ConfDB_CloseDB(sqlite3 * p_sqldb);
 /**
 * @brief ConfDB_IsTabExists 在句柄db的数据库中查找是否存在名为p_ptbName的表.
 *
 * @param[in] p_ptbName  数据表表名.
 * @param[in] p_sqldb    数据库操作句柄.
 *
 * @return  -1 表示查询失败，0 表示查询成功但是没有此表，1 表示查询成功且有此表.
 */
int ConfDB_IsTabExists(char* p_ptbName, sqlite3 *p_sqldb);

 /**
 * @brief 在句柄db的数据库中创建名为p_ptbName的表.
 *
 * @param[in] p_ptbName  数据表表名.
 * @param[in] db         数据库操作句柄.
 *
 * @return  数据库中，对表进行创建操作的执行状态，0表示成功，其他表示失败.
 */
int ConfDB_CreatTab(char* p_ptbName, sqlite3 *db);

 /**
 * @brief 在句柄db的数据库中删除表名为p_ptbName的表.
 * @param[in] p_ptbName  数据表表名.
 * @param[in] db         数据库操作句柄.
 * @return  数据库中，对表进行删除操作的执行状态，0表示成功，其他表示失败.
 */

int ConfDB_DeleteTab(char* p_ptbName, sqlite3 *db);

 /**
 * @brief 在句柄db的数据库中清空表名为p_ptbName的表.
 *
 * @param[in] p_ptbName  数据表表名.
 * @param[in] db         数据库操作句柄.
 *
 * @return  数据库中，对表进行清空操作的执行状态，0表示成功，其他表示失败.
 */
int ConfDB_ClearTab(char* p_ptbName, sqlite3 *db);

 /**
 * @brief 向数据库的指定表中插入数据.
 *
 * @param[in] p_ptbName  数据库中表名.
 * @param[in] db         数据库中数据库操作句柄.
 * @param[in] pConfItem  数据指针.
 *
 * @return  插入数据的成功状态，0：表示执行成功，非0表示失败.
 */
int ConfDB_Tab_Insert(char* p_ptbName, sqlite3 *db, ITEM pConfItem);

/**
 * @brief 在数据库的指定表中，删除字段名为p_pkeyName的字段中，字段值为p_pkeyValue的信息.
 *
 * @param[in] p_ptbName     数据库中表名.
 * @param[in] p_sqldb       数据库中数据库操作句柄.
 * @param[in] p_pkeyName    字段名称.
 * @param[in] p_pkeyValue   字段值.
 *
 * @return  删除数据的执行状态，0：表示执行成功，非0表示失败.
 */
int ConfDB_Tab_Delete(char* p_ptbName, sqlite3 *p_sqldb, char* p_pkeyName, char *p_pkeyValue);

 /**
 * @brief 在数据库的指定表p_ptbName中查找字段名为p_pkeyName的字段的值，并输出到p_pkeyValue中.
 *
 * @param[in] p_ptbName     数据库中表名.
 * @param[in] p_sqldb       数据库中数据库操作句柄.
 * @param[in] p_pkeyName    字段名称.
 * @param[out] p_pkeyValue  字段值.
 *
 * @return  查询数据的执行状态，0：表示执行成功，非0表示失败.
 */
int ConfDB_Tab_SelectInt(char* p_ptbName, sqlite3 *p_sqldb, char* p_pkeyName, int *p_pkeyValue);
 
/**
 * @brief 在数据库的指定表p_ptbName中查找字段名为p_pkeyName的字段的值，并输出到p_pkeyValue中.
 *
 * @param[in] p_ptbName     数据库中表名.
 * @param[in] p_sqldb       数据库中数据库操作句柄.
 * @param[in] p_pkeyName    字段名称.
 * @param[out] p_pkeyValue  字段值.
 *
 * @return  查询数据的执行状态，0：表示执行成功，非0表示失败.
 */
int ConfDB_Tab_SelectString(char* p_ptbName, sqlite3 *p_sqldb, char* p_pkeyName, char *p_pkeyValue);

/**
 * @brief 在数据库的指定表中查找数据ID = nID的所有记录.
 *
 * @param[in] p_ptbName     数据库中表名.
 * @param[in] p_sqldb       数据库中数据库操作句柄.
 * @param[in] nID           目标ID.
 * @param[out] p_pKeyItem   存储查询结果的结构体.
 *
 * @return  查询数据的执行状态，0：表示执行成功，非0表示失败.
 */
int ConfDB_Tab_SelectAll(char* p_ptbName, sqlite3 *p_sqldb, int nID, ITEM *p_pKeyItem);

/**
 * @brief 在数据库的指定表中将字段p_pkeyName中值为p_pkeyName的元素更新为p_nkeyNewValue.
 * @param[in] p_ptbName         数据库中表名.
 * @param[in] p_sqldb           数据库中数据库操作句柄.
 * @param[in] p_pkeyName        元素旧值（数值）.
 * @param[in] p_nkeyNewValue    元素新值（数值）.
 * @return  更新数据的成功状态，0：表示插入成功，非0表示失败.
 */
int ConfDB_Tab_UpdateInt(char* p_ptbName, sqlite3 *p_sqldb, char* p_pkeyName, int p_nkeyNewValue);

/**
 * @brief 在数据库的指定表中将字段p_pkeyName中值为p_pkeyName的元素更新为p_pkeyNewValue..
 * @param[in] p_ptbName         数据库中表名.
 * @param[in] p_sqldb           数据库中数据库操作句柄.
 * @param[in] p_pkeyName        元素旧值（字符串）.
 * @param[in] p_nkeyNewValue    元素新值（字符串）.
 * @return  更新数据的成功状态，0：表示插入成功，非0表示失败.
 */
int ConfDB_Tab_UpdateString(char* p_ptbName, sqlite3 *p_sqldb, char* p_pkeyName, char *p_pkeyNewValue);

#endif//#ifndef __CONF_DB_H_

/**
 * @}
 */
