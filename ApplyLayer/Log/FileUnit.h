/*
 * FileUnit.h
 *
 *  Created on: Nov 24, 2017
 *      Author: root
 */

#ifndef FILEUNIT_H_
#define FILEUNIT_H_

#include "./../../DataSet/DataSet.h"

void FileUpdata(char *pOldPath);
int FileSetName(char *pFilePath);

int FileGetsize(char *pFilePath);
int FileWrite(char *pFilePath, int Bufsize, void *pBuff);
int FileRead(char *pFilePath, void *pBuff, int offset);

#endif /* FILEUNIT_H_ */
