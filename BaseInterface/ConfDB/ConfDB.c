#include "ConfDB.h"

/**
 * 函数名称：sqlite3 * ConfDB_OpenDB(char* p_pdbName, int nCreateMode)
 * 函数功能：创建或者打开接收数据库
 * 参数说明：
 *	输入参数：char* p_pdbName 数据库文件名称 例如：a.db
 * 		      int nCreateMode 数据库打开模式，0：打开时不做任何操作，1：打开时清空删除数据库当前所有表
 * 返回值：sqlite3 *p_sqldb 数据库的操作句柄，
 * 函数返回值：数据库创建成败状态
 * */
sqlite3 *ConfDB_OpenDB(char* p_pdbName, int nCreateMode)
{
	sqlite3 *l_sqldb;
	int rc;
	rc = sqlite3_open(p_pdbName, &l_sqldb);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(l_sqldb));
		sqlite3_close(l_sqldb);
	} else {
		//db = 0;
		sqlite3_exec(l_sqldb, "PRAGMA synchronous = OFF; ", 0, 0, 0);
		if(nCreateMode == 1)
		{
//			//删除发送表
//			sqlite3_exec(l_sqldb, "drop table if exists TxData", 0, 0, 0);
//			//删除接收表
//			sqlite3_exec(l_sqldb, "drop table if exists RxData", 0, 0, 0);
//			//删除接收RSA表
//			sqlite3_exec(l_sqldb, "drop table if exists RSAData", 0, 0, 0);
//			//删除信息统计表
//			sqlite3_exec(l_sqldb, "drop table if exists StatisData", 0, 0, 0);
		}
		else
		{

		}
	}

	return l_sqldb;
}

/**
 * 函数名称：sqlite3 * ConfDB_CloseDB(char* p_pdbName)
 * 函数功能：关闭数据库
 * 参数说明：sqlite3 *p_sqldb 数据库的操作句柄
 * 返回值：执行状态，0 执行成功，非0 执行失败
 * 函数返回值：数据库关闭成败状态
 * */
int ConfDB_CloseDB(sqlite3 * p_sqldb)
{
	sqlite3_close(p_sqldb);
	return 0;
}
/**
 * 函数名称：int ConfDB_IsTabExists(char* p_ptbName, sqlite3 *p_sqldb)
 * 函数功能：在句柄db的数据库中查找是否存在名为p_ptbName的表
 * 参数说明：char* p_ptbName,数据表表名 sqlite3 *p_sqldb 数据库操作句柄
 * 函数返回值：-1 表示查询失败，0 表示查询成功但是没有此表，1 表示查询成功且有此表
 * */
int ConfDB_IsTabExists(char* p_ptbName, sqlite3 *p_sqldb)
{
	//select * from sqlite_master where type = 'table' and name = 't_cmpt_cp'
	int nRet = -1;
	int nrow = 0, ncolumn = 0;
	char **azResult;
	char *errmsg = 0;

	char sql[200];
	memset(sql, 0x00, 199);
	sprintf(sql, "select * from sqlite_master where type = 'table' and name = '%s';",p_ptbName);

	//从数据表查询数据
	nRet = sqlite3_get_table(p_sqldb, sql, &azResult, &nrow, &ncolumn, &errmsg);
#ifdef __DEBUG
	int i;
	printf("row:%d column=%d", nrow, ncolumn);
	printf("The result of querying is :");
	for (i = 0; i < (nrow + 1) * ncolumn; i++) {
		printf("azResult[%d] = %s \r\n", i, azResult[i]);
	}
#endif
	if (nRet != SQLITE_OK) {
#ifdef __DEBUG
		printf("fail \n");
#endif
		sqlite3_free(errmsg);
		nRet = -1;
	} else {
#ifdef __DEBUG
		printf("OK \n");
#endif
		if (nrow < 1) {
#ifdef __DEBUG
			printf("OK1 \n");
#endif
			nRet = 0;
		} else {
#ifdef __DEBUG
			printf("OK2 \n");
#endif
		}
		sqlite3_free_table(azResult);
	}
	return nRet;
}
/**
 * 函数名称：int ConfDB_CreatTab(char* p_ptbName, sqlite3 *p_sqldb)
 * 函数功能：在句柄db的数据库中创建名为p_ptbName的表
 * 参数说明：char* p_ptbName,数据表表名 sqlite3 *p_sqldb 数据库操作句柄
 * 函数返回值：数据库中，对表进行创建操作的执行状态，0表示成功，其他表示失败
 * */
