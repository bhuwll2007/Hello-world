#include "DataBase.h"
#define IS_SAVE_ALLRECVDATA 1
uint64_t Str2Uint64(char *buf, int bufLen)
{
	uint64_t Ret = 0;
	int i = 0;
	for (i = 0; i < bufLen - 1; i++)
	{
		Ret = Ret + (buf[i] - '0');
		Ret = Ret * 10;
	}
	Ret = Ret + (buf[i] - '0');
	return Ret;
}
/**
 * 获取当前时间的时间戳
 *
 *
 */
int GetCurTimeStamp(uint64_t *lTimeStamp, char* YMDHMS)
{
	time_t at_time = time(NULL);
	struct tm ptr;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	localtime_r(&at_time, &ptr);
	strftime(YMDHMS, 22, "%Y-%m-%d %H:%M:%S", &ptr);

	*lTimeStamp = (uint64_t) ((uint64_t) tv.tv_sec * 1000000
			+ (uint64_t) tv.tv_usec);

	return 0;
}
int sql_callback(void *para, int nCount, char **pValue, char **pName)
{
	printf("USE-----------?\n");
	return 0;
}

int sql_callback_listtable(void *para, int nCount, char **pValue, char **pName)
{
	struct Tbllist *Tbl = (struct Tbllist *) para;

	Tbl->TblStr[Tbl->cNbr] = calloc(1, strlen(pValue[0]));
	memcpy(Tbl->TblStr[Tbl->cNbr], pValue[0], strlen(pValue[0]));
	Tbl->cNbr++;

	return 0;
}

int sql_callback_tableNbr(void *para, int nCount, char **pValue, char **pName)
{
	*(int *) para = atoi(pValue[0]);
	return 0;
}

/**********************************************************************
 * 创建或者打开接收数据库
 * 函数名称：sqlite3 * DataBase_OpenDB(char *p_pdbName, int nCreateMode)
 * 函数功能：创建或者打开接收数据库
 * 参数说明：cahr *p_pdbName 数据库文件名称 例如：a.db
 *          int nCreateMode 数据库打开模式，0：打开时不做任何操作，1：打开是清空删除数据库当前所有表
 * 返回值：sqlite3 *db 数据库的操作句柄
 * 函数返回值：数据库创建成败状态
 * *******************************************************************/
sqlite3 *DataBase_OpenDB(char *p_pdbName, int nCreateMode)
{
	sqlite3 *sql_db;
	char *errmsg = NULL;
	int ret = 0;

	ret = sqlite3_open(p_pdbName, &sql_db);
	if (ret != 0)
	{
//		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(l_sqldb));
		printf("Can't open database: %s\n", sqlite3_errmsg(sql_db));
		sqlite3_close(sql_db);
		sql_db = NULL;
	}
	else
	{
		//若将 synchronous 设为 OFF (0)，那么 SQLite 从操作系统取得数据后将会立即进行处理而不会暂停。
		//如果使用 SQLite 的应用程序崩溃，数据将是安全的。 但如果在数据写入磁盘之前，操作系统死机或机器断电，
		//数据库文件可能会损坏。此外，在该模式下， 某些操作会比其它情况下快 50 倍
		//PRAGMA详细说明:https://blog.csdn.net/u013704635/article/details/80590048
		sqlite3_exec(sql_db, "PRAGMA synchronous = OFF", NULL, NULL, &errmsg);
		sqlite3_free(errmsg);
	}
	return sql_db;
}

/***********************************
 * 关闭数据库
 * 函数名称：int DataBase_CloseDB(sqlite3 *db)
 * 函数功能：关闭数据库
 * 参数说明：sqlite3 *db 数据库的操作句柄
 * 返回值：执行状态，0执行成功，非0执行失败
 * 函数返回值：数据库关闭成败状态
 * ********************************/
int DataBase_CloseDB(sqlite3 *p_sqldb)
{
	int ret = 0;

	if (p_sqldb != NULL)
	{
		sqlite3_close(p_sqldb);
	}
	else
	{
		printf("Invalid sqlite3 pointer!\n");
		ret = 1;
	}

	return ret;
}

