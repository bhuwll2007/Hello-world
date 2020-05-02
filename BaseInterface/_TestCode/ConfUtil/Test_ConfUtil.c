/*************************************************************
    FileName : test.c
    FileFunc : 定义测试功能文件
    Version  : V0.1
    Author   : Sunrier
    Date     : 2012-05-09
    Descp    : Linux下获取配置文件信息
*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../../ConfUtil/Config.h"

int main (int argc,char *argv[])
{
    char szFileName[100];/*配置文件名*/
    char szSectionName[100];/*节名*/
    char szKeyName[100];/*键名*/
    char szKeyValue[100];/*键值*/
    int iRetCode = 0;
    int iPort = -1;
    char szHostIp[30];

    memset(szFileName,0,sizeof(szFileName));
    sprintf(szFileName,"./sysconfig.ini");
    memset(szSectionName,0,sizeof(szSectionName));
    memcpy(szSectionName,argv[1],strlen(argv[1]));
    memset(szKeyName,0,sizeof(szKeyName));
    memcpy(szKeyName,argv[2],strlen(argv[2]));
    memset(szKeyValue,0,sizeof(szKeyValue));
    memset(szHostIp,0,sizeof(szHostIp));

    iRetCode = GetConfigStringValue(szFileName,argv[1],argv[2],szHostIp);
    if( iRetCode )
    {
        printf("iRetCode : %d !\n",iRetCode );
    }
    else
    {
        printf("HOSTIP: %s\n",szHostIp);
    }

    iRetCode = GetConfigIntValue(szFileName,"Config1","PORT",&iPort);
    if( iRetCode )
    {
        printf("iRetCode : %d !\n",iRetCode );
    }
    else
    {
        printf("PORT: %d\n",iPort);
    }

    return 0;
}
