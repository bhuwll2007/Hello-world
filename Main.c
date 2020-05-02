/*
 * Main.c
 * Created on: May 25, 2018
 * Author: root
 */
//432221111111yyyyyyyy
#include "Include/lib_Include.h"
#include "ApplyLayer/CAN/CAN.h"
#include "ApplyLayer/GPS/GPS.h"
#include "ApplyLayer/DataBase/DataBase.h"

#include "ApplyLayer/HostStatus/HostStatus.h"
#include "ApplyLayer/AroundVeh/AroundVeh.h"
#include "ApplyLayer/AroundRsi/AroundRsi.h"

#include "ApplyLayer/Net/TcpServer.h"
#include "ApplyLayer/Net/TcpClient.h"

#include "ApplyLayer/Common/CheckParam.h"
#include "ApplyLayer/Common/Common.h"
#include "ApplyLayer/Common/ReadIniParam.h"

#include "ApplyLayer/V2X/V2X_Init.h"
#include "ApplyLayer/V2X_Transf/RsiDeal.h"


#define _GNU_SOURCE
#define Print_Log 0

void Stop(int signo)
{
	//初始化数据库操作 -- 关闭操作

	//创建本机设备运行状态监听线程 -- 关闭操作

	//创建定时上传周围车辆相关信息帧 -- 关闭操作

	//打开GPS -- 关闭操作
	CloseGPS();

	//打开CAN -- 关闭操作
	CloseCAN();

	//创建TCP服务器 -- 关闭操作
	TcpSer_ClsSoc(&g_StcTcpServer);

	//开启V2X接口 -- 关闭操作

	//数据库关闭
	CloseDataBase();

//	_exit(0);
}

int main(int argc, char *argv[])
{
	int nRet = 0;
	signal(SIGINT, Stop);
	//sleep(10);
	if (argv[1][strlen(argv[1]) - 1] == 'h')
	{
		printf("Program Version:%s!\n", Program_Version);
		return 0;
	}
	//判断参数是否正确
	nRet = CheckParam(argc, argv);
	if (nRet == 0)
	{
#if Print_Log
		printf("CheckParam _OK\n");
#endif
	} else
	{
		printf("CheckParam _ERROR\n");
		return nRet;
	}

	//创建/Log目录
	nRet = CreatDir("/Log");
	if (nRet == 0)
	{
#if Print_Log
		printf("CreatDir _OK\n");
#endif
	} else
	{
		printf("CreatDir _ERROR\n");
		return nRet;
	}

	//读取配置文件参数
	nRet = ReadParam();
    if (nRet != 0)
    {
		printf("ReadParam _ERROR\n");
		return nRet;
	}

	//平台连接
	nRet = FG_PlatformLinkUdpinit(atoi(argv[1]));
	if(nRet == 0)
	{
		printf("Platform UDP OK\n");
	}
	else
	{
		printf("Platform UDP Error\n");
	}
#if 1
	nRet = InitDataBase();
	if (nRet != 0)
	{
		printf("InitDataBase() error\n");
		return nRet;
	}
	
	//到PA端消息队列初始化
	initMsgQueue_Send2Veh();

	//创建本机设备运行状态监听线程
	if (g_tAPP_RunParam.nSwitch_UpD1 != 0)
	{
		nRet = InitHostStatus();
		if (nRet == 0) {
#if Print_Log
			printf("InitHostStatus _OK\n");
#endif
		} else {
			printf("InitHostStatus _ERROR\n");
			return nRet;
		}
	}

	//创建定时上传周围车辆相关信息帧
	if (g_tAPP_RunParam.nSwitch_UpD0 != 0)
	{
		nRet = InitAroundVeh();
		if (nRet == 0)
		{
#if Print_Log
			printf("InitAroundVeh _OK\n");
#endif
		} else
		{
			printf("InitAroundVeh _ERROR\n");
			return nRet;
		}
	}
	//创建定时上传周围RSI相关信息帧
	if (g_tAPP_RunParam.nSwitch_UpD9 != 0)
	{
		nRet = InitAroundRsi();
		if (nRet == 0)
		{
#if Print_Log
			printf("InitAroundRsi _OK\n");
#endif
		} else
		{
			printf("InitAroundRsi _ERROR\n");
			return nRet;
		}
	}
	//打开GPS
	nRet = InitGPS();
	if (nRet > 0)
	{
#if Print_Log
		printf("InitGPS _OK\n");
#endif
	} else
	{
		printf("InitGPS _ERROR\n");
		return nRet;
	}

	//打开CAN
	nRet = InitCAN();
	if (nRet > 0)
	{
#if Print_Log
		printf("InitCAN _OK\n");
#endif
	} else
	{
		printf("InitCAN _ERROR\n");
		return nRet;
	}

	//创建TCP服务器
	nRet = InitTcpServer(atoi(argv[1]));
	if (nRet == 0)
	{
#if Print_Log
		printf("InitTcpServer _OK\n");
#endif
	} else
	{
		printf("InitTcpServer _ERROR\n");
		return nRet;
	}

	//开启V2X接口
	nRet = V2X_Init(&Store);
	if (nRet == 0)
	{

	} else
	{
		return nRet;
	}
#endif
	while (1)
	{
		sleep(1);
	}
	printf("End\n");
	return 0;
}
