#include "./BaseFunct.h"

/*
 * 函数名称：	pkt_code()
 * 功能描述：	数据转义
 * 参数列表：
 *			src_buf			--	转义前数据
 *			target_buf		--	转义后数据
 *			len				--	转义前数据长度
 * 返回结果：
 *			转义后数据长度
 */
uint32_t pkt_code(uint8_t src_buf[], uint8_t target_buf[], uint32_t len)
{
    uint32_t i = 0, j = 0;

    if ((src_buf[0] == 0xFA)&&(src_buf[1] == 0xFB))
    {
        memcpy(target_buf, src_buf, len);
        return len;
    }
    if ((src_buf[0] == '^')&&(src_buf[1] == '^'))
    {
        memcpy(target_buf, src_buf, len);
        return len;
    }
    //帧头
    target_buf[i++] = src_buf[0];
    target_buf[i++] = src_buf[1];
    //数据
    for (j = 2; j < len - 1; j++)
    {
        if (src_buf[j] == 0xFF)
        {
            target_buf[i++] = 0xFE;
            target_buf[i++] = 0x01;
        }
        else if (src_buf[j] == 0xFE)
        {
            target_buf[i++] = 0xFE;
            target_buf[i++] = 0x00;
        }
        else
        {
            target_buf[i++] = src_buf[j];
        }
    }
    //帧尾
    target_buf[i++] = 0xFF;
    return i;
}


uint32_t pkt_code_DF(uint8_t src_buf[], uint8_t target_buf[], uint32_t len)
{
    uint32_t i = 0, j = 0;

    //帧头
    target_buf[i++] = src_buf[0];

    //数据
    for (j = 1; j < len - 1; j++)
    {
        if (src_buf[j] == 0x7E)
        {
            target_buf[i++] = 0x7D;
            target_buf[i++] = 0x03;
        }
        else if (src_buf[j] == 0x7D)
        {
            target_buf[i++] = 0x7D;
            target_buf[i++] = 0x00;
        }
        else
        {
            target_buf[i++] = src_buf[j];
        }
    }
    //帧尾
    target_buf[i++] = 0x7E;
    return i;
}



/**
 * 函数名称：void Fun_Xor(char* ppSendBuf, int pnSendBuflen)
 * 函数功能：计算指定数组的异或校验值
 * 参数说明：char* ppSendBuf,待计算校验值的数组， int pnSendBuflen 待计算校验值的数组的长度
 * 函数返回值：void 无
 * */
void Fun_Xor(uint8_t* ppSendBuf, uint32_t pnSendBuflen)
{
    //循环变量控制
    int nTmpi = 0;

    //计算校验结果
    char cCheckRet = 0;

    //定义指向数组的指针
    uint8_t* pBuf = ppSendBuf;

    //待处理数据的长度
    int nLen;

    nLen = pnSendBuflen;
//	for (nTmpi = 0; nTmpi < nLen; nTmpi++)
//	{
//		cCheckRet ^= *pBuf++;
//	}
//
//	*pBuf++ = cCheckRet;

#if 1
    //FF开头的数据，才会进行数据校验的计算
    if (*pBuf == (uint8_t) 0xff)
    {
        //去掉前两个FF,
        nLen = pnSendBuflen - 2;
        for (nTmpi = 0; nTmpi < nLen; nTmpi++)
        {
            cCheckRet ^= *pBuf++;
        }
    }
    else if ((*pBuf == (uint8_t) 0xfa) )
    {
        pBuf++;
        pBuf++;
        for (nTmpi = 2; nTmpi < nLen-1; nTmpi++)
        {
            cCheckRet ^= *pBuf++;
        }

        *pBuf++ = cCheckRet;
//        printf("-----  %02X ------\n",cCheckRet);
    }
    else
    {
        return;
    }
    *pBuf++ = cCheckRet;
#endif
}
