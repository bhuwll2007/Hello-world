#include "Serial.h"

//int Serial_Param_Get(int *baudrate, int *flowctrl, int *databits, int *stopbits, int *parity)
//{
//	char szFileName[] = "./A.ini";                  //配置文件名
//	int iRetCode = 0;
//
//	iRetCode |= GetConfigIntValue(szFileName,"SerialIni", "FlowCtrl", flowctrl);
//	iRetCode |= GetConfigIntValue(szFileName,"SerialIni", "Databits", databits);
//	iRetCode |= GetConfigIntValue(szFileName,"SerialIni", "StopBits", stopbits);
//	iRetCode |= GetConfigStringValue(szFileName,"SerialIni", "Parity", (char *)parity);
//	iRetCode |= GetConfigIntValue(szFileName,"SerialIni", "BaudRate", baudrate);
//	if (iRetCode)
//	{
//		printf("Read parameters form config file failed!\n");
//		return -1;
//	}
//
//	return 0;
//}
//
//int Serial_Param_Set(int fd, int baudrate, int flow_ctrl, int databits, int stopbits, int parity)
//{
//    int   i;
//    int   speed_arr[] = {B115200, B19200, B57600, B9600, B4800, B2400, B1200, B300};
//    int   name_arr[] = {115200, 19200, 57600, 9600, 4800, 2400, 1200, 300};
//
//    struct termios options;
//
//    //tcgetattr(fd,&options)得到与fd指向对象的相关参数，并将它们保存于options,该函数还可以测试配置是否正确，该串口是否可用等。若调用成功，函数返回值为0，若调用失败，函数返回值为1.
//    if(tcgetattr(fd, &options) != 0)
//    {
//        perror("SetupSerial failed!\n");
//        return -1;
//    }
//	bzero(&options, sizeof(options));
//
//    //设置串口输入波特率和输出波特率
//    for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)
//    {
//        if  (baudrate == name_arr[i])
//        {
//            cfsetispeed(&options, speed_arr[i]);
//            cfsetospeed(&options, speed_arr[i]);
//        }
//    }
//
//    //修改控制模式，保证程序不会占用串口
//    options.c_cflag |= CLOCAL;
//    //修改控制模式，使得能够从串口中读取输入数据
//    options.c_cflag |= CREAD;
//
//    //设置数据流控制
//    switch(flow_ctrl)
//    {
//        case 0 ://不使用流控制
//              options.c_cflag &= ~CRTSCTS;
//              break;
//        case 1 ://使用硬件流控制
//              options.c_cflag |= CRTSCTS;
//              break;
//        case 2 ://使用软件流控制
//              options.c_cflag |= IXON | IXOFF | IXANY;
//              break;
//    }
//    //设置数据位
//    //屏蔽其他标志位
//    options.c_cflag &= ~CSIZE;
//    switch (databits)
//    {
//        case 5:
//        	options.c_cflag |= CS5;
//            break;
//        case 6:
//            options.c_cflag |= CS6;
//            break;
//        case 7:
//            options.c_cflag |= CS7;
//            break;
//        case 8:
//            options.c_cflag |= CS8;
//            break;
//        default:
//            fprintf(stderr,"Unsupported data size\n");
//            return -1;
//    }
//    //设置校验位
//    switch (parity)
//    {
//        case 'n':
//        case 'N': //无奇偶校验位。
//                 options.c_cflag &= ~PARENB;
//                 options.c_iflag &= ~INPCK;
//                 break;
//        case 'o':
//        case 'O'://设置为奇校验
//                 options.c_cflag |= (PARODD | PARENB);
//                 options.c_iflag |= INPCK;
//                 break;
//        case 'e':
//        case 'E'://设置为偶校验
//                 options.c_cflag |= PARENB;
//                 options.c_cflag &= ~PARODD;
//                 options.c_iflag |= INPCK;
//                 break;
//        case 's':
//        case 'S': //设置为空格
//                 options.c_cflag &= ~PARENB;
//                 options.c_cflag &= ~CSTOPB;
//                 break;
//        default:
//                 fprintf(stderr,"Unsupported parity\n");
//                 return -1;
//    }
//    // 设置停止位
//    switch (stopbits)
//    {
//        case 1:
//        	options.c_cflag &= ~CSTOPB;
//        	break;
//        case 2:
//        	options.c_cflag |= CSTOPB;
//        	break;
//        default:
//        	fprintf(stderr, "Unsupported stop bits\n");
//        	return -1;
//    }
//
//    //修改输出模式，原始数据输出
//    options.c_oflag &= ~OPOST;
//
//    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
//
//    //忽略奇偶校验错误
//    options.c_iflag = IGNPAR;
//
//    //设置等待时间和最小接收字符
//    options.c_cc[VTIME] = 0; // 读取一个字符等待1*(1/10)s
//    options.c_cc[VMIN] = 1; // 读取字符的最少个数为1
//
//    //如果发生数据溢出，接收数据，但是不再读取 刷新收到的数据但是不读
//    tcflush(fd, TCIFLUSH);
//
//    //激活配置 (将修改后的termios数据设置到串口中）
//    if (tcsetattr(fd, TCSANOW, &options) != 0)
//    {
//        perror("com set error!\n");
//        return -1;
//    }
//
//    return 0;
//}
//
/*********************************************************************
 * 串口初始化
 * 函数名称：int Serial_Open(StcSerial *pStcSerial)
 * 功能描述：根据串口相关参数，执行串口打开，并清空串口缓存的操作；
 * 输入参数：StcSerial *pStcSerial 串口操作变量，包含串口参数配置及回调函数指针，详见定义
 * 输出参数：无
 * 返回结果：>0--成功 代表操作的串口句柄，<0 --失败
 *********************************************************************/