int ConfDB_CreatTab(char* p_ptbName, sqlite3 *p_sqldb)
{
	ConfDB_DeleteTab(p_ptbName, p_sqldb);
	int nRet = -1;
	//char *zErrMsg = 0;
	char sql[200];
	memset(sql, 0x00, 200);
	sprintf(sql, "CREATE TABLE '%s'(ID INTEGER NOT NULL PRIMARY KEY,KeyName char(30),KeyValue char(30));",p_ptbName);
	//使用sql字符串指定的sql语言 创建数据表SensorData
	char* errmsg;
	nRet = sqlite3_exec(p_sqldb, sql, 0, 0, &errmsg);
	if (nRet != SQLITE_OK) {
		sqlite3_close(p_sqldb);
		printf("create table errmsg : %s\n", errmsg);
		sqlite3_free(errmsg);
		nRet = 1;
	} else {
		nRet = 0;
	}
	return nRet;
}
/**
 * 函数名称：int ConfDB_DeleteTab(char* p_ptbName, sqlite3 *p_sqldb)
 * 函数功能：在句柄db的数据库中删除表名为p_ptbName的表
 * 参数说明：char* p_ptbName,数据表表名 sqlite3 *p_sqldb 数据库操作句柄
 * 函数返回值：数据库中，对表进行删除操作的执行状态，0表示成功，其他表示失败
 * */
int ConfDB_DeleteTab(char* p_ptbName, sqlite3 *p_sqldb)
{
	char sql[200];
	memset(sql, 0x00, 200);
	sprintf(sql, "drop table if exists %s;",p_ptbName);
		//删除发送表
	sqlite3_exec(p_sqldb, sql, 0, 0, 0);
	return 0;
}
/**
 * 函数名称：int ConfDB_ClearTab(char* p_ptbName, sqlite3 *p_sqldb)
 * 函数功能：在句柄db的数据库中清空表名为p_ptbName的表
 * 参数说明：char* p_ptbName,数据表表名 sqlite3 *p_sqldb 数据库操作句柄
 * 函数返回值：数据库中，对表进行清空操作的执行状态，0表示成功，其他表示失败
 * */
int ConfDB_ClearTab(char* p_ptbName, sqlite3 *p_sqldb)
{
	return 0;
}

/**
 * 函数名称：int ConfDB_Tab_Insert(char* p_ptbName, sqlite3 *p_sqldb, char * Data)
 * 函数功能：向数据库的指定表中插入数据
 * 参数说明：char* p_ptbName 数据库中表名, sqlite3 *p_sqldb 数据库中数据库操作句柄, char * Data 数据指针
 * 函数返回值：插入数据的成功状态，0：表示执行成功，非0表示失败
 * */
