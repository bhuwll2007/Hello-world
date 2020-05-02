#include "./../../ConfDB/ConfDB.h"

int main()
{
	char DBFileName[30] = "/etc/Test.db";
	char acTablName[30] = "Test";
//	char g_acConfDBTablCtrlName[30] = "Tab_ConfCtrl";
	sqlite3 *g_db = ConfDB_OpenDB(DBFileName, 1);

	ConfDB_CreatTab(acTablName, g_db);
	ITEM l_ConfItem;
	int i = 0;
	for (i = 0; i <= 3; i++)
	{
		switch (i)
		{
		case 0:
			sprintf(l_ConfItem.keyName, "SRCADDR");
			sprintf(l_ConfItem.keyValue, "0");
			break;
		case 1:
			sprintf(l_ConfItem.keyName, "DUMPPAYLOAD");
			sprintf(l_ConfItem.keyValue, "0");
			break;
		case 2:
			sprintf(l_ConfItem.keyName, "DUMP2STDOUT");
			sprintf(l_ConfItem.keyValue, "0");
			break;
		case 3:
			sprintf(l_ConfItem.keyName, "TXCHNID");
			sprintf(l_ConfItem.keyValue, "172");
			break;
		default:
			break;
		}
		ConfDB_Tab_Insert(DBFileName, g_db, l_ConfItem);
	}

	int nRet = 0;
	for (i = 1; i <= 20; i++)
	{
		nRet = ConfDB_Tab_SelectAll("Test", g_db, i, &l_ConfItem);
		if (nRet == 0)
			printf("%s %s\n", l_ConfItem.keyName, l_ConfItem.keyValue);
		else
			break;
	}
	return 0;
}