/***********************************
 * 数据库表的创建
 * 函数名称：int DataBase_CreateTab(char *p_ptbName, sqlite3 *db)
 * 函数功能：在句柄db的数据库中创建名为p_ptbName的表
 * 参数说明：char *p_ptbName 数据表名; sqlite3 *db 数据库操作句柄
 * 函数返回值：数据库中，对表进行创建操作的执行状态，0表示成功，其他表示失败
 * ********************************/
int DataBase_CreatTab(char *p_ptbName, sqlite3 *db)
{
	int ret = 0;
	char *errmsg = NULL;
	struct Tbllist Tbl;
	int i = 0;

	char *sql = calloc(5120, sizeof(char));

	sqlite3_exec(db,
			"SELECT COUNT(*) FROM sqlite_master WHERE type ='table' ORDER BY name;",
			sql_callback_tableNbr, &Tbl.nNbr, &errmsg);

	Tbl.TblStr = calloc(Tbl.nNbr, sizeof(char *));
	Tbl.cNbr = 0;
	sqlite3_exec(db,
			"SELECT name FROM sqlite_master WHERE type ='table' ORDER BY name;",
			sql_callback_listtable, &Tbl, &errmsg);

	for (i = 0; i < Tbl.nNbr; i++)
	{
		if (strcmp(Tbl.TblStr[i], p_ptbName) == 0)
		{
			// 若文件已存在则返回1
			ret = 1;
			goto EXIT;
		}
	}

	if (sql == NULL)
	{
		printf("Sql command string malloc failed!\n");
		ret = 1;
	}
	else
	{
		if (strcmp("TxData", p_ptbName) == 0
				|| strcmp("RxData", p_ptbName) == 0)
			sprintf(sql,
					"CREATE TABLE '%s'(KeyID INTEGER NOT NULL PRIMARY KEY,msgCnt INTEGER,"
							"MsgType INTEGER,DataTime INTEGER,BufCon BLOB);",
					p_ptbName);
		else if (strcmp("BSMData", p_ptbName) == 0)
		{
			sprintf(sql,
					"CREATE TABLE '%s'(ID INTEGER PRIMARY KEY AUTOINCREMENT,TIME_T VARCHAR(32),TIME_I INT64,"
					//HV
							"HVID VARCHAR(10),HVLongitude FLOAT,HVLatitude FLOAT,HVAltitude FLOAT,HVSpeed FLOAT,"
							"HVHeading FLOAT,HVSteeringWheelAngle FLOAT,HVBreakPadel INTEGER,HVAuxBreakSystem INTEGER,"
							"HVLight INTEGER,HVVehicleWidth FLOAT,HVVehicleLength FLOAT,HVVehicleHight FLOAT,"
							"HVVehicleWheelDis FLOAT,HVVehicleClass INTEGER,HVForwardNode INTEGER,HVBackNode INTEGER,"
							//RV
							"RVID VARCHAR(10),RVLongitude FLOAT,RVLatitude FLOAT,RVAltitude FLOAT,RVSpeed FLOAT,"
							"RVHeading FLOAT,RVSteeringWheelAngle FLOAT,RVBreakPadel INTEGER,RVAuxBreakSystem INTEGER,"
							"RVLight INTEGER,RVVehicleWidth FLOAT,RVVehicleLength FLOAT,RVVehicleHight FLOAT,"
							"RVVehicleWheelDis FLOAT,RVVehicleClass INTEGER,RVForwardNode INTEGER,RVBackNode INTEGER,"
							//Result
							"wType INTEGER,wStr INTEGER,wPrio INTEGER,wLvl INTEGER,TTC FLOAT,Distance FLOAT,"
							"RelBearing FLOAT,Location INTEGER);", p_ptbName);
		}
		else if (strcmp("RSIData", p_ptbName) == 0)
		{
			sprintf(sql,
					"CREATE TABLE '%s'(ID INTEGER PRIMARY KEY AUTOINCREMENT,TIME_T VARCHAR(32),TIME_I INT64,"
					//VehData
							"HVID VARCHAR(10),HVLongitude FLOAT,HVLatitude FLOAT,HVAltitude FLOAT,HVSpeed FLOAT,"
							"HVHeading FLOAT,HVSteeringWheelAngle FLOAT,HVBreakPadel INTEGER,HVAuxBreakSystem INTEGER,"
							"HVLight INTEGER,HVVehicleWidth FLOAT,HVVehicleLength FLOAT,HVVehicleHight FLOAT,"
							"HVVehicleWheelDis FLOAT,HVVehicleClass INTEGER,HVForwardNode INTEGER,HVBackNode INTEGER,"
							//RSI Info
							"RSUID INTEGER,RSIMsgID INTEGER,MsgType INTEGER,Priority INTEGER,Description VARCHAR(128),"
							"AlertRadius FLOAT,Latitude FLOAT,Longitude FLOAT,Altitude FLOAT,PathNbr INTEGER,"
							"Path1_Lat FLOAT,Path1_Lng FLOAT,Path1_Alt FLOAT,Path2_Lat FLOAT,Path2_Lng FLOAT,"
							"Path2_Alt FLOAT,Path3_Lat FLOAT,Path3_Lng FLOAT,Path3_Alt FLOAT,Path4_Lat FLOAT,"
							"Path4_Lng FLOAT,Path4_Alt FLOAT,Path5_Lat FLOAT,Path5_Lng FLOAT,Path5_Alt FLOAT,"
							"Path6_Lat FLOAT,Path6_Lng FLOAT,Path6_Alt FLOAT,Path7_Lat FLOAT,Path7_Lng FLOAT,"
							"Path7_Alt FLOAT,Path8_Lat FLOAT,Path8_Lng FLOAT,Path8_Alt FLOAT,"
							//Rsi Result Info
							"wType INTEGER,wStr INTEGER,wPrio INTEGER,WarningType INTEGER,resMsgType INTEGER,"
							"PosLat FLOAT,PosLng FLOAT,PosAlt FLOAT,AlertPathPosLat0 FLOAT,AlertPathPosLng0 FLOAT,"
							"AlertPathPosAlt0 FLOAT,AlertPathPosLat1 FLOAT,AlertPathPosLng1 FLOAT,"
							"AlertPathPosAlt1 FLOAT,dis FLOAT,SpeakDis INTEGER,RelPos INTEGER,"
							"PathLen FLOAT,DscpTagVal FLOAT,DscpAssisTag INTEGER,DscpAssisVal FLOAT);",
					p_ptbName);
		}
		else if (strcmp("RSMData", p_ptbName) == 0)
		{
			sprintf(sql,
					"CREATE TABLE '%s'(ID INTEGER PRIMARY KEY AUTOINCREMENT,"

					, p_ptbName);
		}
		else if (strcmp("SPATData", p_ptbName) == 0)
		{
			sprintf(sql,
					"CREATE TABLE '%s'(ID INTEGER PRIMARY KEY AUTOINCREMENT,TIME_T VARCHAR(32),TIME_I INT64,"
					//VehData
							"HVID VARCHAR(10),HVLongitude FLOAT,HVLatitude FLOAT,HVAltitude FLOAT,HVSpeed FLOAT,"
							"HVHeading FLOAT,HVSteeringWheelAngle FLOAT,HVBreakPadel INTEGER,HVAuxBreakSystem INTEGER,"
							"HVLight INTEGER,HVVehicleWidth FLOAT,HVVehicleLength FLOAT,HVVehicleHight FLOAT,"
							"HVVehicleWheelDis FLOAT,HVVehicleClass INTEGER,HVForwardNode INTEGER,HVBackNode INTEGER,"
							//SpeedGudie_Input
							"SGLat FLOAT,SGLng FLOAT,SGPhaseCnt INTEGER,"
							"SGID1 INTEGER,SGDirection1 INTEGER,SGGreenTime1 INTEGER,"
							"SGYellowTime1 INTEGER,SGRedTime1 INTEGER,SGStatus1 INTEGER,SGTimeLeft1 INTEGER,"
							"SGID2 INTEGER,SGDirection2 INTEGER,SGGreenTime2 INTEGER,SGYellowTime2 INTEGER,"
							"SGRedTime2 INTEGER,SGStatus2 INTEGER,SGTimeLeft2 INTEGER,"
							"SGID3 INTEGER,SGDirection3 INTEGER,SGGreenTime3 INTEGER,SGYellowTime3 INTEGER,"
							"SGRedTime3 INTEGER,SGStatus3 INTEGER,SGTimeLeft3 INTEGER,"
							"SGID4 INTEGER,SGDirection4 INTEGER,SGGreenTime4 INTEGER,SGYellowTime4 INTEGER,"
							"SGRedTime4 INTEGER,SGStatus4 INTEGER,SGTimeLeft4 INTEGER,"
							"SGID5 INTEGER,SGDirection5 INTEGER,SGGreenTime5 INTEGER,SGYellowTime5 INTEGER,"
							"SGRedTime5 INTEGER,SGStatus5 INTEGER,SGTimeLeft5 INTEGER,"
							//Result_Spat
							"wType INTEGER,wStr INTEGER,wPrio INTEGER,Cycle FLOAT,Distance FLOAT,RelBearing FLOAT,PhaseCnt INTEGER,"
							"Status1 INTEGER,TimeLeft1 INTEGER,Direction1 INTEGER,SuggestSpeed_High1 FLOAT,"
							"SuggestSpeed_Low1 FLOAT,SuggestSpeed1 FLOAT,"
							"Status2 INTEGER,TimeLeft2 INTEGER,Direction2 INTEGER,SuggestSpeed_High2 FLOAT,"
							"SuggestSpeed_Low2 FLOAT,SuggestSpeed2 FLOAT,"
							"Status3 INTEGER,TimeLeft3 INTEGER,Direction3 INTEGER,SuggestSpeed_High3 FLOAT,"
							"SuggestSpeed_Low3 FLOAT,SuggestSpeed3 FLOAT,"
							"Status4 INTEGER,TimeLeft4 INTEGER,Direction4 INTEGER,SuggestSpeed_High4 FLOAT,"
							"SuggestSpeed_Low4 FLOAT,SuggestSpeed4 FLOAT,"
							"Status5 INTEGER,TimeLeft5 INTEGER,Direction5 INTEGER,SuggestSpeed_High5 FLOAT,"
							"SuggestSpeed_Low5 FLOAT,SuggestSpeed5 FLOAT);",
					p_ptbName);
		}
		else if (strcmp("YTRsuData", p_ptbName) == 0)
		{
			sprintf(sql,
					"CREATE TABLE '%s'(ID INTEGER PRIMARY KEY AUTOINCREMENT,TIME_T VARCHAR(32),TIME_I INT64,"
					"ModemStatus INTEGER, Lmi40IpAddr VARCHAR(20),"
					"Lmi40RxBytes INT64,Lmi40TxBytes INT64,"
#if IS_SAVE_ALLRECVDATA
							"Recvbytes INTEGER,RecvPags INTEGER,"
							"SendPags INTEGER,SendBytes INTEGER,"
#endif
							"AddRecvbytes INTEGER,AddRecvPags INTEGER,"
							"AddSendPags INTEGER,AddSendBytes INTEGER);",
					p_ptbName);
		}
		
//		printf("%s\n",sql);
		ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
		if (ret != 0)
		{
			printf("Table create failed: %s", errmsg);
		}
	}

	EXIT: sqlite3_free(errmsg);
	free(sql);
	return ret;
}

