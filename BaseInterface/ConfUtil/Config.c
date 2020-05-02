/*************************************************************
 FileName : config.c
 FileFunc : 定义实现文件
 Version  : V0.1
 Author   : Sunrier
 Date     : 2012-05-09
 Descp    : Linux下获取配置文件信息
 *************************************************************/
#include "Config.h"

/*区分大小写*/
int CompareString(char *pInStr1, char *pInStr2)
{
	if (strlen(pInStr1) != strlen(pInStr2))
	{
		return STRING_LENNOTEQUAL;
	}

	/*while( toupper(*pInStr1)==toupper(*pInStr2) )*/
	while (*pInStr1 == *pInStr2)
	{
		if ('\0' == *pInStr1)
			break;
		pInStr1++;
		pInStr2++;
	}

	if ('\0' == *pInStr1)
		return STRING_EQUAL;

	return STRING_NOTEQUAL;

}

int GetKeyValue(FILE *fpConfig, char *pInKeyName, char *pOutKeyValue)
{
	char szBuffer[10240];
	char *pStr1, *pStr2, *pStr3;
	unsigned int uiLen;
	int iRetCode = 0;

	memset(szBuffer, 0, sizeof(szBuffer));
	while (!feof(fpConfig))
	{
		if (NULL == fgets(szBuffer, 150, fpConfig))
			break;
		pStr1 = szBuffer;
		while ((' ' == *pStr1) || ('\t' == *pStr1))
			pStr1++;
		if ('#' == *pStr1)
			continue;
		if (('/' == *pStr1) && ('/' == *(pStr1 + 1)))
			continue;
		if (('\0' == *pStr1) || (0x0d == *pStr1) || (0x0a == *pStr1))
			continue;
		if ('[' == *pStr1)
		{
			pStr2 = pStr1;
			while ((']' != *pStr1) && ('\0' != *pStr1))
				pStr1++;
			if (']' == *pStr1)
				break;
			pStr1 = pStr2;
		}
		pStr2 = pStr1;
		while (('=' != *pStr1) && ('\0' != *pStr1))
			pStr1++;
		if ('\0' == *pStr1)
			continue;
		pStr3 = pStr1 + 1;
		if (pStr2 == pStr1)
			continue;
		*pStr1 = '\0';
		pStr1--;
		while ((' ' == *pStr1) || ('\t' == *pStr1))
		{
			*pStr1 = '\0';
			pStr1--;
		}

		iRetCode = CompareString(pStr2, pInKeyName);
		if (!iRetCode)/*检查键名*/
		{
			pStr1 = pStr3;
			while ((' ' == *pStr1) || ('\t' == *pStr1))
				pStr1++;
			pStr3 = pStr1;
			while (('\0' != *pStr1) && (0x0d != *pStr1) && (0x0a != *pStr1))
			{
				if (('/' == *pStr1) && ('/' == *(pStr1 + 1)))
					break;
				pStr1++;
			}
			*pStr1 = '\0';
			uiLen = strlen(pStr3);
			memcpy(pOutKeyValue, pStr3, uiLen);
			*(pOutKeyValue + uiLen) = '\0';
			return SUCCESS;
		}
	}

	return KEYNAME_NOTEXIST;
}

/**
 * 函数名称：int GetConfigIntValue(char *pInFileName,char *pInSectionName,char *pInKeyName,int *pOutKeyValue);
 * 函数功能：在配置文件中，查找set名为pInSectionName,Item名为pInKeyName的值，然后以pOutKeyValue返回结果
 * 参数说明：char *pInFileName 配置文件名,char *pInSectionName set文件名,char *pInKeyName item名,int *pOutKeyValue item值
 * 函数返回值：获取配置文件的结果状态，0：表示执行成功，非0表示失败
 * */
int GetConfigIntValue(char *pInFileName, char *pInSectionName, char *pInKeyName,
		int *pOutKeyValue)
{
	int iRetCode = 0;
	char szKeyValue[500], *pStr;

	memset(szKeyValue, 0, sizeof(szKeyValue));
	iRetCode = GetConfigStringValue(pInFileName, pInSectionName, pInKeyName,
			szKeyValue);
	if (iRetCode)
		return iRetCode;
	pStr = szKeyValue;
	while ((' ' == *pStr) || ('\t' == *pStr))
		pStr++;
	if (('0' == *pStr) && (('x' == *(pStr + 1)) || ('X' == *(pStr + 1))))
		sscanf(pStr + 2, "%x", pOutKeyValue);
	else
		sscanf(pStr, "%d", pOutKeyValue);

	return SUCCESS;
}