int Serial_Open(StcSerial *pStcSerial)
{
	int fd;
//	int baudrate = 115200, flowctrl = 0, databits = 8, stopbits = 1, parity = 'N';
	/*
	struct termios 结构体成员
	struct termios
    {
		unsigned short c_iflag;    // 输入模式标志
        unsigned short c_oflag;    // 输出模式标志
        unsigned short c_cflag;    // 控制模式标志
        unsigned short c_lflag;    // 区域模式标志或本地模式标志或局部模式
        unsigned char c_line;      // 行控制line discipline
        unsigned char c_cc[NCC];   // 控制字符特性
    };
    */

	//打开串口
	fd = open(pStcSerial->SerialName, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd < 0)
	{
		perror(pStcSerial->SerialName);
		return -1;
	}
	pStcSerial->fd = fd;
	// 将串口重新设置为阻塞模式
	// 阻塞模式下，VMIN > 0, VTIME = 0时，read将一直等待，知道有MIN个字符可以读取时返回
	if (fcntl(pStcSerial->fd, F_SETFL, 0) < 0)
	{
		printf("Fcntl failed!\n");
		return -2;
	}
#if 0
	//从配置文件中读取串口参数
	if (Serial_Param_Get(&baudrate, &flowctrl, &databits, &stopbits, &parity) < 0)
	{
		printf("Serial parameter get failed!\n");
		return -3;
	}

#endif
	//配置串口参数
//	if (Serial_Param_Set(fd, baudrate, flowctrl, databits, stopbits, parity) < 0)
//	{
//		printf("Serial parameter set failed!\n");
//		return -4;
//	}
	// 返回串口操作句柄
	return fd;
}

/********************************************************************
 * 函数名称：	void *Thread_seriaRecvData(void *arg)
 * 功能描述：	串口线程函数
 * 参数列表：	void *arg 此处无意义
 * 返回结果：	void *
 ********************************************************************/