/***********************************
 * 数据库表的删除
 * 函数名称：int DataBase_DeleteTab(char *p_ptbName, sqlite3 *db)
 * 函数功能：在句柄db的数据库中删除名为p_ptbName的表
 * 参数说明：char *p_ptbName 数据表名; sqlite3 *db 数据库操作句柄
 * 函数返回值：数据库中，对表进行创建操作的执行状态，0表示成功，其他表示失败
 * ********************************/
int DataBase_DeleteTab(char *p_ptbName, sqlite3 *db)
{
	int ret = 0;
	char *errmsg = NULL;

	char *sql = calloc(512, sizeof(char));
	if (sql == NULL)
	{
		printf("Sql command string malloc failed!\n");
		ret = 1;
	}
	else
	{
		sprintf(sql, "DROP TABLE %s", p_ptbName);
		ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
		if (ret != 0)
		{
			printf("Table drop failed: %s", errmsg);

		}
	}
	sqlite3_free(errmsg);
	free(sql);

	return ret;
}

/***********************************
 * 数据库表的清空
 * 函数名称：int DataBase_ClearTab(char *p_ptbName, sqlite3 *db)
 * 函数功能：在句柄db的数据库中清空名为p_ptbName的表
 * 参数说明：char *p_ptbName 数据表名; sqlite3 *db 数据库操作句柄
 * 函数返回值：数据库中，对表进行创建操作的执行状态，0表示成功，其他表示失败
 * ********************************/
