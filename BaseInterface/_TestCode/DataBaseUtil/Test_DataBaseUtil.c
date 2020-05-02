#include "./../../DataBaseUtil/DataBase.h"

int main(int argc, char **argv)
{
    sqlite3 * sqldb;
    struct TxRxTblData data[8];
    int i = 0;
    int ret = 0;

    sqldb = DataBase_OpenDB("./DBTst.db", 1);  // 打开数据库

    ret = DataBase_CreatTab("TxData", sqldb);  // 创建表（表内容固定）
    printf("ret:%d\n", ret);
    for (i = 0; i < 8; i ++)
    {
        data[i].msgCnt = 1;
        data[i].MsgType = 2;
        data[i].DataBuf = calloc(4, sizeof(char));
        data[i].DataBuf[0] = 0x31+i;
        data[i].DataBuf[1] = 0x32;
        data[i].DataBuf[2] = 0x33;
        data[i].DataBuf[3] = 0x34;
        data[i].DataLen = 4;

        data[i].DataTime = 6;
    }
    ret = DataBase_Tab_Insert("TxData", sqldb, data, 8);  // 添加数据
    printf("ret:%d\n", ret);

    memset(data, 0, 8 * sizeof(struct TxRxTblData));
    for (i = 0; i < 5; i ++)
    {
        printf("%d\n", data[i].KeyID);
        printf("\n");
    }

    ret = DataBase_Tab_Select("TxData", sqldb, "DataTime", "6", data, &i, -1);  // 读取数据
    printf("i:%d\n", i);

    for (i = 0; i < 5; i ++)
    {
        printf("%d\n", data[i].KeyID);
        printf("\n");
    }

    printf("msgCnt:%d\n", data[0].msgCnt);
    printf("MsgType:%d\n", data[0].MsgType);
    printf("DataTime:%llu\n", data[0].DataTime);
    printf("KeyID:%d\n", data[0].KeyID);

    ret = DataBase_Tab_Delete("TxData", sqldb, "KeyID", "4");  // 删除制定字段的所有数据条
    printf("ret:%d\n", ret);

//	ret = DataBase_ClearTab("TxData", sqldb);  // 删除制定表中的所有数据
//	printf("ret:%d\n", ret);

//	DataBase_DeleteTab("TxData", sqldb);  // 删除指定表
//	printf("ret:%d\n", ret);

    DataBase_Tab_Update("TxData", sqldb, "DataTime", "100", "9");  //将指定字段的数据内容替换为其他指定内容
    printf("ret:%d\n", ret);

    DataBase_CloseDB(sqldb);
    printf("ret:%d\n", ret);

    free(data[0].DataBuf);
    return 0;
}