int ConfDB_Tab_Insert(char* p_ptbName, sqlite3 *p_sqldb, ITEM pConfItem)
{
	int nRet = 0;
	char sql[200];
	memset(sql, 0x00, 200);
	sqlite3_stmt *stmt;
	sprintf(sql,"INSERT INTO '%s' VALUES(?, ?, ?);",p_ptbName);

	sqlite3_exec(p_sqldb, "BEGIN TRANSACTION;", 0, 0, 0);
	sqlite3_prepare_v2(p_sqldb, sql, strlen(sql), &stmt, 0);

	int j = 1;
	sqlite3_reset(stmt);
	sqlite3_bind_null(stmt, j++);
	sqlite3_bind_text(stmt, j++, pConfItem.keyName, strlen(pConfItem.keyName), 0);
	sqlite3_bind_text(stmt, j++, pConfItem.keyValue, strlen(pConfItem.keyValue), 0);
	sqlite3_step(stmt);

	sqlite3_finalize(stmt);
	sqlite3_exec(p_sqldb, "COMMIT TRANSACTION;", 0, 0, 0);
	return nRet;
}
/**
 * 函数名称：int ConfDB_Tab_Delete(char* p_ptbName, sqlite3 *p_sqldb, char* p_pkeyName, char *p_pkeyValue)
 * 函数功能：在数据库的指定表中，删除字段名为p_pkeyName的字段中，字段值为p_pkeyValue的信息
 * 参数说明：char* p_ptbName 数据库中表名, sqlite3 *p_sqldb 数据库中数据库操作句柄, char* p_pkeyName 字段名称, char *p_pkeyValue 字段值
 * 函数返回值：删除数据的执行状态，0：表示执行成功，非0表示失败
 * */
int ConfDB_Tab_Delete(char* p_ptbName, sqlite3 *p_sqldb, char* p_pkeyName, char *p_pkeyValue)
{
	return 0;
}

/**
 * 函数名称：int ConfDB_Tab_SelectInt(char* p_ptbName, sqlite3 *p_sqldb, char* p_pkeyName, int *p_pkeyValue)
 * 函数功能：在数据库的指定表中查找字段名为KeyName的字段中，字段值为p_pkeyName的这条信息中，获取KeyValue的值
 * 参数说明：char* p_ptbName 数据库中表名, sqlite3 *p_sqldb 数据库中数据库操作句柄, char* p_pkeyName 字段名称, 查询到的char *p_pkeyValue 字段值
 * 函数返回值：查询数据的执行状态，0：表示执行成功，非0表示失败
 * */
int ConfDB_Tab_SelectInt(char* p_ptbName, sqlite3 *p_sqldb, char* p_pkeyName, int *p_pkeyValue)
{
	int nRet = -1;
	int nrow = 0, ncolumn = 0;
	char **azResult;
	char *errmsg = 0;

	char sql[200];
	memset(sql, 0x00, 199);
	sprintf(sql, "SELECT * FROM '%s' where KeyName = '%s'", p_ptbName, p_pkeyName);

	//从数据表查询数据
	nRet = sqlite3_get_table(p_sqldb, sql, &azResult, &nrow, &ncolumn, &errmsg);
#ifdef __DEBUG
	int i;
	printf("row:%d column=%d", nrow, ncolumn);
	printf("The result of querying is :");
	for (i = 0; i < (nrow + 1) * ncolumn; i++) {
		printf("azResult[%d] = %s \r\n", i, azResult[i]);
	}
#endif
	if (nRet != SQLITE_OK) {
#ifdef __DEBUG
		printf("fail \n");
#endif
		sqlite3_free(errmsg);
		nRet = 1;
	} else {
#ifdef __DEBUG
		printf("OK \n");
#endif
		if (nrow < 1) {
#ifdef __DEBUG
			printf("OK1 \n");
#endif
			nRet = -1;
		} else {
#ifdef __DEBUG
			printf("OK2 \n");
#endif
			nRet = 0;
			int nIndex = ncolumn + 1;

			nIndex ++;
			if(strlen(azResult[nIndex]) != 0)
				*p_pkeyValue = atoi(azResult[nIndex++]);
			else
				*p_pkeyValue = 0;

			sqlite3_free_table(azResult);
		}
	}
	return nRet;
}
/**
 * 函数名称：int ConfDB_Tab_SelectString(char* p_ptbName, sqlite3 *p_sqldb, char* p_pkeyName, char *p_pkeyValue)
 * 函数功能：在数据库的指定表中查找字段名为KeyName的字段中，字段值为p_pkeyName的这条信息中，获取KeyValue的值
 * 参数说明：char* p_ptbName 数据库中表名, sqlite3 *p_sqldb 数据库中数据库操作句柄, char* p_pkeyName 字段名称, 查询到的char *p_pkeyValue 字段值
 * 函数返回值：查询数据的执行状态，0：表示执行成功，非0表示失败
 * */