int DataBase_ClearTab(char *p_ptbName, sqlite3 *db)
{
	int ret = 0;
	char *errmsg = NULL;

	char *sql = calloc(512, sizeof(char));
	if (sql == NULL)
	{
		printf("Sql command string malloc failed!\n");
		ret = 1;
	}
	else
	{
		sprintf(sql, "DELETE FROM %s", p_ptbName);
		ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
		if (ret != 0)
		{
			printf("Table clear failed: %s", errmsg);
		}
		else
		{
			sprintf(sql, "UPDATE sqlite_sequence SET seq = 0 WHERE NAME ='%s",
					p_ptbName);
			ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
			if (ret != 0)
			{
//				printf("Id clear failed: %s", errmsg);
			}
		}
	}
	sqlite3_free(errmsg);
	free(sql);

	return ret;
}

/***********************************
 * 数据库表的插入操作
 * 函数名称：int DataBase_Tab_Insert(char *p_ptbName, sqlite3 *db, char *Data)
 * 函数功能：向数据库的指定表中插入数据
 * 参数说明：char *p_ptbName 数据表名; sqlite3 *db 数据库操作句柄; char *Data 数据指针
 * 函数返回值：数据库中，对表进行创建操作的执行状态，0表示成功，其他表示失败
 * ********************************/
