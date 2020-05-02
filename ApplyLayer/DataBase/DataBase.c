#include "DataBase.h"

int InitDataBase()
{
	char p_pdbName[64];
	int CreateDBIndex = 0;

	//获取当前操作的数据库序号
	GetConfigIntValue("./HostInfo.ini", "DataBase", "CreateDBIndex",
			&CreateDBIndex);
	if (CreateDBIndex == 0)
	{
		system("rm /Log/*.db");
		sprintf(p_pdbName, "/Log/V2X_DataBase.db");
	}
	else
	{
		sprintf(p_pdbName, "/Log/V2X_DataBase%d.db", CreateDBIndex);
	}

	//序号++，并写入配置文件中
	CreateDBIndex++;
	SetConfigIntValue("./HostInfo.ini", "DataBase", "CreateDBIndex",
			CreateDBIndex);

	//打开数据库
	sqldb = DataBase_OpenDB(p_pdbName, CreateDBIndex);  // 打开数据库
	if (sqldb == NULL)
		printf("Open %s failed!\n",p_pdbName);
	//创建数据库中表格
	DataBase_CreatTab("TxData", sqldb);	// 创建总发送表（表内容固定）
	DataBase_CreatTab("RxData", sqldb);	// 创建总接收表（表内容固定）
	DataBase_CreatTab("BSMData", sqldb);	// 创建主动安全表（表内容固定）
	DataBase_CreatTab("RSIData", sqldb);	// 创建RSI数据表（表内容固定）
//	DataBase_CreatTab("RSMData", sqldb);	// 创建RSM数据表（表内容固定）
	DataBase_CreatTab("SPATData", sqldb);	// 创建SPAT数据表（表内容固定）
	DataBase_CreatTab("YTRsuData", sqldb);
	return 0;
}
int CloseDataBase()
{
	int nRet = 0;
	return nRet;
}