int ConfDB_Tab_SelectString(char* p_ptbName, sqlite3 *p_sqldb, char* p_pkeyName, char *p_pkeyValue)
{
	int nRet = -1;
	int nrow = 0, ncolumn = 0;
	char **azResult;
	char *errmsg = 0;

	char sql[200];
	memset(sql, 0x00, 199);
	sprintf(sql, "SELECT * FROM '%s' where KeyName = '%s'", p_ptbName, p_pkeyName);

	printf("%s\n",sql);
	//从数据表查询数据
	nRet = sqlite3_get_table(p_sqldb, sql, &azResult, &nrow, &ncolumn, &errmsg);
#ifdef __DEBUG
	int i;
	printf("row:%d column=%d", nrow, ncolumn);
	printf("The result of querying is :");
	for (i = 0; i < (nrow + 1) * ncolumn; i++) {
		printf("azResult[%d] = %s \r\n", i, azResult[i]);
	}
#endif
	if (nRet != SQLITE_OK) {
#ifdef __DEBUG
		//sqlite3_close(p_sqldb);
		printf("fail \n");
#endif
		sqlite3_free(errmsg);
		nRet = 1;
	} else {
#ifdef __DEBUG
		printf("OK \n");
#endif
		if (nrow < 1) {

#ifdef __DEBUG
			printf("OK1 \n");
#endif
			nRet = -1;
		} else {
#ifdef __DEBUG
			printf("OK2 \n");
#endif
			nRet = 0;
			int nIndex = ncolumn + 1;

			nIndex ++;
			memcpy(p_pkeyValue, azResult[nIndex], strlen(azResult[nIndex]));

			sqlite3_free_table(azResult);
		}
	}
	return nRet;
}
//#define __DEBUG 1
/**
 * 函数名称：int ConfDB_Tab_Select(char* p_ptbName, sqlite3 *p_sqldb, char* p_pkeyName, char *p_pkeyValue, char * p_pData)
 * 函数功能：在数据库的指定表中查找字段名为p_pkeyName的字段中，字段值为p_pkeyValue的信息
 * 参数说明：char* p_ptbName 数据库中表名, sqlite3 *p_sqldb 数据库中数据库操作句柄, char* p_pkeyName 字段名称, char *p_pkeyValue 字段值, char * p_pData 查询到的数据指针
 * 函数返回值：查询数据的执行状态，0：表示执行成功，非0表示失败
 * */
int ConfDB_Tab_SelectAll(char* p_ptbName, sqlite3 *p_sqldb, int nID, ITEM *p_pKeyItem)
{
	int nRet = -1;
	int nrow = 0, ncolumn = 0;
	char **azResult;
	char *errmsg = 0;

	int nIndex = 0;
	char sql[200];
	memset(sql, 0x00, 199);
	sprintf(sql, "SELECT * FROM '%s' where ID = %d", p_ptbName, nID);

//	printf("%s\n",sql);
	//从数据表查询数据
	nRet = sqlite3_get_table(p_sqldb, sql, &azResult, &nrow, &ncolumn, &errmsg);
#ifdef __DEBUG
	int i;
	printf("row:%d column=%d", nrow, ncolumn);
	printf("The result of querying is :");
	for (i = 0; i < (nrow + 1) * ncolumn; i++) {
		printf("azResult[%d] = %s \r\n", i, azResult[i]);
	}
#endif
	if (nRet != SQLITE_OK) {
#ifdef __DEBUG
		//sqlite3_close(p_sqldb);
		printf("fail \n");
#endif
		sqlite3_free(errmsg);
		nRet = 1;
	} else {
#ifdef __DEBUG
		printf("OK \n");
#endif
		if (nrow < 1) {

#ifdef __DEBUG
			printf("OK1 \n");
#endif
			nRet = -1;
		} else {
#ifdef __DEBUG
			printf("OK2 \n");
#endif
			nRet = 0;
			nIndex = ncolumn + 1;

			sprintf(p_pKeyItem->keyName, "%s",azResult[nIndex ++]);
			sprintf(p_pKeyItem->keyValue, "%s",azResult[nIndex ++]);

			sqlite3_free_table(azResult);
		}
	}
	return nRet;
}