int DataBase_Tab_Insert(char *p_ptbName, sqlite3 *db, struct TxRxTblData Data[],
		int NbrData)
{
	int ret = 0;
	int i = 0, j = 0;
	struct TxRxTblData *data = NULL;
	sqlite3_stmt *stmt;

	char *sql = calloc(512, sizeof(char));
	if (sql == NULL)
	{
		printf("Sql command string malloc failed!\n");
		ret = 1;
	}
	else
	{
		sprintf(sql, "INSERT INTO '%s' VALUES(?, ?, ?, ?, ?);", p_ptbName);
		sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, 0);
		sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, 0);

		for (i = 0; i < NbrData; i++)
		{
			j = 1;
			data = &Data[i];
			sqlite3_reset(stmt);
			sqlite3_bind_null(stmt, j++);
			sqlite3_bind_int(stmt, j++, data->msgCnt);
			sqlite3_bind_int(stmt, j++, data->MsgType);
			sqlite3_bind_int64(stmt, j++, data->DataTime);
			printf("data->DataTime = %llu\n", data->DataTime);
			sqlite3_bind_blob(stmt, j++, data->DataBuf, data->DataLen, 0);
			sqlite3_step(stmt);
		}
		sqlite3_finalize(stmt);
		sqlite3_exec(db, "COMMIT TRANSACTION;", 0, 0, 0);
		free(sql);
	}
	return ret;
}

/***********************************
 * 数据表的删除操作
 * 函数名称：int DataBase_Tab_Delete(char *p_ptbName, sqlite3 *db, char *p_pkeyName, char *p_pkeyValue)
 * 函数功能：在数据库的指定表中，删除字段名为p_pkeyName的字段中，字段值为p_pkeyValue的信息
 * 参数说明：char *p_ptbName 数据表名; sqlite3 *db 数据库操作句柄;
 *         char *p_pkeyName 字段名称; char *p_pkeyValue 字段值
 * 函数返回值：删除数据的执行状态，0表示成功，其他表示失败
 * ********************************/