/**
 * 函数名称：int GetConfigStringValue(char *pInFileName,char *pInSectionName,char *pInKeyName,char *pOutKeyValue);
 * 函数功能：在配置文件中，查找set名为pInSectionName,Item名为pInKeyName的值，然后以pOutKeyValue返回结果
 * 参数说明：char *pInFileName 配置文件名,char *pInSectionName set文件名,char *pInKeyName item名,char *pOutKeyValue item值
 * 函数返回值：获取配置文件的结果状态，0：表示执行成功，非0表示失败
 * */
int GetConfigStringValue(char *pInFileName, char *pInSectionName,
		char *pInKeyName, char *pOutKeyValue)
{
	FILE *fpConfig;
	char szBuffer[10240];
	char *pStr1, *pStr2;
	int iRetCode = 0;

	/*test*/
	/*
	 printf("pInFileName: %s !\n",pInFileName);
	 printf("pInSectionName: %s !\n",pInSectionName);
	 printf("pInKeyName: %s !\n",pInKeyName);
	 */

	memset(szBuffer, 0, sizeof(szBuffer));
	if (NULL == (fpConfig = fopen(pInFileName, "r")))
		return FILENAME_NOTEXIST;

	while (!feof(fpConfig))
	{
		if (NULL == fgets(szBuffer, 150, fpConfig))
			break;
		pStr1 = szBuffer;
		while ((' ' == *pStr1) || ('\t' == *pStr1))
			pStr1++;
		if ('[' == *pStr1)
		{
			pStr1++;
			while ((' ' == *pStr1) || ('\t' == *pStr1))
				pStr1++;
			pStr2 = pStr1;
			while ((']' != *pStr1) && ('\0' != *pStr1))
				pStr1++;
			if ('\0' == *pStr1)
				continue;
			while (' ' == *(pStr1 - 1))
				pStr1--;
			*pStr1 = '\0';

			iRetCode = CompareString(pStr2, pInSectionName);
			if (!iRetCode)/*检查节名*/
			{
				iRetCode = GetKeyValue(fpConfig, pInKeyName, pOutKeyValue);
				fclose(fpConfig);
				return iRetCode;
			}
		}
	}

	fclose(fpConfig);
	return SECTIONNAME_NOTEXIST;
}

/**
 * 函数名称：int SetConfigIntValue(char *pInFileName,char *pInSectionName,char *pInKeyName,int NewKeyValue);
 * 函数功能：在配置文件中，设置set名为pInSectionName,Item名为pInKeyName的值，设置为NewKeyValue值
 * 参数说明：char *pInFileName 配置文件名,char *pInSectionName set文件名,char *pInKeyName item名,int NewKeyValue item值
 * 函数返回值：设置配置文件的结果状态，0：表示执行成功，非0表示失败
 * */
int SetConfigIntValue(char *pInFileName, char *pInSectionName, char *pInKeyName,
		int pNewKeyValue)
{
	int nRet = 0;
	char str[20];
	memset(str, 0x00, sizeof(str));
	sprintf(str, "%d", pNewKeyValue);
	nRet = ConfigSetKey(pInFileName, pInSectionName, pInKeyName, str);
	return nRet;
}

/**
 * 函数名称：int SetConfigStringValue(char *pInFileName,char *pInSectionName,char *pInKeyName,char *pNewKeyValue);
 * 函数功能：在配置文件中，设置set名为pInSectionName,Item名为pInKeyName的值，设置为NewKeyValue值
 * 参数说明：char *pInFileName 配置文件名,char *pInSectionName set文件名,char *pInKeyName item名,char *pNewKeyValue item值
 * 函数返回值：设置配置文件的结果状态，0：表示执行成功，非0表示失败
 * */
int SetConfigStringValue(char *pInFileName, char *pInSectionName,
		char *pInKeyName, char *pNewKeyValue)
{
	int nRet = 0;
	nRet = ConfigSetKey(pInFileName, pInSectionName, pInKeyName, pNewKeyValue);
	return nRet;
}
