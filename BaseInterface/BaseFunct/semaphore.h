/**
 * @ingroup BaseFunct API
 * @{
 *
 * 本模块提供基本操作的API接口.
 *
 * @file semaphore.h
 * @brief API file.
 *
 */

#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <errno.h>
#include <string.h>



union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};


/**
 * @brief 初始化一个信号量.
 * @param[in] sem_id  信号量id.
 * @param[in] value   信号量初始化值.
 * @return  函数执行成功返回1,执行失败返回0.
 */
int set_semvalue(int sem_id, int value);
/**
 * @brief 删除一个信号量.
 * @param[in] sem_id  信号量id.
 * @return  函数执行成功返回1,执行失败返回0.
 */
void del_semvalue(int sem_id);
/**
 * @brief 申请一个信号量.
 * @param[in] sem_id  信号量id.
 * @return  函数执行成功返回1,执行失败返回0.
 */
int semaphore_p(int sem_id);
/**
 * @brief 释放一个信号量.
 * @param[in] sem_id  信号量id.
 * @return  函数执行成功返回1,执行失败返回0.
 */
int semaphore_v(int sem_id);

#endif

/**
 * @}
 */