int DataBase_Tab_Delete(char *p_ptbName, sqlite3 *db, char *p_pkeyName,
		char *p_pkeyValue)
{
	int ret = 0;
	char *errmsg = NULL;

	char *sql = calloc(512, sizeof(char));
	if (sql == NULL)
	{
		printf("Sql command string malloc failed!\n");
		ret = 1;
	}
	else
	{
		sprintf(sql, "DELETE FROM %s WHERE %s = %s;", p_ptbName, p_pkeyName,
				p_pkeyValue);
		ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
		if (ret != 0)
		{
			printf("Table delete failed: %s", errmsg);
		}
		else
		{

		}

	}
	sqlite3_free(errmsg);
	free(sql);

	return ret;
}

/***********************************
 * 数据表的查询操作
 * 函数名称：int DataBase_Tab_Select(char *p_ptbName, sqlite3 *db, char *p_pkeyName, char *p_pkeyValue, char *p_pData)
 * 函数功能：在数据库的指定表中，查找字段名为p_pkeyName的字段中，字段值为p_pkeyValue的信息
 * 参数说明：char *p_ptbName 数据表名; sqlite3 *db 数据库操作句柄;
 *         char *p_pkeyName 字段名称; char *p_pkeyValue 字段值; char *p_pData 查询到的数据指针
 * 函数返回值：查询数据的执行状态，0表示成功，其他表示失败
 * ********************************/
int DataBase_Tab_Select(char *p_ptbName, sqlite3 *db, char *p_pkeyName,
		char *p_pkeyValue, struct TxRxTblData p_pData[], int *NbrData,
		int limitNbr)
{
	int ret = 0;
	int cnt = 0;
	sqlite3_stmt *stmt;
	struct TxRxTblData *data = NULL;

	char *sql = calloc(512, sizeof(char));
	if (sql == NULL)
	{
		printf("Sql command string malloc failed!\n");
		ret = 1;
	}
	else
	{
		sprintf(sql, "SELECT * FROM %s WHERE %s = %s;", p_ptbName, p_pkeyName,
				p_pkeyValue);

		if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK)
		{
			while (sqlite3_step(stmt) == SQLITE_ROW)
			{
				if (limitNbr != -1)
				{
					if (cnt >= limitNbr)
					{
						break;
					}
				}
				data = &p_pData[cnt++];

				data->KeyID = sqlite3_column_int(stmt, 0);
				data->msgCnt = sqlite3_column_int(stmt, 1);
				data->MsgType = sqlite3_column_int(stmt, 2);
				data->DataTime = sqlite3_column_int64(stmt, 3);
				data->DataLen = sqlite3_column_bytes(stmt, 4);
				// 防止用户提供的存储空间不足
				data->DataBuf = realloc(data->DataBuf, data->DataLen);
				memcpy(data->DataBuf, sqlite3_column_blob(stmt, 4),
						data->DataLen);
			}
			*NbrData = cnt;
		}
		sqlite3_finalize(stmt);
	}
	free(sql);

	return ret;
}
/***********************************
 * 数据表的更新操作
 * 函数名称：int DataBase_Tab_Update(char *p_ptbName, sqlite3 *db, char *p_pkeyName, char *p_pkeyOldValue, char *p_pkeyNewValue)
 * 函数功能：在数据库的指定表中，将字段p_pkeyName中值为p_pkeyOldValue的元素更新为p_pkeyNewValue
 * 参数说明：char *p_ptbName 数据表名; sqlite3 *db 数据库操作句柄;
 *         char *p_pkeyName 字段名称; char *p_pkeyOldValue 替换前字段值; char *p_pkeyOldValue 替换后字段值
 * 函数返回值：插入数据的执行状态，0表示成功，其他表示失败
 * ********************************/
int DataBase_Tab_Update(char *p_ptbName, sqlite3 *db, char *p_pkeyName,
		char *p_pkeyOldValue, char *p_pkeyNewValue)
{
	int ret = 0;
	char *errmsg = NULL;

	char *sql = calloc(512, sizeof(char));
	if (sql == NULL)
	{
		printf("Sql command string malloc failed!\n");
		ret = 1;
	}
	else
	{
		sprintf(sql, "UPDATE %s SET %s = %s WHERE %s = %s;", p_ptbName,
				p_pkeyName, p_pkeyNewValue, p_pkeyName, p_pkeyOldValue);
		ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
		if (ret != 0)
		{
			printf("Table delete failed: %s", errmsg);
		}
		else
		{

		}
	}
	sqlite3_free(errmsg);
	free(sql);

	return ret;
}