/**
 * 函数名称：int ConfDB_Tab_Update(char* p_ptbName, sqlite3 *p_sqldb, char* p_pkeyName, char *p_pkeyOldValue, char *p_pkeyNewValue)
 * 函数功能：在数据库的指定表中将字段p_pkeyName中值为p_pkeyOldValue的元素更新为p_pkeyNewValue
 * 参数说明：char* p_ptbName 数据库中表名, sqlite3 *p_sqldb 数据库中数据库操作句柄, STCRecvData p_stcRecvData 接收数据的结构体
 * 函数返回值：插入数据的成功状态，0：表示插入成功，非0表示失败
 * */
int ConfDB_Tab_UpdateInt(char* p_ptbName, sqlite3 *p_sqldb, char* p_pkeyName, int p_nkeyNewValue)
{
	int nRet = -1;
	int nIsChange = 0;
	char sql[200];
	memset(sql, 0x00, 200);

	if(ConfDB_IsTabExists("Tab_ConfCtrl",p_sqldb) == 0)//表存在
	{
		nRet = ConfDB_Tab_SelectInt("Tab_ConfCtrl",p_sqldb, p_pkeyName, &nIsChange);
		if(nIsChange == 1)//字段可修改
		{
			sprintf(sql,"UPDATE '%s' SET KeyValue = '%d' where KeyName = '%s';",p_ptbName,p_nkeyNewValue,p_pkeyName);
			nRet = sqlite3_exec(p_sqldb, sql, 0, 0, 0);
		}
		else
		{
			nRet = -2;
		}
	}
	else
	{
		nRet = -1;
	}
	return nRet;
}
/**
 * 函数名称：int ConfDB_Tab_Update(char* p_ptbName, sqlite3 *p_sqldb, char* p_pkeyName, char *p_pkeyOldValue, char *p_pkeyNewValue)
 * 函数功能：在数据库的指定表中将字段p_pkeyName中值为p_pkeyOldValue的元素更新为p_pkeyNewValue
 * 参数说明：char* p_ptbName 数据库中表名, sqlite3 *p_sqldb 数据库中数据库操作句柄, STCRecvData p_stcRecvData 接收数据的结构体
 * 函数返回值：插入数据的成功状态，0：表示插入成功，非0表示失败
 * */
int ConfDB_Tab_UpdateString(char* p_ptbName, sqlite3 *p_sqldb, char* p_pkeyName, char *p_pkeyNewValue)
{
	int nRet = -1;
	int nIsChange = 0;
	char sql[200];
	memset(sql, 0x00, 200);

	if(ConfDB_IsTabExists("Tab_ConfCtrl",p_sqldb) == 0)//表存在
	{
		nRet = ConfDB_Tab_SelectInt("Tab_ConfCtrl",p_sqldb, p_pkeyName, &nIsChange);
		if(nIsChange == 1)//字段可修改
		{
			sprintf(sql,"UPDATE '%s' SET KeyValue = '%s' where KeyName = '%s';",p_ptbName,p_pkeyNewValue,p_pkeyName);
			nRet = sqlite3_exec(p_sqldb, sql, 0, 0, 0);
		}
		else
		{
			nRet = -2;
		}
	}
	else
	{
		nRet = -1;
	}
	return nRet;
}

