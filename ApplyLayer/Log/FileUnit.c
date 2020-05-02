/*
 * FileUnit.c
 *
 *  Created on: Nov 24, 2017
 *      Author: root
 */

#include "FileUnit.h"


void FileUpdata(char *pOldPath)
{
	char OldFilepath[30];
	char CurFilepath[30];

	memcpy(OldFilepath, pOldPath, strlen(pOldPath));

	FileSetName(CurFilepath);

	if (strcmp(CurFilepath, OldFilepath) == 0)
		return ;
	else
		memcpy(pOldPath, CurFilepath, strlen(CurFilepath));

}


int FileSetName(char *pFilePath)
{
	time_t at_time = time(NULL);
	struct tm ptr;
	char YMDHMS[22];
	localtime_r(&at_time, &ptr);

	strftime(YMDHMS, 22, "%Y-%m-%d", &ptr);

	sprintf(pFilePath, "./Log/%s.txt", YMDHMS);

	return 0;
}

int FileGetsize(char *pFilePath)
{
    FILE *pFile = fopen(pFilePath, "r");
    if (!pFile)
        return -1;

    fseek(pFile, 0, SEEK_END);
    int	fsize = ftell(pFile);

    return fsize;
}


int FileWrite(char *pFilePath, int Bufsize, void *pBuff)
{
    char *p = (char *)pBuff;
    int i = 0;

    FILE *pFile = fopen(pFilePath, "a+");
    if (!pFile)
        return -1;


    for ( ;i<Bufsize;i++)
    {
    	fwrite(&p[i], sizeof(char), 1, pFile);
    }

    fclose(pFile);

    return 0;
}


int FileRead(char *pFilePath, void *pBuff, int offset)
{
    char buf[1024];
    int Bufsize = 0;

    FILE *pFile = fopen(pFilePath, "r");
    if (!pFile)
        return -1;

    fseek(pFile, (long)offset, SEEK_SET );

    Bufsize = fread(buf, sizeof(char), sizeof(buf), pFile);
    fclose(pFile);

    memcpy((char *)pBuff, buf, Bufsize);
    return Bufsize;
}

/*
void main()
{
    char Buf[1024] = {0};
    int len = 0;

    int filesize = FileGetsize(pFilePath);
    printf("File Size %d Bytes\n", filesize);

    int i = 0, j = 0;

    for (; j<=filesize;j+=1024)
    {
        len = FileRead(pFilePath, Buf, j);
        printf("Load buff size: %d\n", len);

        for (i = 0;i< len;i++)
        {
            printf("0x%02X ", Buf[i]);
        }
        printf("\n");

        memset(Buf, 0x00, sizeof(Buf));
   }

}
*/