int DataBase_Tab_SelectTabSize(char *p_ptbName, sqlite3 *db, int *TabInfoCount,
		int *TabInfoMaxId)
{
	int ret = 0;
	int cnt = 0;
	sqlite3_stmt *stmt;

	char *sql = calloc(512, sizeof(char));
	if (sql == NULL)
	{
		printf("Sql command string malloc failed!\n");
		ret = 1;
	} else
	{
		sprintf(sql, "SELECT Count(ID) FROM %s ;", p_ptbName);

		if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK)
		{
			if (sqlite3_step(stmt) == SQLITE_ROW)
			{
				cnt = sqlite3_column_int(stmt, 0);
			}
		}
		sqlite3_finalize(stmt);
		*TabInfoCount = cnt;

		sprintf(sql, "select MAX(ID) from '%s' ;", p_ptbName);

		if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK)
		{
			if (sqlite3_step(stmt) == SQLITE_ROW)
			{
				cnt = sqlite3_column_int(stmt, 0);
			}
		}
		sqlite3_finalize(stmt);

		if (cnt > 0)
		{
			*TabInfoMaxId = cnt;
		}

	}
	free(sql);

	return ret;
}

#define MAXSAVEROW 12000
int DataBase_Insert_YuTong(sqlite3 *db, tSPAT_INFO SpatInfo)
{
	int ret;
	char sql[1024];
	char *errmsg = NULL;
	char YMDHMS[100];
	uint64_t lTimeStamp = 0;
	GetCurTimeStamp(&lTimeStamp, YMDHMS);
	sprintf(sql, "INSERT INTO YTRsuData (ID,TIME_T,TIME_I,"
			"ModemStatus, Lmi40IpAddr,"
			"Lmi40RxBytes, Lmi40TxBytes,"
#if IS_SAVE_ALLRECVDATA
			"Recvbytes,RecvPags,"
			"SendPags,SendBytes,"
#endif
			"AddRecvbytes,AddRecvPags,"
			"AddSendPags,AddSendBytes) VALUES(NULL,'%s',%llu,"
			"%d,'%s',"
			"%llu,%llu,"
#if IS_SAVE_ALLRECVDATA
			"%d,%d,"
			"%d,%d,"
#endif
			"%d,%d,"
			"%d,%d);", YMDHMS, lTimeStamp,
			SpatInfo.nModemStatus,
			SpatInfo.acLmi40IpAddr,
			SpatInfo.ullLmi40RxBytes,
			SpatInfo.ullLmi40TxBytes,
#if IS_SAVE_ALLRECVDATA
			SpatInfo.nRecvbytes,
			SpatInfo.nRecvPags,
			SpatInfo.nSendPags,
			SpatInfo.nSendBytes,
#endif
			SpatInfo.nAddRecvbytes,
			SpatInfo.nAddRecvPags,
			SpatInfo.nAddSendPags,
			SpatInfo.nAddSendBytes);

	ret = sqlite3_exec(db, sql, 0, 0, &errmsg);
	if (ret != 0)
	{
		printf("Insert YTRsuData failed: %s", errmsg);
		printf("%s\n", sql);
		return 0;
	} else
	{
	}
	int TabInfoCount = 0;
	int TabInfoMaxId = 0;
	int nnn = DataBase_Tab_SelectTabSize("YTRsuData", db, &TabInfoCount,
			&TabInfoMaxId);
//	printf("nnn = %d\nTabInfoCount = %d\nTabInfoMaxId = %d\n", nnn,
//			TabInfoCount, TabInfoMaxId);
	if (TabInfoCount > MAXSAVEROW+50)
	{
		memset(sql, 0,sizeof(sql));
		sprintf(sql, "DELETE FROM YTRsuData WHERE ID < %d;",
				TabInfoMaxId - MAXSAVEROW);
		printf("sql = %s\n",sql);
		ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
		if (ret != 0)
		{
			printf("Table delete failed: %s", errmsg);
		} else
		{
			printf("Table delete OK\n");
		}
	}
	sqlite3_free(errmsg);

	return 0;
}