void *Thread_seriaRecvData(void *arg)
{
	StcSerial *pStcSerial= (StcSerial *)arg;
	int rxlen = 0;
	uint8_t rxbuf[512];

	while (1)
	{
		if (pStcSerial->fd <= 0)
			break;
		rxlen = read(pStcSerial->fd, rxbuf, 512);
		if (rxlen > 0)
		{
			// TODO:处理串口接收到的数据
			// 注意：此处应将收到的数据放入队列后再处理 队列中 的数据，否则数据不完整
			memset(pStcSerial->m_SerialRecvbuf, 0x00,sizeof(pStcSerial->m_SerialRecvbuf));
			memcpy(pStcSerial->m_SerialRecvbuf, rxbuf, rxlen);
			pStcSerial->m_SerialRecvlen = rxlen;
#if DEBUG_PRINT_INFO
			printf("%d Recv： %s\n",pStcSerial->fd, pStcSerial->m_SerialRecvbuf);
#endif
			if(pStcSerial->pCallBackDealData != NULL)
			{
				pStcSerial->pCallBackDealData(pStcSerial->m_SerialRecvbuf, pStcSerial->m_SerialRecvlen);
			}
#if 0
			pTcpClient->pThread_TcpCln_DealData = *pThread_TcpCln_DealData;
			//创建接收线程
			pthread_t ptid;
			pthread_create(&ptid, 0, pTcpClient->pThread_TcpCln_DealData, pTcpClient);
#endif
		}
		else if (rxlen == 0)
		{

		}
		else
		{
			printf("Read failed!\n");
			break;
		}
	}
	close(pStcSerial->fd);

	return NULL;
}

/*********************************************************************
 * 开启串口数据接收
 * 函数名称：int Serial_CreatThread(StcSerial *pStcSerial)
 * 功能描述：创建接收数据的线程，等待接收数据，并将接收到的数据区存放至处理区；
 * 参数列表：StcSerial *pStcSerial 串口操作变量，包含串口参数配置及回调函数指针，详见定义
 * 输出参数：无
 * 返回结果：0--成功，非0--失败
 *********************************************************************/
int Serial_CreatThread(StcSerial *pStcSerial)
{
	int ret = 0;
	if (pStcSerial->fd < 0)
	{
		// 句柄不合法
		ret = -1;
		return ret;
	}

	//创建串口线程
	pthread_t serialThread;
	ret = pthread_create(&serialThread, NULL, Thread_seriaRecvData, pStcSerial);

	return ret;
}

/*********************************************************************
 * 串口发送数据
 * 函数名称：nint32_t Serial_Send(nint32_t fd, nint8_t *pBuf, nint32_t nBufLen)
 * 功能描述：提供通过串口向外发送数据的函数接口，并返回发送状态；
 * 输入参数：nint32_t fd 串口操作句柄, nint8_t *pBuf 发送数据的buf, nint32_t nBufLen 发送数据的长度
 * 输出参数：无
 * 返回结果：nint32_t 发送数据的状态 >0 表示发送成功，且值代表成功发送的数据，<0 表示发送失败，尝试三次发送仍然没有发送成功，
 *********************************************************************/
nint32_t Serial_Send(nint32_t fd, nint8_t *pBuf, nint32_t nBufLen)
{
	uint8_t send_ret = 0;
	uint8_t pos = 0, count = 0;
	nint32_t ret = 0;

	send_ret = write(fd, pBuf, nBufLen);
	while (send_ret < nBufLen)
	{
		pos += send_ret;
		nBufLen -= send_ret;
		send_ret = write(fd, &pBuf[pos], nBufLen);
		count++;
		if (count >= 3)
		{
			ret = -1;
			break;
		}
	}
	//给返回值赋值
	if(ret == 0)
		ret = nBufLen;
	return ret;
}
/*********************************************************************
 * 关闭串口操作
 * 函数名称：int Serial_Close(StcSerial *pStcSerial)
 * 功能描述：关闭串口操作，即关闭串口，停止串口数据接收线程，清空相关变量内容；
 * 输入参数：StcSerial *pStcSerial 串口操作变量，包含串口参数配置及回调函数指针，详见定义
 * 输出参数：无
 * 返回结果：0--成功，非0--失败
 *********************************************************************/
int Serial_Close(StcSerial *pStcSerial)
{
	if (pStcSerial->fd > 0)
	{
		close(pStcSerial->fd);
	}
	pStcSerial->fd = 0;
	pStcSerial->pCallBackDealData = NULL;
	return 0;
}
